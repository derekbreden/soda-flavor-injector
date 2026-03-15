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
                        .fill(Color(white: 0.15))
                        .frame(width: size, height: size)
                    if ble.imageDownloadProgress != nil {
                        ProgressView()
                            .tint(.white)
                    } else {
                        Image(systemName: "photo")
                            .font(.system(size: 24))
                            .foregroundStyle(.gray)
                    }
                }
            }
        }
        .animation(.easeInOut(duration: 0.2), value: editing)
    }
}

// ────────────────────────────────────────────────────────────
// Settings sub-views (About, version display)
// ────────────────────────────────────────────────────────────

private struct AboutView: View {
    @Environment(BLEManager.self) var ble

    var body: some View {
        VStack(spacing: 0) {
            Text("About")
                .font(.system(size: 16, weight: .medium))
                .foregroundStyle(.white)
                .padding(.top, 80)

            Spacer().frame(height: 32)

            let labels = ["S3", "ESP32", "RP2040"]
            let versions = [ble.s3Version, ble.espVersion, ble.rpVersion]

            ForEach(0..<3, id: \.self) { i in
                VStack(spacing: 4) {
                    Text(labels[i])
                        .font(.system(size: 14))
                        .foregroundStyle(.gray)
                    Text(versions[i].isEmpty ? "..." : versions[i])
                        .font(.system(size: 14))
                        .foregroundStyle(.white)
                }
                if i < 2 { Spacer().frame(height: 16) }
            }

            Spacer()
        }
    }
}

// ────────────────────────────────────────────────────────────

private enum SettingsItem: Int, CaseIterable {
    case back, manageImages, factoryReset, about
    var label: String {
        switch self {
        case .back: "Back"
        case .manageImages: "Manage Images"
        case .factoryReset: "Factory Reset"
        case .about: "About"
        }
    }
}

struct ConfigView: View {
    @Environment(BLEManager.self) var ble
    @State private var currentPage = 0
    @State private var editing = false
    @State private var showImageManager = false

    // Settings submenu state
    @State private var inSettings = false
    @State private var settingsIndex = 0
    @State private var settingsConfirm = false
    @State private var confirmIndex = 1  // default to No
    @State private var inAbout = false
    @State private var factoryResetPending = false

    private let pageCount = 5
    private let pageLabels = ["Flavor 1 Image", "Flavor 1 Ratio", "Flavor 2 Image", "Flavor 2 Ratio", "Settings"]

    var body: some View {
        ZStack {
            Color.black.ignoresSafeArea()

            if !ble.configSynced {
                VStack(spacing: 16) {
                    ProgressView()
                        .scaleEffect(1.5)
                        .tint(.white)
                    Text("Loading configuration...")
                        .font(.title3.weight(.medium))
                        .foregroundStyle(.white)
                }
            } else if inAbout {
                aboutContent
            } else if inSettings {
                settingsContent
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
                inSettings = false
                inAbout = false
                settingsConfirm = false
                factoryResetPending = false
            }
        }
        .onChange(of: ble.factoryResetCompleted) { _, completed in
            if completed {
                factoryResetPending = false
                settingsConfirm = false
                inSettings = false
                ble.factoryResetCompleted = false
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

                        Text(pageLabels[i])
                            .font(.system(size: 16, weight: .medium))
                            .foregroundStyle(editing && i == currentPage ? .white : .gray)

                        Spacer().frame(height: 12)

                        pageView(for: i)
                            .frame(height: 180)

                        Spacer()
                    }
                    .contentShape(Rectangle())
                    .onTapGesture {
                        if i == 4 {
                            enterSettings()
                        } else {
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
                        .fill(j == currentPage ? Color.white : Color(white: 0.3))
                        .frame(width: 8, height: 8)
                }
            }
            .padding(.bottom, 50)
            .allowsHitTesting(false)
        }
    }

    // MARK: - Settings submenu

    private var settingsContent: some View {
        VStack(spacing: 0) {
            Spacer().frame(height: 80)

            Text(settingsConfirm ? "Factory Reset" : "Settings")
                .font(.system(size: 16, weight: .medium))
                .foregroundStyle(.white)

            Spacer()

            if factoryResetPending {
                Text("Resetting...")
                    .font(.system(size: 18, weight: .medium))
                    .foregroundStyle(.red)
            } else if settingsConfirm {
                confirmList
            } else {
                settingsList
            }

            Spacer()
        }
        .contentShape(Rectangle())
        .onTapGesture { handleSettingsTap() }
        .gesture(
            DragGesture(minimumDistance: 30)
                .onEnded { value in
                    guard !factoryResetPending else { return }
                    let dx = value.translation.width
                    if dx > 30 {
                        handleSettingsNav(-1)
                    } else if dx < -30 {
                        handleSettingsNav(1)
                    }
                }
        )
    }

    private var settingsList: some View {
        VStack(spacing: 28) {
            ForEach(SettingsItem.allCases, id: \.rawValue) { item in
                Text(item.label)
                    .font(.system(size: 18, weight: .medium))
                    .foregroundStyle(item.rawValue == settingsIndex ? .white : Color(white: 0.25))
            }
        }
    }

    private var confirmList: some View {
        VStack(spacing: 28) {
            ForEach(["Yes", "No"], id: \.self) { option in
                let idx = option == "Yes" ? 0 : 1
                Text(option)
                    .font(.system(size: 18, weight: .medium))
                    .foregroundStyle(idx == confirmIndex ? .white : Color(white: 0.25))
            }
        }
    }

    // MARK: - About

    private var aboutContent: some View {
        AboutView()
            .contentShape(Rectangle())
            .onTapGesture {
                inAbout = false
            }
    }

    // MARK: - Settings logic

    private func enterSettings() {
        inSettings = true
        settingsIndex = 0
        settingsConfirm = false
    }

    private func handleSettingsNav(_ dir: Int) {
        if settingsConfirm {
            confirmIndex = (confirmIndex + dir + 2) % 2
        } else {
            let count = SettingsItem.allCases.count
            settingsIndex = (settingsIndex + dir + count) % count
        }
    }

    private func handleSettingsTap() {
        if factoryResetPending { return }

        if settingsConfirm {
            if confirmIndex == 0 {
                // Yes — execute factory reset
                factoryResetPending = true
                ble.factoryReset()
            } else {
                // No — back to settings list
                settingsConfirm = false
            }
            return
        }

        guard let item = SettingsItem(rawValue: settingsIndex) else { return }
        switch item {
        case .back:
            inSettings = false
        case .manageImages:
            showImageManager = true
        case .factoryReset:
            settingsConfirm = true
            confirmIndex = 1  // default to No
        case .about:
            inAbout = true
            ble.requestVersions()
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
            settingsIcon
        default:
            EmptyView()
        }
    }

    private func ratioDisplay(ratio: Int) -> some View {
        Text("1:\(ratio)")
            .font(.system(size: editing ? 72 : 36, weight: .regular, design: .rounded))
            .foregroundStyle(editing ? .white : .gray)
            .animation(.easeInOut(duration: 0.2), value: editing)
    }

    private var settingsIcon: some View {
        Image(systemName: "gearshape")
            .font(.system(size: 48))
            .foregroundStyle(.gray)
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
