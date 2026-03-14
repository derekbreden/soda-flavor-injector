import SwiftUI

struct ScanView: View {
    @EnvironmentObject var ble: BLEManager

    var body: some View {
        Group {
            switch ble.connectionState {
            case .connected:
                ConfigView()
            case .notFound:
                notFoundView
            default:
                searchingView
            }
        }
    }

    // MARK: - Searching

    private var searchingView: some View {
        ZStack {
            Color.black.ignoresSafeArea()

            VStack(spacing: 16) {
                ProgressView()
                    .scaleEffect(1.5)
                    .tint(.white)
                    .padding(.bottom, 8)

                if ble.connectionState == .bluetoothOff {
                    Text("Turn on Bluetooth")
                        .font(.title3.weight(.medium))
                        .foregroundStyle(.white)
                    Text("Bluetooth is required to connect to your Soda Machine.")
                        .font(.callout)
                        .foregroundStyle(.gray)
                        .multilineTextAlignment(.center)
                } else if ble.connectionState == .connecting {
                    Text("Connecting...")
                        .font(.title3.weight(.medium))
                        .foregroundStyle(.white)
                } else {
                    Text("Searching for Soda Machine...")
                        .font(.title3.weight(.medium))
                        .foregroundStyle(.white)
                }
            }
            .padding(.horizontal, 32)
        }
    }

    // MARK: - Not Found

    private var notFoundView: some View {
        ZStack {
            Color.black.ignoresSafeArea()

            VStack(spacing: 16) {
                Image(systemName: "exclamationmark.triangle")
                    .font(.system(size: 48))
                    .foregroundStyle(.orange)
                    .padding(.bottom, 8)

                Text("Soda Machine Not Found")
                    .font(.title3.weight(.medium))
                    .foregroundStyle(.white)

                VStack(alignment: .leading, spacing: 8) {
                    Label("Get closer to the Soda Machine", systemImage: "figure.walk")
                    Label("Make sure the Soda Machine is powered on", systemImage: "power")
                    Label("Try rebooting the Soda Machine", systemImage: "arrow.clockwise")
                }
                .font(.callout)
                .foregroundStyle(.gray)

                Button("Try Again") {
                    ble.retry()
                }
                .buttonStyle(.borderedProminent)
                .padding(.top, 8)
            }
            .padding(.horizontal, 32)
        }
    }
}
