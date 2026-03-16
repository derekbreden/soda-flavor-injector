import SwiftUI
import PhotosUI

struct ImageManagerView: View {
    @Environment(BLEManager.self) var ble
    @Environment(\.dismiss) private var dismiss
    @State private var selectedPhotos: [PhotosPickerItem] = []
    @State private var pickedImages: [UIImage] = []
    @State private var showUploadSheet = false
    @State private var deleteSlot: Int?
    @State private var showDeleteConfirm = false

    private let maxImages = 23

    /// Total slots committed: on-device + active upload + queued
    private var totalPendingSlots: Int {
        ble.numImages + ble.uploadQueue.count + (ble.uploadProgress != nil ? 1 : 0)
    }

    var body: some View {
        NavigationView {
            List {
                if ble.uploadProgress != nil || !ble.uploadQueue.isEmpty {
                    uploadProgressSection
                }

                if ble.imageDownloadProgress != nil {
                    downloadProgressSection
                }

                imagesSection

                if totalPendingSlots < maxImages && ble.imageDownloadProgress == nil {
                    addImageSection
                }
            }
            .navigationTitle("Images")
            .navigationBarTitleDisplayMode(.inline)
            .toolbar {
                ToolbarItem(placement: .navigationBarLeading) {
                    Button("Done") { dismiss() }
                }
            }
            .sheet(isPresented: $showUploadSheet) {
                UploadQueueSheet(images: pickedImages)
                    .environment(ble)
            }
            .alert("Delete Image?", isPresented: $showDeleteConfirm) {
                Button("Delete", role: .destructive) {
                    if let slot = deleteSlot {
                        ble.deleteImage(slot: slot)
                    }
                }
                Button("Cancel", role: .cancel) {}
            } message: {
                Text("This cannot be undone.")
            }
            .alert("Delete Failed", isPresented: Binding(
                get: { ble.deleteError != nil },
                set: { if !$0 { ble.deleteError = nil } }
            )) {
                Button("OK") { ble.deleteError = nil }
            } message: {
                Text(ble.deleteError ?? "")
            }
            .onChange(of: selectedPhotos) { _, newItems in
                guard !newItems.isEmpty else { return }
                Task {
                    var images: [UIImage] = []
                    for item in newItems {
                        if let data = try? await item.loadTransferable(type: Data.self),
                           let image = UIImage(data: data) {
                            images.append(image)
                        }
                    }
                    selectedPhotos = []
                    if !images.isEmpty {
                        pickedImages = images
                        showUploadSheet = true
                    }
                }
            }
        }
    }

    // MARK: - Sections

    private var uploadProgressSection: some View {
        Section("Uploads") {
            // Active upload row
            if let activeImage = ble.activeUploadImage, ble.uploadProgress != nil {
                HStack(spacing: 12) {
                    Image(uiImage: activeImage)
                        .resizable()
                        .scaledToFill()
                        .frame(width: 44, height: 44)
                        .clipShape(Circle())

                    VStack(alignment: .leading, spacing: 4) {
                        Text("Slot \(ble.activeUploadSlot)")
                            .font(.callout)
                        ProgressView(value: ble.uploadProgress ?? 0)
                            .tint(.blue)
                    }
                }
                .padding(.vertical, 2)
            }

            // Queued upload rows
            ForEach(ble.uploadQueue) { item in
                HStack(spacing: 12) {
                    Image(uiImage: item.image)
                        .resizable()
                        .scaledToFill()
                        .frame(width: 44, height: 44)
                        .clipShape(Circle())
                        .opacity(0.6)

                    Text("Queued")
                        .font(.callout)
                        .foregroundStyle(.secondary)
                }
                .padding(.vertical, 2)
            }
        }
    }

    private var downloadProgressSection: some View {
        Section {
            HStack(spacing: 12) {
                ProgressView()
                Text("Loading images…")
                    .font(.callout)
                    .foregroundStyle(.secondary)
            }
            .padding(.vertical, 4)
        }
    }

    private var imagesSection: some View {
        Section {
            ForEach(0..<ble.numImages, id: \.self) { index in
                HStack(spacing: 12) {
                    imageThumb(slot: index)
                        .frame(width: 44, height: 44)

                    Spacer()

                    if index == ble.flavor1Image {
                        badge("F1", color: .blue)
                    }
                    if index == ble.flavor2Image {
                        badge("F2", color: .green)
                    }
                }
            }
            .onDelete { offsets in
                if ble.numImages > 1, ble.imageDownloadProgress == nil, ble.uploadQueue.isEmpty, ble.uploadProgress == nil, let index = offsets.first {
                    deleteSlot = index
                    showDeleteConfirm = true
                }
            }
        } footer: {
            Text("\(totalPendingSlots) of \(maxImages) image slots used")
        }
    }

    private var addImageSection: some View {
        Section {
            let remaining = maxImages - totalPendingSlots
            PhotosPicker(
                selection: $selectedPhotos,
                maxSelectionCount: remaining,
                selectionBehavior: .ordered,
                matching: .images
            ) {
                Label("Add Image", systemImage: "plus.circle.fill")
            }
        }
    }

    // MARK: - Helpers

    private func imageThumb(slot: Int) -> some View {
        Group {
            if let uiImage = ble.imageFor(slot: slot) {
                Image(uiImage: uiImage)
                    .resizable()
                    .scaledToFill()
                    .clipShape(Circle())
            } else {
                Circle()
                    .fill(Theme.placeholder)
                    .overlay {
                        ProgressView()
                            .scaleEffect(0.7)
                    }
            }
        }
    }

    private func badge(_ text: String, color: Color) -> some View {
        Text(text)
            .font(.caption.weight(.bold))
            .padding(.horizontal, 6)
            .padding(.vertical, 2)
            .background(color.opacity(0.2))
            .clipShape(RoundedRectangle(cornerRadius: 4))
    }
}
