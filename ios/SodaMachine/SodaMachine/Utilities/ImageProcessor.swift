import UIKit
import ImageIO
import UniformTypeIdentifiers

struct ImageProcessor {
    /// Normalize EXIF orientation, crop to centered square, resize to target dimensions (1x scale)
    static func cropAndResize(_ image: UIImage, to size: CGSize) -> UIImage? {
        // Draw through UIImage first to apply EXIF orientation (iPhone photos are often rotated)
        let format = UIGraphicsImageRendererFormat()
        format.scale = 1.0
        let normalized = UIGraphicsImageRenderer(
            size: CGSize(width: image.size.width, height: image.size.height), format: format
        ).image { ctx in
            image.draw(in: CGRect(origin: .zero, size: CGSize(width: image.size.width, height: image.size.height)))
        }

        guard let cgImage = normalized.cgImage else { return nil }
        let sourceW = CGFloat(cgImage.width)
        let sourceH = CGFloat(cgImage.height)
        let minDim = min(sourceW, sourceH)
        let cropRect = CGRect(
            x: (sourceW - minDim) / 2,
            y: (sourceH - minDim) / 2,
            width: minDim, height: minDim
        )
        guard let cropped = cgImage.cropping(to: cropRect) else { return nil }

        let renderer = UIGraphicsImageRenderer(size: size, format: format)
        return renderer.image { ctx in
            UIImage(cgImage: cropped).draw(in: CGRect(origin: .zero, size: size))
        }
    }

    /// Extract raw RGB pixels from a CGImage as [UInt8] in R,G,B,X,R,G,B,X,... layout
    private static func extractRGBA(_ cgImage: CGImage, width: Int, height: Int) -> [UInt8]? {
        var pixels = [UInt8](repeating: 0, count: width * height * 4)
        let colorSpace = CGColorSpaceCreateDeviceRGB()
        guard let ctx = CGContext(
            data: &pixels, width: width, height: height,
            bitsPerComponent: 8, bytesPerRow: width * 4,
            space: colorSpace,
            bitmapInfo: CGImageAlphaInfo.noneSkipLast.rawValue
        ) else { return nil }
        ctx.draw(cgImage, in: CGRect(x: 0, y: 0, width: width, height: height))
        return pixels
    }

    /// Generate optimized indexed-color PNG from image resized to 240x240.
    /// Uses median-cut quantization to 64 colors for TinyPNG-level file sizes.
    static func generatePNG(from image: UIImage) -> Data? {
        guard let resized = cropAndResize(image, to: CGSize(width: 240, height: 240)),
              let cgImage = resized.cgImage else { return nil }

        let w = cgImage.width, h = cgImage.height
        guard let pixels = extractRGBA(cgImage, width: w, height: h) else { return nil }

        let palette = medianCut(pixels: pixels, pixelCount: w * h, maxColors: 64)
        let indices = mapToPalette(pixels: pixels, pixelCount: w * h, palette: palette)

        return writeIndexedPNG(indices: indices, width: w, height: h, palette: palette)
    }

    // MARK: - Median-cut color quantization

    private struct ColorBox {
        var minR: UInt8 = 255, maxR: UInt8 = 0
        var minG: UInt8 = 255, maxG: UInt8 = 0
        var minB: UInt8 = 255, maxB: UInt8 = 0
        var pixels: [(r: UInt8, g: UInt8, b: UInt8)] = []

        mutating func computeBounds() {
            minR = 255; maxR = 0; minG = 255; maxG = 0; minB = 255; maxB = 0
            for p in pixels {
                if p.r < minR { minR = p.r }; if p.r > maxR { maxR = p.r }
                if p.g < minG { minG = p.g }; if p.g > maxG { maxG = p.g }
                if p.b < minB { minB = p.b }; if p.b > maxB { maxB = p.b }
            }
        }

        var longestAxis: Int {
            let rRange = Int(maxR) - Int(minR)
            let gRange = Int(maxG) - Int(minG)
            let bRange = Int(maxB) - Int(minB)
            if rRange >= gRange && rRange >= bRange { return 0 }
            if gRange >= bRange { return 1 }
            return 2
        }

        var averageColor: (UInt8, UInt8, UInt8) {
            guard !pixels.isEmpty else { return (0, 0, 0) }
            var sumR = 0, sumG = 0, sumB = 0
            for p in pixels { sumR += Int(p.r); sumG += Int(p.g); sumB += Int(p.b) }
            let n = pixels.count
            return (UInt8(sumR / n), UInt8(sumG / n), UInt8(sumB / n))
        }
    }

    /// Median-cut quantization: reduce pixel colors to maxColors palette entries.
    /// Subsamples input for performance (full mapping is done separately).
    private static func medianCut(pixels: [UInt8], pixelCount: Int, maxColors: Int) -> [(UInt8, UInt8, UInt8)] {
        // Subsample for quantization speed — every 4th pixel is plenty for 240x240
        let stride = max(1, pixelCount / (240 * 240 / 4))
        var samples: [(r: UInt8, g: UInt8, b: UInt8)] = []
        samples.reserveCapacity(pixelCount / stride)
        for i in Swift.stride(from: 0, to: pixelCount, by: stride) {
            let off = i * 4
            samples.append((pixels[off], pixels[off + 1], pixels[off + 2]))
        }

        var box = ColorBox()
        box.pixels = samples
        box.computeBounds()
        var boxes = [box]

        while boxes.count < maxColors {
            // Find the box with the most pixels to split
            guard let splitIdx = boxes.enumerated()
                .filter({ $0.element.pixels.count > 1 })
                .max(by: { $0.element.pixels.count < $1.element.pixels.count })?
                .offset else { break }

            var toSplit = boxes.remove(at: splitIdx)
            let axis = toSplit.longestAxis

            toSplit.pixels.sort { a, b in
                switch axis {
                case 0: return a.r < b.r
                case 1: return a.g < b.g
                default: return a.b < b.b
                }
            }

            let mid = toSplit.pixels.count / 2
            var box1 = ColorBox()
            box1.pixels = Array(toSplit.pixels[..<mid])
            box1.computeBounds()
            var box2 = ColorBox()
            box2.pixels = Array(toSplit.pixels[mid...])
            box2.computeBounds()

            boxes.append(box1)
            boxes.append(box2)
        }

        return boxes.map { $0.averageColor }
    }

    /// Map each pixel to the nearest palette color index
    private static func mapToPalette(pixels: [UInt8], pixelCount: Int, palette: [(UInt8, UInt8, UInt8)]) -> [UInt8] {
        var indices = [UInt8](repeating: 0, count: pixelCount)
        for i in 0..<pixelCount {
            let off = i * 4
            let r = Int(pixels[off]), g = Int(pixels[off + 1]), b = Int(pixels[off + 2])
            var bestDist = Int.max
            var bestIdx: UInt8 = 0
            for (j, c) in palette.enumerated() {
                let dr = r - Int(c.0), dg = g - Int(c.1), db = b - Int(c.2)
                let dist = dr * dr + dg * dg + db * db
                if dist < bestDist {
                    bestDist = dist
                    bestIdx = UInt8(j)
                    if dist == 0 { break }
                }
            }
            indices[i] = bestIdx
        }
        return indices
    }

    /// Write an indexed-color PNG using CGImageDestination
    private static func writeIndexedPNG(indices: [UInt8], width: Int, height: Int, palette: [(UInt8, UInt8, UInt8)]) -> Data? {
        let baseSpace = CGColorSpaceCreateDeviceRGB()
        var colorTable = [UInt8]()
        colorTable.reserveCapacity(palette.count * 3)
        for c in palette {
            colorTable.append(c.0); colorTable.append(c.1); colorTable.append(c.2)
        }

        guard let indexedSpace = CGColorSpace(
            indexedBaseSpace: baseSpace,
            last: palette.count - 1,
            colorTable: &colorTable
        ) else { return nil }

        guard let provider = CGDataProvider(data: Data(indices) as CFData) else { return nil }
        guard let indexedImage = CGImage(
            width: width, height: height,
            bitsPerComponent: 8, bitsPerPixel: 8, bytesPerRow: width,
            space: indexedSpace,
            bitmapInfo: CGBitmapInfo(rawValue: 0),
            provider: provider,
            decode: nil, shouldInterpolate: false,
            intent: .defaultIntent
        ) else { return nil }

        let data = NSMutableData()
        guard let dest = CGImageDestinationCreateWithData(
            data, UTType.png.identifier as CFString, 1, nil
        ) else { return nil }
        CGImageDestinationAddImage(dest, indexedImage, nil)
        guard CGImageDestinationFinalize(dest) else { return nil }

        return data as Data
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
