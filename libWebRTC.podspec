#
# Be sure to run `pod lib lint webrtcLib.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html
#
Pod::Spec.new do |s|
  s.name             = 'libWebRTC'
  s.version          = '1.0.2'
  s.summary          = 'static library of webrtc.'

# This description is used to generate tags and improve search results.
#   * Think: What does it do? Why did you write it? What is the focus?
#   * Try to keep it short, snappy and to the point.
#   * Write the description between the DESC delimiters below.
#   * Finally, don't worry about the indent, CocoaPods strips it!

  s.description      = <<-DESC
webrtc(google opensource) is p2p video chat framework
                       DESC

  s.homepage         = 'https://github.com/animeng/libWebRTC'
  s.license          = 'MIT'
  s.author           = { 'wang animeng' => 'animeng68@gmail.com' }
  s.source           = { :git => 'https://github.com/animeng/libWebRTC.git' }
  s.social_media_url = 'http://weibo.com/mengtnt'
  s.platform     = :ios, "8.0"

  s.xcconfig = { 
    'HEADER_SEARCH_PATHS' => '"${PODS_ROOT}/Pod/**"', 
    'GCC_PREPROCESSOR_DEFINITIONS' => "WEBRTC_POSIX LOGGING=1 FEATURE_ENABLE_SSL SYSTEM_NATIVELY_SIGNALS_MEMORY_PRESSURE IOS WEBRTC_IOS CARBON_DEPRECATED=YES USE_OPENSSL=1 NDEBUG NVALGRIND",
    'OTHER_LDFLAGS' => '-ObjC'
  }
  s.source_files = [
    "Pod/webrtc/video_frame.h", 
    "Pod/webrtc/typedefs.h",
    "Pod/webrtc/common_types.h",
    "Pod/webrtc/base/*.h", 
    "Pod/webrtc/media/base/*.h",
    "Pod/webrtc/media/engine/*.h",
    "Pod/webrtc/common_video/include/*.h",
    "Pod/webrtc/common_video/*.h",
    "Pod/webrtc/system_wrappers/include/*.h",
    "Pod/webrtc/modules/audio_device/ios/objc/RTCAudioSession.h",
    "Pod/webrtc/modules/audio_device/ios/objc/RTCAudioSessionConfiguration.h",
    ]
  s.vendored_frameworks = "Pod/lib/WebRTC.framework"
  s.public_header_files = [
    "Pod/lib/WebRTC.framework/Headers/**/*.h", 
    "Pod/webrtc/modules/audio_device/ios/objc/RTCAudioSession.h",
    "Pod/webrtc/modules/audio_device/ios/objc/RTCAudioSessionConfiguration.h"
  ]
  s.library = 'icucore','c++','stdc++.6','sqlite3'
  s.frameworks = 'UIKit','Security','CFNetwork','GLKit','AudioToolbox','AVFoundation','CoreAudio','CoreMedia','CoreVideo','CoreGraphics','OpenGLES','QuartzCore'
  s.module_name = 'WebRTC'
  s.module_map = "Pod/lib/WebRTC.framework/Modules/module.modulemap"
  s.requires_arc     = true
end
