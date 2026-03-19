import SwiftUI

struct UploadQueueSheet: View {
    @Environment(BLEManager.self) var ble
    @Environment(\.dismiss) private var dismiss
    let images: [UIImage]

    var body: some View {
        NavigationView {
            VStack(spacing: 24) {
                Spacer()

                ScrollView {
                    LazyVGrid(columns: [GridItem(.adaptive(minimum: 80))], spacing: 12) {
                        ForEach(Array(images.enumerated()), id: \.offset) { _, image in
                            Image(uiImage: image)
                                .resizable()
                                .scaledToFill()
                                .frame(width: 70, height: 70)
                                .clipShape(Circle())
                        }
                    }
                    .padding()
                }

                Spacer()

                Button {
                    let items = images.map { BLEManager.UploadQueueItem(image: $0) }
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
            .background(Theme.background)
            .navigationTitle("Upload Images")
            .navigationBarTitleDisplayMode(.inline)
            .toolbarColorScheme(.dark, for: .navigationBar)
            .toolbarBackground(Theme.background, for: .navigationBar)
            .toolbarBackground(.visible, for: .navigationBar)
            .toolbar {
                ToolbarItem(placement: .navigationBarLeading) {
                    Button("Cancel") { dismiss() }
                }
            }
        }
    }
}
