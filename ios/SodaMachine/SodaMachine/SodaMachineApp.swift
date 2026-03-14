import SwiftUI

@main
struct SodaMachineApp: App {
    @StateObject private var bleManager = BLEManager()

    var body: some Scene {
        WindowGroup {
            ScanView()
                .environmentObject(bleManager)
        }
    }
}
