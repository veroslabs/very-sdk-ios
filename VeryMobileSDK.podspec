Pod::Spec.new do |s|
  s.name             = 'VeryMobileSDK'
  s.version          = '1.0.3'
  s.summary          = 'Very Mobile SDK - Palm biometrics verification for iOS'
  s.description      = <<-DESC
  VeryMobileSDK provides palm biometrics verification for iOS applications.
  Scan and verify user identity using palm recognition technology.
                       DESC
  s.homepage         = 'https://very.org'
  s.license          = { :type => 'Commercial', :text => 'Copyright (c) Very Mobile Inc. All rights reserved.' }
  s.author           = { 'Very Mobile Inc.' => 'mail@very.org' }
  s.source           = { :git => 'https://github.com/veroslabs/very-mobile-sdk-ios.git', :tag => s.version.to_s }
  s.platform         = :ios, '13.0'
  s.swift_version    = '5.0'

  s.vendored_frameworks = 'VeryMobileSDK.xcframework', 'PalmAPI.xcframework'

  s.pod_target_xcconfig = {
    'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'i386 arm64',
  }
  s.user_target_xcconfig = {
    'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'i386 arm64',
  }
end
