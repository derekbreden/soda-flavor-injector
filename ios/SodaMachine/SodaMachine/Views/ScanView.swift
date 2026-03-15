import SwiftUI

struct ScanView: View {
    @Environment(BLEManager.self) var ble

    var body: some View {
        Group {
            switch ble.connectionState {
            case .connected:
                ConfigView()
            default:
                searchingView
            }
        }
    }

    private var searchingView: some View {
        ZStack {
            Theme.background.ignoresSafeArea()

            VStack(spacing: 16) {
                Spacer()

                ProgressView()
                    .scaleEffect(1.5)
                    .tint(Theme.textPrimary)
                    .padding(.bottom, 8)

                if ble.connectionState == .bluetoothOff {
                    Text("Turn on Bluetooth")
                        .font(.system(size: 16, weight: .medium))
                        .foregroundStyle(Theme.textPrimary)
                    Text("Bluetooth is required to connect to your Soda Machine.")
                        .font(.system(size: 16))
                        .foregroundStyle(Theme.textSecondary)
                        .multilineTextAlignment(.center)
                } else if ble.connectionState == .connecting {
                    Text("Connecting...")
                        .font(.system(size: 16, weight: .medium))
                        .foregroundStyle(Theme.textPrimary)
                } else if ble.connectionState == .searchingLong {
                    Text("Searching for Soda Machine...")
                        .font(.system(size: 16, weight: .medium))
                        .foregroundStyle(Theme.textPrimary)
                        .padding(.bottom, 8)

                    VStack(alignment: .leading, spacing: 8) {
                        Label("Get closer to the Soda Machine", systemImage: "figure.walk")
                        Label("Make sure it is powered on", systemImage: "power")
                        Label("Try rebooting the Soda Machine", systemImage: "arrow.clockwise")
                    }
                    .font(.system(size: 16))
                    .foregroundStyle(Theme.textSecondary)
                } else {
                    Text("Searching for Soda Machine...")
                        .font(.system(size: 16, weight: .medium))
                        .foregroundStyle(Theme.textPrimary)
                }

                Spacer()

                Button("Try Demo Mode") {
                    ble.enterDemoMode()
                }
                .font(.system(size: 14))
                .foregroundStyle(Theme.textSecondary)
                .padding(.bottom, 20)
            }
            .padding(.horizontal, 32)
        }
    }
}
