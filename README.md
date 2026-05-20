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
    .package(url: "https://github.com/veroslabs/very-sdk-ios.git", from: "1.0.0")
]
```

### iOS — CocoaPods

```ruby
pod 'VerySDK', '~> 1.0.0'
```

### Android — Gradle

```gradle
dependencies {
    implementation 'org.very:sdk:1.0.0'
}
```

### React Native — npm

```bash
npm install @veryai/react-native-sdk
cd ios && pod install
```

See [`react-native/README.md`](react-native/README.md) for full RN integration docs.

## Asset Loading: Bundled vs Slim

The native palm-recognition asset (~8 MB on iOS, ~18 MB per ABI on Android) can either ship inside the SDK (instant first scan, larger binary) or be fetched from CDN on first scan (smaller binary, ~5–15 s one-time download). Defaults differ per artifact:

| Artifact                                                       | Default | Rationale                                              |
|----------------------------------------------------------------|---------|--------------------------------------------------------|
| `pod 'VerySDK'` / `org.very:sdk` (full SDK)                    | Bundled | Auth flow needs offline-capable first scan             |
| `pod 'VeryAILiveness'` / `org.very:liveness` (liveness-only)   | Slim    | Liveness partners optimize for smallest install size   |

Cached assets persist across launches; the download only happens once.

**Switch the full SDK to slim:**

```ruby
# iOS — CocoaPods
pod 'VerySDK/Core'
```

```swift
// iOS — SPM
.product(name: "VerySDKSlim", package: "very-sdk-ios")
```

```gradle
// Android — in your app build.gradle
android {
    packaging {
        jniLibs {
            excludes += '**/libPalmAPISaas.so'
        }
    }
}
```

**Switch the liveness SDK to bundled:**

```ruby
# iOS — CocoaPods
pod 'VeryAILiveness/Bundled'
```

For Android `org.very:liveness`, add the companion `sdk-native-bundle` artifact:

```gradle
dependencies {
    implementation 'org.very:liveness:X.Y.Z'
    // Drops libPalmAPISaas.so for both ABIs into your APK (+18 MB per ABI).
    implementation 'org.very:sdk-native-bundle:X.Y.Z'
}
```

> **Don't combine `org.very:sdk-native-bundle` with `org.very:sdk`** — the full SDK already bundles the same `.so`, and adding both produces a jniLibs merge conflict at AGP packaging time.

Alternatively, drop a matching `libPalmAPISaas.so` into your **app's** `src/main/jniLibs/<abi>/` manually — AGP merges it into the APK and the SDK picks it up automatically.

**CDN endpoints** (allowlist if your network restricts egress):

| Asset                                | Primary                                       | Backup                                          |
|--------------------------------------|-----------------------------------------------|-------------------------------------------------|
| Android `libPalmAPISaas.so` per ABI  | `assets.very.org/sdk/v2/<abi>/libPalmAPISaas.so` | `r2.assets.very.org/sdk/v2/<abi>/libPalmAPISaas.so` |
| iOS `packed_data.bin`                | `assets.very.org/sdk/v2/packed_data.bin`      | `r2.assets.very.org/sdk/v2/sdk_packed_data.bin` |

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

[VerySDK authenticateFrom:self config:config presentationStyle:VeryPresentationStyleModal completion:^(VeryResult *result) {
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
| 6001–6006, 6999 | Network / client   | No internet, timeout, DNS, TLS, server error, unknown |
| 6101–6104   | SDK state             | Camera permission denied, capture failed, session expired, user cancelled |
| 6106        | Native asset          | Could not load palm scanning library (slim-mode CDN download failed) |

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

The SDK ships with **36 localizations**. Set the language via `config.language` using one of the codes below. Codes are case-insensitive, and BCP-47 region tags (e.g. `en-US`, `pt-BR`) are accepted as aliases. Unknown or empty codes fall back to English.

| Language               | Native name        | Code     | Accepted aliases          |
|------------------------|--------------------|----------|---------------------------|
| English                | English            | `en`     | `en-US`, `en-GB`, `en-TR` |
| English (India)        | English (India)    | `enIn`   | `en-IN`                   |
| Chinese (Simplified)   | 简体中文           | `zh`     | `zh-MY`                   |
| Chinese (Traditional)  | 繁體中文           | `zhTw`   | `zh-TW`                   |
| Chinese (Hong Kong)    | 繁體中文（香港）   | `zhHk`   | `zh-HK`                   |
| Japanese               | 日本語             | `ja`     | `ja-JP`                   |
| Korean                 | 한국어             | `ko`     | `ko-KR`                   |
| French                 | Français           | `fr`     | `fr-FR`                   |
| German                 | Deutsch            | `de`     | `de-DE`                   |
| Spanish                | Español            | `es`     | `es-ES`                   |
| Portuguese             | Português          | `pt`     | `pt-BR`, `pt-PT`          |
| Italian                | Italiano           | `it`     | `it-IT`                   |
| Dutch                  | Nederlands         | `nl`     | `nl-NL`                   |
| Russian                | Русский            | `ru`     | `ru-RU`                   |
| Arabic                 | العربية            | `ar`     | `ar-AE`                   |
| Turkish                | Türkçe             | `tr`     | `tr-TR`, `tr-CT`          |
| Vietnamese             | Tiếng Việt         | `vi`     | `vi-VN`                   |
| Indonesian             | Bahasa Indonesia   | `id`     | `id-ID`                   |
| Filipino               | Filipino           | `fil`    | `fil-PH`                  |
| Swedish                | Svenska            | `sv`     | `sv-SE`                   |
| Danish                 | Dansk              | `da`     | `da-DK`                   |
| Polish                 | Polski             | `pl`     | `pl-PL`                   |
| Romanian               | Română             | `ro`     | `ro-RO`                   |
| Hungarian              | Magyar             | `hu`     | `hu-HU`                   |
| Czech                  | Čeština            | `cs`     | `cs-CZ`                   |
| Slovak                 | Slovenčina         | `sk`     | `sk-SK`                   |
| Slovenian              | Slovenščina        | `sl`     | `sl-SI`                   |
| Bulgarian              | Български          | `bg`     | `bg-BG`                   |
| Ukrainian              | Українська         | `uk`     | `uk-UA`                   |
| Greek                  | Ελληνικά           | `el`     | `el-GR`                   |
| Latvian                | Latviešu           | `lv`     | `lv-LV`                   |
| Persian                | فارسی              | `fa`     | `fa-IR`                   |
| Azerbaijani            | Azərbaycanca       | `az`     | `az-AZ`                   |
| Kazakh                 | Қазақша            | `kk`     | `kk-KZ`                   |
| Lao                    | ລາວ                | `lo`     | `lo-LA`                   |
| Sinhala                | සිංහල              | `si`     | `si-LK`                   |

**Resource locations**

| Platform | Default (English)                       | Other locales                                         |
|----------|-----------------------------------------|-------------------------------------------------------|
| Android  | `android/src/main/res/values/strings.xml` | `values-{lang}/strings.xml` (e.g. `values-zh-rTW`)  |
| iOS      | `ios/Resources/en.lproj/Localizable.strings` | `{lang}.lproj/Localizable.strings` (e.g. `zhTw.lproj`) |

**Notes**
- Android uses `snake_case` keys; iOS uses `camelCase.dotSeparated`. The English file is authoritative — any text change must update English plus all 35 other locale files on both platforms.
- Language mapping is implemented in `LocalizationManager` on both platforms; add new aliases there, not at call sites.

## Requirements

| Platform | Minimum | Architecture        |
|----------|---------|---------------------|
| iOS      | 13.0+   | arm64               |
| Android  | API 23+ | armeabi-v7a, arm64-v8a |

## Example Projects

| Platform     | Location                          | Language        |
|--------------|-----------------------------------|-----------------|
| iOS (Swift)  | `examples/IOSMobileExample/`      | Swift / SwiftUI |
| iOS (ObjC)   | `examples/IOSExampleOC/`          | Objective-C     |
| Android      | `examples/android/`               | Kotlin          |
| React Native | `examples/ReactNativeExample/`    | TypeScript      |
| Expo         | `examples/ExpoNewArchExample/`    | TypeScript      |

## License

Proprietary. Contact [support@very.org](mailto:support@very.org) for licensing.
