#ifndef RTC_BASE_H
#define RTC_BASE_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <string>
#include <list>


#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <windef.h>
    #define RTC_CALL __cdecl
    
    #if defined(RTCSDK_EXPORTS)
        #define RTCSDKDLL_API __declspec(dllexport)
    #else
        #define RTCSDKDLL_API __declspec(dllimport)
    #endif
#elif defined(__APPLE__)
    //#define RTCSDKDLL_API __attribute__((visibility("default"))) extern "C"
    #define RTCSDKDLL_API
    #define RTC_CALL
#elif defined(__ANDROID__) || defined(__linux__)
    #define RTCSDKDLL_API extern "C" __attribute__((visibility("default")))
    #define RTC_CALL
#else
    #define RTCSDKDLL_API extern "C"
    #define RTC_CALL
#endif

namespace rtc {    
    typedef unsigned int UserId;
    
    const unsigned MAX_DEVICE_ID_LENGTH = 512;
    const unsigned MAX_DEVICE_COUNT = 16;
#if defined(__APPLE__)
    typedef void* WindowIdType;
#else
    typedef HWND WindowIdType;
#endif

    enum RTCEngineType {
        RTCEngine_Agora = 1,
        RTCEngine_XES = 3,
        RTCEngine_XES_ALI = 5,
        RTCEngine_TTT = 7
    };
    enum RTCLOGTYPE 
    {
        RTCLOGTYPE_DEBUG = 0,
        RTCLOGTYPE_INFO,
        RTCLOGTYPE_WARN,
        RTCLOGTYPE_ERROR,
        RTCLOGTYPE_FATAL
    };
    /**
    * @brief sdk event handler onError return-code
    */
    enum RTC_RET_CODE
    {            
        RTC_SUCCESS                        = 0,    /** RTC_OK  success*/
        RTC_FAILED                        = 1,    /** RTC_ERR_FAILED */
        RTC_WARN                        = 2,    /** other warning*/
        /// 基本的错误信息，如果用到，需要说明场景
        RTC_ERR_INVALID_ARGUMENT        = 3,
        RTC_ERR_NOT_READY                = 4,
        RTC_ERR_NOT_SUPPORTED            = 5,
        RTC_ERR_REFUSED                    = 6,
        RTC_ERR_INVALID_TOKEN            = 7,    /**invalid token*/
        RTC_ERR_TOKEN_EXPIRED            = 8,    /**token is expired, maybe handle this error event when join room*/
        RTC_ERR_NOT_INITIALIZED            = 9,
        RTC_ERR_NO_PERMISSION            = 10,
        RTC_ERR_ACCOUNT_DISABLED        = 11,
        RTC_ERR_SERVER_ERROR            = 12,
        RTC_ERR_CONNECTION_INTERRUPTED    = 13,    /**connection is lost*/
        RTC_ERR_INVALID_APP_ID            = 14,
        RTC_ERR_REPEAT                    = 15,    /** case 1. join room repeat;*/
        RTC_ERR_SEND_SEI_ERROR            = 16,


        // device related
        RTC_ERR_VIDEO_DEVICE            = 23,    /**video related error*/
        RTC_ERR_AUDIO_DEVICE            = 24,    /**audio related error*/

        RTC_ERR_NO_RECORDING_DEVICE        = 26,    /**no mic*/
        RTC_ERR_NO_PLAYOUT_DEVICE        = 27,    /**no speaker*/

        RTC_ERR_START_CAMERA            = 50,    /**cann't start camera(maybe occupyed) */
        RTC_ERR_START_VIDEO_RENDER        = 51,    /**can not display video*/


        // join/leave room
        RTC_ERR_ROOM_TIMEOUT            = 1050, /**timeout to join room, maybe user network is poor, cannot connect to room server.*/
        RTC_ERR_RESOURCE_UNAVAILABLE    = 1051,    /** no enough resource for user, for example, no room resource.*/

        RTC_ERR_ROOM_KICKED                = 1060, /**user is kicked off from this room*/
        

        RTC_ERR_PUBLISH_FAILED            = 1080, /**fail to publish stream*/
        RTC_ERR_REPUBLISH_FAILED        = 1081,
        RTC_ERR_SUBSCRIBE_FAILED        = 1082,
        RTC_ERR_RESUBSCRIBE_FAILED      = 1083,
        

        // video control
        RTC_ERR_ENABLE_VIDEO            = 1100,
        
        // audio control
        RTC_ERR_ENABLE_AUDIO            = 1120,    
    };

    /** Media device state.
*/
    enum RTC_MEDIA_DEVICE_STATE_TYPE
    {
        /** 1: The device is active. 使用中
        */
        RTC_MEDIA_DEVICE_STATE_ACTIVE = 1,
        /** 2: The device is disabled. 被禁用
        */
        RTC_MEDIA_DEVICE_STATE_DISABLED = 2,
        /** 4: The device is not present. 一开始就没有
        */
        RTC_MEDIA_DEVICE_STATE_NOT_PRESENT = 4,
        /** 8: The device is unplugged. 被拔出
        */
        RTC_MEDIA_DEVICE_STATE_UNPLUGGED = 8
    };

    /** Media device type.
    */
    enum RTC_MEDIA_DEVICE_TYPE
    {
        /** -1: Unknown audio device type.  未知
        */
        RTC_UNKNOWN_AUDIO_DEVICE = -1,
        /** 0: Audio playback device.    播放器
        */
        RTC_AUDIO_PLAYOUT_DEVICE = 0,
        /** 1: Audio recording device.  麦克风
        */
        RTC_AUDIO_RECORDING_DEVICE = 1,
        /** 2: Video renderer.  视频显示设备
        */
        RTC_VIDEO_RENDER_DEVICE = 2,
        /** 3: Video capturer.  视频捕捉设备（摄像头）
        */
        RTC_VIDEO_CAPTURE_DEVICE = 3        
    };


    
    enum RTC_VIDEO_RENDER_MODE
    {        
        RTC_VIDEO_RENDER_MODE_HIDDEN = 1,    // 如果视频大小与显示窗口不同，大则裁剪小则拉伸以达到适配的目的。
        RTC_VIDEO_RENDER_MODE_FIT = 2,        // 填充，此模式下画面无黑边，但是会裁剪掉一部分超出渲染区域的部分，裁剪模式为居中裁剪        
    };
    
    enum CLIENT_ROLE_TYPE
    {
        /** 1: Host */
        CLIENT_ROLE_BROADCASTER = 1,
        /** 2: Audience */
        CLIENT_ROLE_AUDIENCE = 2,
    };


    /** The external video frame.
    */
    struct ExternalVideoFrame
    {
        /** The video buffer type.
        */
        enum RTC_VIDEO_BUFFER_TYPE
        {
            /** 1: The video buffer in the format of raw data.
            */
            VIDEO_BUFFER_RAW_DATA = 1,
        };

        /** The video pixel format.
        */
        enum RTC_VIDEO_PIXEL_FORMAT
        {
            /** 0: The video pixel format is unknown.
            */
            VIDEO_PIXEL_UNKNOWN = 0,
            /** 1: The video pixel format is I420.
            */
            VIDEO_PIXEL_I420 = 1,
            /** 2: The video pixel format is BGRA.
            */
            VIDEO_PIXEL_BGRA = 2,
            /** 8: The video pixel format is NV12.
            */
            VIDEO_PIXEL_NV12 = 8,
        };

        /** The buffer type. See #RTC_VIDEO_BUFFER_TYPE
        */
        RTC_VIDEO_BUFFER_TYPE type;
        /** The pixel format. See #RTC_VIDEO_PIXEL_FORMAT
        */
        RTC_VIDEO_PIXEL_FORMAT format;
        /** The video buffer.
        */
        void* buffer;
        /** Line spacing of the incoming video frame, which must be in pixels instead of bytes. For textures, it is the width of the texture.
        */
        int stride;
        /** Height of the incoming video frame.
        */
        int height;
        /** [Raw data related parameter] The number of pixels trimmed from the left. The default value is 0.
        */
        int cropLeft;
        /** [Raw data related parameter] The number of pixels trimmed from the top. The default value is 0.
        */
        int cropTop;
        /** [Raw data related parameter] The number of pixels trimmed from the right. The default value is 0.
        */
        int cropRight;
        /** [Raw data related parameter] The number of pixels trimmed from the bottom. The default value is 0.
        */
        int cropBottom;
        /** [Raw data related parameter] The clockwise rotation of the video frame. You can set the rotation angle as 0, 90, 180, or 270. The default value is 0.
        */
        int rotation;
        /** Timestamp of the incoming video frame (ms). An incorrect timestamp results in frame loss or unsynchronized audio and video.
        */
        long long timestamp;
    };
    struct YuvI420VideoFrame {        
        /** Video pixel width.
        */
        int width;  //width of video frame
                    /** Video pixel height.
                    */
        int height;  //height of video frame
                        /** Line span of the Y buffer within the YUV data.
                        */
        int yStride;  //stride of Y data buffer
                        /** Line span of the U buffer within the YUV data.
                        */
        int uStride;  //stride of U data buffer
                        /** Line span of the V buffer within the YUV data.
                        */
        int vStride;  //stride of V data buffer
                        /** Pointer to the Y buffer pointer within the YUV data.
                        */
        void* yBuffer;  //Y data buffer
                        /** Pointer to the U buffer pointer within the YUV data.
                        */
        void* uBuffer;  //U data buffer
                        /** Pointer to the V buffer pointer within the YUV data.
                        */
        void* vBuffer;  //V data buffer
                        /** Set the rotation of this frame before rendering the video. Supports 0, 90, 180, 270 degrees clockwise.
                        */
        int rotation; // rotation of this frame (0, 90, 180, 270)
                        /** Timestamp (ms) for the video stream render. Use this timestamp to synchronize the video stream render while rendering the video streams.

                        @note This timestamp is for rendering the video stream, and not for capturing the video stream.
                        */
        int64_t renderTimeMs;
        int avsync_type; /*audio/video sync mode, set to 0*/
    };
    
    class IRTCVideoFrameObserver {
    public:
        virtual ~IRTCVideoFrameObserver() {};

        /** Retrieves the camera captured image.

        @param videoFrame VideoFrame
        */
        virtual bool onCaptureVideoFrame(YuvI420VideoFrame& videoFrame) = 0;
        virtual bool onScaleRenderVideoFrame(YuvI420VideoFrame& videoFrame) = 0;
        /** Processes the received image of the specified user (post-processing).

        @param uid User ID of the specified user sending the image.
        @param videoFrame VideoFrame
        */
        virtual bool onRenderVideoFrame(UserId uid, YuvI420VideoFrame& videoFrame) = 0;
        /** Retrieves the Video image before encode.
        @param videoFrame VideoFrame
        */
        virtual bool onPreEncodeVideoFrame(YuvI420VideoFrame& videoFrame)
        { 
            (void)videoFrame;
            return true; 
        }
    };

    /** Definition of the rectangular region. */
    typedef struct RtcRect {
        /** Y-axis of the top line.
            */
        int top;
        /** X-axis of the left line.
            */
        int left;
        /** Y-axis of the bottom line.
            */
        int bottom;
        /** X-axis of the right line.
            */
        int right;

        RtcRect() : top(0), left(0), bottom(0), right(0) {}
        RtcRect(int t, int l, int b, int r) : top(t), left(l), bottom(b), right(r) {}
    } Rect;

    typedef struct SIZE {
        int width;
        int height;

        SIZE() :width(0), height(0) {}
        SIZE(int w, int h) :width(w), height(h) {}
    } SIZE;

    enum ScreenCaptureSourceType
    {
        /** -1: 未知 */
        ScreenCaptureSourceType_Unknown = -1,
        /** 0: 共享目标为某一个窗口 */
        ScreenCaptureSourceType_Window = 0,
        /** 1: 共享目标为某一个显示器的屏幕 */
        ScreenCaptureSourceType_Screen = -1,
        /** 2: 预留参数 */
        ScreenCaptureSourceType_Custom = 0,
    };

    typedef struct ThumbImageBuffer {
        const char* buffer;
        unsigned int length;
        unsigned int width;
        unsigned int height;

        ThumbImageBuffer(): buffer(NULL), length(0), width(0), height(0){}
    } ThumbImageBuffer;

    typedef void* view_t;
    typedef struct ScreenCaptureSourceInfo {
        ScreenCaptureSourceType type;
        view_t sourceId;
        const char* sourceName;
        ThumbImageBuffer thumbImage;
        ThumbImageBuffer iconImage;
        const char* processPath;
        const char* sourceTitle;
        bool primaryMonitor;

        ScreenCaptureSourceInfo() {}
    } ScreenCaptureSourceInfo;

    typedef struct Rectangle {
        int x;
        int y;
        int width;
        int height;

        Rectangle() :x(0), y(0), width(0), height(0) {}
        Rectangle(int xx, int yy, int ww, int hh) :x(xx), y(yy), width(ww), height(hh) {}
    } Rectangle;

    typedef struct VideoDimensions {
        int width;
        int height;

        VideoDimensions() :width(0), height(0) {}
        VideoDimensions(int w, int h) :width(w), height(h) {}
    } VideoDimensions;

    typedef struct ScreenCaptureParameters {
        VideoDimensions dimensions;
        int frameRate;
        int bitrate;
        bool captureMouseCursor;
        bool windowFocus;
        view_t* excludeWindowList;
        int excludeWindowCount;

        ScreenCaptureParameters() 
            : dimensions(1920, 1080)
            , frameRate(5)
            , bitrate(0)
            , captureMouseCursor(true)
            , windowFocus(false)
            , excludeWindowList(NULL)
            , excludeWindowCount(0)
        {}
        ScreenCaptureParameters(const VideoDimensions &d, int f, int b, bool c, bool focus, view_t *ex = NULL, int cnt = 0)
            : dimensions(d)
            , frameRate(f)
            , bitrate(b)
            , captureMouseCursor(c)
            , windowFocus(focus)
            , excludeWindowList(ex)
            , excludeWindowCount(cnt)
        {}
        ScreenCaptureParameters(int width, int height, int f, int b, bool c, bool focus, view_t *ex = NULL, int cnt = 0)
            : dimensions(width, height)
            , frameRate(f)
            , bitrate(b)
            , captureMouseCursor(c)
            , windowFocus(focus)
            , excludeWindowList(ex)
            , excludeWindowCount(cnt)
        {}
    } ScreenCaptureParameters;

    /** Definition of ExternalAudioFrame */
    struct ExternalAudioFrame {
        enum AUDIO_FRAME_TYPE {
            FRAME_TYPE_PCM16 = 0,  // PCM 16bit little endian
        };

        AUDIO_FRAME_TYPE type;
        /** Number of samples in the audio frame: samples = (int)samplesPerCall = (int)(sampleRate &times; sampleInterval)
        */
        int samples;  //number of samples in this frame
                        /** Number of bytes per audio sample. For example, each PCM audio sample usually takes up 16 bits (2 bytes).
                        */
        int bytesPerSample;  //number of bytes per sample: 2 for PCM16
                                /** Number of audio channels.
                                - 1: Mono
                                - 2: Stereo (the data is interleaved)
                                */
        int channels;  //number of channels (data are interleaved if stereo)
                        /** Sampling rate per second for the audio frame: 16000 or 48000 bps.
                        */
        int samplesPerSec;  //sampling rate
                            /** Audio frame data buffer. The valid data length is: samples &times; channels &times; bytesPerSample
                            */
        void* buffer;  //data buffer
                        /** Timestamp to render the audio stream.
                        */
        int64_t renderTimeMs;
        int avsync_type;    /*audio/video sync mode, set to 0*/
    };

    /** Stream fallback options. */
    enum STREAM_FALLBACK_OPTIONS
    {
        /** 0: No fallback behavior for the local/remote video stream when the uplink/downlink network conditions are poor. */
        STREAM_FALLBACK_OPTION_DISABLED = 0,
        /** 1: Under poor downlink network conditions, the remote video stream, to which you subscribe, falls back to the low-stream (low resolution and low bitrate) video. */
        STREAM_FALLBACK_OPTION_VIDEO_STREAM_LOW = 1,
        /** 2: Under poor uplink network conditions, the locally published video stream falls back to audio only.*/
        STREAM_FALLBACK_OPTION_AUDIO_ONLY = 2,
    };

    class IRTCAudioFrameObserver {
    public:
        virtual ~IRTCAudioFrameObserver() {};

        /**@brief 本机麦克风采集到的音频数据
        @note: 腾讯有但数据不可修改，声网有且可修改
        Callback occuring once every 10 ms. Retrieves the recorded audio frame.
        @param audioFrame Pointer to AudioFrame.
        @return
        - true: Valid buffer in AudioFrame, and the recorded audio frame is sent out.
        - false: Invalid buffer in AudioFrame, and the recorded audio frame ise discarded.
        */
        virtual bool onRecordAudioFrame(ExternalAudioFrame& audioFrame) = 0;
        /**@brief 各路音频数据混合后送入扬声器播放的音频数据
        @note: 腾讯有但数据不可修改，声网有且可修改
        Callback occuring once every 10 ms. Retrieves the audio playback frame.
        This callback returns after \ref agora::rtc::RtcEngineParameters::playEffect "playEffect" is successfully called.
        @param audioFrame Pointer to AudioFrame.
        @return
        - true: Valid buffer in AudioFrame, and the audio playback frame is sent out.
        - false: Invalid buffer in AudioFrame, and the audio playback frame is discarded.
        */
        virtual bool onPlaybackAudioFrame(ExternalAudioFrame& audioFrame) = 0;

        /**@brief 各路音频数据 以及 本机麦克采集的 混合后的音频数据 
        @note: 腾讯无，声网有且数据可修改
        Callback occuring every 10 ms. Retrieves the mixed recorded and playback audio frame.
        @note This method only returns the single-channel data.
        @param audioFrame Pointer to AudioFrame.
        @return
        - true: Valid buffer in AudioFrame and the mixed recorded and playback audio frame is sent out.
        - false: Invalid buffer in AudioFrame and the mixed recorded and playback audio frame is discarded.
        */
        virtual bool onMixedAudioFrame(ExternalAudioFrame& audioFrame) = 0;
        
        /**@brief  混音前的每一路远程用户的音频数据
        @note: 腾讯有但数据不可修改，声网有且可修改
        Callback occuring once every 10 ms. Retrieves the audio frame of a specified user before mixing.
        @param uid The user ID
        @param audioFrame Pointer to AudioFrame.
        @return
        - true: Valid buffer in AudioFrame, and the mixed recorded and playback audio frame is sent out.
        - false: Invalid buffer in AudioFrame, and the mixed recorded and playback audio frame is discarded.
        */
        virtual bool onPlaybackAudioFrameBeforeMixing(unsigned int uid,
                                                        ExternalAudioFrame& audioFrame) = 0;

        virtual bool onPlaybackAudioFrameBeforeMixingEx(const char *channelId,
            unsigned int uid, ExternalAudioFrame& audioFrame) {
            (void)channelId;
            (void)uid;
            (void)audioFrame;

            return false;
        }
    };


    /**
    * \brief：音频数据源类型
    */
    enum RTC_AUDIO_CAPTURE_SRC_TYPE
    {
        AUDIO_SRC_SDK_DATA = 1,                //音频采集源包括本地麦克风，本地播放器、系统等声音
        AUDIO_SRC_USER_PCM = 1001,            //音频采集源来自用户传入的PCM裸音频的数据
        AUDIO_SRC_USER_AAC = 1002,            //音频采集源来自用户传入AAC音频编码数据
    };


    /** @brief 统计信息
*/
    struct RTCStats
    {
        /**
            *@brief音频接收比特率（kbps），由瞬时值表示
            */
        unsigned short rxAudioKBitRate;
        /**
            *@brief 音频传输比特率（kbps），由瞬时值表示
            */
        unsigned short txAudioKBitRate;
        /**
            *@brief视频接收比特率（kbps），由瞬时值表示
            */
        unsigned short rxVideoKBitRate;
        /**
            *@brief视频传输比特率（kbps），由瞬时值表示
            */
        unsigned short txVideoKBitRate;
        /**
            * @brief CPU使用率 (%)
            */
        double cpuAppUsage;
        /**
            * @brief 通话时长
            */
        unsigned int    duration;
        /**
            * @brief 当前系统CPU使用率 (%)
            */
        double cpuTotalUsage;

        /**上行丢包率%， 从本端到服务器的推流丢包率*/
        unsigned short txPacketLossRate;
        
        /**下行丢包率%， 从服务器到本端的拉流丢包率*/
        unsigned short rxPacketLossRate;

        /*用户数量*/
        unsigned int userCount;

        /**
        Application memory usage (%).
        */
        double memoryAppUsageRatio;
        /**
            System memory usage (%).
            */
        double memoryTotalUsageRatio;
        /**
            Application memory size (KB).
            */
        int memoryAppUsageInKbytes;

        /**
        Total number of bytes transmitted, represented by an aggregate value.
        */
        unsigned int txBytes;
        /**
            Total number of bytes received, represented by an aggregate value.
            */
        unsigned int rxBytes;

        unsigned short lastmileDelay;

        int gatewayRtt;

        RTCStats()
            : rxAudioKBitRate(0)
            , txAudioKBitRate(0)
            , rxVideoKBitRate(0)
            , txVideoKBitRate(0)
            , cpuAppUsage(0)
            , duration(0)
            , cpuTotalUsage(0)
            , txPacketLossRate(0)
            , rxPacketLossRate(0)
            , userCount(0)
            , memoryAppUsageRatio(0)
            , memoryTotalUsageRatio(0)
            , memoryAppUsageInKbytes(0)
            , txBytes(0)
            , rxBytes(0)
            , lastmileDelay(0)
            , gatewayRtt(0){}
    };
    


    /**
    * @brief 推流视频比例
    */
    enum RTC_VIDEO_RATIO
    {
        RTC_VIDEO_RATIO_4_3 = 0,
        RTC_VIDEO_RATIO_16_9 = 1,
        RTC_VIDEO_RATIO_9_16 = 2,
    };

    enum RTC_VIDEO_BITRATE {
        RTC_VIDEO_BITRATE_200 = 200,    // MAX 200kbps
        RTC_VIDEO_BITRATE_350 = 350,    // MAX 350kbps
        RTC_VIDEO_BITRATE_400 = 400,    // MAX 400kbps
        RTC_VIDEO_BITRATE_600 = 600,    // MAX 600kbps
        RTC_VIDEO_BITRATE_1000 = 1000,    // MAX 1000kbps
    };

    enum RTC_VIDEO_RESOLUTION {
        // 普屏 4:3
        RTC_VIDEO_RESOLUTION_320x240 = 1,
        RTC_VIDEO_RESOLUTION_480x360 = 2,
        RTC_VIDEO_RESOLUTION_640x480 = 3,
        RTC_VIDEO_RESOLUTION_800x600 = 4,
        RTC_VIDEO_RESOLUTION_960x720 = 5,

        // 普屏 3:4
        RTC_VIDEO_RESOLUTION_240x320 = 50,
        RTC_VIDEO_RESOLUTION_360x480 = 51,
        RTC_VIDEO_RESOLUTION_480x640 = 52,
        RTC_VIDEO_RESOLUTION_720x960 = 53,

        // 宽屏16:9
        RTC_VIDEO_RESOLUTION_320x180 = 101,
        RTC_VIDEO_RESOLUTION_480x272 = 102,
        RTC_VIDEO_RESOLUTION_640x360 = 103,
        RTC_VIDEO_RESOLUTION_960x540 = 104,
        RTC_VIDEO_RESOLUTION_1280x720 = 105,

        // 宽屏9:16
        RTC_VIDEO_RESOLUTION_180x320 = 201,
        RTC_VIDEO_RESOLUTION_272x480 = 202,
        RTC_VIDEO_RESOLUTION_360x640 = 203,
        RTC_VIDEO_RESOLUTION_540x960 = 204,

        RTC_VIDEO_RESOLUTION_DEFAULT = 1000
    };

    struct VideoProfile {
        RTC_VIDEO_RESOLUTION profile;    /// 配置类型
        int width;                        /// 宽度
        int height;                        /// 高度
        
        VideoProfile() : profile(RTC_VIDEO_RESOLUTION_320x240) , width(320), height(240) {}
        VideoProfile(RTC_VIDEO_RESOLUTION p, int w, int h) : profile(p), width(w), height(h)  {}
    };

    /**
    * 获取视频分辨率配置表
    * @return 返回视频分辨率配置表，
    *         以各项数据为0表示结束，外部不能释放这个配置表
    */
    RTCSDKDLL_API VideoProfile* getVideoProfile();
    /**
    * 获取视频配置数据
    * @param [in] profile 配置类型
    * @param [out] width 视频宽度
    * @param [out] height 视频高度
    * @param [out] frameRate 视频帧率
    * @return false：没有找到对应的配置信息； true: 找到了    
    */
    RTCSDKDLL_API bool getVideoResolution(RTC_VIDEO_RESOLUTION profile, int& width, int& height);


    enum RTC_VIDEO_ROTATION
    {
        RTC_VIDEO_ROTATION_NONE = 1,            // 保持原图像的角度
        RTC_VIDEO_ROTATION_90 = 2,              // 顺时针旋转90度，最终图像的宽度和高度互换
        RTC_VIDEO_ROTATION_180 = 3,             // 顺时针旋转180度，最终图像颠倒
        RTC_VIDEO_ROTATION_270 = 4,             // 顺时针旋转270度，最终图像的宽度和高度互换
    };
    /**
    *  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  推流的画面质量预设选项  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    *
    *  - 1.2.0 版本开始引入推流画质接口 setVideoQualityParamPreset 用于傻瓜化的选择推流画质效果。
    *  - LivePush::setVideoQualityParamPreset 内部通过预先配置不同的视频参数调整到对应的质量
    *  - 目前支持的几种画质选项如下：
    *
    *  （1）标清 - 采用 640 * 360 级别分辨率，码率会在 400kbps - 800kbps 之间自适应，如果主播的网络条件不理想，
    *              直播的画质会偏模糊，但总体卡顿率不会太高。
    *              Android平台下这一档我们会选择采用软编码，软编码虽然更加耗电，但在运动画面的表现要优于硬编码。
    *
    *  （2）高清 - 采用 960 * 540 级别分辨率，码率会锁定在 1200kbps，如果主播的网络条件不理想，直播画质不会有变化，
    *              但这段时间内会出现频繁的卡顿和跳帧。 两个平台下，这一档我们都会采用硬编码。
    *
    *  （3）超清 - 采用 1280 * 720 级别分辨率，码率会锁定在 1500kbps，对主播的上行带宽要求比较高，适合观看端是大屏的业务场景。
    *
    *  （4）大主播 - 顾名思义，连麦中大主播使用，因为是观众的主画面，追求清晰一些的效果，所以分辨率会优先选择 960 * 540。
    *
    *  （5）小主播 - 顾名思义，连麦中小主播使用，因为是小画面，画面追求流畅，分辨率采用 480 * 320， 码率 350kbps 固定。
    *
    *  （6）实时音视频通话 - 时延低，小画面，画面追求流畅，分辨率采用 480 * 320， 码率 350kbps 固定。
    *
    *  （7）静态画质场景   - 画面变动小，能保证非常低的码率下，视频相对清晰，如截屏正在播放的ppt，1080p，400kb左右码率。
    * 【特别说明】
    *  1. 使用 setVideoQualityParamPreset 之后，依然可以使用setVideoBitrate、setVideoFPS、setAutoAdjustStrategy等设置自定义画质，以最后一次的设置为准。
    */
    /**
    * \brief：SDK推流画质预设选项
    */
    enum RTC_VIDEO_QUALITY_PARAM_PRESET
    {
        RTC_VIDEO_QUALITY_STANDARD_DEFINITION = 1,      //标清：建议追求流畅性的客户使用该选项
        RTC_VIDEO_QUALITY_HIGH_DEFINITION = 2,            //高清：建议对清晰度有要求的客户使用该选项
        RTC_VIDEO_QUALITY_SUPER_DEFINITION = 3,            //超清：如果不是大屏观看，不推荐使用
        RTC_VIDEO_QUALITY_LINKMIC_MAIN_PUBLISHER = 4,   //连麦大主播
        RTC_VIDEO_QUALITY_LINKMIC_SUB_PUBLISHER = 5,    //连麦小主播
        RTC_VIDEO_QUALITY_REALTIME_VIDEOCHAT = 6,       //实时音视频
        RTC_VIDEO_QUALITY_STILLIMAGE_DEFINITION = 7,    //静态画质场景，一般用户截屏推流，画面动态变化比较小。内部有自适应码率和分辨率算法，不建议做自定义设置。
    };

    /**
    * \brief：设置输出的视频格式
    */
    enum RTC_OUTPUT_VIDEO_FORMAT
    {
        RTC_OUTPUT_VIDEO_WITHOUT_OUTPUT = 1,        // 不输出数据
        RTC_OUTPUT_VIDEO_FORMAT_YUV420 = 2,         // yuv420格式
        RTC_OUTPUT_VIDEO_FORMAT_RGBA = 3,           // RBGA格式
    };


    /**
    * 视频数据结构类型
    */
    enum RTC_VIDEO_BUFFER_TYPE
    {
        RTC_VIDEO_BUFFER_TYPE_UNKNOWN = 0,
        RTC_VIDEO_BUFFER_TYPE_BUFFER = 1,       ///< 二进制BUFFER类型
        RTC_VIDEO_BUFFER_TYPE_TEXTURE = 3,      ///< 纹理类型
    };

    /**
    * 视频帧的格式
    */
    enum RTC_VIDEO_PIXEL_FORMAT
    {
        RTC_VIDEO_PIXEL_FORMAT_UNKNOWN = 0,
        RTC_VIDEO_PIXEL_FORMAT_I420 = 1,        ///< I420
        RTC_VIDEO_PIXEL_FORMAT_TEXTURE_2D = 2,  ///< OPENGL 2D 纹理
        RTC_VIDEO_PIXEL_FORMAT_BGRA32 = 3,      ///< BGRA32
    };

    enum RTC_BEAUTY_STYLE
    {
        RTC_BEAUTY_STYLE_SMOOTH = 0,      // 光滑
        RTC_BEAUTY_STYLE_NATURE = 1,      // 自然
        RTC_BEAUTY_STYLE_BLUR = 2,        // 朦胧
    };


    /**
    * 音频帧的格式
    */
    enum RTC_AUDIO_FRAME_FORMAT
    {
        RTC_AUDIO_FRAME_FORMAT_NONE = 0,
        RTC_AUDIO_FRAME_FORMAT_PCM,              ///< PCM
    };

    ///////////////////////  network  /////////////////////
    // current network quality
    enum NETWORK_QUALITY_TYPE {
        NETWORK_QUALITY_UNKNOWN        = 0,    // 网络质量未知。
        NETWORK_QUALITY_EXCELLENT    = 1,    // 网络质量极好。
        NETWORK_QUALITY_GOOD        = 2,    // 用户主观感觉和 excellent 差不多，但码率可能略低于 excellent。
        NETWORK_QUALITY_POOR        = 3,    // 用户主观感受有瑕疵但不影响沟通。
        NETWORK_QUALITY_BAD            = 4,    // 勉强能沟通但不顺畅。
        NETWORK_QUALITY_VBAD        = 5,    // 网络质量非常差，基本不能沟通。
        NETWORK_QUALITY_DOWN        = 6,    // 完全无法沟通。
        NETWORK_QUALITY_UNSUPPORTED = 7,    // 暂时无法检测网络质量（未使用）。
        NETWORK_QUALITY_DETECTING    = 8        // 网络质量检测已开始还没完成。
    };

    enum RTC_REMOTE_VIDEO_STATE {
        /** 0: The remote video is in the default state, probably due to #REMOTE_VIDEO_STATE_REASON_LOCAL_MUTED (3), #REMOTE_VIDEO_STATE_REASON_REMOTE_MUTED (5), or #REMOTE_VIDEO_STATE_REASON_REMOTE_OFFLINE (7).
    */
        REMOTE_VIDEO_STATE_STOPPED = 0,

        /** 1: The first remote video packet is received.
            */
        REMOTE_VIDEO_STATE_STARTING = 1,

        /** 2: The remote video stream is decoded and plays normally, probably due to #REMOTE_VIDEO_STATE_REASON_NETWORK_RECOVERY (2), #REMOTE_VIDEO_STATE_REASON_LOCAL_UNMUTED (4), #REMOTE_VIDEO_STATE_REASON_REMOTE_UNMUTED (6), or #REMOTE_VIDEO_STATE_REASON_AUDIO_FALLBACK_RECOVERY (9).
            */
        REMOTE_VIDEO_STATE_DECODING = 2,

        /** 3: The remote video is frozen, probably due to #REMOTE_VIDEO_STATE_REASON_NETWORK_CONGESTION (1) or #REMOTE_VIDEO_STATE_REASON_AUDIO_FALLBACK (8).
            */
        REMOTE_VIDEO_STATE_FROZEN = 3,

        /** 4: The remote video fails to start, probably due to #REMOTE_VIDEO_STATE_REASON_INTERNAL (0).
            */
        REMOTE_VIDEO_STATE_FAILED = 4
    };

    enum RTC_REMOTE_VIDEO_STATE_REASON {
        /** 0: Internal reasons.
            */
        REMOTE_VIDEO_STATE_REASON_INTERNAL = 0,

        /** 1: Network congestion.
            */
        REMOTE_VIDEO_STATE_REASON_NETWORK_CONGESTION = 1,

        /** 2: Network recovery.
            */
        REMOTE_VIDEO_STATE_REASON_NETWORK_RECOVERY = 2,

        /** 3: The local user stops receiving the remote video stream or disables the video module.
            */
        REMOTE_VIDEO_STATE_REASON_LOCAL_MUTED = 3,

        /** 4: The local user resumes receiving the remote video stream or enables the video module.
            */
        REMOTE_VIDEO_STATE_REASON_LOCAL_UNMUTED = 4,

        /** 5: The remote user stops sending the video stream or disables the video module.
            */
        REMOTE_VIDEO_STATE_REASON_REMOTE_MUTED = 5,

        /** 6: The remote user resumes sending the video stream or enables the video module.
            */
        REMOTE_VIDEO_STATE_REASON_REMOTE_UNMUTED = 6,

        /** 7: The remote user leaves the channel.
            */
        REMOTE_VIDEO_STATE_REASON_REMOTE_OFFLINE = 7,

        /** 8: The remote media stream falls back to the audio-only stream due to poor network conditions.
            */
        REMOTE_VIDEO_STATE_REASON_AUDIO_FALLBACK = 8,

        /** 9: The remote media stream switches back to the video stream after the network conditions improve.
            */
        REMOTE_VIDEO_STATE_REASON_AUDIO_FALLBACK_RECOVERY = 9

    };

    /** Local video state types
    */
    enum RTC_LOCAL_VIDEO_STREAM_STATE
    {
        /** Initial state */
        LOCAL_VIDEO_STREAM_STATE_STOPPED = 0,
        /** The capturer starts successfully. */
        LOCAL_VIDEO_STREAM_STATE_CAPTURING = 1,
        /** The first video frame is successfully encoded. */
        LOCAL_VIDEO_STREAM_STATE_ENCODING = 2,
        /** The local video fails to start. */
        LOCAL_VIDEO_STREAM_STATE_FAILED = 3
    };

    /** Local video state error codes
    */
    enum RTC_LOCAL_VIDEO_STREAM_ERROR {
        /** The local video is normal. */
        LOCAL_VIDEO_STREAM_ERROR_OK = 0,
        /** No specified reason for the local video failure. */
        LOCAL_VIDEO_STREAM_ERROR_FAILURE = 1,
        /** No permission to use the local video device. */
        LOCAL_VIDEO_STREAM_ERROR_DEVICE_NO_PERMISSION = 2,
        /** The local video capturer is in use. */
        LOCAL_VIDEO_STREAM_ERROR_DEVICE_BUSY = 3,
        /** The local video capture fails. Check whether the capturer is working properly. */
        LOCAL_VIDEO_STREAM_ERROR_CAPTURE_FAILURE = 4,
        /** The local video encoding fails. */
        LOCAL_VIDEO_STREAM_ERROR_ENCODE_FAILURE = 5,
        /** No camera device. */
        LOCAL_VIDEO_STREAM_ERROR_DEVICE_NOT_FOUND = 6
    };

    enum RTMP_STREAM_PUBLISH_STATE
    {
        /** The RTMP streaming has not started or has ended. This state is also triggered after you remove an RTMP address from the CDN by calling removePublishStreamUrl.
            */
        RTMP_STREAM_PUBLISH_STATE_IDLE = 0,
        /** The SDK is connecting to Agora's streaming server and the RTMP server. This state is triggered after you call the \ref IRtcEngine::addPublishStreamUrl "addPublishStreamUrl" method.
            */
        RTMP_STREAM_PUBLISH_STATE_CONNECTING = 1,
        /** The RTMP streaming publishes. The SDK successfully publishes the RTMP streaming and returns this state.
            */
        RTMP_STREAM_PUBLISH_STATE_RUNNING = 2,
        /** The RTMP streaming is recovering. When exceptions occur to the CDN, or the streaming is interrupted, the SDK tries to resume RTMP streaming and returns this state.

            - If the SDK successfully resumes the streaming, #RTMP_STREAM_PUBLISH_STATE_RUNNING (2) returns.
            - If the streaming does not resume within 60 seconds or server errors occur, #RTMP_STREAM_PUBLISH_STATE_FAILURE (4) returns. You can also reconnect to the server by calling the \ref IRtcEngine::removePublishStreamUrl "removePublishStreamUrl" and \ref IRtcEngine::addPublishStreamUrl "addPublishStreamUrl" methods.
            */
        RTMP_STREAM_PUBLISH_STATE_RECOVERING = 3,
        /** The RTMP streaming fails. See the errCode parameter for the detailed error information. You can also call the \ref IRtcEngine::addPublishStreamUrl "addPublishStreamUrl" method to publish the RTMP streaming again.
            */
        RTMP_STREAM_PUBLISH_STATE_FAILURE = 4,
    };

    enum RTMP_STREAM_PUBLISH_ERROR
    {
        /** The RTMP streaming publishes successfully. */
        RTMP_STREAM_PUBLISH_ERROR_OK = 0,
        /** Invalid argument used. If, for example, you do not call the \ref IRtcEngine::setLiveTranscoding "setLiveTranscoding" method to configure the LiveTranscoding parameters before calling the addPublishStreamUrl method, the SDK returns this error. Check whether you set the parameters in the *setLiveTranscoding* method properly. */
        RTMP_STREAM_PUBLISH_ERROR_INVALID_ARGUMENT = 1,
        /** The RTMP streaming is encrypted and cannot be published. */
        RTMP_STREAM_PUBLISH_ERROR_ENCRYPTED_STREAM_NOT_ALLOWED = 2,
        /** Timeout for the RTMP streaming. Call the \ref IRtcEngine::addPublishStreamUrl "addPublishStreamUrl" method to publish the streaming again. */
        RTMP_STREAM_PUBLISH_ERROR_CONNECTION_TIMEOUT = 3,
        /** An error occurs in Agora's streaming server. Call the addPublishStreamUrl method to publish the streaming again. */
        RTMP_STREAM_PUBLISH_ERROR_INTERNAL_SERVER_ERROR = 4,
        /** An error occurs in the RTMP server. */
        RTMP_STREAM_PUBLISH_ERROR_RTMP_SERVER_ERROR = 5,
        /** The RTMP streaming publishes too frequently. */
        RTMP_STREAM_PUBLISH_ERROR_TOO_OFTEN = 6,
        /** The host publishes more than 10 URLs. Delete the unnecessary URLs before adding new ones. */
        RTMP_STREAM_PUBLISH_ERROR_REACH_LIMIT = 7,
        /** The host manipulates other hosts' URLs. Check your app logic. */
        RTMP_STREAM_PUBLISH_ERROR_NOT_AUTHORIZED = 8,
        /** Agora's server fails to find the RTMP streaming. */
        RTMP_STREAM_PUBLISH_ERROR_STREAM_NOT_FOUND = 9,
        /** The format of the RTMP streaming URL is not supported. Check whether the URL format is correct. */
        RTMP_STREAM_PUBLISH_ERROR_FORMAT_NOT_SUPPORTED = 10,
    };

    struct Stru_RtmpUplinesAddr {
        std::string addr;
        bool direct = false;
        int  nid = 0;
        std::string loc;
    };

    struct Stru_RtmpUplinesInfo {
        std::string cip;
        std::string lac;
        std::string lid;
        uint64_t timestamp;
        uint32_t max_age;
        std::list<Stru_RtmpUplinesAddr> addrs;

        Stru_RtmpUplinesInfo()
            : cip("")
            , lac("")
            , lid("")
            , timestamp(0)
            , max_age(0)
            {}
    };

    struct Stru_RtmpTranscodingUser {
        UserId uid;
        int x;
        int y;
        int width;
        int height;
        int zOrder;
        double alpha;
        int audioChannel;
        Stru_RtmpTranscodingUser()
            : uid(0)
            , x(0)
            , y(0)
            , width(0)
            , height(0)
            , zOrder(0)
            , alpha(1.0)
            , audioChannel(0)
        {}
    };

    struct Stru_RtmpRtcImage {

        const char* url;
        int x;
        int y;
        int width;
        int height;
        Stru_RtmpRtcImage() :
            url(NULL),
            x(0),
            y(0),
            width(0),
            height(0)
        {}
    };

    struct Stru_RtmpLiveTranscoding {
        int width;
        int height;
        int videoBitrate;
        int videoFramerate;
        int videoGop;
        unsigned int backgroundColor;

        std::list<Stru_RtmpTranscodingUser> transcodingUsers;
        Stru_RtmpRtcImage* watermark;
        Stru_RtmpRtcImage* backgroundImage;
        unsigned int audioSampleRate;//32000,44100,48000
        int audioBitrate;
        int audioChannels; //默认1单声道，支持 1 2 3 4 5


        Stru_RtmpLiveTranscoding()
            : width(360)
            , height(640)
            , videoBitrate(400)
            , videoFramerate(15)
            , videoGop(30)
            , backgroundColor(0x000000)
            , watermark(nullptr)
            , backgroundImage(nullptr)
            , audioSampleRate(48000)
            , audioBitrate(48)
            , audioChannels(1)
        {}
    };

    enum REMOTE_AUDIO_STATE {
        /*默认状态*/
        REMOTE_AUDIO_STATE_STOPPED = 0,  // Default state, audio is started or remote user disabled/muted audio stream
        /*收到首帧音频数据*/
        REMOTE_AUDIO_STATE_STARTING = 1,  // The first audio frame packet has been received
        /*远端音频正常播放*/
        REMOTE_AUDIO_STATE_DECODING = 2,  // The first remote audio frame has been decoded or fronzen state ends
        /*远端音频停止播放*/
        REMOTE_AUDIO_STATE_FROZEN = 3,    // Remote audio is frozen, probably due to network issue
        /*远端音频播放失败*/
        REMOTE_AUDIO_STATE_FAILED = 4,    // Remote audio play failed
    };

    enum REMOTE_AUDIO_STATE_REASON
    {
        /** 0: Internal reasons.
            */
        REMOTE_AUDIO_REASON_INTERNAL = 0,
        /** 1: Network congestion.
            */
        REMOTE_AUDIO_REASON_NETWORK_CONGESTION = 1,
        /** 2: Network recovery.
            */
        REMOTE_AUDIO_REASON_NETWORK_RECOVERY = 2,
        /** 3: The local user stops receiving the remote audio stream or
            * disables the audio module.
            */
        REMOTE_AUDIO_REASON_LOCAL_MUTED = 3,
        /** 4: The local user resumes receiving the remote audio stream or
            * enables the audio module.
            */
        REMOTE_AUDIO_REASON_LOCAL_UNMUTED = 4,
        /** 5: The remote user stops sending the audio stream or disables the
            * audio module.
            */
        REMOTE_AUDIO_REASON_REMOTE_MUTED = 5,
        /** 6: The remote user resumes sending the audio stream or enables the
            * audio module.
            */
        REMOTE_AUDIO_REASON_REMOTE_UNMUTED = 6,
        /** 7: The remote user leaves the channel.
            */
        REMOTE_AUDIO_REASON_REMOTE_OFFLINE = 7,
    };
    /** Reasons for a connection state change. */
    enum CONNECTION_CHANGED_REASON_TYPE
    {
        /** 0: The SDK is connecting to Agora's edge server. */
        CONNECTION_CHANGED_CONNECTING = 0,
        /** 1: The SDK has joined the channel successfully. */
        CONNECTION_CHANGED_JOIN_SUCCESS = 1,
        /** 2: The connection between the SDK and Agora's edge server is interrupted. */
        CONNECTION_CHANGED_INTERRUPTED = 2,
        /** 3: The connection between the SDK and Agora's edge server is banned by Agora's edge server. */
        CONNECTION_CHANGED_BANNED_BY_SERVER = 3,
        /** 4: The SDK fails to join the channel for more than 20 minutes and stops reconnecting to the channel. */
        CONNECTION_CHANGED_JOIN_FAILED = 4,
        /** 5: The SDK has left the channel. */
        CONNECTION_CHANGED_LEAVE_CHANNEL = 5,
        /** 6: The connection failed since Appid is not valid. */
        CONNECTION_CHANGED_INVALID_APP_ID = 6,
        /** 7: The connection failed since channel name is not valid. */
        CONNECTION_CHANGED_INVALID_CHANNEL_NAME = 7,
        /** 8: The connection failed since token is not valid, possibly because:

            - The App Certificate for the project is enabled in Dashboard, but you do not use Token when joining the channel. If you enable the App Certificate, you must use a token to join the channel.
            - The uid that you specify in the \ref agora::rtc::IRtcEngine::joinChannel "joinChannel" method is different from the uid that you pass for generating the token.
            */
        CONNECTION_CHANGED_INVALID_TOKEN = 8,
        /** 9: The connection failed since token is expired. */
        CONNECTION_CHANGED_TOKEN_EXPIRED = 9,
        /** 10: The connection is rejected by server. */
        CONNECTION_CHANGED_REJECTED_BY_SERVER = 10,
        /** 11: The connection changed to reconnecting since SDK has set a proxy server. */
        CONNECTION_CHANGED_SETTING_PROXY_SERVER = 11,
        /** 12: When SDK is in connection failed, the renew token operation will make it connecting. */
        CONNECTION_CHANGED_RENEW_TOKEN = 12,
        /** 13: The IP Address of SDK client has changed. i.e., Network type or IP/Port changed by network operator might change client IP address. */
        CONNECTION_CHANGED_CLIENT_IP_ADDRESS_CHANGED = 13,
        /** 14: Timeout for the keep-alive of the connection between the SDK and Agora's edge server. The connection state changes to CONNECTION_STATE_RECONNECTING(4). */
        CONNECTION_CHANGED_KEEP_ALIVE_TIMEOUT = 14,
        CONNECTION_CHANGED_VIDEO_SWITCH_SERVER = 15,
        CONNECTION_CHANGED_AUDIO_SWITCH_SERVER = 16
    };

    /** Connection states. */
    enum RTC_CONNECTION_STATE_TYPE
    {
        /** 1: The SDK is disconnected from server.
            */
        CONNECTION_STATE_DISCONNECTED = 1,
        /** 2: The SDK is connecting to server.
            */
        CONNECTION_STATE_CONNECTING = 2,
        /** 3: The SDK is connected to server and has joined a room. You can now publish or subscribe to a media stream in the room.
            */
        CONNECTION_STATE_CONNECTED = 3,
        /** 4: The SDK keeps rejoining the channel after being disconnected from a joined room because of network issues.
            */
        CONNECTION_STATE_RECONNECTING = 4,
        /** 5: The SDK fails to connect to server or join the room.
            */
        CONNECTION_STATE_FAILED = 5,
    };

    enum VIDEO_BUFFERING_STATE {
        VIDEO_BUFFERING_STATE_BUFFERING_START = 0,
        VIDEO_BUFFERING_STATE_BUFFERING_END = 1,
    };

    enum AUDIO_BUFFERING_STATE {
        AUDIO_BUFFERING_STATE_BUFFERING_START = 0,
        AUDIO_BUFFERING_STATE_BUFFERING_END = 1,
    };


    /** Remote video stream types. */
    enum REMOTE_VIDEO_STREAM_TYPE
    {
        /** 0: High-stream video. */
        REMOTE_VIDEO_STREAM_HIGH = 0,
        /** 1: Low-stream video. */
        REMOTE_VIDEO_STREAM_LOW = 1,
        /** 2: Auto-stream video. */
        REMOTE_VIDO_STREAM_AUDIO = 2

    };

    struct LocalVideoStats
    {
        /** Bitrate (Kbps) sent in the reported interval, which does not include
            * the bitrate of the re-transmission video after packet loss.
            */
        int sentBitrate;
        /** Frame rate (fps) sent in the reported interval, which does not include
            * the frame rate of the re-transmission video after packet loss.
            */
        int sentFrameRate;
        /** The encoder output frame rate (fps) of the local video.
            */
        int videoEncodeFps;
        /** The target bitrate (Kbps) of the current encoder. This value is estimated by the SDK based on the current network conditions.
            */
        int targetBitrate;
        /** The target frame rate (fps) of the current encoder.
            */
        int targetFrameRate;
        /** The width of the encoding frame (px).
            */
        int encodedFrameWidth;
        /** The height of the encoding frame (px).
            */
        int encodedFrameHeight;
        /** The capture frame rate (fps) of the local video.
            */
            int captureFrameRate;
        /** Video jitter
            */
        int vJitter;
        /** Video Decding time (ms)
            */
        int vDecodeDur;
        /** Video Upink rtt
        */
        int vRtt;


        LocalVideoStats() 
        : sentBitrate(0)
        , sentFrameRate(0)
        , videoEncodeFps(0)
        , targetBitrate(0)
        , targetFrameRate(0)
        , encodedFrameWidth(0)
        , encodedFrameHeight(0)
        , captureFrameRate(0)
        , vJitter(0)
        , vDecodeDur(0)
        , vRtt(0){}
    };
    enum BUSI_USER_ROLE
    {
        BUSI_USER_ROLE_STUDENT = 0,
        BUSI_USER_ROLE_TEACHER = 1,
        BUSI_USER_ROLE_TUTOR   = 2,
    };

    /** Use modes of the \ref media::IAudioFrameObserver::onRecordAudioFrame "onRecordAudioFrame" callback. */
    enum RAW_AUDIO_FRAME_OP_MODE_TYPE
    {
        /** 0: Read-only mode: Users only read the \ref agora::media::IAudioFrameObserver::AudioFrame "AudioFrame" data without modifying anything. For example, when users acquire the data with the Agora SDK, then push the RTMP streams. */
        RAW_AUDIO_FRAME_OP_MODE_READ_ONLY = 0,
        /** 1: Write-only mode: Users replace the \ref agora::media::IAudioFrameObserver::AudioFrame "AudioFrame" data with their own data and pass the data to the SDK for encoding. For example, when users acquire the data. */
        RAW_AUDIO_FRAME_OP_MODE_WRITE_ONLY = 1,
        /** 2: Read and write mode: Users read the data from \ref agora::media::IAudioFrameObserver::AudioFrame "AudioFrame", modify it, and then play it. For example, when users have their own sound-effect processing module and perform some voice pre-processing, such as a voice change. */
        RAW_AUDIO_FRAME_OP_MODE_READ_WRITE = 2,
    };

    /** Subscribing Remote media stream types */
    enum SUBSCRIPTION_STREAM_TYPE
    {
        /** 0: Subscribing High-stream video. */
        SUBSCRIBE_VIDEO_HIGH,
        /** 1: Subscribing Low-stream video. */
        SUBSCRIBE_VIDEO_LOW,
        /** 2: Subscribing audio-stream. */
        SUBSCRIBE_AUDIO_ONLY
    };

    /** The priority of the remote user.
    */
    enum PRIORITY_TYPE
    {
        /** 50: The user's priority is high.
            */
        PRIORITY_HIGH = 50,
        /** 100: (Default) The user's priority is normal.
        */
        PRIORITY_NORMAL = 100,
    };
    
    enum RTCFeature {
        FeatureAudioBeauty // 美声
    };

    enum CHANNEL_MEDIA_RELAY_STATE {
        /** 0: The SDK is initializing.
            */
        RELAY_STATE_IDLE = 0,
        /** 1: The SDK tries to relay the media stream to the destination channel.
            */
        RELAY_STATE_CONNECTING = 1,
        /** 2: The SDK successfully relays the media stream to the destination
            * channel.
            */
        RELAY_STATE_RUNNING = 2,
        /** 3: A failure occurs. See the details in code.
            */
        RELAY_STATE_FAILURE = 3,
    };

    enum CHANNEL_MEDIA_RELAY_ERROR {
        /** 0: The state is normal.
            */
        RELAY_OK = 0,
        /** 1: An error occurs in the server response.
            */
        RELAY_ERROR_SERVER_ERROR_RESPONSE = 1,
        /** 2: No server response. You can call the
            * \ref agora::rtc::IRtcEngine::leaveChannel "leaveChannel" method to
            * leave the channel.
            */
        RELAY_ERROR_SERVER_NO_RESPONSE = 2,
        /** 3: The SDK fails to access the service, probably due to limited
            * resources of the server.
            */
        RELAY_ERROR_NO_RESOURCE_AVAILABLE = 3,
        /** 4: The server fails to join the source channel.
            */
        RELAY_ERROR_FAILED_JOIN_SRC = 4,
        /** 5: The server fails to join the destination channel.
            */
        RELAY_ERROR_FAILED_JOIN_DEST = 5,
        /** 6: The server fails to receive the data from the source channel.
            */
        RELAY_ERROR_FAILED_PACKET_RECEIVED_FROM_SRC = 6,
        /** 7: The source channel fails to transmit data.
            */
        RELAY_ERROR_FAILED_PACKET_SENT_TO_DEST = 7,
        /** 8: The SDK disconnects from the server due to poor network
            * connections. You can call the \ref agora::rtc::IRtcEngine::leaveChannel
            * "leaveChannel" method to leave the channel.
            */
        RELAY_ERROR_SERVER_CONNECTION_LOST = 8,
        /** 9: An internal error occurs in the server.
            */
        RELAY_ERROR_INTERNAL_ERROR = 9,
        /** 10: The token of the source channel has expired.
            */
        RELAY_ERROR_SRC_TOKEN_EXPIRED = 10,
        /** 11: The token of the destination channel has expired.
            */
        RELAY_ERROR_DEST_TOKEN_EXPIRED = 11,
    };

    enum CHANNEL_MEDIA_RELAY_EVENT {
        /** 0: The user disconnects from the server due to poor network
            * connections.
            */
        RELAY_EVENT_NETWORK_DISCONNECTED = 0,
        /** 1: The network reconnects.
            */
        RELAY_EVENT_NETWORK_CONNECTED = 1,
        /** 2: The user joins the source channel.
            */
        RELAY_EVENT_PACKET_JOINED_SRC_CHANNEL = 2,
        /** 3: The user joins the destination channel.
            */
        RELAY_EVENT_PACKET_JOINED_DEST_CHANNEL = 3,
        /** 4: The SDK starts relaying the media stream to the destination channel.
            */
        RELAY_EVENT_PACKET_SENT_TO_DEST_CHANNEL = 4,
        /** 5: The server receives the video stream from the source channel.
            */
        RELAY_EVENT_PACKET_RECEIVED_VIDEO_FROM_SRC = 5,
        /** 6: The server receives the audio stream from the source channel.
            */
        RELAY_EVENT_PACKET_RECEIVED_AUDIO_FROM_SRC = 6,
        /** 7: The destination channel is updated.
            */
        RELAY_EVENT_PACKET_UPDATE_DEST_CHANNEL = 7,
        /** 8: The destination channel update fails due to internal reasons.
            */
        RELAY_EVENT_PACKET_UPDATE_DEST_CHANNEL_REFUSED = 8,
        /** 9: The destination channel does not change, which means that the
            * destination channel fails to be updated.
            */
        RELAY_EVENT_PACKET_UPDATE_DEST_CHANNEL_NOT_CHANGE = 9,
        /** 10: The destination channel name is NULL.
            */
        RELAY_EVENT_PACKET_UPDATE_DEST_CHANNEL_IS_NULL = 10,
        /** 11: The video profile is sent to the server.
            */
        RELAY_EVENT_VIDEO_PROFILE_UPDATE = 11,
    };

    enum RTC_AUDIO_PROCESS_MODE {
        RTC_AUDIO_PROCESS_MODE_NORMAL = 0,//  全流程 48k（Audio Processing｜Opus codec）
        RTC_AUDIO_PROCESS_MODE_FASTPROCESSING = 1,  //16k-48k（Audio Processing 16k｜Opus codec 48k）
        RTC_AUDIO_PROCESS_MODE_FASTRECORDING = 2 ,  //全流程 16k（Audio Processing｜Opus codec）
    };
	enum STREAM_SUBSCRIBE_STATE {
		SUB_STATE_IDLE = 0,
		SUB_STATE_NO_SUBSCRIBED = 1,
		SUB_STATE_SUBSCRIBING = 2,
		SUB_STATE_SUBSCRIBED = 3
	};

	enum STREAM_PUBLISH_STATE {
		PUB_STATE_IDLE = 0,
		PUB_STATE_NO_PUBLISHED = 1,
		PUB_STATE_PUBLISHING = 2,
		PUB_STATE_PUBLISHED = 3
	};

	struct LocalAudioStats
	{
		int64_t cap_samples = 0;                  // total capture samples
		int64_t bytes_encoded = 0;
		int32_t bitrate_kbps = 0;
		int32_t retrans_bitrate_kbps = 0;
		int64_t rtt_ms = 0;
		float fraction_lost = 0.0;
		int32_t encoder_fps_in = 0;
		int32_t encoder_fps_out = 0;
		int32_t encode_fps = 0;
	};
	struct RemoteVideoStats
	{
		/* userId: video stream */
		int64_t userID = 0;
		int32_t width = 0;
		int32_t height = 0;
		int32_t bitrateKbps = 0;
		int32_t frameRate = 0;
		int32_t videoDelay = 0;
		float videoLost = 0.0;
		int32_t videoDecFps = 0;
		int64_t avDiff = 0;
		int32_t latency = 0;
		int32_t receivedBitrate = 0;
		int32_t decoderOutputFrameRate = 0;
	};

	struct RemoteAudioStats {
		int64_t userID = 0;
		int32_t bitrate_kbps = 0;
		int64_t rtt_ms = 0;
		int32_t decode_fps_in = 0;
		int32_t decode_fps_out = 0;

		float lost_rate = 0.0;
		float fraction_lost = 0.0;
		int32_t delay_estimate_ms = 0;

		int64_t bytes_rcvd = 0;
		int64_t bytes_decoded = 0;
		int32_t carton_ms = 0;
		int32_t carton_count = 0;
		int32_t carton_rate = 0;

		int32_t av_sync_diff = 0;
		int32_t audio_dec_fps = 0;
		int32_t sample_rate = 0;
	};

    struct EnterConfig{
        const char * roomId; // 可选, 但需要调用 joinChannelWithRealToken 进房间 
        long uid;           // 必须
        RTCEngineType engineType; // 必须
        const char * appId;  // 必须
        const char * planId; // 课程Id. 可传空
        const char * token;  // 鉴权时需要, 不需要鉴权时可传空


        EnterConfig()
            : roomId("")
            , uid(0)
            , engineType(RTCEngine_TTT)
            , appId("")
            , planId("")
            , token("")
        {}
    };

} // namespace rtc

#endif
