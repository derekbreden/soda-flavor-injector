import SwiftUI

struct UploadQueueSheet: View {
    @Environment(BLEManager.self) var ble
    @Environment(\.dismiss) private var dismiss
    let images: [UIImage]

    private var startSlot: Int {
        ble.numImages + ble.uploadQueue.count
    }

    var body: some View {
        NavigationView {
            VStack(spacing: 24) {
                Spacer()

                ScrollView {
                    LazyVGrid(columns: [GridItem(.adaptive(minimum: 80))], spacing: 12) {
                        ForEach(Array(images.enumerated()), id: \.offset) { index, image in
                            VStack(spacing: 4) {
                                Image(uiImage: image)
                                    .resizable()
                                    .scaledToFill()
                                    .frame(width: 70, height: 70)
                                    .clipShape(Circle())
                                    .overlay(Circle().stroke(Color.gray.opacity(0.3), lineWidth: 1))
                                Text("Slot \(startSlot + index)")
                                    .font(.caption2)
                                    .foregroundStyle(.secondary)
                            }
                        }
                    }
                    .padding()
                }

                Spacer()

                Button {
                    let items = images.enumerated().map { index, image in
                        (image: image, slot: startSlot + index)
                    }
                    ble.queueUploads(items)
                    dismiss()
                } label: {
                    Text("Upload \(images.count) Image\(images.count == 1 ? "" : "s")")
                        .frame(maxWidth: .infinity)
                        .padding(.vertical, 12)
                }
                .buttonStyle(.borderedProminent)
                .padding(.horizontal, 40)
                .padding(.bottom, 20)
            }
            .navigationTitle("Upload Images")
            .navigationBarTitleDisplayMode(.inline)
            .toolbar {
                ToolbarItem(placement: .navigationBarLeading) {
                    Button("Cancel") { dismiss() }
                }
            }
        }
    }
}
