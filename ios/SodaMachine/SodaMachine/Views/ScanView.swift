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
            Color.black.ignoresSafeArea()

            VStack(spacing: 16) {
                Spacer()

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
                } else if ble.connectionState == .searchingLong {
                    Text("Searching for Soda Machine...")
                        .font(.title3.weight(.medium))
                        .foregroundStyle(.white)
                        .padding(.bottom, 8)

                    VStack(alignment: .leading, spacing: 8) {
                        Label("Get closer to the Soda Machine", systemImage: "figure.walk")
                        Label("Make sure it is powered on", systemImage: "power")
                        Label("Try rebooting the Soda Machine", systemImage: "arrow.clockwise")
                    }
                    .font(.callout)
                    .foregroundStyle(.gray)
                } else {
                    Text("Searching for Soda Machine...")
                        .font(.title3.weight(.medium))
                        .foregroundStyle(.white)
                }

                Spacer()
            }
            .padding(.horizontal, 32)
        }
    }
}
