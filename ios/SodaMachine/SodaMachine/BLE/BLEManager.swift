import Foundation
import CoreBluetooth
import UIKit
import SwiftUI
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
    var uploadQueue: [UploadQueueItem] = []
    var activeUploadImage: UIImage? = nil
    var activeUploadSlot: Int = -1

    struct UploadQueueItem: Identifiable {
        let id = UUID()
        let image: UIImage
        let slot: Int
    }

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

    // Chart data (populated by GET_CHART_DATA response)
    var chartData24H: [[Double]] = [Array(repeating: 0, count: 24), Array(repeating: 0, count: 24)]
    var chartData30D: [[Double]] = [Array(repeating: 0, count: 30), Array(repeating: 0, count: 30)]
    var chartDataHOD: [[Double]] = [Array(repeating: 0, count: 24), Array(repeating: 0, count: 24)]
    var chartDataHODDays: Int = 1
    var chartDataSynced: Bool = false

    @ObservationIgnored fileprivate var chartLinesReceived: Int = 0

    // Live chart baselines (for computing delta from CHART_LIVE pushes)
    @ObservationIgnored fileprivate var chartBaseFlowSum: [UInt32] = [0, 0]
    @ObservationIgnored fileprivate var chartBase24H_last: [Double] = [0, 0]
    @ObservationIgnored fileprivate var chartBase30D_last: [Double] = [0, 0]
    @ObservationIgnored fileprivate var chartBaseHOD_slot: [Double] = [0, 0]
    @ObservationIgnored fileprivate var chartBaseHOD_hour: Int = Calendar.current.component(.hour, from: Date())
    @ObservationIgnored fileprivate var lastLiveFS: [UInt32] = [0, 0]

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
    @ObservationIgnored fileprivate var uploadQueueTotal = 0
    @ObservationIgnored fileprivate var uploadImageRef: UIImage?

    // Image download state — accessed from bleQueue during downloads
    @ObservationIgnored fileprivate var imgDownloadSlot: Int = -1
    @ObservationIgnored fileprivate var imgDownloadData = Data()
    @ObservationIgnored fileprivate var imgDownloadExpected: Int = 0
    @ObservationIgnored fileprivate var imgDownloadCRC: UInt32 = 0
    @ObservationIgnored fileprivate var imgDownloadRetries: Int = 0
    @ObservationIgnored fileprivate var imgDownloadQueue: [Int] = []
    @ObservationIgnored fileprivate var isDownloading = false
    @ObservationIgnored fileprivate var binStartReceived = false
    @ObservationIgnored fileprivate var pendingStatsRequest = false
    @ObservationIgnored fileprivate var timeSyncReceived = false
    @ObservationIgnored fileprivate var nusReady = false

    // BLE frame buffer — accumulates partial frames from notifications
    @ObservationIgnored fileprivate var frameBuffer = Data()

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

    /// Send a text command to the S3 via BLE, wrapped in a TEXT frame.
    /// Always dispatched to bleQueue to avoid cross-queue blocking.
    func send(_ text: String) {
        bleQueue.async { [weak self] in
            guard let self, let rx = self.rxCharacteristic, let p = self.connectedPeripheral else {
                log.warning("Cannot send: not connected")
                return
            }
            guard let payload = text.data(using: .utf8) else { return }
            var frame = Data([0x01, UInt8(payload.count & 0xFF), UInt8((payload.count >> 8) & 0xFF)])
            frame.append(payload)
            p.writeValue(frame, for: rx, type: .withResponse)
            log.debug("TX: \(text)")
        }
    }

    /// Send a raw BLE frame on bleQueue. Caller must already be on bleQueue.
    fileprivate func sendBLEFrame(type: UInt8, payload: Data) {
        guard let rx = rxCharacteristic, let p = connectedPeripheral else { return }
        var frame = Data([type, UInt8(payload.count & 0xFF), UInt8((payload.count >> 8) & 0xFF)])
        frame.append(payload)
        p.writeValue(frame, for: rx, type: .withResponse)
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

    func requestChartData() {
        if demoMode {
            populateDemoChartData()
            return
        }
        chartDataSynced = false
        chartLinesReceived = 0
        send("GET_CHART_DATA")
    }

    func requestStatsAndCharts() {
        if demoMode {
            populateDemoStats()
            populateDemoChartData()
            return
        }
        statsSynced = false
        chartDataSynced = false
        chartLinesReceived = 0
        // Defer until image downloads finish — responses would be
        // swallowed by the binary image accumulator otherwise.
        if isDownloading {
            pendingStatsRequest = true
            return
        }
        sendStatsCommands()
    }

    func subscribeStats() {
        if demoMode { return }
        send("STATS_SUBSCRIBE")
    }

    func unsubscribeStats() {
        if demoMode { return }
        send("STATS_UNSUBSCRIBE")
    }

    fileprivate func sendStatsCommands() {
        pendingStatsRequest = false
        send("GET_STATS")
        send("GET_CHART_DATA")
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

    func queueUploads(_ items: [UploadQueueItem]) {
        uploadQueue.append(contentsOf: items)
        uploadQueueTotal = uploadQueue.count + (isUploading ? 1 : 0)
        if !isUploading {
            startNextUpload()
        }
    }

    func cancelQueuedUpload(id: UUID) {
        uploadQueue.removeAll { $0.id == id }
    }

    private func startNextUpload() {
        guard !uploadQueue.isEmpty else {
            uploadQueueTotal = 0
            activeUploadImage = nil
            activeUploadSlot = -1
            cachedImages = [:]
            requestImageList()
            return
        }
        let item = uploadQueue.removeFirst()
        uploadImageRef = item.image
        activeUploadImage = item.image
        activeUploadSlot = item.slot
        let position = uploadQueueTotal - uploadQueue.count
        uploadStatus = "Uploading \(position) of \(uploadQueueTotal)..."
        uploadImage(item.image, toSlot: item.slot)
    }

    private func uploadImage(_ image: UIImage, toSlot slot: Int) {
        if demoMode {
            uploadDemoImage(image, toSlot: slot)
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
        isUploading = true
        uploadSlot = slot
        uploadImageRef = image
        uploadProgress = 0
        var step = 0
        let totalSteps = 20
        Timer.scheduledTimer(withTimeInterval: 0.1, repeats: true) { [weak self] timer in
            guard let self, self.demoMode else { timer.invalidate(); return }
            step += 1
            self.uploadProgress = Double(step) / Double(totalSteps)
            if step >= totalSteps {
                timer.invalidate()
                self.imageNames.append("image_\(slot)")
                self.completeUpload()
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

    private func populateDemoChartData() {
        // HOD: sine wave peaking at noon (hour 12)
        var hod0 = [Double](repeating: 0, count: 24)
        var hod1 = [Double](repeating: 0, count: 24)
        for h in 0..<24 {
            let angle = Double(h - 12) / 12.0 * .pi
            let base = max(0, cos(angle)) * 8.0
            hod0[h] = base * 1.2
            hod1[h] = base * 0.8
        }
        chartDataHOD = [hod0, hod1]
        chartDataHODDays = 14

        // 24H: based on HOD pattern with some noise
        let calendar = Calendar.current
        let currentHour = calendar.component(.hour, from: Date())
        var h24_0 = [Double](repeating: 0, count: 24)
        var h24_1 = [Double](repeating: 0, count: 24)
        for i in 0..<24 {
            let hour = (currentHour - 23 + i + 24) % 24
            let angle = Double(hour - 12) / 12.0 * .pi
            let base = max(0, cos(angle))
            h24_0[i] = base * 10.0 * (0.7 + Double.random(in: 0...0.6))
            h24_1[i] = base * 7.0 * (0.7 + Double.random(in: 0...0.6))
        }
        chartData24H = [h24_0, h24_1]

        // 30D: gradual ramp-up with variation
        var d30_0 = [Double](repeating: 0, count: 30)
        var d30_1 = [Double](repeating: 0, count: 30)
        for i in 0..<30 {
            let ramp = Double(i + 1) / 30.0
            d30_0[i] = ramp * 60.0 * (0.5 + Double.random(in: 0...1.0))
            d30_1[i] = ramp * 40.0 * (0.5 + Double.random(in: 0...1.0))
        }
        chartData30D = [d30_0, d30_1]

        chartDataSynced = true
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

        let fileType: UInt8 = step.type == "png" ? 0 : (step.type == "s3" ? 1 : 2)

        // Build BIN_START payload: [slot(1B), fileType(1B), size(4B LE), crc32(4B LE), label...]
        var startPayload = Data(capacity: 10 + 32)
        startPayload.append(UInt8(uploadSlot))
        startPayload.append(fileType)
        var sizeLE = UInt32(step.data.count).littleEndian
        withUnsafeBytes(of: &sizeLE) { startPayload.append(contentsOf: $0) }
        var crcLE = crc.littleEndian
        withUnsafeBytes(of: &crcLE) { startPayload.append(contentsOf: $0) }
        if currentUploadStep == 0, let labelData = uploadLabel.data(using: .utf8) {
            startPayload.append(labelData)
        }

        uploadBytesSent = 0

        bleQueue.async { [weak self] in
            guard let self else { return }
            self.sendBLEFrame(type: 0x02, payload: startPayload)
            self.bleQueue.asyncAfter(deadline: .now() + 0.05) {
                self.sendUploadChunks()
            }
        }
    }

    private func sendUploadChunks() {
        guard currentUploadStep < uploadSteps.count else { return }
        let data = uploadSteps[currentUploadStep].data
        guard let rx = rxCharacteristic, let p = connectedPeripheral else { return }
        let mtu = p.maximumWriteValueLength(for: .withResponse)
        let chunkSize = min(mtu - 3, 240)  // leave room for frame header

        func sendChunk() {
            self.bleQueue.async {
                guard self.uploadBytesSent < data.count, self.isUploading else { return }
                let end = min(self.uploadBytesSent + chunkSize, data.count)
                let chunk = data[self.uploadBytesSent..<end]

                // Build BIN_DATA frame
                var frame = Data([0x03, UInt8(chunk.count & 0xFF), UInt8((chunk.count >> 8) & 0xFF)])
                frame.append(chunk)
                p.writeValue(frame, for: rx, type: .withResponse)
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
                } else {
                    // All data sent, send BIN_END frame
                    self.bleQueue.asyncAfter(deadline: .now() + 0.02) {
                        self.sendBLEFrame(type: 0x04, payload: Data())
                    }
                }
            }
        }
        sendChunk()
    }

    private func completeUpload() {
        isUploading = false
        uploadSteps = []
        numImages = max(numImages, uploadSlot + 1)
        if let img = uploadImageRef { cachedImages[uploadSlot] = img }
        uploadImageRef = nil

        if !uploadQueue.isEmpty {
            let position = uploadQueueTotal - uploadQueue.count + 1
            uploadStatus = "Uploading \(position) of \(uploadQueueTotal)..."
            uploadProgress = 0
            DispatchQueue.main.asyncAfter(deadline: .now() + 0.3) { [weak self] in
                self?.startNextUpload()
            }
        } else {
            uploadProgress = 1.0
            uploadStatus = "Upload complete!"
            uploadQueueTotal = 0
            activeUploadImage = nil
            activeUploadSlot = -1
            DispatchQueue.main.asyncAfter(deadline: .now() + 0.5) { [weak self] in
                self?.uploadProgress = nil
                self?.cachedImages = [:]
                self?.requestImageList()
            }
        }
    }

    private func failUpload(_ reason: String) {
        log.error("Upload failed (slot \(self.uploadSlot)): \(reason)")
        isUploading = false
        uploadSteps = []
        uploadImageRef = nil

        if !uploadQueue.isEmpty {
            uploadStatus = "Slot \(self.uploadSlot) failed, continuing..."
            DispatchQueue.main.asyncAfter(deadline: .now() + 0.5) { [weak self] in
                self?.startNextUpload()
            }
        } else {
            uploadStatus = "Upload failed: \(reason)"
            uploadProgress = nil
            uploadQueueTotal = 0
            activeUploadImage = nil
            activeUploadSlot = -1
        }
    }

    // MARK: - Image download (runs entirely on bleQueue)

    fileprivate func startNextDownload() {
        guard !imgDownloadQueue.isEmpty else {
            DispatchQueue.main.async {
                self.imageDownloadProgress = nil
                self.isDownloading = false
                if self.pendingStatsRequest {
                    self.sendStatsCommands()
                }
            }
            return
        }
        let slot = imgDownloadQueue.removeFirst()
        imgDownloadData = Data()
        imgDownloadExpected = 0
        imgDownloadRetries = 0
        guard let rx = rxCharacteristic, let p = connectedPeripheral,
              let payload = "GETPNG:\(slot)".data(using: .utf8) else { return }
        // Send as TEXT frame
        var frame = Data([0x01, UInt8(payload.count & 0xFF), UInt8((payload.count >> 8) & 0xFF)])
        frame.append(payload)
        p.writeValue(frame, for: rx, type: .withResponse)
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
        } else if text.hasPrefix("IMG_OK:") {
            currentUploadStep += 1
            let stepNames = ["PNG", "S3 RGB565", "RP2040 RGB565"]
            if currentUploadStep < uploadSteps.count {
                uploadStatus = "Uploading \(stepNames[currentUploadStep])..."
            }
            sendNextUploadStep()
        } else if text.hasPrefix("IMG_ERR:") {
            guard isUploading else {
                log.debug("Ignoring IMG_ERR (not uploading): \(text)")
                return
            }
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
        } else if text.hasPrefix("CHART_") {
            parseChartLine(text)
        } else if text == "OK:TIME_SYNCED" {
            log.info("Time synced with ESP32")
            if !timeSyncReceived {
                timeSyncReceived = true
                // Stats requested before time sync completed get zeroed responses.
                // Re-request now that the ESP32 has real data.
                if statsSynced || chartDataSynced || pendingStatsRequest {
                    log.info("Re-requesting stats after time sync")
                    sendStatsCommands()
                }
            }
        } else if text == "OK:FACTORY_RESET" {
            log.info("Factory reset confirmed, re-syncing")
            cachedImages = [:]
            imageNames = []
            statsSynced = false
            chartDataSynced = false
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

        withAnimation {
            if flavor == 0 {
                flavor1Stats = stats
            } else {
                flavor2Stats = stats
                statsSynced = true  // both flavors received
            }
        }
    }

    private func parseChartLine(_ text: String) {
        guard let firstColon = text.firstIndex(of: ":") else { return }
        let prefix = String(text[text.startIndex..<firstColon])
        let body = String(text[text.index(after: firstColon)...])
        let parts = body.split(separator: ",")
        guard parts.count >= 2 else { return }

        // Parse key=value pairs at start
        var flavor = 0
        var kvValues: [String: String] = [:]
        var dataStart = 0
        for (i, part) in parts.enumerated() {
            let kv = part.split(separator: "=", maxSplits: 1)
            if kv.count == 2 {
                kvValues[String(kv[0])] = String(kv[1])
                if kv[0] == "F" { flavor = Int(kv[1]) ?? 0 }
                else if kv[0] == "D" { chartDataHODDays = max(Int(kv[1]) ?? 1, 1) }
                dataStart = i + 1
            } else {
                break
            }
        }
        guard flavor >= 0, flavor <= 1 else { return }

        // CHART_CUR: store baseline flow_sum and snapshot chart slot values
        if prefix == "CHART_CUR" {
            if let fsStr = kvValues["FS"], let fs = UInt32(fsStr) {
                chartBaseFlowSum[flavor] = fs
                chartBase24H_last[flavor] = chartData24H[flavor][23]
                chartBase30D_last[flavor] = chartData30D[flavor][29]
                let curHour = Calendar.current.component(.hour, from: Date())
                chartBaseHOD_hour = curHour
                chartBaseHOD_slot[flavor] = chartDataHOD[flavor][curHour]
                lastLiveFS[flavor] = fs
            }
            chartLinesReceived += 1
            if chartLinesReceived >= 8 {
                chartDataSynced = true
            }
            return
        }

        // CHART_LIVE: push update — apply delta to live slots
        // Reassign full arrays to ensure @Observable triggers SwiftUI re-render
        if prefix == "CHART_LIVE" {
            if let fsStr = kvValues["FS"], let newFS = UInt32(fsStr) {
                let delta = Double(newFS - chartBaseFlowSum[flavor]) * 0.05

                var new24H = chartData24H
                new24H[flavor][23] = chartBase24H_last[flavor] + delta

                var new30D = chartData30D
                new30D[flavor][29] = chartBase30D_last[flavor] + delta

                var newHOD = chartDataHOD
                newHOD[flavor][chartBaseHOD_hour] = chartBaseHOD_slot[flavor] + delta

                // Incremental flow delta for stats (pie chart)
                let incr = newFS - lastLiveFS[flavor]
                lastLiveFS[flavor] = newFS

                withAnimation {
                    chartData24H = new24H
                    chartData30D = new30D
                    chartDataHOD = newHOD
                    if flavor == 0 {
                        flavor1Stats.todayFlowSum += incr
                        flavor1Stats.weekFlowSum += incr
                        flavor1Stats.monthFlowSum += incr
                    } else {
                        flavor2Stats.todayFlowSum += incr
                        flavor2Stats.weekFlowSum += incr
                        flavor2Stats.monthFlowSum += incr
                    }
                }
            }
            return
        }

        // CHART_24H / CHART_30D / CHART_HOD: full array data
        let values = parts[dataStart...].map { Double(UInt32($0) ?? 0) * 0.05 }

        withAnimation {
            if prefix == "CHART_24H" && values.count == 24 {
                chartData24H[flavor] = values
            } else if prefix == "CHART_30D" && values.count == 30 {
                chartData30D[flavor] = values
            } else if prefix == "CHART_HOD" && values.count == 24 {
                chartDataHOD[flavor] = values
            }
        }

        chartLinesReceived += 1
        if chartLinesReceived >= 8 {
            chartDataSynced = true
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
        ble.binStartReceived = false
        ble.connectedPeripheral = nil
        ble.rxCharacteristic = nil
        ble.nusReady = false
        ble.frameBuffer = Data()
        ble.timeSyncReceived = false
        DispatchQueue.main.async {
            self.ble.connectionState = .searching
            self.ble.configSynced = false
            self.ble.statsSynced = false
            self.ble.chartDataSynced = false
            self.ble.imgDownloadQueue = []
            self.ble.isDownloading = false
            self.ble.imageDownloadProgress = nil
            self.ble.isUploading = false
            self.ble.uploadProgress = nil
            self.ble.uploadSteps = []
            self.ble.uploadQueue = []
            self.ble.uploadQueueTotal = 0
            self.ble.uploadImageRef = nil
            self.ble.activeUploadImage = nil
            self.ble.activeUploadSlot = -1
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
        // This runs on bleQueue. Parse framed protocol.

        ble.frameBuffer.append(data)

        while ble.frameBuffer.count >= 3 {
            let si = ble.frameBuffer.startIndex
            let type = ble.frameBuffer[si]
            let len = Int(ble.frameBuffer[si + 1]) | (Int(ble.frameBuffer[si + 2]) << 8)
            let frameLen = 3 + len
            guard ble.frameBuffer.count >= frameLen else { break }  // wait for more data
            let payload = Data(ble.frameBuffer[(si + 3)..<(si + frameLen)])
            ble.frameBuffer.removeFirst(frameLen)

            switch type {
            case 0x01:  // TEXT
                guard let text = String(data: payload, encoding: .utf8) else { continue }
                if text.hasPrefix("DBG:") { continue }
                DispatchQueue.main.async {
                    self.ble.handleTextResponse(text)
                }

            case 0x02:  // BIN_START
                guard ble.isDownloading else {
                    log.debug("Ignoring unsolicited BIN_START")
                    continue
                }
                guard payload.count >= 10 else { continue }
                let slot = Int(payload[0])
                // payload[1] = file_type (0=png, 1=s3_rgb)
                let size = UInt32(payload[2]) | (UInt32(payload[3]) << 8) |
                           (UInt32(payload[4]) << 16) | (UInt32(payload[5]) << 24)
                let crc = UInt32(payload[6]) | (UInt32(payload[7]) << 8) |
                          (UInt32(payload[8]) << 16) | (UInt32(payload[9]) << 24)
                ble.imgDownloadSlot = slot
                ble.imgDownloadExpected = Int(size)
                ble.imgDownloadCRC = crc
                ble.imgDownloadData = Data()
                ble.binStartReceived = true
                log.info("BIN_START: slot \(slot), \(size) bytes, CRC=0x\(String(crc, radix: 16))")

            case 0x03:  // BIN_DATA
                guard ble.binStartReceived else { continue }
                ble.imgDownloadData.append(payload)

            case 0x04:  // BIN_END
                guard ble.binStartReceived else { continue }
                ble.binStartReceived = false
                let imgData = ble.imgDownloadData
                let slot = ble.imgDownloadSlot
                let expectedSize = ble.imgDownloadExpected
                let expectedCRC = ble.imgDownloadCRC
                ble.imgDownloadSlot = -1
                ble.imgDownloadData = Data()

                // Verify size
                if imgData.count != expectedSize {
                    log.error("Image \(slot) size mismatch: got \(imgData.count) expected \(expectedSize)")
                    retryDownload(slot: slot)
                    continue
                }

                // Verify CRC-32
                let actualCRC = ImageProcessor.crc32(imgData)
                if actualCRC != expectedCRC {
                    log.error("Image \(slot) CRC mismatch: got 0x\(String(actualCRC, radix: 16)) expected 0x\(String(expectedCRC, radix: 16))")
                    retryDownload(slot: slot)
                    continue
                }

                ble.imgDownloadRetries = 0
                let image = UIImage(data: imgData)
                DispatchQueue.main.async {
                    if let image {
                        self.ble.cachedImages[slot] = image
                        log.info("Image \(slot) cached (\(imgData.count) bytes, CRC verified)")
                    } else {
                        log.error("Image \(slot) decode failed: \(imgData.count) bytes")
                    }
                }
                ble.startNextDownload()

            default:
                break
            }
        }
    }

    private func retryDownload(slot: Int) {
        self.ble.imgDownloadRetries += 1
        if self.ble.imgDownloadRetries <= 3 {
            log.info("Retrying image \(slot) download (attempt \(self.ble.imgDownloadRetries))")
            guard let rx = self.ble.rxCharacteristic, let p = self.ble.connectedPeripheral,
                  let payload = "GETPNG:\(slot)".data(using: .utf8) else { return }
            var frame = Data([0x01, UInt8(payload.count & 0xFF), UInt8((payload.count >> 8) & 0xFF)])
            frame.append(payload)
            p.writeValue(frame, for: rx, type: .withResponse)
        } else {
            log.error("Image \(slot) download failed after 3 retries")
            self.ble.imgDownloadRetries = 0
            self.ble.startNextDownload()
        }
    }
}
