import SwiftUI
import PhotosUI

struct ImageManagerView: View {
    @Environment(BLEManager.self) var ble
    @Environment(\.dismiss) private var dismiss
    @State private var selectedPhotos: [PhotosPickerItem] = []
    @State private var deleteSlot: Int?
    @State private var showDeleteConfirm = false

    private let maxImages = 10  // 5 factory + 5 user custom

    /// Total slots committed: on-device + active upload + queued
    private var totalPendingSlots: Int {
        ble.numImages + ble.uploadQueue.count + (ble.uploadProgress != nil ? 1 : 0)
    }

    private var canDelete: Bool {
        ble.numImages > 1 && ble.imageDownloadProgress == nil && ble.uploadQueue.isEmpty && ble.uploadProgress == nil
    }

    var body: some View {
        NavigationView {
            ScrollView {
                VStack(spacing: 0) {
                    imagesSection

                    if ble.imageDownloadProgress != nil {
                        downloadProgressSection
                    }

                    if ble.uploadProgress != nil || !ble.uploadQueue.isEmpty {
                        uploadProgressSection
                    }

                    if totalPendingSlots < maxImages && ble.imageDownloadProgress == nil {
                        addImageSection
                    }

                    Text("\(totalPendingSlots) of \(maxImages) image slots used")
                        .font(.system(size: 13))
                        .foregroundStyle(Theme.textSecondary)
                        .padding(.top, 16)
                }
                .padding(.vertical, 20)
            }
            .background(Theme.background)
            .navigationBarTitleDisplayMode(.inline)
            .toolbar {
                ToolbarItem(placement: .principal) {
                    Text("Manage Images")
                        .font(.system(size: 16, weight: .medium))
                        .foregroundStyle(Theme.textSecondary)
                }
                ToolbarItem(placement: .confirmationAction) {
                    Button("Done") { dismiss() }
                        .foregroundStyle(Theme.textSecondary)
                }
            }
            .toolbarColorScheme(.dark, for: .navigationBar)
            .toolbarBackground(Theme.background, for: .navigationBar)
            .toolbarBackground(.visible, for: .navigationBar)
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
                        let items = images.map { BLEManager.UploadQueueItem(image: $0) }
                        ble.queueUploads(items)
                    }
                }
            }
        }
    }

    // MARK: - Sections

    private var uploadProgressSection: some View {
        VStack(spacing: 0) {
            Text("Uploads")
                .font(.system(size: 13))
                .foregroundStyle(Theme.textSecondary)
                .frame(maxWidth: .infinity, alignment: .leading)
                .padding(.horizontal, 20)
                .padding(.bottom, 8)

            // Active upload row
            if let activeImage = ble.activeUploadImage, ble.uploadProgress != nil {
                HStack(spacing: 12) {
                    Image(uiImage: activeImage)
                        .resizable()
                        .scaledToFill()
                        .frame(width: 44, height: 44)
                        .clipShape(Circle())

                    VStack(alignment: .leading, spacing: 4) {
                        Text(ble.uploadStatus)
                            .font(.system(size: 14))
                            .foregroundStyle(Theme.textPrimary)
                        ProgressView(value: ble.uploadProgress ?? 0)
                            .tint(.blue)
                    }

                    Button {
                        ble.cancelActiveUpload()
                    } label: {
                        Image(systemName: "xmark.circle.fill")
                            .foregroundStyle(Theme.textSecondary)
                    }
                    .buttonStyle(.plain)
                }
                .padding(.horizontal, 20)
                .padding(.vertical, 6)
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
                        .font(.system(size: 14))
                        .foregroundStyle(Theme.textSecondary)

                    Spacer()

                    Button {
                        ble.cancelQueuedUpload(id: item.id)
                    } label: {
                        Image(systemName: "xmark.circle.fill")
                            .foregroundStyle(Theme.textSecondary)
                    }
                    .buttonStyle(.plain)
                }
                .padding(.horizontal, 20)
                .padding(.vertical, 6)
            }

            Spacer().frame(height: 20)
        }
    }

    private var downloadProgressSection: some View {
        HStack(spacing: 12) {
            ProgressView()
                .tint(Theme.textPrimary)
            Text("Loading images…")
                .font(.system(size: 14))
                .foregroundStyle(Theme.textSecondary)
        }
        .padding(.vertical, 12)
    }

    private var imagesSection: some View {
        VStack(spacing: 0) {
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

                    if canDelete {
                        Button {
                            deleteSlot = index
                            showDeleteConfirm = true
                        } label: {
                            Image(systemName: "trash")
                                .font(.system(size: 14))
                                .foregroundStyle(Theme.textSecondary)
                        }
                        .buttonStyle(.plain)
                    }
                }
                .padding(.horizontal, 20)
                .padding(.vertical, 8)
            }
        }
    }

    private var addImageSection: some View {
        VStack {
            let remaining = maxImages - totalPendingSlots
            PhotosPicker(
                selection: $selectedPhotos,
                maxSelectionCount: remaining,
                selectionBehavior: .ordered,
                matching: .images
            ) {
                Label("Add Image", systemImage: "plus.circle.fill")
                    .font(.system(size: 16, weight: .medium))
                    .foregroundStyle(Theme.textSecondary)
            }
            .padding(.top, 16)
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
                            .tint(Theme.textPrimary)
                            .scaleEffect(0.7)
                    }
            }
        }
    }

    private func badge(_ text: String, color: Color) -> some View {
        Text(text)
            .font(.caption.weight(.bold))
            .foregroundStyle(Theme.textPrimary)
            .padding(.horizontal, 6)
            .padding(.vertical, 2)
            .background(color.opacity(0.2))
            .clipShape(RoundedRectangle(cornerRadius: 4))
    }
}
