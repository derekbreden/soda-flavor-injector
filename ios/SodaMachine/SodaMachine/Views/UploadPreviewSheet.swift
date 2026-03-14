import SwiftUI

struct UploadPreviewSheet: View {
    @EnvironmentObject var ble: BLEManager
    @Environment(\.dismiss) private var dismiss
    let image: UIImage
    let slot: Int

    var body: some View {
        NavigationView {
            VStack(spacing: 24) {
                Spacer()

                Image(uiImage: image)
                    .resizable()
                    .scaledToFill()
                    .frame(width: 180, height: 180)
                    .clipShape(Circle())
                    .overlay(Circle().stroke(Color.gray.opacity(0.3), lineWidth: 1))

                Text("Slot \(slot)")
                    .font(.caption)
                    .foregroundStyle(.secondary)

                Spacer()

                Button {
                    ble.uploadImage(image, toSlot: slot)
                    dismiss()
                } label: {
                    Text("Upload")
                        .frame(maxWidth: .infinity)
                        .padding(.vertical, 12)
                }
                .buttonStyle(.borderedProminent)
                .padding(.horizontal, 40)
                .padding(.bottom, 20)
            }
            .navigationTitle("Upload Image")
            .navigationBarTitleDisplayMode(.inline)
            .toolbar {
                ToolbarItem(placement: .navigationBarLeading) {
                    Button("Cancel") { dismiss() }
                }
            }
        }
    }
}
