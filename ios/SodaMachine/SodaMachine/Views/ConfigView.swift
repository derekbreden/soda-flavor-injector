import SwiftUI

// ────────────────────────────────────────────────────────────
// Separate View structs create their own @Observable observation
// scopes. Changes to cachedImages / imageDownloadProgress only
// re-render these leaf views — NOT the parent ConfigView / TabView.
// This prevents image downloads from interrupting swipe gestures.
// ────────────────────────────────────────────────────────────

private struct ImageSlotView: View {
    @Environment(BLEManager.self) var ble
    let slot: Int
    let editing: Bool

    var body: some View {
        let size: CGFloat = editing ? 160 : 120
        Group {
            if let uiImage = ble.imageFor(slot: slot) {
                Image(uiImage: uiImage)
                    .resizable()
                    .scaledToFill()
                    .frame(width: size, height: size)
                    .clipShape(Circle())
            } else {
                ZStack {
                    Circle()
                        .fill(Theme.placeholder)
                        .frame(width: size, height: size)
                    if ble.imageDownloadProgress != nil {
                        ProgressView()
                            .tint(Theme.textPrimary)
                    } else {
                        Image(systemName: "photo")
                            .font(.system(size: 24))
                            .foregroundStyle(Theme.textSecondary)
                    }
                }
            }
        }
        .animation(.easeInOut(duration: 0.2), value: editing)
    }
}

// ────────────────────────────────────────────────────────────
// Settings page — shown inline on page 4 of the carousel.
// Tappable items with press-to-highlight feedback.
// ────────────────────────────────────────────────────────────

private struct SettingsItemButtonStyle: ButtonStyle {
    func makeBody(configuration: Configuration) -> some View {
        configuration.label
            .foregroundStyle(configuration.isPressed ? Theme.textPrimary : Theme.textSecondary)
    }
}

private struct SettingsPageView: View {
    @Environment(BLEManager.self) var ble
    @Binding var showImageManager: Bool
    @Binding var inAbout: Bool
    @State private var showResetAlert = false
    @State private var resetting = false

    var body: some View {
        VStack(spacing: 12) {
            Image(systemName: "gearshape")
                .font(.system(size: 36))
                .foregroundStyle(Theme.textSecondary)

            if resetting {
                ProgressView()
                    .tint(Theme.textPrimary)
                    .padding(.top, 8)
                Text("Resetting...")
                    .font(.system(size: 16))
                    .foregroundStyle(Theme.textSecondary)
            } else {
                VStack(spacing: 0) {
                    settingsButton("Manage Images") {
                        showImageManager = true
                    }
                    settingsButton("Factory Reset") {
                        showResetAlert = true
                    }
                    settingsButton("About") {
                        ble.requestVersions()
                        inAbout = true
                    }
                    if ble.demoMode {
                        settingsButton("Exit Demo") {
                            ble.exitDemoMode()
                        }
                    }
                }
            }
        }
        .alert("Factory Reset?", isPresented: $showResetAlert) {
            Button("Reset", role: .destructive) {
                resetting = true
                ble.factoryReset()
            }
            Button("Cancel", role: .cancel) {}
        } message: {
            Text("This will reset all settings to factory defaults.")
        }
        .onChange(of: ble.factoryResetCompleted) { _, completed in
            if completed {
                resetting = false
                ble.factoryResetCompleted = false
            }
        }
    }

    private func settingsButton(_ title: String, action: @escaping () -> Void) -> some View {
        Button(action: action) {
            Text(title)
                .font(.system(size: 16, weight: .medium))
                .frame(maxWidth: .infinity)
                .padding(.vertical, 12)
        }
        .buttonStyle(SettingsItemButtonStyle())
    }
}

// ────────────────────────────────────────────────────────────
// About screen (version display)
// ────────────────────────────────────────────────────────────

private struct AboutView: View {
    @Environment(BLEManager.self) var ble

    var body: some View {
        VStack(spacing: 0) {
            Spacer()

            Text("About")
                .font(.system(size: 16, weight: .medium))
                .foregroundStyle(Theme.textPrimary)

            Spacer().frame(height: 32)

            let labels = ["S3", "ESP32", "RP2040"]
            let versions = [ble.s3Version, ble.espVersion, ble.rpVersion]

            ForEach(0..<3, id: \.self) { i in
                VStack(spacing: 4) {
                    Text(labels[i])
                        .font(.system(size: 16))
                        .foregroundStyle(Theme.textSecondary)
                    Text(versions[i].isEmpty ? "..." : versions[i])
                        .font(.system(size: 16))
                        .foregroundStyle(Theme.textPrimary)
                }
                if i < 2 { Spacer().frame(height: 16) }
            }

            Spacer()
        }
    }
}

// ────────────────────────────────────────────────────────────

struct ConfigView: View {
    @Environment(BLEManager.self) var ble
    @State private var currentPage = 0
    @State private var editing = false
    @State private var showImageManager = false
    @State private var inAbout = false

    private let pageCount = 5
    private let pageLabels = ["Flavor 1 Image", "Flavor 1 Ratio", "Flavor 2 Image", "Flavor 2 Ratio", "Settings"]

    var body: some View {
        ZStack {
            Theme.background.ignoresSafeArea()

            if !ble.configSynced {
                VStack(spacing: 16) {
                    ProgressView()
                        .scaleEffect(1.5)
                        .tint(Theme.textPrimary)
                    Text("Loading configuration...")
                        .font(.system(size: 16, weight: .medium))
                        .foregroundStyle(Theme.textPrimary)
                }
            } else if inAbout {
                AboutView()
                    .contentShape(Rectangle())
                    .onTapGesture { inAbout = false }
            } else {
                carouselContent
            }
        }
        .sheet(isPresented: $showImageManager) {
            ImageManagerView()
                .environment(ble)
        }
        .onChange(of: ble.connectionState) { _, state in
            if state != .connected {
                inAbout = false
            }
        }
    }

    // MARK: - Carousel (main menu)

    private var carouselContent: some View {
        ZStack(alignment: .bottom) {
            TabView(selection: $currentPage) {
                ForEach(0..<pageCount, id: \.self) { i in
                    VStack {
                        Spacer()

                        if i < pageCount - 1 {
                            Text(pageLabels[i])
                                .font(.system(size: 16, weight: .medium))
                                .foregroundStyle(editing && i == currentPage ? Theme.textPrimary : Theme.textSecondary)

                            Spacer().frame(height: 12)
                        }

                        pageView(for: i)
                            .frame(height: 180)

                        Spacer()
                    }
                    .contentShape(Rectangle())
                    .onTapGesture {
                        // Pages 0-3 enter editing mode on tap.
                        // Page 4 (settings) handles its own taps
                        // via buttons, so no action needed here.
                        if i < 4 {
                            editing = true
                        }
                    }
                    .tag(i)
                }
            }
            .tabViewStyle(.page(indexDisplayMode: .never))
            .allowsHitTesting(!editing)
            .overlay {
                if editing {
                    Color.clear
                        .contentShape(Rectangle())
                        .onTapGesture {
                            editing = false
                            sendCurrentValue()
                        }
                        .gesture(
                            DragGesture(minimumDistance: 30)
                                .onEnded { value in
                                    let dx = value.translation.width
                                    if dx > 30 {
                                        adjustValue(by: -1)
                                    } else if dx < -30 {
                                        adjustValue(by: 1)
                                    }
                                }
                        )
                }
            }

            // Fixed nav dots
            HStack(spacing: 12) {
                ForEach(0..<pageCount, id: \.self) { j in
                    Circle()
                        .fill(j == currentPage ? Theme.dotActive : Theme.dotInactive)
                        .frame(width: 8, height: 8)
                }
            }
            .padding(.bottom, 50)
            .allowsHitTesting(false)
        }
    }

    // MARK: - Page Views

    @ViewBuilder
    private func pageView(for index: Int) -> some View {
        switch index {
        case 0:
            ImageSlotView(slot: ble.flavor1Image, editing: editing)
        case 1:
            ratioDisplay(ratio: ble.flavor1Ratio)
        case 2:
            ImageSlotView(slot: ble.flavor2Image, editing: editing)
        case 3:
            ratioDisplay(ratio: ble.flavor2Ratio)
        case 4:
            SettingsPageView(showImageManager: $showImageManager, inAbout: $inAbout)
        default:
            EmptyView()
        }
    }

    private func ratioDisplay(ratio: Int) -> some View {
        Text("1:\(ratio)")
            .font(.system(size: editing ? 72 : 36, weight: .regular, design: .rounded))
            .foregroundStyle(editing ? Theme.textPrimary : Theme.textSecondary)
            .animation(.easeInOut(duration: 0.2), value: editing)
    }

    // MARK: - Value Adjustment

    private func adjustValue(by delta: Int) {
        switch currentPage {
        case 0:
            let newVal = ((ble.flavor1Image + delta) % ble.numImages + ble.numImages) % ble.numImages
            ble.flavor1Image = newVal
        case 1:
            ble.flavor1Ratio = max(6, min(24, ble.flavor1Ratio + delta))
        case 2:
            let newVal = ((ble.flavor2Image + delta) % ble.numImages + ble.numImages) % ble.numImages
            ble.flavor2Image = newVal
        case 3:
            ble.flavor2Ratio = max(6, min(24, ble.flavor2Ratio + delta))
        default:
            break
        }
    }

    private func sendCurrentValue() {
        switch currentPage {
        case 0:
            ble.sendSet("F1_IMAGE", value: ble.flavor1Image)
        case 1:
            ble.sendSet("F1_RATIO", value: ble.flavor1Ratio)
        case 2:
            ble.sendSet("F2_IMAGE", value: ble.flavor2Image)
        case 3:
            ble.sendSet("F2_RATIO", value: ble.flavor2Ratio)
        default:
            break
        }
    }
}
