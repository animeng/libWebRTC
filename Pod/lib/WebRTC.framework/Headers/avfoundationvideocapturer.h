/*
 *  Copyright 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_API_OBJC_AVFOUNDATION_VIDEO_CAPTURER_H_
#define WEBRTC_API_OBJC_AVFOUNDATION_VIDEO_CAPTURER_H_

#import <AVFoundation/AVFoundation.h>
#import <CoreMedia/CoreMedia.h>
#import <CoreVideo/CoreVideo.h>
#include "webrtc/media/base/videocapturer.h"
#include "webrtc/video_frame.h"

@class HBCapturePipeline;

namespace rtc {
class Thread;
}  // namespace rtc

namespace webrtc {

class AVFoundationVideoCapturer : public cricket::VideoCapturer,
                                  public rtc::MessageHandler {
 public:
  AVFoundationVideoCapturer();
  ~AVFoundationVideoCapturer();

  cricket::CaptureState Start(const cricket::VideoFormat& format) override;
  void Stop() override;
  bool IsRunning() override;
  bool IsScreencast() const override {
    return false;
  }
  bool GetPreferredFourccs(std::vector<uint32_t> *fourccs) override {
    fourccs->push_back(cricket::FOURCC_NV12);
    return true;
  }

  // Returns the active capture session. Calls to the capture session should
  // occur on the RTCDispatcherTypeCaptureSession queue in RTCDispatcher.
  AVCaptureSession* GetCaptureSession();

  // Returns whether the rear-facing camera can be used.
  // e.g. It can't be used because it doesn't exist.
  bool CanUseBackCamera() const;

  // Switches the camera being used (either front or back).
  void SetUseBackCamera(bool useBackCamera);
  bool GetUseBackCamera() const;

  // Converts the sample buffer into a cricket::CapturedFrame and signals the
  // frame for capture.
  void CaptureSampleBuffer(CVPixelBufferRef pixelBufferRef);

  // Handles messages from posts.
  void OnMessage(rtc::Message *msg) override;

  // capture pipeline
  HBCapturePipeline *capturePipeline;
 private:
  void OnFrameMessage(CVImageBufferRef image_buffer, int64_t capture_time);

//  RTCAVFoundationVideoCapturerInternal *_capturer;
  rtc::Thread *_startThread;  // Set in Start(), unset in Stop().
};  // AVFoundationVideoCapturer

}  // namespace webrtc

#pragma mark - render protocol
@protocol HBRenderProtocol <NSObject>

@required

/* Format/Processing Requirements */
@property(nonatomic, readonly) BOOL operatesInPlace; // When YES the input pixel buffer is written to by the renderer instead of writing the result to a new pixel buffer.
@property(nonatomic, readonly) FourCharCode inputPixelFormat; // One of 420f, 420v, or BGRA

/* Resource Lifecycle */
// Prepare and destroy expensive resources inside these callbacks.
// The outputRetainedBufferCountHint tells out of place renderers how many of their output buffers will be held onto by the downstream pipeline at one time.
// This can be used by the renderer to size and preallocate their pools.
- (void)prepareForInputWithFormatDescription:(CMFormatDescriptionRef)inputFormatDescription outputRetainedBufferCountHint:(size_t)outputRetainedBufferCountHint;
- (void)reset;

/* Rendering */
// Renderers which operate in place should return the input pixel buffer with a +1 retain count.
// Renderers which operate out of place should create a pixel buffer to return from a pool they own.
// When rendering to a pixel buffer with the GPU it is not necessary to block until rendering has completed before returning.
// It is sufficient to call glFlush() to ensure that the commands have been flushed to the GPU.
- (CVPixelBufferRef)copyRenderedPixelBuffer:(CVPixelBufferRef)pixelBuffer;

@optional

// This property must be implemented if operatesInPlace is NO and the output pixel buffers have a different format description than the input.
// If implemented a non-NULL value must be returned once the renderer has been prepared (can be NULL after being reset).
@property(nonatomic, readonly) CMFormatDescriptionRef __attribute__((NSObject)) outputFormatDescription;

@end

#pragma mark - capture pipeline
@protocol HBCapturePipelineDelegate <NSObject>
@required

- (void)capturePipeline:(HBCapturePipeline *)capturePipeline didStopRunningWithError:(NSError *)error;

// Preview
- (void)capturePipeline:(HBCapturePipeline *)capturePipeline previewPixelBufferReadyForDisplay:(CVPixelBufferRef)previewPixelBuffer;
- (void)capturePipelineDidRunOutOfPreviewBuffers:(HBCapturePipeline *)capturePipeline;
@optional
// output
- (void)capturePipeline:(HBCapturePipeline *)capturePipeline didOutputPixelBuffer:(CVPixelBufferRef)renderedPixelBuffer presentationTime:(CMTime)presentationTime;
@end

@interface HBCapturePipeline : NSObject
@property (nonatomic, strong) AVCaptureSession *captureSession;

@property (nonatomic, strong) id<HBRenderProtocol> renderer;

- (instancetype)initWithCapturer:(webrtc::AVFoundationVideoCapturer *)capturer;

// delegate is weak referenced
- (void)setDelegate:(id<HBCapturePipelineDelegate>)delegate callbackQueue:(dispatch_queue_t) delegateCallbackQueue;

/// start capture running
- (void)startRunning;
/// stop capture running
- (void)stopRunning;

/// When set to false the GPU will not be used after the setRenderingEnabled: call returns.
@property(nonatomic) BOOL renderingEnabled;

/// back / front camera switch
- (BOOL)canUseBackCamera;
- (BOOL)useBackCamera;
- (void)setUseBackCamera:(BOOL)useBackCamera;

// Stats
@property(readonly, nonatomic) float videoFrameRate;
@property(readonly, nonatomic) CMVideoDimensions videoDimensions;

// compute transform
- (CGAffineTransform)transformFromVideoBufferOrientationToOrientation:(AVCaptureVideoOrientation)orientation
                                                    withAutoMirroring:(BOOL)mirroring;

@end

#endif  // WEBRTC_API_OBJC_AVFOUNDATION_VIDEO_CAPTURER_H_
