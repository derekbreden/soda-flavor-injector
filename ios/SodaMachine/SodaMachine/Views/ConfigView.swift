import SwiftUI

struct ConfigView: View {
    @EnvironmentObject var ble: BLEManager
    @State private var currentPage = 0
    @State private var editing = false
    @State private var dragOffset: CGFloat = 0

    private let pageCount = 5
    private let pageLabels = ["Flavor 1 Image", "Flavor 1 Ratio", "Flavor 2 Image", "Flavor 2 Ratio", "Images"]

    var body: some View {
        GeometryReader { geo in
            let pageWidth = geo.size.width

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

                        // Carousel
                        HStack(spacing: 0) {
                            ForEach(0..<pageCount, id: \.self) { i in
                                pageView(for: i)
                                    .frame(width: pageWidth, height: 200)
                            }
                        }
                        .offset(x: -CGFloat(currentPage) * pageWidth + dragOffset)
                        .gesture(
                            DragGesture()
                                .onChanged { value in
                                    if editing {
                                        // No carousel movement in edit mode
                                    } else {
                                        dragOffset = value.translation.width
                                    }
                                }
                                .onEnded { value in
                                    if editing {
                                        let dx = value.translation.width
                                        if dx > 40 {
                                            adjustValue(by: 1)
                                        } else if dx < -40 {
                                            adjustValue(by: -1)
                                        }
                                    } else {
                                        let threshold = pageWidth * 0.2
                                        let dx = value.predictedEndTranslation.width
                                        withAnimation(.easeOut(duration: 0.25)) {
                                            if dx < -threshold && currentPage < pageCount - 1 {
                                                currentPage += 1
                                            } else if dx > threshold && currentPage > 0 {
                                                currentPage -= 1
                                            }
                                            dragOffset = 0
                                        }
                                    }
                                }
                        )

                        Spacer()

                        // Nav dots
                        HStack(spacing: 12) {
                            ForEach(0..<pageCount, id: \.self) { i in
                                Circle()
                                    .fill(i == currentPage ? Color.white : Color(white: 0.3))
                                    .frame(width: 8, height: 8)
                            }
                        }
                        .padding(.bottom, 50)
                    }
                    .simultaneousGesture(
                        TapGesture()
                            .onEnded {
                                if editing {
                                    editing = false
                                    sendCurrentValue()
                                } else if currentPage < 4 {
                                    editing = true
                                }
                            }
                    )
                }
            }
        }
        .onAppear {
            ble.requestConfig()
            ble.requestImageList()
        }
    }

    // MARK: - Page Views

    @ViewBuilder
    private func pageView(for index: Int) -> some View {
        switch index {
        case 0:
            imageDisplay(index: ble.flavor1Image)
        case 1:
            ratioDisplay(ratio: ble.flavor1Ratio)
        case 2:
            imageDisplay(index: ble.flavor2Image)
        case 3:
            ratioDisplay(ratio: ble.flavor2Ratio)
        case 4:
            imageManagementIcon
        default:
            EmptyView()
        }
    }

    private func imageDisplay(index: Int) -> some View {
        let size: CGFloat = editing ? 160 : 120
        return VStack(spacing: 6) {
            if let uiImage = ble.imageFor(slot: index) {
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
            Text(ble.displayName(for: index))
                .font(.system(size: 12))
                .foregroundStyle(.gray)
        }
        .animation(.easeInOut(duration: 0.2), value: editing)
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
