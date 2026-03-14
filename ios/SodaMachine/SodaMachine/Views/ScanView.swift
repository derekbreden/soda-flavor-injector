import SwiftUI

struct ScanView: View {
    @EnvironmentObject var ble: BLEManager
    @State private var textToSend = ""

    var body: some View {
        NavigationStack {
            Group {
                switch ble.state {
                case .connected:
                    connectedView
                default:
                    scanningView
                }
            }
            .navigationTitle("Soda Machine")
        }
    }

    // MARK: - Scanning / Device List

    private var scanningView: some View {
        VStack(spacing: 20) {
            if ble.state == .connecting {
                ProgressView("Connecting...")
            } else {
                if ble.discoveredDevices.isEmpty && ble.state == .scanning {
                    ProgressView("Scanning...")
                        .padding(.top, 40)
                }

                List(ble.discoveredDevices) { device in
                    Button {
                        ble.connect(to: device)
                    } label: {
                        HStack {
                            VStack(alignment: .leading) {
                                Text(device.name)
                                    .font(.headline)
                                Text("RSSI: \(device.rssi) dBm")
                                    .font(.caption)
                                    .foregroundStyle(.secondary)
                            }
                            Spacer()
                            Image(systemName: "chevron.right")
                                .foregroundStyle(.secondary)
                        }
                    }
                }
            }
        }
        .toolbar {
            ToolbarItem(placement: .primaryAction) {
                Button(ble.state == .scanning ? "Stop" : "Scan") {
                    if ble.state == .scanning {
                        ble.stopScan()
                    } else {
                        ble.startScan()
                    }
                }
            }
        }
        .onAppear {
            if ble.state == .idle {
                // Small delay to let CBCentralManager power on
                DispatchQueue.main.asyncAfter(deadline: .now() + 0.5) {
                    ble.startScan()
                }
            }
        }
    }

    // MARK: - Connected / Echo Test

    private var connectedView: some View {
        VStack(spacing: 24) {
            HStack {
                Circle()
                    .fill(.green)
                    .frame(width: 10, height: 10)
                Text("Connected")
                    .font(.headline)
            }
            .padding(.top, 20)

            // Echo test
            VStack(alignment: .leading, spacing: 8) {
                Text("Echo Test")
                    .font(.subheadline)
                    .foregroundStyle(.secondary)

                HStack {
                    TextField("Type a message...", text: $textToSend)
                        .textFieldStyle(.roundedBorder)
                        .autocorrectionDisabled()

                    Button("Send") {
                        guard !textToSend.isEmpty else { return }
                        ble.send(textToSend)
                    }
                    .buttonStyle(.borderedProminent)
                }

                if !ble.lastResponse.isEmpty {
                    HStack {
                        Text("Response:")
                            .foregroundStyle(.secondary)
                        Text(ble.lastResponse)
                            .font(.system(.callout, design: .monospaced))
                    }
                    .font(.callout)
                }
            }
            .padding(.horizontal)

            Spacer()

            Button("Disconnect", role: .destructive) {
                ble.disconnect()
            }
            .padding(.bottom, 20)
        }
    }
}
