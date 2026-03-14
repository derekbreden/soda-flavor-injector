import UIKit

struct ImageProcessor {
    /// Crop to centered square and resize to target dimensions
    static func cropAndResize(_ image: UIImage, to size: CGSize) -> UIImage? {
        guard let cgImage = image.cgImage else { return nil }
        let sourceW = CGFloat(cgImage.width)
        let sourceH = CGFloat(cgImage.height)
        let minDim = min(sourceW, sourceH)
        let cropRect = CGRect(
            x: (sourceW - minDim) / 2,
            y: (sourceH - minDim) / 2,
            width: minDim, height: minDim
        )
        guard let cropped = cgImage.cropping(to: cropRect) else { return nil }
        let renderer = UIGraphicsImageRenderer(size: size)
        return renderer.image { ctx in
            UIImage(cgImage: cropped).draw(in: CGRect(origin: .zero, size: size))
        }
    }

    /// Generate PNG data from image resized to 240x240
    static func generatePNG(from image: UIImage) -> Data? {
        guard let resized = cropAndResize(image, to: CGSize(width: 240, height: 240)) else { return nil }
        return resized.pngData()
    }

    /// Generate RGB565 little-endian data at specified dimensions
    static func generateRGB565(from image: UIImage, width: Int, height: Int) -> Data? {
        guard let resized = cropAndResize(image, to: CGSize(width: width, height: height)),
              let cgImage = resized.cgImage else { return nil }

        let bytesPerRow = width * 4
        var rgba = [UInt8](repeating: 0, count: width * height * 4)
        let colorSpace = CGColorSpaceCreateDeviceRGB()
        guard let context = CGContext(
            data: &rgba, width: width, height: height,
            bitsPerComponent: 8, bytesPerRow: bytesPerRow,
            space: colorSpace,
            bitmapInfo: CGImageAlphaInfo.noneSkipLast.rawValue
        ) else { return nil }
        context.draw(cgImage, in: CGRect(x: 0, y: 0, width: width, height: height))

        // Convert RGBA to RGB565 little-endian
        var rgb565 = Data(capacity: width * height * 2)
        for i in 0..<(width * height) {
            let off = i * 4
            let r = UInt16(rgba[off])
            let g = UInt16(rgba[off + 1])
            let b = UInt16(rgba[off + 2])
            let pixel: UInt16 = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)
            rgb565.append(UInt8(pixel & 0xFF))
            rgb565.append(UInt8(pixel >> 8))
        }
        return rgb565
    }

    /// CRC-32 matching firmware's uartCrc32Update(0, data, len)
    /// Standard CRC-32: polynomial 0xEDB88320, init 0xFFFFFFFF, final XOR
    static func crc32(_ data: Data) -> UInt32 {
        var crc: UInt32 = 0xFFFFFFFF
        for byte in data {
            crc ^= UInt32(byte)
            for _ in 0..<8 {
                crc = (crc & 1 != 0) ? (crc >> 1) ^ 0xEDB88320 : crc >> 1
            }
        }
        return ~crc
    }
}
