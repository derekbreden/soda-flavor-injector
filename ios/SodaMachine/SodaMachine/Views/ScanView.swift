import SwiftUI

struct ScanView: View {
    @Environment(BLEManager.self) var ble
    @State private var searchTextIndex = 0

    private let searchMessages = [
        "Searching for Soda Machine...",
        "Looking for your device...",
    ]

    var body: some View {
        Group {
            if ble.readyToShow {
                ConfigView()
            } else {
                animatedSearchView
            }
        }
    }

    private var animatedSearchView: some View {
        ZStack {
            Theme.background.ignoresSafeArea()

            // Glass animation — centered in full screen (not safe area)
            // to match LaunchScreen.storyboard's centerX + centerY constraints
            GeometryReader { geo in
                GlassAnimationView()
                    .frame(width: 200, height: 200)
                    .position(x: geo.size.width / 2, y: geo.size.height / 2)
            }
            .ignoresSafeArea()

            // Status text + demo button pinned to bottom
            VStack {
                Spacer()

                statusContent
                    .frame(height: 100)
                    .padding(.bottom, 16)

                Button("Try Demo Mode") {
                    ble.enterDemoMode()
                }
                .font(.system(size: 14))
                .foregroundStyle(Theme.textSecondary)

                Spacer().frame(height: 20)
            }
            .padding(.horizontal, 32)
        }
        .onReceive(Timer.publish(every: 3, on: .main, in: .common).autoconnect()) { _ in
            guard ble.connectionState == .searching else { return }
            searchTextIndex = (searchTextIndex + 1) % searchMessages.count
        }
    }

    @ViewBuilder
    private var statusContent: some View {
        if ble.connectionState == .bluetoothOff {
            VStack(spacing: 8) {
                Text("Turn on Bluetooth")
                    .font(.system(size: 16, weight: .medium))
                    .foregroundStyle(Theme.textSecondary)
                Text("Bluetooth is required to connect to your Soda Machine.")
                    .font(.system(size: 16))
                    .foregroundStyle(Theme.textSecondary)
                    .multilineTextAlignment(.center)
            }
        } else if ble.connectionState == .searchingLong {
            VStack(spacing: 8) {
                Text("Searching for Soda Machine...")
                    .font(.system(size: 16, weight: .medium))
                    .foregroundStyle(Theme.textSecondary)
                    .padding(.bottom, 4)
                VStack(alignment: .leading, spacing: 8) {
                    Label("Get closer to the Soda Machine", systemImage: "figure.walk")
                    Label("Make sure it is powered on", systemImage: "power")
                    Label("Try rebooting the Soda Machine", systemImage: "arrow.clockwise")
                }
                .font(.system(size: 16))
                .foregroundStyle(Theme.textSecondary)
            }
        } else if ble.connectionState == .connecting {
            Text("Connecting...")
                .font(.system(size: 16, weight: .medium))
                .foregroundStyle(Theme.textSecondary)
        } else {
            Text(searchMessages[searchTextIndex])
                .font(.system(size: 16, weight: .medium))
                .foregroundStyle(Theme.textSecondary)
        }
    }
}
