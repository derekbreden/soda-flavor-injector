import SwiftUI

struct ImageListView: View {
    @Environment(BLEManager.self) var ble

    var body: some View {
        List {
            if ble.imageNames.isEmpty {
                Section {
                    HStack {
                        ProgressView()
                            .padding(.trailing, 8)
                        Text("Loading images...")
                            .foregroundStyle(.secondary)
                    }
                }
            } else {
                Section {
                    ForEach(Array(ble.imageNames.enumerated()), id: \.offset) { index, name in
                        HStack {
                            Text("\(index)")
                                .font(.system(.body, design: .monospaced))
                                .foregroundStyle(.secondary)
                                .frame(width: 30, alignment: .leading)
                            Text(ble.displayName(for: index))

                            Spacer()

                            // In-use indicators
                            if index == ble.flavor1Image {
                                Text("F1")
                                    .font(.caption.weight(.bold))
                                    .padding(.horizontal, 6)
                                    .padding(.vertical, 2)
                                    .background(.blue.opacity(0.2))
                                    .clipShape(RoundedRectangle(cornerRadius: 4))
                            }
                            if index == ble.flavor2Image {
                                Text("F2")
                                    .font(.caption.weight(.bold))
                                    .padding(.horizontal, 6)
                                    .padding(.vertical, 2)
                                    .background(.green.opacity(0.2))
                                    .clipShape(RoundedRectangle(cornerRadius: 4))
                            }
                        }
                    }
                } footer: {
                    Text("Image upload and reordering coming soon.")
                }
            }
        }
        .navigationTitle("Images")
        .onAppear {
            ble.requestImageList()
        }
    }
}
