import SwiftUI

struct ConfigView: View {
    @EnvironmentObject var ble: BLEManager

    var body: some View {
        List {
            if !ble.configSynced {
                Section {
                    HStack {
                        ProgressView()
                            .padding(.trailing, 8)
                        Text("Loading configuration...")
                            .foregroundStyle(.secondary)
                    }
                }
            } else {
                flavorSection(
                    title: "Flavor 1",
                    imageIndex: ble.flavor1Image,
                    ratio: ble.flavor1Ratio,
                    imageKey: "F1_IMAGE",
                    ratioKey: "F1_RATIO"
                )

                flavorSection(
                    title: "Flavor 2",
                    imageIndex: ble.flavor2Image,
                    ratio: ble.flavor2Ratio,
                    imageKey: "F2_IMAGE",
                    ratioKey: "F2_RATIO"
                )

                Section {
                    NavigationLink {
                        ImageListView()
                    } label: {
                        Label("Manage Images", systemImage: "photo.stack")
                    }
                }
            }
        }
        .onAppear {
            ble.requestConfig()
        }
    }

    // MARK: - Flavor Section

    private func flavorSection(title: String, imageIndex: Int, ratio: Int,
                                imageKey: String, ratioKey: String) -> some View {
        Section(title) {
            // Image picker
            HStack {
                Text("Image")
                Spacer()
                if ble.imageNames.isEmpty {
                    Text("Image \(imageIndex)")
                        .foregroundStyle(.secondary)
                } else {
                    Text(ble.displayName(for: imageIndex))
                        .foregroundStyle(.secondary)
                }
                Stepper("", value: Binding(
                    get: { imageIndex },
                    set: { newVal in
                        let clamped = ((newVal % ble.numImages) + ble.numImages) % ble.numImages
                        ble.sendSet(imageKey, value: clamped)
                    }
                ), in: 0...(max(0, ble.numImages - 1)))
                .labelsHidden()
            }

            // Ratio slider
            HStack {
                Text("Ratio")
                Spacer()
                Text("1:\(ratio)")
                    .foregroundStyle(.secondary)
                    .frame(width: 40, alignment: .trailing)
                Stepper("", value: Binding(
                    get: { ratio },
                    set: { newVal in
                        ble.sendSet(ratioKey, value: newVal)
                    }
                ), in: 6...24)
                .labelsHidden()
            }
        }
    }
}
