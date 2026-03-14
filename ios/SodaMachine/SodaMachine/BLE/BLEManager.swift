import Foundation
import CoreBluetooth
import os

/// Nordic UART Service UUIDs
private let nusServiceUUID = CBUUID(string: "6E400001-B5A3-F393-E0A9-E50E24DCCA9E")
private let nusRxUUID       = CBUUID(string: "6E400002-B5A3-F393-E0A9-E50E24DCCA9E") // phone writes here
private let nusTxUUID       = CBUUID(string: "6E400003-B5A3-F393-E0A9-E50E24DCCA9E") // phone subscribes here

private let log = Logger(subsystem: "com.derekbreden.SodaMachine", category: "BLE")

enum BLEState: Equatable {
    case idle
    case scanning
    case connecting
    case connected
    case disconnected
}

struct DiscoveredDevice: Identifiable {
    let id: UUID
    let name: String
    let peripheral: CBPeripheral
    var rssi: Int
}

class BLEManager: NSObject, ObservableObject {
    @Published var state: BLEState = .idle
    @Published var discoveredDevices: [DiscoveredDevice] = []
    @Published var lastResponse: String = ""

    private var centralManager: CBCentralManager!
    private var connectedPeripheral: CBPeripheral?
    private var rxCharacteristic: CBCharacteristic?
    private var txCharacteristic: CBCharacteristic?

    override init() {
        super.init()
        centralManager = CBCentralManager(delegate: self, queue: nil)
    }

    func startScan() {
        guard centralManager.state == .poweredOn else { return }
        discoveredDevices = []
        state = .scanning
        centralManager.scanForPeripherals(withServices: [nusServiceUUID], options: [
            CBCentralManagerScanOptionAllowDuplicatesKey: false
        ])
        log.info("Scanning for NUS peripherals...")
    }

    func stopScan() {
        centralManager.stopScan()
        if state == .scanning { state = .idle }
    }

    func connect(to device: DiscoveredDevice) {
        stopScan()
        state = .connecting
        connectedPeripheral = device.peripheral
        centralManager.connect(device.peripheral, options: nil)
        log.info("Connecting to \(device.name)...")
    }

    func disconnect() {
        if let p = connectedPeripheral {
            centralManager.cancelPeripheralConnection(p)
        }
    }

    func send(_ text: String) {
        guard let rx = rxCharacteristic, let p = connectedPeripheral else {
            log.warning("Cannot send: not connected")
            return
        }
        guard let data = text.data(using: .utf8) else { return }
        p.writeValue(data, for: rx, type: .withResponse)
        log.info("TX: \(text)")
    }
}

// MARK: - CBCentralManagerDelegate

extension BLEManager: CBCentralManagerDelegate {
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        log.info("Central state: \(central.state.rawValue)")
        if central.state == .poweredOn && state == .idle {
            // Ready to scan
        }
    }

    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral,
                         advertisementData: [String: Any], rssi RSSI: NSNumber) {
        let name = peripheral.name ?? advertisementData[CBAdvertisementDataLocalNameKey] as? String ?? "Unknown"
        if let idx = discoveredDevices.firstIndex(where: { $0.id == peripheral.identifier }) {
            discoveredDevices[idx].rssi = RSSI.intValue
        } else {
            let device = DiscoveredDevice(
                id: peripheral.identifier,
                name: name,
                peripheral: peripheral,
                rssi: RSSI.intValue
            )
            discoveredDevices.append(device)
            log.info("Discovered: \(name) (RSSI: \(RSSI.intValue))")
        }
    }

    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        log.info("Connected to \(peripheral.name ?? "device")")
        peripheral.delegate = self
        peripheral.discoverServices([nusServiceUUID])
    }

    func centralManager(_ central: CBCentralManager, didFailToConnect peripheral: CBPeripheral, error: Error?) {
        log.error("Connection failed: \(error?.localizedDescription ?? "unknown")")
        state = .disconnected
        connectedPeripheral = nil
    }

    func centralManager(_ central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: Error?) {
        log.info("Disconnected")
        state = .disconnected
        connectedPeripheral = nil
        rxCharacteristic = nil
        txCharacteristic = nil
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
        if rxCharacteristic != nil && txCharacteristic != nil {
            state = .connected
            // Request higher MTU
            peripheral.maximumWriteValueLength(for: .withResponse)
            log.info("NUS ready")
        }
    }

    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
        guard characteristic.uuid == nusTxUUID, let data = characteristic.value else { return }
        if let text = String(data: data, encoding: .utf8) {
            log.info("RX: \(text)")
            DispatchQueue.main.async {
                self.lastResponse = text
            }
        }
    }
}
