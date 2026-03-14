import SwiftUI

struct ScanView: View {
    @EnvironmentObject var ble: BLEManager
    @State private var textToSend = ""

    var body: some View {
        NavigationStack {
            Group {
                switch ble.connectionState {
                case .connected:
                    connectedView
                case .notFound:
                    notFoundView
                default:
                    searchingView
                }
            }
            .navigationTitle("Soda Machine")
        }
    }

    // MARK: - Searching

    private var searchingView: some View {
        VStack(spacing: 16) {
            Spacer()

            ProgressView()
                .scaleEffect(1.5)
                .padding(.bottom, 8)

            if ble.connectionState == .bluetoothOff {
                Text("Turn on Bluetooth")
                    .font(.title3.weight(.medium))
                Text("Bluetooth is required to connect to your Soda Machine.")
                    .font(.callout)
                    .foregroundStyle(.secondary)
                    .multilineTextAlignment(.center)
            } else if ble.connectionState == .connecting {
                Text("Connecting...")
                    .font(.title3.weight(.medium))
            } else {
                Text("Searching for Soda Machine...")
                    .font(.title3.weight(.medium))
            }

            Spacer()
        }
        .padding(.horizontal, 32)
    }

    // MARK: - Not Found

    private var notFoundView: some View {
        VStack(spacing: 16) {
            Spacer()

            Image(systemName: "exclamationmark.triangle")
                .font(.system(size: 48))
                .foregroundStyle(.orange)
                .padding(.bottom, 8)

            Text("Soda Machine Not Found")
                .font(.title3.weight(.medium))

            VStack(alignment: .leading, spacing: 8) {
                Label("Get closer to the Soda Machine", systemImage: "figure.walk")
                Label("Make sure the Soda Machine is powered on", systemImage: "power")
                Label("Try rebooting the Soda Machine", systemImage: "arrow.clockwise")
            }
            .font(.callout)
            .foregroundStyle(.secondary)

            Button("Try Again") {
                ble.retry()
            }
            .buttonStyle(.borderedProminent)
            .padding(.top, 8)

            Spacer()
        }
        .padding(.horizontal, 32)
    }

    // MARK: - Connected

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

            // Echo test (temporary — will be replaced with config UI)
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
        }
    }
}
