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

    // Upload state
    @Published var uploadProgress: Double? = nil  // nil = not uploading
    @Published var uploadStatus: String = ""

    private var pendingImageList: [String] = []

    // Image upload state
    private var isUploading = false
    private var uploadSlot: Int = -1
    private var uploadLabel: String = ""
    private var uploadSteps: [(type: String, data: Data)] = []
    private var currentUploadStep = 0
    private var uploadBytesSent = 0

    // Image download state — accessed from bleQueue during downloads
    private var imgDownloadSlot: Int = -1
    private var imgDownloadData = Data()
    private var imgDownloadExpected: Int = 0
    private var imgDownloadQueue: [Int] = []
    private var isDownloading = false
    private var nusReady = false

    // BLE runs on a dedicated background queue so binary data accumulation
    // and BLE writes don't block the main thread during image downloads.
    private let bleQueue = DispatchQueue(label: "com.derekbreden.SodaMachine.BLE", qos: .userInitiated)
    private var centralManager: CBCentralManager!
    private var connectedPeripheral: CBPeripheral?
    private var rxCharacteristic: CBCharacteristic?
    private var txCharacteristic: CBCharacteristic?
    private var scanTimer: Timer?
    private var reconnectTimer: Timer?

    override init() {
        super.init()
        centralManager = CBCentralManager(delegate: self, queue: bleQueue)
    }

    // MARK: - Public API

    /// Send a text command to the S3 via BLE. Always dispatched to bleQueue
    /// to avoid cross-queue blocking with CoreBluetooth's internal synchronization.
    func send(_ text: String) {
        bleQueue.async { [weak self] in
            guard let self, let rx = self.rxCharacteristic, let p = self.connectedPeripheral else {
                log.warning("Cannot send: not connected")
                return
            }
            guard let data = text.data(using: .utf8) else { return }
            p.writeValue(data, for: rx, type: .withResponse)
            log.info("TX: \(text)")
        }
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
        bleQueue.asyncAfter(deadline: .now() + 0.05) { [weak self] in
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

    func uploadImage(_ image: UIImage, toSlot slot: Int) {
        guard !isUploading else {
            log.warning("Upload already in progress")
            return
        }
        guard let pngData = ImageProcessor.generatePNG(from: image),
              let s3Data = ImageProcessor.generateRGB565(from: image, width: 240, height: 240),
              let rpData = ImageProcessor.generateRGB565(from: image, width: 128, height: 115) else {
            uploadStatus = "Image processing failed"
            return
        }

        let label = "image_\(slot)"
        log.info("Upload: slot \(slot), png=\(pngData.count)B, s3=\(s3Data.count)B, rp=\(rpData.count)B")

        isUploading = true
        uploadSlot = slot
        uploadLabel = label
        uploadSteps = [
            (type: "png", data: pngData),
            (type: "s3",  data: s3Data),
            (type: "rp",  data: rpData)
        ]
        currentUploadStep = 0
        uploadBytesSent = 0
        uploadProgress = 0
        uploadStatus = "Uploading image..."

        sendNextUploadStep()
    }

    func deleteImage(slot: Int) {
        send("DELETE_STORE_IMG:\(slot)")
        // Response "OK:STORE_DELETED=..." triggers cache clear + list refresh in handleTextResponse
    }

    func downloadAllImages() {
        guard !isDownloading else { return }
        let queue = Array(0..<numImages).filter { cachedImages[$0] == nil }
        if queue.isEmpty { return }
        isDownloading = true
        imageDownloadProgress = 0  // signal download active (set once)
        // Hand off to bleQueue for the entire download loop
        bleQueue.async {
            self.imgDownloadQueue = queue
            self.startNextDownload()
        }
    }



    // MARK: - Image upload

    private func sendNextUploadStep() {
        guard currentUploadStep < uploadSteps.count else {
            // All 3 transfers done — send FINALIZE_UPLOAD
            uploadStatus = "Finalizing..."
            send("FINALIZE_UPLOAD:\(uploadSlot):\(uploadLabel)")
            return
        }

        let step = uploadSteps[currentUploadStep]
        let crc = ImageProcessor.crc32(step.data)
        var header = "IMG_UPLOAD:slot=\(uploadSlot),type=\(step.type),size=\(step.data.count),crc32=0x\(String(crc, radix: 16))"
        if currentUploadStep == 0 {
            header += ",label=\(uploadLabel)"
        }

        uploadBytesSent = 0
        send(header)
        // Wait for UPLOAD_READY before sending data
    }

    private func sendUploadChunks() {
        guard currentUploadStep < uploadSteps.count else { return }
        let data = uploadSteps[currentUploadStep].data
        guard let rx = rxCharacteristic, let p = connectedPeripheral else { return }
        let mtu = p.maximumWriteValueLength(for: .withResponse)
        let chunkSize = min(mtu, 240)

        // Send upload chunks on bleQueue to avoid blocking main
        func sendChunk() {
            self.bleQueue.async {
                guard self.uploadBytesSent < data.count, self.isUploading else { return }
                let end = min(self.uploadBytesSent + chunkSize, data.count)
                let chunk = data[self.uploadBytesSent..<end]
                p.writeValue(chunk, for: rx, type: .withResponse)
                self.uploadBytesSent = end

                DispatchQueue.main.async {
                    // Update progress: 3 steps, each ~33%
                    let stepBase = Double(self.currentUploadStep) / 3.0
                    let stepProgress = Double(self.uploadBytesSent) / Double(data.count) / 3.0
                    self.uploadProgress = stepBase + stepProgress
                }

                if self.uploadBytesSent < data.count {
                    self.bleQueue.asyncAfter(deadline: .now() + 0.02) {
                        sendChunk()
                    }
                }
            }
        }
        sendChunk()
    }

    private func completeUpload() {
        uploadProgress = 1.0
        uploadStatus = "Upload complete!"
        isUploading = false
        uploadSteps = []
        DispatchQueue.main.asyncAfter(deadline: .now() + 0.5) { [weak self] in
            self?.uploadProgress = nil
            self?.cachedImages = [:]
            self?.requestImageList()
        }
    }

    private func failUpload(_ reason: String) {
        log.error("Upload failed: \(reason)")
        uploadStatus = "Upload failed: \(reason)"
        uploadProgress = nil
        isUploading = false
        uploadSteps = []
    }

    // MARK: - Image download (runs entirely on bleQueue)

    /// Called on bleQueue to start the next image download.
    private func startNextDownload() {
        // Already on bleQueue
        guard !imgDownloadQueue.isEmpty else {
            DispatchQueue.main.async {
                self.imageDownloadProgress = nil
                self.isDownloading = false
            }
            return
        }
        let slot = imgDownloadQueue.removeFirst()
        imgDownloadData = Data()
        imgDownloadExpected = 0
        // Write directly on bleQueue — no cross-queue synchronization
        guard let rx = rxCharacteristic, let p = connectedPeripheral,
              let data = "GETPNG:\(slot)".data(using: .utf8) else { return }
        p.writeValue(data, for: rx, type: .withResponse)
        log.info("TX: GETPNG:\(slot)")
    }

    // MARK: - Response parsing (main thread only)

    private func handleTextResponse(_ text: String) {
        if text.hasPrefix("CONFIG:") {
            parseConfig(text)
        } else if text.hasPrefix("IMG:") {
            parseImageLine(text)
        } else if text == "END" {
            imageNames = pendingImageList
            pendingImageList = []
            // Auto-download images after getting list
            downloadAllImages()
        } else if text == "UPLOAD_READY" {
            sendUploadChunks()
        } else if text.hasPrefix("IMG_OK:") {
            // One upload transfer complete — advance
            currentUploadStep += 1
            let stepNames = ["PNG", "S3 RGB565", "RP2040 RGB565"]
            if currentUploadStep < uploadSteps.count {
                uploadStatus = "Uploading \(stepNames[currentUploadStep])..."
            }
            sendNextUploadStep()
        } else if text.hasPrefix("IMG_ERR:") {
            failUpload(text)
        } else if text.hasPrefix("OK:UPLOAD_DONE:") {
            completeUpload()
        } else if text.hasPrefix("OK:STORE_DELETED=") {
            // OK:STORE_DELETED=3,NUM_IMAGES=3 — update state and refresh
            let body = String(text.dropFirst(3))
            for pair in body.split(separator: ",") {
                let parts = pair.split(separator: "=", maxSplits: 1)
                if parts.count == 2, let val = Int(parts[1]), String(parts[0]) == "NUM_IMAGES" {
                    numImages = max(val, 1)
                }
            }
            cachedImages = [:]
            requestImageList()
            log.info("Image deleted, refreshing list")
        } else if text == "OK:FACTORY_RESET" {
            // Factory reset completed — clear everything and re-sync
            log.info("Factory reset confirmed, re-syncing")
            cachedImages = [:]
            imageNames = []
            requestConfig()
            DispatchQueue.main.asyncAfter(deadline: .now() + 0.1) { [weak self] in
                self?.requestImageList()
            }
        } else if text.hasPrefix("ERR:") {
            log.error("Error response: \(text)")
            if isUploading {
                failUpload(text)
            } else if isDownloading {
                log.error("Skipping failed image download, continuing queue")
                bleQueue.async { self.startNextDownload() }
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
        log.info("Config synced: F1=\(self.flavor1Image)/\(self.flavor1Ratio) F2=\(self.flavor2Image)/\(self.flavor2Ratio) numImages=\(self.numImages)")
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
        DispatchQueue.main.async {
            self.connectionState = .searching
            self.configSynced = false
            self.cachedImages = [:]
        }
        centralManager.scanForPeripherals(withServices: [nusServiceUUID], options: [
            CBCentralManagerScanOptionAllowDuplicatesKey: false
        ])
        log.info("Auto-scanning for Soda Machine...")

        // After timeout, show hints but keep scanning
        DispatchQueue.main.async {
            self.scanTimer?.invalidate()
            self.scanTimer = Timer.scheduledTimer(withTimeInterval: scanTimeout, repeats: false) { [weak self] _ in
                guard let self, self.connectionState == .searching else { return }
                self.connectionState = .searchingLong
                log.info("Still scanning, showing hints")
            }
        }
    }

    private func scheduleReconnect() {
        DispatchQueue.main.async {
            self.reconnectTimer?.invalidate()
            self.reconnectTimer = Timer.scheduledTimer(withTimeInterval: 2, repeats: false) { [weak self] _ in
                self?.startScan()
            }
        }
    }
}

// MARK: - CBCentralManagerDelegate
// All delegate methods run on bleQueue. Dispatch to main for @Published updates.

extension BLEManager: CBCentralManagerDelegate {
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        log.info("Central state: \(central.state.rawValue)")
        if central.state == .poweredOn {
            startScan()
        } else {
            DispatchQueue.main.async {
                self.connectionState = .bluetoothOff
            }
        }
    }

    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral,
                         advertisementData: [String: Any], rssi RSSI: NSNumber) {
        let name = peripheral.name ?? advertisementData[CBAdvertisementDataLocalNameKey] as? String ?? "Unknown"
        log.info("Found: \(name) (RSSI: \(RSSI.intValue))")

        DispatchQueue.main.async {
            self.scanTimer?.invalidate()
        }
        centralManager.stopScan()
        DispatchQueue.main.async {
            self.connectionState = .connecting
        }
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
        imgDownloadSlot = -1  // stop binary routing on bleQueue
        connectedPeripheral = nil
        rxCharacteristic = nil
        txCharacteristic = nil
        nusReady = false
        DispatchQueue.main.async {
            self.connectionState = .searching
            self.configSynced = false
            self.imgDownloadQueue = []
            self.isDownloading = false
            self.imageDownloadProgress = nil
            self.isUploading = false
            self.uploadProgress = nil
            self.uploadSteps = []
            self.scheduleReconnect()
        }
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
            DispatchQueue.main.async {
                self.connectionState = .connected
            }
            peripheral.maximumWriteValueLength(for: .withResponse)
            log.info("NUS ready")
            // Request config and image list once on connect
            send("GET_CONFIG")
            bleQueue.asyncAfter(deadline: .now() + 0.1) { [weak self] in
                self?.send("LIST")
            }
        }
    }

    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
        guard characteristic.uuid == nusTxUUID, let data = characteristic.value else { return }
        // This runs on bleQueue.

        // Fast path: binary image data — stays entirely on bleQueue, never touches main
        if imgDownloadSlot >= 0 {
            if data.count < 20, let text = String(data: data, encoding: .utf8), text == "IMGEND" {
                let imgData = self.imgDownloadData
                let slot = self.imgDownloadSlot
                self.imgDownloadSlot = -1
                self.imgDownloadData = Data()
                // Decode image on bleQueue to keep main free
                let image = UIImage(data: imgData)
                // Brief main dispatch for @Published update only
                DispatchQueue.main.async {
                    if let image {
                        self.cachedImages[slot] = image
                        log.info("Image \(slot) cached (\(imgData.count) bytes)")
                    } else {
                        log.error("Image \(slot) decode failed: \(imgData.count) bytes")
                    }
                }
                // Continue download loop on bleQueue (no main involvement)
                self.startNextDownload()
            } else {
                imgDownloadData.append(data)
            }
            return
        }

        // IMGSTART: parse on bleQueue to set imgDownloadSlot before binary chunks arrive
        if let text = String(data: data, encoding: .utf8), text.hasPrefix("IMGSTART:") {
            let parts = text.split(separator: ":")
            if parts.count >= 3, let slot = Int(parts[1]), let size = Int(parts[2]) {
                imgDownloadSlot = slot
                imgDownloadExpected = size
                imgDownloadData = Data()
                log.info("Starting image download: slot \(slot), \(size) bytes")
            }
            return
        }

        // Filter out DBG: lines — informational only, no main dispatch needed
        if let text = String(data: data, encoding: .utf8), text.hasPrefix("DBG:") {
            log.info("RX: \(text)")
            return
        }

        // All other text responses: dispatch to main for @Published updates
        DispatchQueue.main.async {
            if let text = String(data: data, encoding: .utf8) {
                log.info("RX: \(text)")
                self.handleTextResponse(text)
            }
        }
    }
}
