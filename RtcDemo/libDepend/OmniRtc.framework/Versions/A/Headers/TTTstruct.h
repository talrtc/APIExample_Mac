#ifndef __TTTSTRUCT_H__
#define __TTTSTRUCT_H__
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string>
#include <vector>

namespace TTTRtc {
    constexpr auto DEVICE_NAME_LENGTH = 512;
    constexpr auto MAX_DEVICE_ID_LENGTH = 512;

    // default params for video out
    constexpr auto DEFAULT_VIDEO_OUT_MIN_BITRATE = -1;
    constexpr auto DEFAULT_VIDEO_OUT_MIN_FRAMERATE = -1;
    // 
    constexpr auto DEFAULT_VIDEO_OUT_BITRATE_720P = 800;
    constexpr auto DEFAULT_MINOR_VIDEO_OUT_BITRATE_720P = 320; // 按 2/5 
    constexpr auto DEFAULT_START_VIDEO_OUT_BITRATE_720P = 400;

    constexpr auto DEFAULT_VIDEO_OUT_BITRATE_540P = 700;
    constexpr auto DEFAULT_MINOR_VIDEO_OUT_BITRATE_540P = 280; // 按 2/5 
    constexpr auto DEFAULT_START_VIDEO_OUT_BITRATE_540P = 300;

    constexpr auto DEFAULT_VIDEO_OUT_BITRATE_480P = 500;
    constexpr auto DEFAULT_MINOR_VIDEO_OUT_BITRATE_480P = 200; // 按 2/5 
    constexpr auto DEFAULT_START_VIDEO_OUT_BITRATE_480P = 300;

    // 
    constexpr auto DEFAULT_VIDEO_OUT_BITRATE = 350;
    constexpr auto DEFAULT_MINOR_VIDEO_OUT_BITRATE = 300; // 除此处初始值外，小流码率相对大流码率，按照 2/5 计算 -- 具体见 ExtVideoDelegate::createDualVideo、RtcEngineImpl::OnSetVideoEncoderBitrate、RtcEngineImpl::_setBitrate 同
    constexpr auto DEFAULT_START_VIDEO_OUT_BITRATE = 300;

    constexpr auto DEFAULT_VIDEO_OUT_WIDTH = 320;
    constexpr auto DEFAULT_VIDEO_OUT_HEIGHT = 240;
    constexpr auto DEFAULT_VIDEO_OUT_FRAMERATE = 15;
    constexpr auto WZLICENSE_MAX_SIZE = 4096;
    // _MAX_U64TOSTR_BASE10_COUNT = 21 + byte alignment
    constexpr auto MAX_CHANNEL_ID_LENGTH = 32;
    constexpr auto MAX_BITRATE_RATIO = 1.2;
    constexpr auto MIN_BITRATE_RATIO = 0.8;

    //typedef long long uid_t;
    typedef void* view_t;
    typedef int(*getPcmDataFunc)(char *buffer, int buffer_size, void* userdata);

    typedef enum {
        /* communicate mode -- */
        CHANNEL_PROFILE_COMMUNICATION = 0,

        /* live mode -- user role: broadcaster | audience */
        CHANNEL_PROFILE_LIVE_BROADCASTING = 1,

        CHANNEL_PROFILE_FORCE_DWORD = 0x7fffffff,
    } CHANNEL_PROFILE_TYPE;

    typedef enum {
        /*default : no conntion*/
        CONNECT_STATE_DEFAULT = 0,
        CONNECT_STATE_CONNECTING,
        CONNECT_STATE_CONNECTFAIL,
        CONNECT_STATE_CONNECTED,
        CONNECT_STATE_DISCONNECT,
        CONNECT_STATE_RECONNECTTIMEOUT,
        CONNECT_STATE_FORCE_DWORD = 0x7fffffff

    }CONNECT_STATE_TYPE;

    typedef enum {
        TTT_SDK_VERSION_XES = 0,
        TTT_SDK_VERSION_TTT = 1
    }SDK_VERSION_MODE;

    typedef enum {
        TTT_IID_AUDIO_DEVICE_MANAGER = 1,
        TTT_IID_VIDEO_DEVICE_MANAGER = 2,
    }INTERFACE_ID_TYPE;

    typedef enum {
        /*  */
        CLIENT_ROLE_BROADCASTER = 1,

        /*  */
        CLIENT_ROLE_AUDIENCE = 2,

        CLIENT_ROLE_FORCE_DWORD = 0x7fffffff,
    } CLIENT_ROLE_TYPE;

    typedef enum {
        // quit normal
        USER_OFFLINE_QUIT = 0,
        // quit timeout
        USER_OFFLINE_DROPPED = 1,
        // quit connection lost
        USE_OFFLINE_LINKLOST = 2,
    } USER_OFFLINE_REASON_TYPE;

    typedef enum {
        VIDEO_CAPTURE_CAMERA,
        VIDEO_CAPTURE_SCREEN,
        VIDEO_CAPTURE_RAWDATA,
        VIDEO_CAPTURE_WINDOW,
        VIDEO_CAPTURE_FORCE_DWORD = 0x7fffffff,
    } VIDEO_CAPTURE_TYPE;

    typedef enum {
        VIDEO_RENDER_MODE_HIDDEN = 1,
        VIDEO_RENDER_MODE_FIT = 2,
    } VIDEO_RENDER_MODE_TYPE;

    /** Video output orientation modes.*/
    enum ORIENTATION_MODE {
        /** 0: (Default) Adaptive mode.

         The video encoder adapts to the orientation mode of the video input device.

         - If the width of the captured video from the SDK is greater than the height, the encoder sends the video in landscape mode. The encoder also sends the rotational information of the video, and the receiver uses the rotational information to rotate the received video.
         - When you use a custom video source, the output video from the encoder inherits the orientation of the original video. If the original video is in portrait mode, the output video from the encoder is also in portrait mode. The encoder also sends the rotational information of the video to the receiver.
         */
        ORIENTATION_MODE_ADAPTIVE = 0,
        /** 1: Landscape mode.

         The video encoder always sends the video in landscape mode. The video encoder rotates the original video before sending it and the rotational infomation is 0. This mode applies to scenarios involving CDN live streaming.
         */
         ORIENTATION_MODE_FIXED_LANDSCAPE = 1,
         /** 2: Portrait mode.

          The video encoder always sends the video in portrait mode. The video encoder rotates the original video before sending it and the rotational infomation is 0. This mode applies to scenarios involving CDN live streaming.
          */
          ORIENTATION_MODE_FIXED_PORTRAIT = 2,
    };

    /** Video degradation preferences when the bandwidth is a constraint. */
    enum DEGRADATION_PREFERENCE {
        /** 0: (Default) Degrade the frame rate in order to maintain the video quality. */
        MAINTAIN_QUALITY = 0,
        /** 1: Degrade the video quality in order to maintain the frame rate. */
        MAINTAIN_FRAMERATE = 1,
        /** 2: (For future use) Maintain a balance between the frame rate and video quality. */
        MAINTAIN_BALANCED = 2,
    };

    /** Video mirror modes. */
    enum VIDEO_MIRROR_MODE_TYPE
    {
        /** 0: (Default) The SDK enables the mirror mode.
         */
        VIDEO_MIRROR_MODE_AUTO = 0,//determined by SDK
        /** 1: Enable mirror mode. */
        VIDEO_MIRROR_MODE_ENABLED = 1,//enabled mirror
        /** 2: Disable mirror mode. */
        VIDEO_MIRROR_MODE_DISABLED = 2,//disable mirror
    };

    typedef enum
    {
        RTC_LOG_LEVEL_NULL = 0,
        RTC_LOG_LEVEL_FATAL,
        RTC_LOG_LEVEL_ERROR,
        RTC_LOG_LEVEL_WARN,
        RTC_LOG_LEVEL_INFO,
        RTC_LOG_LEVEL_DEBUG,
        RTC_LOG_LEVEL_TRACE,

        RTC_LOG_LEVEL_FORCE_DWORD = 0x7fffffff,
    } RTC_SDK_LOG_LEVEL;

    /* TODO: ERR_XXXX_XXXX */
    typedef enum {
        ENTERCONFAPI_NOERROR = 0, // succ
        ENTERCONFAPI_TIMEOUT = 1, // timeout: 5s
        ENTERCONFAPI_ENTER_FAILED = 2, // cannot connect to server
        ENTERCONFAPI_VERIFY_FAILED = 3, // 
        ENTERCONFAPI_BAD_VERSION = 4, // 

        //DISCONNECT RESOON 
        KICKEDBYHOST = 101,  // kicked by anchor
        PUSHRTMPFAILED = 102, // rtmp fail
        SERVEROVERLOAD = 103, // 
        MASTER_EXIT = 104,   // 
        RELOGIN = 105,      // 
        NOAUDIODATA = 106,    // 
        NOVIDEODATA = 107,   // 
        NEWCHAIRENTER = 108,  // 
        CHANNELKEYEXPIRED = 109, //
        ENTERCONF_REFUSED = 120, //

        USER_EXIT_REASON_TIMEOUT = 201,   // 
        USER_EXIT_REASON_LINKCLOSE = 202, // 
        ENTERCONFAPI_PARAMERR = 203,            // invalid parameters
        RE_NEW_CHANNEL_KEY_FAILD = 401,  // 
        USER_BAND_BY_SERVER = 402,
        ERRCODE_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
    } RtcErrorCode;

    enum RTMP_STREAM_PUBLISH_STATE
    {
        RTMP_STREAM_PUBLISH_STATE_IDLE = 0,
        RTMP_STREAM_PUBLISH_STATE_CONNECTING = 1,
        RTMP_STREAM_PUBLISH_STATE_RUNNING = 2,
        RTMP_STREAM_PUBLISH_STATE_RECOVERING = 3,
        RTMP_STREAM_PUBLISH_STATE_FAILURE = 4,
    };

    enum DATA_STREAM_CHANNEL_ID
    {
        DATA_STREAM_CHANNEL_ONE = 1,
        DATA_STREAM_CHANNEL_TWO = 2,
        DATA_STREAM_CHANNEL_THREE = 3,
        DATA_STREAM_CHANNEL_FOUR = 4,
        DATA_STREAM_CHANNEL_FIVE = 5,
    };

    enum RTMP_STREAM_PUBLISH_ERROR
    {
        RTMP_STREAM_PUBLISH_ERROR_OK = 0,
        RTMP_STREAM_PUBLISH_ERROR_INVALID_ARGUMENT = 1,
        RTMP_STREAM_PUBLISH_ERROR_ENCRYPTED_STREAM_NOT_ALLOWED = 2,
        RTMP_STREAM_PUBLISH_ERROR_CONNECTION_TIMEOUT = 3,
        RTMP_STREAM_PUBLISH_ERROR_INTERNAL_SERVER_ERROR = 4,
        RTMP_STREAM_PUBLISH_ERROR_RTMP_SERVER_ERROR = 5,
        RTMP_STREAM_PUBLISH_ERROR_TOO_OFTEN = 6,
        RTMP_STREAM_PUBLISH_ERROR_REACH_LIMIT = 7,
        RTMP_STREAM_PUBLISH_ERROR_NOT_AUTHORIZED = 8,
        RTMP_STREAM_PUBLISH_ERROR_STREAM_NOT_FOUND = 9,
        RTMP_STREAM_PUBLISH_ERROR_FORMAT_NOT_SUPPORTED = 10,
    };

    // 
    typedef enum {
        DEVICE_REMOVE = 0,
        DEVICE_ADD,
        DEVICE_UNKNOWN,

        DEVICE_FORCE_DWORD = 0x7fffffff,
    } RTC_DEVICE_STATE;

    typedef enum{
        LS_VERBOSE = 0, 
        LS_INFO, 
        LS_WARNING, 
        LS_ERROR, 
        LS_NONE = 4,
    } LoggingSeverity;


    /**  */
    enum CONNECTION_STATE_TYPE
    {
        //
        CONNECTION_STATE_DISCONNECTED = 1,
        //
        CONNECTION_STATE_CONNECTING = 2,
        //
        CONNECTION_STATE_CONNECTED = 3,
        //
        CONNECTION_STATE_RECONNECTING = 4,
        //
        CONNECTION_STATE_FAILED = 5,
    };

    /** Reason of CONNECTION_STATE_TYPE */
    enum CONNECTION_CHANGED_REASON_TYPE
    {
        CONNECTION_CHANGED_CONNECTING = 0,
        CONNECTION_CHANGED_JOIN_SUCCESS = 1,
        CONNECTION_CHANGED_INTERRUPTED = 2,
        CONNECTION_CHANGED_BANNED_BY_SERVER = 3,
        CONNECTION_CHANGED_JOIN_FAILED = 4,
        CONNECTION_CHANGED_LEAVE_CHANNEL = 5,
        CONNECTION_CHANGED_INVALID_APP_ID = 6,
        CONNECTION_CHANGED_INVALID_CHANNEL_NAME = 7,
        CONNECTION_CHANGED_INVALID_TOKEN = 8,
        CONNECTION_CHANGED_TOKEN_EXPIRED = 9,
        CONNECTION_CHANGED_REJECTED_BY_SERVER = 10,
        CONNECTION_CHANGED_SETTING_PROXY_SERVER = 11,
        CONNECTION_CHANGED_RENEW_TOKEN = 12,
        CONNECTION_CHANGED_CLIENT_IP_ADDRESS_CHANGED = 13,
        CONNECTION_CHANGED_KEEP_ALIVE_TIMEOUT = 14,
        CONNECTION_CHANGED_VIDEO_SWITCH_SERVER = 15,
        CONNECTION_CHANGED_AUDIO_SWITCH_SERVER = 16
    };

    enum REMOTE_VIDEO_STATE {
        REMOTE_VIDEO_STATE_STOPPED = 0,
        REMOTE_VIDEO_STATE_STARTING = 1,
        REMOTE_VIDEO_STATE_DECODING = 2,
        REMOTE_VIDEO_STATE_FROZEN = 3,
        REMOTE_VIDEO_STATE_FAILED = 4
    };

    enum REMOTE_VIDEO_STATE_REASON {
        REMOTE_VIDEO_STATE_REASON_INTERNAL = 0,
        REMOTE_VIDEO_STATE_REASON_NETWORK_CONGESTION = 1,
        REMOTE_VIDEO_STATE_REASON_NETWORK_RECOVERY = 2,
        REMOTE_VIDEO_STATE_REASON_LOCAL_MUTED = 3,
        REMOTE_VIDEO_STATE_REASON_LOCAL_UNMUTED = 4,
        REMOTE_VIDEO_STATE_REASON_REMOTE_MUTED = 5,
        REMOTE_VIDEO_STATE_REASON_REMOTE_UNMUTED = 6,
        REMOTE_VIDEO_STATE_REASON_REMOTE_OFFLINE = 7,
        REMOTE_VIDEO_STATE_REASON_AUDIO_FALLBACK = 8,
        REMOTE_VIDEO_STATE_REASON_AUDIO_FALLBACK_RECOVERY = 9
    };

    enum VIDEO_STUCK_EVENT {
        VIDEO_STUCK_EVENT_ASSEMBLE = 1,
        VIDEO_STUCK_EVENT_LOST = 2,
        VIDEO_STUCK_EVENT_DECODE = 3,
        VIDEO_STUCK_EVENT_RENDER = 4,
    };

#pragma pack(1)

    typedef struct {
        uint32_t width;
        uint32_t height;
    } VideoResolution;

    typedef struct {
        int32_t index;
        char pixfmt[8];
        char desc[16];
        uint32_t resolutions;
        VideoResolution resolution[16]; // max 16
    } VideoFormatInfo;

    typedef struct {
        int32_t index;
        char frendlyName[128];
        char devicePath[128];
        int32_t maxWidth;
        int32_t maxHeight;
        uint32_t formats;
        VideoFormatInfo format[8];
    } VideoDeviceInfo;

    typedef struct {
        char frendlName[128];
        char deviceID[128];
    } VideoDeviceInfo2;

    typedef struct {
        int32_t fmt;  //unused, only support I420 now
        uint8_t *data[3];   //YUV: 0-y, 1-u, 2-v,  RGB: 0-data
        int linesize[3];
        int32_t width;
        int32_t height;
        int64_t timestamp;
        int32_t framerate;
    } TVideoFrame;

    typedef struct {
        uint8_t* data;              //PCM 16bit little endian
        int32_t length;             //samples_per_channel * num_channels * 2
        int32_t num_channels;
        int32_t samples_per_channel;  // only support 480 * n
        int32_t sample_rate_hz;     // 16000 32000 48000 (not support 44100)
        int64_t timestamp;
    } TAudioFrame;

    struct ExternalVideoFrame
    {
        /** The video buffer type.
         */
        enum VIDEO_BUFFER_TYPE
        {
            /** 1: The video buffer in the format of raw data.
             */
            VIDEO_BUFFER_RAW_DATA = 1,
        };

        /** The video pixel format.
         */
        enum VIDEO_PIXEL_FORMAT
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

        /** The buffer type. See #VIDEO_BUFFER_TYPE
         */
        VIDEO_BUFFER_TYPE type;
        /** The pixel format. See #VIDEO_PIXEL_FORMAT
         */
        VIDEO_PIXEL_FORMAT format;
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


    typedef struct {
        int32_t enable_playout;
        int32_t enable_publish;
        int32_t enable_echoCanceller;
        int32_t cache_ms;
    } ExtAudioConfig;

    typedef struct {
        char filePath[512];
        int32_t enable_playout;
        int32_t enable_publish;
        int32_t cycle;
    } AudioMixingConfig;

    typedef struct {
        int64_t user_id;
        std::string channelId;
        int level;
    } RemoteAudioLevel;

    typedef struct {
        int id;
        int playout_level;
        int publish_level;
    } ExtAudioLevel;

    typedef struct _AudioLevelInfo {
        static const int itemCount = 32;
        // Local
        int local_level;

        // Remote
        RemoteAudioLevel remote[itemCount];
        int remote_num;

        // AudioMixing
        int mixing_playout_level;
        int mixing_publish_level;

        // ExtAudio and AudioEffect
        ExtAudioLevel external[itemCount];
        int external_num;
    } AudioLevelInfo;

    struct AudioVolumeInfo
    {
        static const int channelIDLen = 128;
        /**
       User ID of the speaker. The uid of the local user is 0. The user ID may come from different RtcConnection.
       */
        unsigned int uid;
        /** The volume of the speaker. The volume ranges between 0 (lowest volume) and 255 (highest volume).
     */
        unsigned int volume;
        /** Whether current signal contains human voice.
     */
        unsigned int vad;
        /** The channel ID, which indicates which channel the speaker is in.
         */
        char channelId[channelIDLen + 1];
    };

    typedef struct _ExtPcmSourceInfo
    {
        int32_t id;
        int32_t fps;
        int32_t level;
        int64_t length;
        int64_t muted_frames;
    } ExtPcmSourceInfo;

    typedef struct _LocalAudioStats {
        int32_t muted = 0;
        int32_t muted_ms = 0;    // duration of audio muted.
        int64_t cap_samples = 0;                  // total capture samples
        int64_t bytes_encoded = 0;
        int32_t bitrate_kbps = 0;
        int32_t retrans_bitrate_kbps = 0;
        int64_t rtt_ms = 0;
        float fraction_lost = 0.0;
        int32_t encoder_fps_in = 0;
        int32_t encoder_fps_out = 0;
        int32_t audio_level = 0;                 // FullRange
        int32_t original_level = 0;
        // 
        int32_t rtp_sender_audio_level = 0;
        int32_t audio_level_rms_average = 0;
        int32_t audio_level_rms_peak = 0;
        // 

        int32_t volume_set = 0;
        int32_t encode_fps = 0;
        int32_t ext_pcm_size = 0;
        ExtPcmSourceInfo ext_pcm_sources[20];
    } LocalAudioStats;

    typedef struct _RemoteAudioStats {
        int64_t userID = 0;
        uint32_t ssrc = 0;
        int32_t muted = 0;
        int32_t muted_ms = 0;
        int32_t remote_muted = 0;  //muted by is self
        int32_t bitrate_kbps = 0;
        int64_t rtt_ms = 0;
        int32_t audio_level = 0;
        int32_t audio_level_rms = -1; // 
        int32_t recv_fps = 0;
        int32_t decode_fps_in = 0;
        int32_t decode_fps_out = 0;
        int32_t render_fps = 0;
        int32_t volume_set = 0;

        float lost_rate = 0.0;
        float fraction_lost = 0.0;
        int32_t delay_estimate_ms = 0;
        int32_t jitter_ms = 0;
        int32_t max_jitter_ms = 0;
        int32_t jitter_buffer_ms = 0;

        int64_t bytes_rcvd = 0;
        int64_t bytes_decoded = 0;
        int32_t carton_ms = 0;
        int32_t carton_count = 0;
        int32_t carton_rate = 0;

        int32_t av_sync_diff = 0;
        int32_t audio_dec_fps = 0;
        int32_t sample_rate = 0;
        int32_t channels = 1;
        int32_t aDecodeDur = 0;
        int32_t sys_playout_fps = 0;

        // 
        int64_t rtp_dispatch_max_elapsed = 0; // recv rtp & 处理耗时 - 统计周期内的最大值
        int64_t rtp_dispatch_avg_elapsed = 0; // recv rtp & 处理耗时 - 统计周期内的平均值
        // 
        int32_t audio_level_rms_average = 0;
        int32_t audio_level_rms_peak = 0;
    } RemoteAudioStats;


    typedef struct _LocalVideoConfig {
        int64_t userID = 0;
        VIDEO_CAPTURE_TYPE type = VIDEO_CAPTURE_RAWDATA;// VIDEO_CAPTURE_CAMERA;
        void *viewHwnd = nullptr;
        int32_t enable_hwaccel = 0;
        int32_t encode_codec = 0; // 0 H.264 1 H.265 2 wz264

        int32_t encMirror = 0; // encoder video mirror -- 0: no-mirror, !0: mirror
        int32_t renderMirror = 1; // render video mirror -- 0: no-mirror, !0: mirror

        VIDEO_RENDER_MODE_TYPE encoderScaleType = VIDEO_RENDER_MODE_HIDDEN;
        VIDEO_RENDER_MODE_TYPE renderScaleType = VIDEO_RENDER_MODE_HIDDEN;

        int32_t width = DEFAULT_VIDEO_OUT_WIDTH;      //
        int32_t height = DEFAULT_VIDEO_OUT_HEIGHT;     //
        int32_t framerate = DEFAULT_VIDEO_OUT_FRAMERATE;  //

        float maxBitrateRatio = MAX_BITRATE_RATIO;
        float minBitrateRatio = MIN_BITRATE_RATIO;
        int32_t startBitrate = -1;
        int32_t bitrateKbps = DEFAULT_VIDEO_OUT_BITRATE; // bitrate -- 0: h * w / 800
        int32_t targetBitrate = DEFAULT_VIDEO_OUT_BITRATE; // bitrate -- 0: h * w / 800
        int32_t minorBitrateKbps = DEFAULT_MINOR_VIDEO_OUT_BITRATE;
        /* for camera capture */

        int32_t devIndex = 0;
        int32_t srcCapWidth = DEFAULT_VIDEO_OUT_WIDTH;      //摄像头采集默认分辨率 宽
        int32_t srcCapHeight = DEFAULT_VIDEO_OUT_HEIGHT;     //摄像头采集默认分辨率 高

        char deviceId[MAX_DEVICE_ID_LENGTH] = {0};   // use device id instead of device index

        /* for screen capture */
        int32_t fullScreen = 1; // 0: screenRect 1: full screen
        struct {
            int32_t x, y, w, h;  // screen capture area -- x: left, y: top, w: width, h: height
        } screenRect = {0, 0, 0, 0};
        /* for window capture */
        void *shareWindowHwnd = nullptr; //
        bool isShareWindow = false; //是否共享窗口
        /* for rawdata input */
        int32_t srcWidth = DEFAULT_VIDEO_OUT_WIDTH; // width of yuv
        int32_t srcHeight = DEFAULT_VIDEO_OUT_HEIGHT; // height of yuv
        int32_t srcFramerate = DEFAULT_VIDEO_OUT_FRAMERATE;

        int32_t defaultDev = 1; // 0: non-defaultDev -- 1：default dev

        bool isSetByUser = false; // 是否用户显式设置码率
        bool isSetByConfigService = false; // 是否由 配置下发 服务 配置

        bool isPreviewing = false;        // 当前是否做视频预览
        bool isLocalVideoMute = false;    // 当前是否发送本地视频
        bool isLocalVideoMuteStatus = true;
        bool isLocalVideoEnalble = false;
        char wzLicenseInfo[WZLICENSE_MAX_SIZE] = {0};
        int  svcLayerNum = 1;
    } LocalVideoConfig;

    typedef struct RtcPlayerConfig  {
        int32_t enable_video = true;    //
        int32_t enable_audio = true;    //
        int32_t enable_hwaccel = false; // qsv/vaapi
        int32_t encode_codec = 0;    // 0: H.264 1 H.265 2 wz264

        // auddio
        int32_t enable_playout = true;
        int32_t enable_publish = true;
        int playout_volume = 30;
        int publish_volume = 30;

        // video
        void *viewHwnd = nullptr;     // handler of render
        int32_t width = 0;      //
        int32_t height = 0;     //
        int32_t framerate = 15;  //
        int32_t bitrateKbps = 0; // 0: h * w / 800

        VIDEO_RENDER_MODE_TYPE encoderScaleType = VIDEO_RENDER_MODE_HIDDEN;      //
        VIDEO_RENDER_MODE_TYPE renderScaleType = VIDEO_RENDER_MODE_HIDDEN;   //

        int32_t encMirror = 0; // 0: no-mirror -- !0: mirror
        int32_t renderMirror = 0; // 0: no-mirror -- !0: mirror
    } RtcPlayerConfig;

    typedef struct _RemoteVideoConfig {
        std::string channelID = "";
        int64_t userID = 0;
        char mediaID[128] = { 0 };
        void *hwnd = nullptr;
        VIDEO_RENDER_MODE_TYPE renderScaleType = VIDEO_RENDER_MODE_HIDDEN;
        int32_t renderMirror = 0; // 0: no-mirror -- !0: mirror
        int32_t enable_hwaccel = 0; // hardware accel
        int32_t decode_codec = 0;
    } RemoteVideoConfig;

    typedef struct _RemoteUserMuteStatus {
        int64_t userId = 0;
        std::string channelId = "";
        bool    muted = false;
    } RemoteUserMuteStatus;
    
    typedef struct _MixerVideoConfig  {
        char mediaID[128] = { 0 };
        void *hwnd = nullptr;
        VIDEO_RENDER_MODE_TYPE renderScaleType = VIDEO_RENDER_MODE_HIDDEN;
        int32_t enable_hwaccel = 0; // hardware accel
        int32_t decode_codec = 0;
    } MixerVideoConfig;

    typedef struct {
        /* duration of communication -- accumulated time -- (s) */
        uint32_t duration;

        /* bytes -- accumulated */
        uint32_t txBytes;

        /* bytes -- accumulated */
        uint32_t rxBytes;

        /* kbps -- instantaneous value */
        uint16_t rxAudioKBitRate;

        /* kbps -- instantaneous value */
        uint16_t txAudioKBitRate;

        /* kbps -- instantaneous value */
        uint16_t rxVideoKBitRate;

        /* kbps -- instantaneous value */
        uint16_t txVideoKBitRate;

        /*Application memory usage (%)*/
        double memoryAppUsageRatio;

        /*System memory usage (%).*/
        double memoryTotalUsageRatio;

        /*Application memory size (KB)*/
        int memoryAppUsageInKbytes;

        uint32_t users;
        double cpuAppUsage;
        double cpuTotalUsage;

        uint32_t lastmileDelay;
        int32_t  gatewayRtt;

        uint16_t txPacketLossRate;
        uint16_t rxPacketLossRate;
    } RtcStats;

    typedef struct {
        /* userId: video stream */
        int64_t userID;
        /**/
        char mediaID[128];
        /**/
        int32_t width;
        /**/
        int32_t height;
        /**/
        int32_t bitrateKbps;
        /**/
        int32_t frameRate;

        int32_t videoDelay;
        float videoLost;
        int32_t videoDecFps;
        int64_t avDiff;
        int32_t latency;
        int32_t receivedBitrate;
        int32_t decoderOutputFrameRate;
        int32_t vJitter;
        int32_t vRtt;
        int32_t vDecodeDur;
    } RemoteVideoStats;

    typedef struct _LocalVideoStats  {
        char mediaID[128] = { 0 };
        /*Target width*/
        int32_t frameWidth = 0;
        /*Target height*/
        int32_t frameHeight = 0;

        /* kbps */
        int32_t sentBitrate = 0;    //videoBps
        /* Target height bitrate */
        int32_t videoTargetBps = 0; 

        /* fps */
        int32_t sentFrameRate = 0;  //videoFps
        /**/
        int32_t videoTargetFps = 0;
        /**/
        int32_t videoEncodeFps = 0;

        /**/
        int32_t encodedFrameCount = 0;
        uint16_t fractionLost = 0;
        int32_t vRtt = 0;
    } LocalVideoStats;


    /**
    * for rtmp pusher
    */
    typedef struct _PublisherConfig {
        /* Description: rtmp url */
        char publishUrl[1024] = { 0 };

        // 
        int bAudioOnly = false;

    }PublisherConfig;

    /**
    * Layout of PIP
    */
    typedef struct {
        /* userId of the innner-picture -- 0: invalid */
        int64_t userID;
        char mediaID[128];
        /* x of the innner-picture -- values: [0,1] */
        double x;
        /* y of the innner-picture -- values: [0,1] */
        double y;
        /* width of the innner-picture -- values: [0,1] */
        double width;
        /* height of the innner-picture -- values: [0,1] */
        double height;
        /* z-order of the innner-picture */
        int32_t zOrder;

        double alpha;
        int32_t isRegin;
    } VideoRegion;

#define MAX_VIDEO_REGIONS 32
    typedef struct {
        int32_t canvasWidth;
        int32_t canvasHeight;
        /* backgroundColor -- RGB: [0-255 , 0-255 , 0-255] */
        /* e.g. "#C0C0C0" in RGB */
        char backgroundColor[8];

        /* layout */
        VideoRegion regions[MAX_VIDEO_REGIONS];
        int32_t regionCount;

        char appData[1024];
        int32_t appDataLength;
    } VideoCompositingLayout;

    typedef enum {
        kFlagPlayAudio = 0x00000001,
        kFlagPlayVideo = 0x00000002,
        kFlagLoopback = 0x00000004,

        kFlagAll = 0x000000ff,
    } PlayerFlag;
    typedef struct {
        float   volume;     // volume of audio-out: 0.0-2.0
    } AudioConfig;
    typedef struct {
        void *viewHwnd;     // Handler of render
        int32_t width;      //
        int32_t height;     //
        int32_t framerate;  //

        VIDEO_RENDER_MODE_TYPE encScaleType;      // scale type of enc-out
        VIDEO_RENDER_MODE_TYPE renderScaleType;   // scale type of local-render
    } VideoConfig;

    typedef struct {
        char url[1024]; // url of the bgi or watermark -- length: < 1024 bytes
        int32_t x;      // 
        int32_t y;     //
        int32_t width;  //
        int32_t height;  //
    } RtcImage;

    /**
     * Video dimensions.
     */
    struct VideoDimensions {
        /** Width (pixels) of the video. */
        int width;
        /** Height (pixels) of the video. */
        int height;
        VideoDimensions()
            : width(640), height(480)
        {}
        VideoDimensions(int w, int h)
            : width(w), height(h)
        {}
    };
    
    struct VideoCanvas
    {
        view_t view;
        VIDEO_RENDER_MODE_TYPE renderMode;
        char channelId[MAX_CHANNEL_ID_LENGTH];
        int64_t uid;
        void* priv;

        VideoCanvas()
            : view(nullptr)
            , renderMode(VIDEO_RENDER_MODE_HIDDEN)
            , uid(0)
            , priv(nullptr)
        {
            channelId[0] = '\0';
        }
        VideoCanvas(view_t v, VIDEO_RENDER_MODE_TYPE m, int64_t u)
            : view(v)
            , renderMode(m)
            , uid(u)
            , priv(nullptr)
        {
            channelId[0] = '\0';
        }
        VideoCanvas(view_t v, VIDEO_RENDER_MODE_TYPE m, const char* ch, int64_t u)
            : view(v)
            , renderMode(m)
            , uid(u)
            , priv(nullptr)
        {
            memset(channelId, 0, sizeof(channelId));
            if (strlen(ch) >= MAX_CHANNEL_ID_LENGTH)
                return;
            memcpy(channelId, ch, strlen(ch));
        }
    };


    /** Video encoder configurations.
    */
    struct VideoEncoderConfiguration {
        /** The video frame dimensions (px) used to specify the video quality and measured by the total number of pixels along a frame's width and height: VideoDimensions. The default value is 640 x 360.
        */
        VideoDimensions dimensions;
        /** Set the camera capture resolution
         */
        VideoDimensions vCapResolution;
        /** The frame rate of the video: #FRAME_RATE. The default value is 15.
         Note that we do not recommend setting this to a value greater than 30.
        */
        int frameRate;
        /** The minimum frame rate of the video. The default value is -1.
         */
        int minFrameRate;
        /** The video encoding bitrate (Kbps).
         */
        int bitrate;
        /** The minimum encoding bitrate (Kbps).
         */
        int minBitrate;
        /** The video orientation mode 
        */
        ORIENTATION_MODE orientationMode;
        /** The video encoding degradation preference under limited bandwidth.
         */
        DEGRADATION_PREFERENCE degradationPreference;
        /** Sets the mirror mode of the published local video stream. It only affects the video that the remote user sees.
        */
        VIDEO_MIRROR_MODE_TYPE mirrorMode;

        VideoEncoderConfiguration(
            const VideoDimensions& d, int f,
            int b, ORIENTATION_MODE m, VIDEO_MIRROR_MODE_TYPE mr = VIDEO_MIRROR_MODE_AUTO)
            : dimensions(d), frameRate(f), minFrameRate(DEFAULT_VIDEO_OUT_MIN_FRAMERATE), bitrate(b),
            minBitrate(DEFAULT_VIDEO_OUT_MIN_BITRATE), orientationMode(m),
            degradationPreference(MAINTAIN_QUALITY), mirrorMode(mr)
        {}
        VideoEncoderConfiguration(
            int width, int height, int f,
            int b, ORIENTATION_MODE m, VIDEO_MIRROR_MODE_TYPE mr = VIDEO_MIRROR_MODE_AUTO)
            : dimensions(width, height), frameRate(f),
            minFrameRate(DEFAULT_VIDEO_OUT_MIN_FRAMERATE), bitrate(b),
            minBitrate(DEFAULT_VIDEO_OUT_MIN_BITRATE), orientationMode(m),
            degradationPreference(MAINTAIN_QUALITY), mirrorMode(mr)
        {}
        VideoEncoderConfiguration()
            : dimensions(DEFAULT_VIDEO_OUT_WIDTH, DEFAULT_VIDEO_OUT_HEIGHT)
            , frameRate(DEFAULT_VIDEO_OUT_FRAMERATE)
            , minFrameRate(DEFAULT_VIDEO_OUT_MIN_FRAMERATE)
            , bitrate(DEFAULT_VIDEO_OUT_BITRATE)
            , minBitrate(DEFAULT_VIDEO_OUT_MIN_BITRATE)
            , orientationMode(ORIENTATION_MODE_ADAPTIVE)
            , degradationPreference(MAINTAIN_QUALITY)
            , mirrorMode(VIDEO_MIRROR_MODE_AUTO)
        {}
    };

    /** Video codec profile types. */
    enum VIDEO_CODEC_PROFILE_TYPE
    {  /** 66: Baseline video codec profile. Generally used in video calls on mobile phones. */
        VIDEO_CODEC_PROFILE_BASELINE = 66,
        /** 77: Main video codec profile. Generally used in mainstream electronics such as MP4 players, portable video players, PSP, and iPads. */
        VIDEO_CODEC_PROFILE_MAIN = 77,
        /** 100: (Default) High video codec profile. Generally used in high-resolution broadcasts or television. */
        VIDEO_CODEC_PROFILE_HIGH = 100,
    };

    /** Video Codec types. */
    enum VIDEO_CODEC_TRANSCODING_TYPE
    {
        VIDEO_CODEC_H264_TRANSCODING = 1,
        VIDEO_CODEC_H265_TRANSCODING = 2,
    };

    typedef struct TranscodingUser {
        /** User ID of the user displaying the video in the CDN live.
        */
        int64_t uid;

        int x;

        int y;

        int width;

        int height;

        int zOrder;

        double alpha;

        int audioChannel;
        TranscodingUser()
            : uid(0)
            , x(0)
            , y(0)
            , width(0)
            , height(0)
            , zOrder(0)
            , alpha(1.0)
            , audioChannel(0)
        {}

    } TranscodingUser;


    enum RTC_MEDIA_DEVICE_STATE_TYPE
    {
        /** 1: The device is active. 
        */
        RTC_MEDIA_DEVICE_STATE_ACTIVE = 1,
        /** 2: The device is disabled. 
        */
        RTC_MEDIA_DEVICE_STATE_DISABLED = 2,
        /** 4: The device is not present. 
        */
        RTC_MEDIA_DEVICE_STATE_NOT_PRESENT = 4,
        /** 8: The device is unplugged. 
        */
        RTC_MEDIA_DEVICE_STATE_UNPLUGGED = 8
    };

    /** Media device type.
    */
    enum RTC_MEDIA_DEVICE_TYPE
    {
        /** -1: Unknown audio device type.  
        */
        RTC_UNKNOWN_AUDIO_DEVICE = -1,
        /** 0: Audio playback device.
        */
        RTC_AUDIO_PLAYOUT_DEVICE = 0,
        /** 1: Audio recording device.  
        */
        RTC_AUDIO_RECORDING_DEVICE = 1,
        /** 2: Video renderer.  
        */
        RTC_VIDEO_RENDER_DEVICE = 2,
        /** 3: Video capturer. 
        */
        RTC_VIDEO_CAPTURE_DEVICE = 3
    };


    /** Audio-sample rates. */
    enum AUDIO_SAMPLE_RATE_TYPE
    {
        /** 32000: 32 kHz */
        AUDIO_SAMPLE_RATE_32000 = 32000,
        /** 44100: 44.1 kHz */
        AUDIO_SAMPLE_RATE_44100 = 44100,
        /** 48000: 48 kHz */
        AUDIO_SAMPLE_RATE_48000 = 48000,
    };

    /** Audio codec profile types. The default value is LC_ACC. */
    enum AUDIO_CODEC_PROFILE_TYPE
    {
        /** 0: LC-AAC, which is the low-complexity audio codec type. */
        AUDIO_CODEC_PROFILE_LC_AAC = 0,
        /** 1: HE-AAC, which is the high-efficiency audio codec type. */
        AUDIO_CODEC_PROFILE_HE_AAC = 1,
    };

    typedef struct LiveTranscoding {
        /** Width of the video. The default value is 360. The minimum value of width &times; height is 16 &times; 16.
         */
        int width;
        /** Height of the video. The default value is 640. The minimum value of width &times; height is 16 &times; 16.
        */
        int height;
        /** Bitrate of the CDN live output video stream. The default value is 400 Kbps.

        Set this parameter according to the Video Bitrate Table. If you set a bitrate beyond the proper range, the SDK automatically adapts it to a value within the range.
        */
        int videoBitrate;
        /** Frame rate of the output video stream set for the CDN live broadcast. The default value is 15 fps.

        @note Agora adjusts all values over 30 to 30.
        */
        int videoFramerate;

        /** **DEPRECATED** Latency mode:

         - true: Low latency with unassured quality.
         - false: (Default) High latency with assured quality.
         */
        bool lowLatency;

        /** Video GOP in frames. The default value is 30 fps.
        */
        int videoGop;
        /** Self-defined video codec profile: #VIDEO_CODEC_PROFILE_TYPE.

        @note If you set this parameter to other values, Agora adjusts it to the default value of 100.
        */
        VIDEO_CODEC_PROFILE_TYPE videoCodecProfile;
        /** The background color in RGB hex value. Value only, do not include a #. For example, 0xFFB6C1 (light pink). The default value is 0x000000 (black).
         */
        unsigned int backgroundColor;

        /** video codec type */
        VIDEO_CODEC_TRANSCODING_TYPE videoCodecType;

        /** The number of users in the live broadcast.
         */
        unsigned int userCount;
        /** TranscodingUser
        */
        TranscodingUser *transcodingUsers;
        /** Reserved property. Extra user-defined information to send SEI for the H.264/H.265 video stream to the CDN live client. Maximum length: 4096 Bytes.

         For more information on SEI frame, see [SEI-related questions](https://docs.agora.io/cn/Agora%20Platform/live_related_faq?platform=%E7%9B%B4%E6%92%AD%E7%9B%B8%E5%85%B3#sei).
         */
        const char *transcodingExtraInfo;

        /** **DEPRECATED** The metadata sent to the CDN live client defined by the RTMP or FLV metadata.
         */
        const char *metadata;
        /** The watermark image added to the CDN live publishing stream.

        Ensure that the format of the image is PNG. Once a watermark image is added, the audience of the CDN live publishing stream can see the watermark image. See RtcImage.
        */
        RtcImage* watermark;
        /** The background image added to the CDN live publishing stream.

         Once a background image is added, the audience of the CDN live publishing stream can see the background image. See RtcImage.
        */
        RtcImage* backgroundImage;
        /** Self-defined audio-sample rate: #AUDIO_SAMPLE_RATE_TYPE.
        */
        AUDIO_SAMPLE_RATE_TYPE audioSampleRate;
        /** Bitrate of the CDN live audio output stream. The default value is 48 Kbps, and the highest value is 128.
         */
        int audioBitrate;
        /** Agora's self-defined audio-channel types. We recommend choosing option 1 or 2. A special player is required if you choose option 3, 4, or 5:

         - 1: (Default) Mono
         - 2: Two-channel stereo
         - 3: Three-channel stereo
         - 4: Four-channel stereo
         - 5: Five-channel stereo
         */
        int audioChannels;
        /** Self-defined audio codec profile: #AUDIO_CODEC_PROFILE_TYPE.
         */

        AUDIO_CODEC_PROFILE_TYPE audioCodecProfile;

        LiveTranscoding()
            : width(360)
            , height(640)
            , videoBitrate(400)
            , videoFramerate(15)
            , lowLatency(false)
            , videoGop(30)
            , videoCodecProfile(VIDEO_CODEC_PROFILE_HIGH)
            , backgroundColor(0x000000)
            , videoCodecType(VIDEO_CODEC_H264_TRANSCODING)
            , userCount(0)
            , transcodingUsers(NULL)
            , transcodingExtraInfo(NULL)
            , metadata(NULL)
            , watermark(NULL)
            , backgroundImage(NULL)
            , audioSampleRate(AUDIO_SAMPLE_RATE_48000)
            , audioBitrate(48)
            , audioChannels(1)
            , audioCodecProfile(AUDIO_CODEC_PROFILE_LC_AAC)
        {}
    } LiveTranscoding;

    /** The definition of ChannelMediaInfo.
     */
    struct ChannelMediaInfo {
        /** The channel name. The default value is NULL, which means that the SDK
         * applies the current channel name.
         */
        const char* channelName;
        /** The token that enables the user to join the channel. The default value
         * is NULL, which means that the SDK applies the current token.
         */
        const char* token;
        /** The user ID.
         *
         * @note
         * String user accounts are not supported in media stream relay.
         */
        int64_t uid;
    };

    /** The definition of ChannelMediaRelayConfiguration.
     */
    struct ChannelMediaRelayConfiguration {
        /** Pointer to the source channel: ChannelMediaInfo.
         *
         * @note
         * - `uid`: ID of the user whose media stream you want to relay. We
         * recommend setting it as 0, which means that the SDK relays the media
         * stream of the current broadcaster.
         * - If you do not use a token, we recommend using the default values of
         * the parameters in ChannelMediaInfo.
         * - If you use a token, set uid as 0, and ensure that the token is
         * generated with the uid set as 0.
         */
        ChannelMediaInfo *srcInfo;
        /** Pointer to the destination channel: ChannelMediaInfo. If you want to
         * relay the media stream to multiple channels, define as many
         * ChannelMediaInfo structs (at most four).
         *
         * @note `uid`: ID of the user who is in the source channel.
         */
        ChannelMediaInfo *destInfos;
        /** The number of destination channels. The default value is 0, and the
         * value range is [0,4). Ensure that the value of this parameter
         * corresponds to the number of ChannelMediaInfo structs you define in
         * `destInfos`.
         */
        int destCount;

        ChannelMediaRelayConfiguration()
                : srcInfo(nullptr)
                , destInfos(nullptr)
                , destCount(0)
        {}
    };

    typedef struct CaptureSize {
        int width;
        int height;

        CaptureSize() :width(0), height(0) {}
        CaptureSize(int w, int h) :width(w), height(h) {}
    } CaptureSize;

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

        ThumbImageBuffer() : buffer(NULL), length(0), width(0), height(0) {}
    } ThumbImageBuffer;

    typedef struct Rectangle {
        int x;
        int y;
        int width;
        int height;

        Rectangle() :x(0), y(0), width(0), height(0) {}
        Rectangle(int xx, int yy, int ww, int hh) :x(xx), y(yy), width(ww), height(hh) {}
    } Rectangle;

    typedef void* view_t;
    typedef struct ScreenCaptureSourceInfo {
        ScreenCaptureSourceType type;
        view_t sourceId;
        char sourceName[256] = { 0 };
        ThumbImageBuffer thumbImage;
        ThumbImageBuffer iconImage;
        char processPath[256] = { 0 };
        char sourceTitle[256] = { 0 };
        bool primaryMonitor;
        Rectangle screenMode;

        ScreenCaptureSourceInfo() {}
    } ScreenCaptureSourceInfo;
    typedef std::vector<ScreenCaptureSourceInfo> ScreenCaptureSourceInfoVector;

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

    enum RAW_AUDIO_FRAME_OP_MODE_TYPE
    {
        RAW_AUDIO_FRAME_OP_MODE_READ_ONLY = 0,
        RAW_AUDIO_FRAME_OP_MODE_WRITE_ONLY = 1,
        RAW_AUDIO_FRAME_OP_MODE_READ_WRITE = 2,
    };

    typedef enum
    {
        CODEC_KEEP = -1,
        CODEC_DEFAULT = 0,
        CODEC_AAC = 1,
        CODEC_ISAC_WB = 2,
        CODEC_ISAC_UWB = 3,
        CODEC_OPUS = 4,
        CODEC_HEAAC = 5
    } AUDIOCODEC;

    enum REMOTE_AUDIO_STATE {
        /*default*/
        REMOTE_AUDIO_STATE_STOPPED = 0,  // Default state, audio is started or remote user disabled/muted audio stream
        /*first audio frame*/
        REMOTE_AUDIO_STATE_STARTING = 1,  // The first audio frame packet has been received
        /*remote audio frame has been decoded*/
        REMOTE_AUDIO_STATE_DECODING = 2,  // The first remote audio frame has been decoded or fronzen state ends
        /*remote audio is frozen*/
        REMOTE_AUDIO_STATE_FROZEN = 3,    // Remote audio is frozen, probably due to network issue
        /*remote audio play failed*/
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

    enum LOCAL_VIDEO_STREAM_STATE
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
    enum LOCAL_VIDEO_STREAM_ERROR {
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
        LOCAL_VIDEO_STREAM_ERROR_DEVICE_NOT_FOUND = 6,
    };

    enum LOCAL_AUDIO_STREAM_STATE
    {
        /** 0: The local audio is in the initial state.
         */
        LOCAL_AUDIO_STREAM_STATE_STOPPED = 0,
        /** 1: The recording device starts successfully.
         */
        LOCAL_AUDIO_STREAM_STATE_RECORDING = 1,
        /** 2: The first audio frame encodes successfully.
         */
        LOCAL_AUDIO_STREAM_STATE_ENCODING = 2,
        /** 3: The local audio fails to start.
         */
        LOCAL_AUDIO_STREAM_STATE_FAILED = 3
    };

    /** Local audio state error codes.
     */
    enum LOCAL_AUDIO_STREAM_ERROR
    {
        /** 0: The local audio is normal.
         */
        LOCAL_AUDIO_STREAM_ERROR_OK = 0,
        /** 1: No specified reason for the local audio failure.
         */
        LOCAL_AUDIO_STREAM_ERROR_FAILURE = 1,
        /** 2: No permission to use the local audio device.
         */
        LOCAL_AUDIO_STREAM_ERROR_DEVICE_NO_PERMISSION = 2,
        /** 3: The microphone is in use.
         */
        LOCAL_AUDIO_STREAM_ERROR_DEVICE_BUSY = 3,
        /** 4: The local audio recording fails. Check whether the recording device
         * is working properly.
         */
        LOCAL_AUDIO_STREAM_ERROR_RECORD_FAILURE = 4,
        /** 5: The local audio encoding fails.
         */
        LOCAL_AUDIO_STREAM_ERROR_ENCODE_FAILURE = 5,
        /** 6: No recording audio device.
        */
        LOCAL_AUDIO_STREAM_ERROR_NO_RECORDING_DEVICE = 6,
        /** 7: No playout audio device.
        */
        LOCAL_AUDIO_STREAM_ERROR_NO_PLAYOUT_DEVICE = 7
    };

    enum VIDEO_BUFFERING_STATE {
        VIDEO_BUFFERING_STATE_BUFFERING_START = 0,
        VIDEO_BUFFERING_STATE_BUFFERING_END = 1,
    };

    enum AUDIO_BUFFERING_STATE {
        AUDIO_BUFFERING_STATE_BUFFERING_START = 0,
        AUDIO_BUFFERING_STATE_BUFFERING_END = 1,
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

    /** Remote video stream types. */
    enum REMOTE_VIDEO_STREAM_TYPE
    {
        /** 0: High-stream video. */
        REMOTE_VIDEO_STREAM_HIGH = 0,
        /** 1: Low-stream video. */
        REMOTE_VIDEO_STREAM_LOW = 1,
        /** 2: Auto-stream video. */
        REMOTE_VIDO_STREAM_AUDIO = 2,
    };

    enum STREAM_PUBLISH_STATE {
        PUB_STATE_IDLE = 0,
        PUB_STATE_NO_PUBLISHED = 1,
        PUB_STATE_PUBLISHING = 2,
        PUB_STATE_PUBLISHED = 3
    };

    enum STREAM_SUBSCRIBE_STATE {
        SUB_STATE_IDLE = 0,
        SUB_STATE_NO_SUBSCRIBED = 1,
        SUB_STATE_SUBSCRIBING = 2,
        SUB_STATE_SUBSCRIBED = 3
    };

    enum BUSI_USER_ROLE {
        BUSI_USER_ROLE_STUDENT = 0,
        BUSI_USER_ROLE_TEACHER = 1,
        BUSI_USER_ROLE_TUTOR = 2,
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

    // 
    enum AUDIO_PROCESS_MODE {
        AUDIO_PROCESS_MODE_NORMAL = 0, // 默认
        AUDIO_PROCESS_MODE_MEDIUM = 1, // 音频处理环节使用16K
        AUDIO_PROCESS_MODE_HIGHEST = 2, // 全流程16K
    };

#pragma pack()
}
#endif
