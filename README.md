# VerySDK

[![iOS](https://img.shields.io/badge/iOS-13.0+-blue.svg)](https://developer.apple.com/ios/)
[![Android](https://img.shields.io/badge/Android-API%2023+-green.svg)](https://developer.android.com/)
[![Swift](https://img.shields.io/badge/Swift-5.0+-orange.svg)](https://swift.org/)
[![Kotlin](https://img.shields.io/badge/Kotlin-1.7+-purple.svg)](https://kotlinlang.org/)

Native palm biometric SDK for iOS and Android. Provides session-based palm enrollment and verification with on-device scanning, liveness detection, and Ed25519-signed result tokens.

## Installation

### iOS — Swift Package Manager

```swift
dependencies: [
    .package(url: "https://github.com/nicklin99/very-sdk.git", from: "1.0.0")
]
```

### iOS — CocoaPods

```ruby
pod 'VerySDK', '~> 1.0.0'
```

### Android — Gradle

```gradle
dependencies {
    implementation 'org.very:verysdk:1.0.0'
}
```

## Quick Start

### iOS (Swift)

```swift
import VerySDK

let config = VeryConfig(
    sdkKey: "your_sdk_key",
    userId: nil,        // nil → enrollment, non-nil → verification
    language: "en",
    themeMode: "dark"
)

VerySDK.authenticate(from: self, config: config) { result in
    if result.isSuccess {
        print("userId: \(result.userId)")
        print("signedToken: \(result.signedToken ?? "")")
    } else {
        print("error [\(result.error ?? "")]: \(result.errorMessage ?? "")")
    }
}
```

### iOS (Objective-C)

```objc
#import <VerySDK/VerySDK-Swift.h>

VeryConfig *config = [[VeryConfig alloc] init];
config.sdkKey = @"your_sdk_key";

[VerySDK authenticateFrom:self config:config completion:^(VeryResult *result) {
    if (result.isSuccess) {
        NSLog(@"userId: %@, signedToken: %@", result.userId, result.signedToken);
    } else {
        NSLog(@"error [%@]: %@", result.error, result.errorMessage);
    }
}];
```

### Android (Kotlin)

```kotlin
import org.very.sdk.*

val config = VeryConfig(
    sdkKey = "your_sdk_key",
    userId = null,       // null → enrollment, non-null → verification
    language = "en",
    themeMode = "dark"
)

VerySDK.authenticate(
    context = this,
    config = config,
    presentationStyle = VeryPresentationStyle.FULL_SCREEN  // or BOTTOM_SHEET
) { result ->
    if (result.isSuccess) {
        println("userId: ${result.userId}")
        println("signedToken: ${result.signedToken}")
    } else {
        println("error [${result.error}]: ${result.errorMessage}")
    }
}
```

### Android (Java)

```java
import org.very.sdk.*;

VeryConfig config = new VeryConfig("your_sdk_key");

VerySDK.authenticate(this, config, VeryPresentationStyle.FULL_SCREEN, result -> {
    if (result.isSuccess()) {
        Log.d("Very", "userId: " + result.getUserId());
    } else {
        Log.e("Very", "error: " + result.getError());
    }
});
```

## API Reference

### VeryConfig

| Parameter   | Type    | Required | Default        | Description                                          |
|-------------|---------|----------|----------------|------------------------------------------------------|
| `sdkKey`    | String  | Yes      | —              | SDK API key provided by Very                         |
| `userId`    | String? | No       | `nil` / `null` | `nil` → enrollment, existing ID → verification      |
| `language`  | String? | No       | `"en"`         | UI language code                                     |
| `themeMode` | String  | No       | `"dark"`       | `"dark"` or `"light"`                                |
| `baseUrl`   | String? | No       | production     | Override API base URL for staging/testing             |

### VeryPresentationStyle

| Style          | iOS                  | Android              |
|----------------|----------------------|----------------------|
| `modal` / `FULL_SCREEN` | PageSheet with blur  | Full-screen Activity |
| `push`         | Push onto nav stack  | —                    |
| `embed`        | Child view controller| —                    |
| `BOTTOM_SHEET` | —                    | Bottom sheet with dimmed background, drag-to-dismiss |

### VeryResult

| Field         | Type    | Description                                                    |
|---------------|---------|----------------------------------------------------------------|
| `isSuccess`   | Bool    | `true` if enrollment or verification succeeded                 |
| `code`        | String  | `"success"`, `"cancelled"`, or `"error"`                       |
| `userId`      | String  | User's external ID (returned on success)                       |
| `signedToken` | String? | Ed25519-signed JWT proving the result                          |
| `error`       | String? | Error code (e.g. `"9004"` for user cancelled)                  |
| `errorMessage`| String? | Human-readable error description                               |

## Enrollment vs Verification

| Flow           | `userId`         | Scans | Result                    |
|----------------|------------------|-------|---------------------------|
| **Enrollment** | `nil` / `null`   | 4     | New `userId` + `signedToken` returned |
| **Verification** | existing user ID | 1   | `signedToken` returned on match |

- **Enrollment**: The SDK guides the user through a 4-palm scan (2 left + 2 right). On success, a new `userId` is returned. Store this for future verifications.
- **Verification**: A single palm scan is performed and verified against enrolled palms. Supports retry with remaining attempts.

## Email Bypass (Partner Configuration)

Some partners already have verified user emails and want to skip the SDK's email input + OTP screens. This is controlled by a **backend flag** on the partner's app record, not by the SDK configuration.

When email bypass is enabled for your app:
- The session creation response includes `"skipEmail": true`
- The SDK skips the email input and OTP verification screens, going directly to consent and palm scan
- Calling the email endpoints returns error code `5018`

To enable email bypass for your app, contact VeryAI. This is not self-serve.

## Account Restriction

If a user's account has been flagged (e.g., due to fraud detection), SDK operations will return error code `5017` (`SDK_ACCOUNT_RESTRICTED`). The SDK should display an appropriate message and prevent further enrollment or verification attempts.

## Signed Token

On success, the SDK returns a `signedToken` — an Ed25519-signed JWT that cryptographically proves the enrollment or verification result. Forward this to your backend for server-side validation.

## Error Codes

| Range       | Category              | Examples                                    |
|-------------|-----------------------|---------------------------------------------|
| 1001–1040   | API Service           | Internal error, palm match failure, invalid request |
| 2001–2011   | User Service          | User not found, email in use                |
| 3000        | Palm Matching Service | Internal server error                       |
| 4000        | Connect Service       | Internal server error                       |
| 5001–5014   | SDK Service           | Session expired, enrollment failed, API key invalid |
| 5015        | SDK Email             | Email verification required before palm operations |
| 5016        | SDK Email             | Email already verified for this session     |
| 5017        | SDK Account           | Account restricted — blocked from all operations |
| 5018        | SDK Email             | Email flow disabled (partner has `skipEmail` enabled) |
| 9001–9006   | Client-side           | Camera permission denied, capture failed, user cancelled, timeout |

## Platform Setup

### iOS

Add to `Info.plist`:

```xml
<key>NSCameraUsageDescription</key>
<string>Camera access is required for palm scanning.</string>
```

### Android

The SDK declares these permissions in its manifest (merged automatically):

```xml
<uses-permission android:name="android.permission.CAMERA" />
<uses-permission android:name="android.permission.INTERNET" />
```

## Supported Languages

36 languages: English, Chinese (Simplified / Traditional / Hong Kong), Japanese, Korean, French, German, Spanish, Portuguese, Italian, Dutch, Russian, Arabic, Turkish, Vietnamese, Indonesian, Filipino, Swedish, Danish, Polish, Romanian, Hungarian, Czech, Slovak, Slovenian, Bulgarian, Ukrainian, Greek, Latvian, Persian, Azerbaijani, Kazakh, Lao, Sinhala, English (India).

Set via `config.language` using codes: `en`, `zh`, `zhTw`, `zhHk`, `ja`, `ko`, `fr`, `de`, `es`, `pt`, `it`, `nl`, `ru`, `ar`, `tr`, `vi`, `id`, `fil`, `sv`, `da`, `pl`, `ro`, `hu`, `cs`, `sk`, `sl`, `bg`, `uk`, `el`, `lv`, `fa`, `az`, `kk`, `lo`, `si`, `enIn`.

## Requirements

| Platform | Minimum | Architecture        |
|----------|---------|---------------------|
| iOS      | 13.0+   | arm64               |
| Android  | API 23+ | armeabi-v7a, arm64-v8a |

## Example Projects

| Platform    | Location                     | Language      |
|-------------|------------------------------|---------------|
| iOS (Swift) | `examples/IOSMobileExample/` | Swift / SwiftUI |
| iOS (ObjC)  | `examples/IOSExampleOC/`     | Objective-C   |
| Android     | `examples/android/`          | Kotlin        |

## License

Proprietary. Contact [support@very.org](mailto:support@very.org) for licensing.
