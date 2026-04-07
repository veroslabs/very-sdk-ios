// swift-tools-version:5.7
import PackageDescription
let package = Package(
    name: "VeryMobileSDK",
    platforms: [.iOS(.v13)],
    products: [
        .library(name: "VeryMobileSDK", targets: ["VeryMobileSDKWrapper"]),
    ],
    dependencies: [
        .package(url: "https://github.com/airbnb/lottie-spm.git", from: "4.4.0"),
    ],
    targets: [
        .target(
            name: "VeryMobileSDKWrapper",
            dependencies: [
                "VeryMobileSDKBinary",
                "PalmAPISaas",
                .product(name: "Lottie", package: "lottie-spm"),
            ],
            path: "Sources"
        ),
        .binaryTarget(name: "VeryMobileSDKBinary", path: "VeryMobileSDK.xcframework"),
        .binaryTarget(name: "PalmAPISaas", path: "PalmAPISaas.xcframework"),
    ]
)
