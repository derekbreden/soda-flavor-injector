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

// ────────────────────────────────────────────────────────────
// BLEManager — @Observable so SwiftUI only re-renders views
// that read the specific property that changed.
// ────────────────────────────────────────────────────────────

@Observable
class BLEManager {
    var connectionState: ConnectionState = .bluetoothOff

    // Config state (synced from ESP32 via S3 bridge)
    var configSynced = false
    var flavor1Image: Int = 0
    var flavor2Image: Int = 1
    var flavor1Ratio: Int = 20
    var flavor2Ratio: Int = 20
    var numImages: Int = 0

    // Image list and cached images
    var imageNames: [String] = []
    var cachedImages: [Int: UIImage] = [:]
    var imageDownloadProgress: Double? = nil  // nil = not downloading

    // Upload state
    var uploadProgress: Double? = nil  // nil = not uploading
    var uploadStatus: String = ""

    // Firmware versions (populated by GET_VERSION response)
    var s3Version: String = ""
    var espVersion: String = ""
    var rpVersion: String = ""

    // Factory reset completion signal (toggled on OK:FACTORY_RESET)
    var factoryResetCompleted = false

    // Delete error (shown as alert in UI, nil = no error)
    var deleteError: String? = nil

    // Demo mode (no hardware needed)
    var demoMode = false

    // Usage statistics
    struct FlavorStats {
        var todayFlowSum: UInt32 = 0
        var todayFlowCount: UInt32 = 0
        var todayBurstSum: UInt32 = 0
        var todayBurstCount: UInt32 = 0
        var weekFlowSum: UInt32 = 0
        var weekFlowCount: UInt32 = 0
        var weekBurstSum: UInt32 = 0
        var weekBurstCount: UInt32 = 0
        var monthFlowSum: UInt32 = 0
        var monthFlowCount: UInt32 = 0
        var monthBurstSum: UInt32 = 0
        var monthBurstCount: UInt32 = 0
    }
    var flavor1Stats = FlavorStats()
    var flavor2Stats = FlavorStats()
    var statsSynced = false

    // ── Internal state (not observed by SwiftUI) ──

    // All @ObservationIgnored properties below are fileprivate so
    // the CBDelegateAdapter (same file) can access them directly.

    @ObservationIgnored fileprivate var pendingImageList: [String] = []

    // Pending delete state (for optimistic UI rollback)
    @ObservationIgnored fileprivate var pendingDeleteSlot: Int = -1
    @ObservationIgnored fileprivate var preDeleteNumImages: Int = 0
    @ObservationIgnored fileprivate var preDeleteCachedImages: [Int: UIImage] = [:]
    @ObservationIgnored fileprivate var preDeleteImageNames: [String] = []

    // Image upload state
    @ObservationIgnored fileprivate var isUploading = false
    @ObservationIgnored fileprivate var uploadSlot: Int = -1
    @ObservationIgnored fileprivate var uploadLabel: String = ""
    @ObservationIgnored fileprivate var uploadSteps: [(type: String, data: Data)] = []
    @ObservationIgnored fileprivate var currentUploadStep = 0
    @ObservationIgnored fileprivate var uploadBytesSent = 0

    // Image download state — accessed from bleQueue during downloads
    @ObservationIgnored fileprivate var imgDownloadSlot: Int = -1
    @ObservationIgnored fileprivate var imgDownloadData = Data()
    @ObservationIgnored fileprivate var imgDownloadExpected: Int = 0
    @ObservationIgnored fileprivate var imgDownloadQueue: [Int] = []
    @ObservationIgnored fileprivate var isDownloading = false
    @ObservationIgnored fileprivate var nusReady = false

    // BLE runs on a dedicated background queue so binary data accumulation
    // and BLE writes don't block the main thread during image downloads.
    @ObservationIgnored let bleQueue = DispatchQueue(label: "com.derekbreden.SodaMachine.BLE", qos: .userInitiated)
    @ObservationIgnored fileprivate var cbAdapter: CBDelegateAdapter!
    @ObservationIgnored fileprivate var centralManager: CBCentralManager!
    @ObservationIgnored fileprivate var connectedPeripheral: CBPeripheral?
    @ObservationIgnored fileprivate var rxCharacteristic: CBCharacteristic?
    @ObservationIgnored fileprivate var txCharacteristic: CBCharacteristic?
    @ObservationIgnored fileprivate var scanTimer: Timer?
    @ObservationIgnored fileprivate var reconnectTimer: Timer?

    init() {
        cbAdapter = CBDelegateAdapter(self)
        centralManager = CBCentralManager(delegate: cbAdapter, queue: bleQueue)
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
            log.debug("TX: \(text)")
        }
    }

    func requestConfig() {
        if demoMode { return }
        send("GET_CONFIG")
    }

    func requestImageList() {
        if demoMode { return }
        pendingImageList = []
        send("LIST")
    }

    func requestVersions() {
        if demoMode { return }
        s3Version = ""
        espVersion = ""
        rpVersion = ""
        send("GET_VERSION")
    }

    func requestStats() {
        if demoMode {
            populateDemoStats()
            return
        }
        statsSynced = false
        send("GET_STATS")
    }

    func factoryReset() {
        if demoMode {
            flavor1Image = 0
            flavor2Image = 1
            flavor1Ratio = 20
            flavor2Ratio = 20
            numImages = 3
            imageNames = ["diet_wild_cherry_pepsi", "diet_mtn_dew", "diet_coke"]
            cachedImages = [
                0: generateDemoImage(label: "Cherry Pepsi", color: UIColor(red: 0.7, green: 0.1, blue: 0.15, alpha: 1)),
                1: generateDemoImage(label: "Mtn Dew", color: UIColor(red: 0.2, green: 0.6, blue: 0.2, alpha: 1)),
                2: generateDemoImage(label: "Diet Coke", color: UIColor(red: 0.4, green: 0.4, blue: 0.45, alpha: 1))
            ]
            factoryResetCompleted = true
            return
        }
        send("FACTORY_RESET")
    }

    func sendSet(_ key: String, value: Int) {
        send("SET:\(key)=\(value)")
        bleQueue.asyncAfter(deadline: .now() + 0.05) { [weak self] in
            self?.send("SAVE")
        }
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
        if demoMode {
            uploadDemoImage(image, toSlot: slot)
            return
        }
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
        if demoMode {
            deleteDemoImage(slot: slot)
            return
        }

        // Save state for rollback on error
        pendingDeleteSlot = slot
        preDeleteNumImages = numImages
        preDeleteCachedImages = cachedImages
        preDeleteImageNames = imageNames

        // Optimistic UI: remove immediately
        numImages -= 1
        // Shift cached images above deleted slot down
        var newCache: [Int: UIImage] = [:]
        for (key, img) in cachedImages {
            if key < slot {
                newCache[key] = img
            } else if key > slot {
                newCache[key - 1] = img
            }
        }
        cachedImages = newCache
        if slot < imageNames.count {
            imageNames.remove(at: slot)
        }

        send("DELETE_STORE_IMG:\(slot)")
    }

    func downloadAllImages() {
        guard !isDownloading else { return }
        let queue = Array(0..<numImages).filter { cachedImages[$0] == nil }
        if queue.isEmpty { return }
        isDownloading = true
        imageDownloadProgress = 0
        bleQueue.async {
            self.imgDownloadQueue = queue
            self.startNextDownload()
        }
    }

    // MARK: - Demo mode

    func enterDemoMode() {
        centralManager.stopScan()
        scanTimer?.invalidate()
        reconnectTimer?.invalidate()
        demoMode = true
        connectionState = .connected
        configSynced = true
        flavor1Image = 0
        flavor2Image = 1
        flavor1Ratio = 20
        flavor2Ratio = 20
        numImages = 3
        imageNames = ["diet_wild_cherry_pepsi", "diet_mtn_dew", "diet_coke"]
        cachedImages = [
            0: generateDemoImage(label: "Cherry Pepsi", color: UIColor(red: 0.7, green: 0.1, blue: 0.15, alpha: 1)),
            1: generateDemoImage(label: "Mtn Dew", color: UIColor(red: 0.2, green: 0.6, blue: 0.2, alpha: 1)),
            2: generateDemoImage(label: "Diet Coke", color: UIColor(red: 0.4, green: 0.4, blue: 0.45, alpha: 1))
        ]
        s3Version = "Demo"
        espVersion = "Demo"
        rpVersion = "Demo"
    }

    func exitDemoMode() {
        demoMode = false
        configSynced = false
        cachedImages = [:]
        imageNames = []
        numImages = 0
        s3Version = ""
        espVersion = ""
        rpVersion = ""
        if centralManager.state == .poweredOn {
            startScan()
        } else {
            connectionState = .bluetoothOff
        }
    }

    private func generateDemoImage(label: String, color: UIColor) -> UIImage {
        let size = CGSize(width: 240, height: 240)
        let renderer = UIGraphicsImageRenderer(size: size)
        return renderer.image { ctx in
            color.setFill()
            ctx.cgContext.fillEllipse(in: CGRect(origin: .zero, size: size))
            let attrs: [NSAttributedString.Key: Any] = [
                .font: UIFont.systemFont(ofSize: 24, weight: .bold),
                .foregroundColor: UIColor.white
            ]
            let text = label as NSString
            let textSize = text.size(withAttributes: attrs)
            let textRect = CGRect(
                x: (size.width - textSize.width) / 2,
                y: (size.height - textSize.height) / 2,
                width: textSize.width,
                height: textSize.height
            )
            text.draw(in: textRect, withAttributes: attrs)
        }
    }

    private func uploadDemoImage(_ image: UIImage, toSlot slot: Int) {
        uploadProgress = 0
        uploadStatus = "Uploading image..."
        var step = 0
        let totalSteps = 20
        Timer.scheduledTimer(withTimeInterval: 0.1, repeats: true) { [weak self] timer in
            guard let self, self.demoMode else { timer.invalidate(); return }
            step += 1
            self.uploadProgress = Double(step) / Double(totalSteps)
            if step >= totalSteps {
                timer.invalidate()
                self.cachedImages[slot] = image
                if slot >= self.numImages {
                    self.numImages = slot + 1
                    self.imageNames.append("image_\(slot)")
                }
                self.uploadProgress = nil
                self.uploadStatus = ""
            }
        }
    }

    private func populateDemoStats() {
        flavor1Stats = FlavorStats(
            todayFlowSum: 2400, todayFlowCount: 800,
            todayBurstSum: 12000, todayBurstCount: 80,
            weekFlowSum: 15000, weekFlowCount: 5000,
            weekBurstSum: 75000, weekBurstCount: 500,
            monthFlowSum: 60000, monthFlowCount: 20000,
            monthBurstSum: 300000, monthBurstCount: 2000
        )
        flavor2Stats = FlavorStats(
            todayFlowSum: 1800, todayFlowCount: 600,
            todayBurstSum: 9000, todayBurstCount: 60,
            weekFlowSum: 10000, weekFlowCount: 3500,
            weekBurstSum: 50000, weekBurstCount: 350,
            monthFlowSum: 40000, monthFlowCount: 14000,
            monthBurstSum: 200000, monthBurstCount: 1400
        )
        statsSynced = true
    }

    private func deleteDemoImage(slot: Int) {
        guard numImages > 1 else { return }

        var newNames: [String] = []
        var newCache: [Int: UIImage] = [:]
        var j = 0
        for i in 0..<numImages where i != slot {
            if i < imageNames.count { newNames.append(imageNames[i]) }
            if let img = cachedImages[i] { newCache[j] = img }
            j += 1
        }

        numImages -= 1
        imageNames = newNames
        cachedImages = newCache

        if flavor1Image > slot { flavor1Image -= 1 }
        else if flavor1Image == slot { flavor1Image = max(0, slot - 1) }
        if flavor2Image > slot { flavor2Image -= 1 }
        else if flavor2Image == slot { flavor2Image = max(0, slot - 1) }
    }

    // MARK: - Image upload

    private func sendNextUploadStep() {
        guard currentUploadStep < uploadSteps.count else {
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
    }

    private func sendUploadChunks() {
        guard currentUploadStep < uploadSteps.count else { return }
        let data = uploadSteps[currentUploadStep].data
        guard let rx = rxCharacteristic, let p = connectedPeripheral else { return }
        let mtu = p.maximumWriteValueLength(for: .withResponse)
        let chunkSize = min(mtu, 240)

        func sendChunk() {
            self.bleQueue.async {
                guard self.uploadBytesSent < data.count, self.isUploading else { return }
                let end = min(self.uploadBytesSent + chunkSize, data.count)
                let chunk = data[self.uploadBytesSent..<end]
                p.writeValue(chunk, for: rx, type: .withResponse)
                self.uploadBytesSent = end

                DispatchQueue.main.async {
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

    fileprivate func startNextDownload() {
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
        guard let rx = rxCharacteristic, let p = connectedPeripheral,
              let data = "GETPNG:\(slot)".data(using: .utf8) else { return }
        p.writeValue(data, for: rx, type: .withResponse)
    }

    // MARK: - Response parsing (main thread only)

    fileprivate func handleTextResponse(_ text: String) {
        if text.hasPrefix("CONFIG:") {
            parseConfig(text)
        } else if text.hasPrefix("IMG:") {
            parseImageLine(text)
        } else if text == "END" {
            imageNames = pendingImageList
            pendingImageList = []
            downloadAllImages()
        } else if text == "UPLOAD_READY" {
            sendUploadChunks()
        } else if text.hasPrefix("IMG_OK:") {
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
            pendingDeleteSlot = -1
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
        } else if text.hasPrefix("VERSION:S3=") {
            s3Version = String(text.dropFirst(11))
        } else if text.hasPrefix("VERSION:ESP32=") {
            espVersion = String(text.dropFirst(14))
        } else if text.hasPrefix("VERSION:RP2040=") {
            rpVersion = String(text.dropFirst(15))
        } else if text.hasPrefix("STATS:") {
            parseStats(text)
        } else if text == "OK:TIME_SYNCED" {
            log.info("Time synced with ESP32")
        } else if text == "OK:FACTORY_RESET" {
            log.info("Factory reset confirmed, re-syncing")
            cachedImages = [:]
            imageNames = []
            statsSynced = false
            factoryResetCompleted = true
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
            } else if pendingDeleteSlot >= 0 {
                // Roll back optimistic delete
                numImages = preDeleteNumImages
                cachedImages = preDeleteCachedImages
                imageNames = preDeleteImageNames
                pendingDeleteSlot = -1
                deleteError = String(text.dropFirst(4)) // strip "ERR:" prefix
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

    private func parseStats(_ text: String) {
        // STATS:F=0,TD_FS=1234,TD_FC=567,...
        let body = String(text.dropFirst(6)) // drop "STATS:"
        var values: [String: UInt32] = [:]
        for pair in body.split(separator: ",") {
            let parts = pair.split(separator: "=", maxSplits: 1)
            if parts.count == 2, let val = UInt32(parts[1]) {
                values[String(parts[0])] = val
            }
        }

        let flavor = values["F"] ?? 0
        let stats = FlavorStats(
            todayFlowSum: values["TD_FS"] ?? 0,
            todayFlowCount: values["TD_FC"] ?? 0,
            todayBurstSum: values["TD_BS"] ?? 0,
            todayBurstCount: values["TD_BC"] ?? 0,
            weekFlowSum: values["7D_FS"] ?? 0,
            weekFlowCount: values["7D_FC"] ?? 0,
            weekBurstSum: values["7D_BS"] ?? 0,
            weekBurstCount: values["7D_BC"] ?? 0,
            monthFlowSum: values["30D_FS"] ?? 0,
            monthFlowCount: values["30D_FC"] ?? 0,
            monthBurstSum: values["30D_BS"] ?? 0,
            monthBurstCount: values["30D_BC"] ?? 0
        )

        if flavor == 0 {
            flavor1Stats = stats
        } else {
            flavor2Stats = stats
            statsSynced = true  // both flavors received
        }
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

    fileprivate func startScan() {
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

        DispatchQueue.main.async {
            self.scanTimer?.invalidate()
            self.scanTimer = Timer.scheduledTimer(withTimeInterval: scanTimeout, repeats: false) { [weak self] _ in
                guard let self, self.connectionState == .searching else { return }
                self.connectionState = .searchingLong
                log.info("Still scanning, showing hints")
            }
        }
    }

    fileprivate func scheduleReconnect() {
        DispatchQueue.main.async {
            self.reconnectTimer?.invalidate()
            self.reconnectTimer = Timer.scheduledTimer(withTimeInterval: 2, repeats: false) { [weak self] _ in
                self?.startScan()
            }
        }
    }
}

// ────────────────────────────────────────────────────────────
// CBDelegateAdapter — thin NSObject that implements CoreBluetooth
// delegates and forwards to BLEManager. Required because @Observable
// classes cannot inherit from NSObject.
// ────────────────────────────────────────────────────────────

private class CBDelegateAdapter: NSObject, CBCentralManagerDelegate, CBPeripheralDelegate {
    unowned let ble: BLEManager

    init(_ ble: BLEManager) {
        self.ble = ble
        super.init()
    }

    // MARK: - CBCentralManagerDelegate

    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        log.debug("Central state: \(central.state.rawValue)")
        if central.state == .poweredOn {
            ble.startScan()
        } else {
            DispatchQueue.main.async {
                self.ble.connectionState = .bluetoothOff
            }
        }
    }

    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral,
                         advertisementData: [String: Any], rssi RSSI: NSNumber) {
        let name = peripheral.name ?? advertisementData[CBAdvertisementDataLocalNameKey] as? String ?? "Unknown"
        log.info("Found: \(name) (RSSI: \(RSSI.intValue))")

        DispatchQueue.main.async { self.ble.scanTimer?.invalidate() }
        central.stopScan()
        DispatchQueue.main.async { self.ble.connectionState = .connecting }
        ble.connectedPeripheral = peripheral
        central.connect(peripheral, options: nil)
    }

    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        log.info("Connected to \(peripheral.name ?? "device")")
        peripheral.delegate = self
        peripheral.discoverServices([nusServiceUUID])
    }

    func centralManager(_ central: CBCentralManager, didFailToConnect peripheral: CBPeripheral, error: Error?) {
        log.error("Connection failed: \(error?.localizedDescription ?? "unknown")")
        ble.connectedPeripheral = nil
        ble.scheduleReconnect()
    }

    func centralManager(_ central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: Error?) {
        if ble.demoMode { return }
        log.info("Disconnected")
        ble.imgDownloadSlot = -1
        ble.connectedPeripheral = nil
        ble.rxCharacteristic = nil
        ble.nusReady = false
        DispatchQueue.main.async {
            self.ble.connectionState = .searching
            self.ble.configSynced = false
            self.ble.statsSynced = false
            self.ble.imgDownloadQueue = []
            self.ble.isDownloading = false
            self.ble.imageDownloadProgress = nil
            self.ble.isUploading = false
            self.ble.uploadProgress = nil
            self.ble.uploadSteps = []
            if self.ble.pendingDeleteSlot >= 0 {
                self.ble.numImages = self.ble.preDeleteNumImages
                self.ble.cachedImages = self.ble.preDeleteCachedImages
                self.ble.imageNames = self.ble.preDeleteImageNames
                self.ble.pendingDeleteSlot = -1
            }
            self.ble.scheduleReconnect()
        }
    }

    // MARK: - CBPeripheralDelegate

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
                peripheral.setNotifyValue(true, for: char)
                log.debug("Subscribed to TX notifications")
            } else if char.uuid == nusRxUUID {
                ble.rxCharacteristic = char
                log.debug("Found RX characteristic")
            }
        }
        if ble.rxCharacteristic != nil && !ble.nusReady {
            ble.nusReady = true
            DispatchQueue.main.async { self.ble.connectionState = .connected }
            peripheral.maximumWriteValueLength(for: .withResponse)
            log.info("NUS ready")
            let epoch = Int(Date().timeIntervalSince1970)
            ble.send("SET_TIME:\(epoch)")
            ble.bleQueue.asyncAfter(deadline: .now() + 0.05) { [weak ble] in
                ble?.send("GET_CONFIG")
            }
            ble.bleQueue.asyncAfter(deadline: .now() + 0.1) { [weak ble] in
                ble?.send("LIST")
            }
        }
    }

    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
        guard characteristic.uuid == nusTxUUID, let data = characteristic.value else { return }
        // This runs on bleQueue.

        // Fast path: binary image data — stays entirely on bleQueue
        if ble.imgDownloadSlot >= 0 {
            if data.count < 20, let text = String(data: data, encoding: .utf8), text == "IMGEND" {
                let imgData = ble.imgDownloadData
                let slot = ble.imgDownloadSlot
                ble.imgDownloadSlot = -1
                ble.imgDownloadData = Data()
                let image = UIImage(data: imgData)
                DispatchQueue.main.async {
                    if let image {
                        self.ble.cachedImages[slot] = image
                        log.info("Image \(slot) cached (\(imgData.count) bytes)")
                    } else {
                        log.error("Image \(slot) decode failed: \(imgData.count) bytes")
                    }
                }
                ble.startNextDownload()
            } else {
                ble.imgDownloadData.append(data)
            }
            return
        }

        // IMGSTART: parse on bleQueue before binary chunks arrive
        if let text = String(data: data, encoding: .utf8), text.hasPrefix("IMGSTART:") {
            let parts = text.split(separator: ":")
            if parts.count >= 3, let slot = Int(parts[1]), let size = Int(parts[2]) {
                ble.imgDownloadSlot = slot
                ble.imgDownloadExpected = size
                ble.imgDownloadData = Data()
                log.info("Starting image download: slot \(slot), \(size) bytes")
            }
            return
        }

        // Filter out DBG: lines (firmware debug output)
        if let text = String(data: data, encoding: .utf8), text.hasPrefix("DBG:") {
            return
        }

        // All other text: dispatch to main for observable property updates
        DispatchQueue.main.async {
            if let text = String(data: data, encoding: .utf8) {
                self.ble.handleTextResponse(text)
            }
        }
    }
}
