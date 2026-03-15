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

private struct ImageManagementIcon: View {
    @Environment(BLEManager.self) var ble

    var body: some View {
        VStack(spacing: 12) {
            Image(systemName: "photo.stack")
                .font(.system(size: 48))
                .foregroundStyle(.gray)
            Text("\(ble.numImages) images")
                .font(.callout)
                .foregroundStyle(.gray)
        }
    }
}

// ────────────────────────────────────────────────────────────

struct ConfigView: View {
    @Environment(BLEManager.self) var ble
    @State private var currentPage = 0
    @State private var editing = false
    @State private var showImageManager = false

    private let pageCount = 5
    private let pageLabels = ["Flavor 1 Image", "Flavor 1 Ratio", "Flavor 2 Image", "Flavor 2 Ratio", "Images"]

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
            } else {
                ZStack(alignment: .bottom) {
                    // Paging TabView — tap gestures are on per-page
                    // content (not the TabView itself) to avoid
                    // conflicting with the swipe gesture recognizer.
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
                                    showImageManager = true
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
                        // When editing, an overlay captures taps (to
                        // dismiss) and horizontal drags (to adjust the
                        // value). The TabView beneath has hit testing
                        // disabled so swipes don't change pages.
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
                .sheet(isPresented: $showImageManager) {
                    ImageManagerView()
                        .environment(ble)
                }
            }
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
            ImageManagementIcon()
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
