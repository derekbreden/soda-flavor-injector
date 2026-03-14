import SwiftUI

struct ConfigView: View {
    @EnvironmentObject var ble: BLEManager
    @State private var currentPage = 0
    @State private var editing = false
    @State private var dragOffset: CGFloat = 0

    private let pageCount = 5
    private let pageLabels = ["Flavor 1 Image", "Flavor 1 Ratio", "Flavor 2 Image", "Flavor 2 Ratio", "Images"]

    var body: some View {
        ZStack {
            Color.black.ignoresSafeArea()

            if !ble.configSynced {
                ProgressView("Loading configuration...")
                    .foregroundStyle(.white)
            } else {
                VStack(spacing: 0) {
                    Spacer()

                    // Title
                    Text(pageLabels[currentPage])
                        .font(.system(size: 16, weight: .medium))
                        .foregroundStyle(editing ? .white : .gray)
                        .padding(.bottom, 12)

                    // Content area
                    pageContent
                        .frame(height: 200)

                    Spacer()

                    // Nav dots
                    HStack(spacing: 12) {
                        ForEach(0..<pageCount, id: \.self) { i in
                            Circle()
                                .fill(i == currentPage ? Color.white : Color.gray.opacity(0.4))
                                .frame(width: 8, height: 8)
                        }
                    }
                    .padding(.bottom, 40)
                }
                .contentShape(Rectangle())
                .gesture(tapGesture)
                .gesture(dragGesture)
            }
        }
        .navigationBarTitleDisplayMode(.inline)
        .onAppear {
            ble.requestConfig()
            ble.requestImageList()
        }
    }

    // MARK: - Page Content

    @ViewBuilder
    private var pageContent: some View {
        switch currentPage {
        case 0: // Flavor 1 Image
            imageDisplay(index: ble.flavor1Image)
        case 1: // Flavor 1 Ratio
            ratioDisplay(ratio: ble.flavor1Ratio)
        case 2: // Flavor 2 Image
            imageDisplay(index: ble.flavor2Image)
        case 3: // Flavor 2 Ratio
            ratioDisplay(ratio: ble.flavor2Ratio)
        case 4: // Image Management
            imageManagementIcon
        default:
            EmptyView()
        }
    }

    private func imageDisplay(index: Int) -> some View {
        let size: CGFloat = editing ? 160 : 120
        return VStack(spacing: 8) {
            Circle()
                .fill(Color.gray.opacity(0.3))
                .frame(width: size, height: size)
                .overlay(
                    Text(ble.displayName(for: index))
                        .font(.system(size: 14))
                        .foregroundStyle(.white)
                        .multilineTextAlignment(.center)
                        .padding(8)
                )
                .animation(.easeInOut(duration: 0.2), value: editing)
        }
    }

    private func ratioDisplay(ratio: Int) -> some View {
        Text("1:\(ratio)")
            .font(.system(size: editing ? 72 : 36, weight: .regular, design: .rounded))
            .foregroundStyle(editing ? .white : .gray)
            .animation(.easeInOut(duration: 0.2), value: editing)
    }

    private var imageManagementIcon: some View {
        VStack(spacing: 12) {
            Image(systemName: "photo.stack")
                .font(.system(size: 48))
                .foregroundStyle(.gray)
            Text("\(ble.numImages) images")
                .font(.callout)
                .foregroundStyle(.gray)
            if editing {
                Text("Coming soon")
                    .font(.caption)
                    .foregroundStyle(.white.opacity(0.6))
            }
        }
    }

    // MARK: - Gestures

    private var tapGesture: some Gesture {
        TapGesture()
            .onEnded {
                if editing {
                    // Confirm and save
                    editing = false
                    sendCurrentValue()
                } else {
                    editing = true
                }
            }
    }

    private var dragGesture: some Gesture {
        DragGesture(minimumDistance: 30)
            .onEnded { value in
                let dx = value.translation.width
                if editing {
                    // Swipe to adjust value (like encoder rotation)
                    let dir = dx > 20 ? 1 : (dx < -20 ? -1 : 0)
                    if dir != 0 {
                        adjustValue(by: dir)
                    }
                } else {
                    // Swipe to navigate pages
                    if dx < -30 && currentPage < pageCount - 1 {
                        currentPage += 1
                    } else if dx > 30 && currentPage > 0 {
                        currentPage -= 1
                    }
                }
            }
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
