// swift-tools-version:5.7
import PackageDescription
let package = Package(
    name: "VerySDK",
    platforms: [.iOS(.v13)],
    products: [
        .library(name: "VerySDK", targets: ["VerySDKWrapper"]),
    ],
    dependencies: [
        .package(url: "https://github.com/airbnb/lottie-spm.git", from: "4.4.0"),
    ],
    targets: [
        .target(
            name: "VerySDKWrapper",
            dependencies: [
                "VerySDKBinary",
                "PalmAPISaas",
                .product(name: "Lottie", package: "lottie-spm"),
            ],
            path: "Sources"
        ),
        .binaryTarget(name: "VerySDKBinary", path: "VerySDK.xcframework"),
        .binaryTarget(name: "PalmAPISaas", path: "PalmAPISaas.xcframework"),
    ]
)
