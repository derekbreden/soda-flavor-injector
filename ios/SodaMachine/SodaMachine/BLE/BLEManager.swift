import Foundation
import CoreBluetooth
import UIKit
import os

/// Nordic UART Service UUIDs
private let nusServiceUUID = CBUUID(string: "6E400001-B5A3-F393-E0A9-E50E24DCCA9E")
private let nusRxUUID       = CBUUID(string: "6E400002-B5A3-F393-E0A9-E50E24DCCA9E") // phone writes here
private let nusTxUUID       = CBUUID(string: "6E400003-B5A3-F393-E0A9-E50E24DCCA9E") // phone subscribes here

private let log = Logger(subsystem: "com.derekbreden.SodaMachine", category: "BLE")

private let scanTimeout: TimeInterval = 10

enum ConnectionState: Equatable {
    case bluetoothOff
    case searching
    case searchingLong  // been searching a while, show hints
    case connecting
    case connected
}

class BLEManager: NSObject, ObservableObject {
    @Published var connectionState: ConnectionState = .bluetoothOff
    @Published var lastResponse: String = ""

    // Config state (synced from ESP32 via S3 bridge)
    @Published var configSynced = false
    @Published var flavor1Image: Int = 0
    @Published var flavor2Image: Int = 1
    @Published var flavor1Ratio: Int = 20
    @Published var flavor2Ratio: Int = 20
    @Published var numImages: Int = 0

    // Image list and cached images
    @Published var imageNames: [String] = []
    @Published var cachedImages: [Int: UIImage] = [:]
    @Published var imageDownloadProgress: Double? = nil  // nil = not downloading

    private var pendingImageList: [String] = []

    // Image download state
    private var imgDownloadSlot: Int = -1
    private var imgDownloadData = Data()
    private var imgDownloadExpected: Int = 0
    private var imgDownloadQueue: [Int] = []
    private var isDownloading = false
    private var nusReady = false

    private var centralManager: CBCentralManager!
    private var connectedPeripheral: CBPeripheral?
    private var rxCharacteristic: CBCharacteristic?
    private var txCharacteristic: CBCharacteristic?
    private var scanTimer: Timer?
    private var reconnectTimer: Timer?

    override init() {
        super.init()
        centralManager = CBCentralManager(delegate: self, queue: nil)
    }

    // MARK: - Public API

    func send(_ text: String) {
        guard let rx = rxCharacteristic, let p = connectedPeripheral else {
            log.warning("Cannot send: not connected")
            return
        }
        guard let data = text.data(using: .utf8) else { return }
        p.writeValue(data, for: rx, type: .withResponse)
        log.info("TX: \(text)")
    }

    func requestConfig() {
        send("GET_CONFIG")
    }

    func requestImageList() {
        pendingImageList = []
        send("LIST")
    }

    func sendSet(_ key: String, value: Int) {
        send("SET:\(key)=\(value)")
        DispatchQueue.main.asyncAfter(deadline: .now() + 0.05) { [weak self] in
            self?.send("SAVE")
        }
        // No need to request config — ESP32 pushes CONFIG: after SAVE
    }

    func displayName(for index: Int) -> String {
        guard index >= 0, index < imageNames.count else {
            return "Image \(index)"
        }
        let name = imageNames[index]
        if name.isEmpty { return "Image \(index)" }
        return name.replacingOccurrences(of: "_", with: " ").capitalized
    }

    func imageFor(slot: Int) -> UIImage? {
        return cachedImages[slot]
    }

    func downloadAllImages() {
        guard !isDownloading else { return }
        imgDownloadQueue = Array(0..<numImages).filter { cachedImages[$0] == nil }
        if imgDownloadQueue.isEmpty { return }
        isDownloading = true
        downloadNextImage()
    }



    // MARK: - Image download

    private func downloadNextImage() {
        guard !imgDownloadQueue.isEmpty else {
            imageDownloadProgress = nil
            isDownloading = false
            return
        }
        let slot = imgDownloadQueue.removeFirst()
        // Don't set imgDownloadSlot here — wait for IMGSTART response.
        // Setting it early causes IMGSTART text to be routed as binary data.
        imgDownloadData = Data()
        imgDownloadExpected = 0
        imageDownloadProgress = 0
        send("GETPNG:\(slot)")
    }

    private func handleImageData(_ data: Data) {
        imgDownloadData.append(data)
        if imgDownloadExpected > 0 {
            let progress = Double(imgDownloadData.count) / Double(imgDownloadExpected)
            imageDownloadProgress = min(progress, 1.0)
        }
        // Don't finish here — wait for IMGEND from S3 to avoid race condition
        // where the next GETPNG overlaps with the current slot's IMGEND
    }

    private func finishImageDownload() {
        if let image = UIImage(data: imgDownloadData) {
            cachedImages[imgDownloadSlot] = image
            log.info("Image \(self.imgDownloadSlot) cached (\(self.imgDownloadData.count) bytes)")
        } else {
            log.error("Image \(self.imgDownloadSlot) decode failed: \(self.imgDownloadData.count) bytes")
        }
        imgDownloadSlot = -1
        imgDownloadData = Data()
        downloadNextImage()
    }

    // MARK: - Response parsing

    private func handleNotification(_ data: Data) {
        // Log all incoming notifications for debugging
        if let text = String(data: data, encoding: .utf8), data.count < 100 {
            log.info("RX: \(text) (\(data.count) bytes)")
        } else {
            log.info("RX: <binary> (\(data.count) bytes)")
        }

        // If we're downloading an image, check if this is text or binary
        if imgDownloadSlot >= 0 {
            // Check if it's the IMGEND marker
            if data.count < 20, let text = String(data: data, encoding: .utf8), text == "IMGEND" {
                finishImageDownload()
                return
            }

            // Binary image data
            handleImageData(data)
            return
        }

        // Try as text
        if let text = String(data: data, encoding: .utf8) {
            handleTextResponse(text)
        }
    }

    private func handleTextResponse(_ text: String) {
        lastResponse = text

        if text.hasPrefix("CONFIG:") {
            parseConfig(text)
        } else if text.hasPrefix("IMG:") {
            parseImageLine(text)
        } else if text == "END" {
            imageNames = pendingImageList
            pendingImageList = []
            // Auto-download images after getting list
            downloadAllImages()
        } else if text.hasPrefix("IMGSTART:") {
            // IMGSTART:slot:size
            let parts = text.split(separator: ":")
            if parts.count >= 3, let slot = Int(parts[1]), let size = Int(parts[2]) {
                imgDownloadSlot = slot
                imgDownloadExpected = size
                imgDownloadData = Data()
                imageDownloadProgress = 0
                log.info("Starting image download: slot \(slot), \(size) bytes")
            }
        } else if text.hasPrefix("ERR:") {
            log.error("Error response: \(text)")
            if isDownloading {
                log.error("Skipping failed image download, continuing queue")
                downloadNextImage()
            }
        }
    }

    private func parseConfig(_ text: String) {
        let body = String(text.dropFirst(7))
        var values: [String: Int] = [:]
        for pair in body.split(separator: ",") {
            let parts = pair.split(separator: "=", maxSplits: 1)
            if parts.count == 2, let val = Int(parts[1]) {
                values[String(parts[0])] = val
            }
        }

        if let v = values["F1_RATIO"] { flavor1Ratio = v }
        if let v = values["F2_RATIO"] { flavor2Ratio = v }
        if let v = values["F1_IMAGE"] { flavor1Image = v }
        if let v = values["F2_IMAGE"] { flavor2Image = v }
        if let v = values["numImages"] { numImages = max(v, 1) }
        configSynced = true
        log.info("Config synced: F1=\(self.flavor1Image)/\(self.flavor1Ratio) F2=\(self.flavor2Image)/\(self.flavor2Ratio)")
    }

    private func parseImageLine(_ text: String) {
        let parts = text.split(separator: ":", maxSplits: 2)
        if parts.count >= 3 {
            let name = String(parts[2])
            let slot = Int(parts[1]) ?? pendingImageList.count
            while pendingImageList.count <= slot {
                pendingImageList.append("")
            }
            pendingImageList[slot] = name
        }
    }

    // MARK: - Internal

    private func startScan() {
        guard centralManager.state == .poweredOn else { return }
        connectionState = .searching
        configSynced = false
        cachedImages = [:]
        centralManager.scanForPeripherals(withServices: [nusServiceUUID], options: [
            CBCentralManagerScanOptionAllowDuplicatesKey: false
        ])
        log.info("Auto-scanning for Soda Machine...")

        // After timeout, show hints but keep scanning
        scanTimer?.invalidate()
        scanTimer = Timer.scheduledTimer(withTimeInterval: scanTimeout, repeats: false) { [weak self] _ in
            guard let self, self.connectionState == .searching else { return }
            self.connectionState = .searchingLong
            log.info("Still scanning, showing hints")
        }
    }

    private func scheduleReconnect() {
        reconnectTimer?.invalidate()
        reconnectTimer = Timer.scheduledTimer(withTimeInterval: 2, repeats: false) { [weak self] _ in
            self?.startScan()
        }
    }
}

// MARK: - CBCentralManagerDelegate

extension BLEManager: CBCentralManagerDelegate {
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        log.info("Central state: \(central.state.rawValue)")
        if central.state == .poweredOn {
            startScan()
        } else {
            connectionState = .bluetoothOff
        }
    }

    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral,
                         advertisementData: [String: Any], rssi RSSI: NSNumber) {
        let name = peripheral.name ?? advertisementData[CBAdvertisementDataLocalNameKey] as? String ?? "Unknown"
        log.info("Found: \(name) (RSSI: \(RSSI.intValue))")

        scanTimer?.invalidate()
        centralManager.stopScan()
        connectionState = .connecting
        connectedPeripheral = peripheral
        centralManager.connect(peripheral, options: nil)
    }

    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        log.info("Connected to \(peripheral.name ?? "device")")
        peripheral.delegate = self
        peripheral.discoverServices([nusServiceUUID])
    }

    func centralManager(_ central: CBCentralManager, didFailToConnect peripheral: CBPeripheral, error: Error?) {
        log.error("Connection failed: \(error?.localizedDescription ?? "unknown")")
        connectedPeripheral = nil
        scheduleReconnect()
    }

    func centralManager(_ central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: Error?) {
        log.info("Disconnected")
        connectionState = .searching
        connectedPeripheral = nil
        rxCharacteristic = nil
        txCharacteristic = nil
        configSynced = false
        imgDownloadSlot = -1
        imgDownloadQueue = []
        isDownloading = false
        nusReady = false
        scheduleReconnect()
    }
}

// MARK: - CBPeripheralDelegate

extension BLEManager: CBPeripheralDelegate {
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        guard let services = peripheral.services else { return }
        for service in services where service.uuid == nusServiceUUID {
            peripheral.discoverCharacteristics([nusRxUUID, nusTxUUID], for: service)
        }
    }

    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        guard let chars = service.characteristics else { return }
        for char in chars {
            if char.uuid == nusTxUUID {
                txCharacteristic = char
                peripheral.setNotifyValue(true, for: char)
                log.info("Subscribed to TX notifications")
            } else if char.uuid == nusRxUUID {
                rxCharacteristic = char
                log.info("Found RX characteristic")
            }
        }
        if rxCharacteristic != nil && txCharacteristic != nil && !nusReady {
            nusReady = true
            connectionState = .connected
            peripheral.maximumWriteValueLength(for: .withResponse)
            log.info("NUS ready")
            // Request config and image list once on connect
            // ESP32 will push CONFIG: updates proactively after any changes
            requestConfig()
            DispatchQueue.main.asyncAfter(deadline: .now() + 0.1) { [weak self] in
                self?.requestImageList()
            }
        }
    }

    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
        guard characteristic.uuid == nusTxUUID, let data = characteristic.value else { return }
        DispatchQueue.main.async {
            self.handleNotification(data)
        }
    }
}
