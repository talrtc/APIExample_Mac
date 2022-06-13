#ifndef __TTT_RTCENGINE_H__
#define __TTT_RTCENGINE_H__

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include "TTTBase.h"
#include "TTTstruct.h"
#include "TTTRtcChannel.h"
#include "TTTBase.h"
#include "TTTDeviceMgr.h"

#include <iostream>
typedef void(*sdkLogCallback)(TTTRtc::LoggingSeverity loglevel, const char *str);
namespace TTTRtc {
    class IRtcEngine;

    class TTT_API IVideoDevice {
    public:
        /**
        * Get the video count
        * @return videoCount
        */
        static int getCount(void);

        /**
        * Get the video device info
        * @param [in]  index      index of dev
        * @param [out] outInfo    info of dev
        * @return 0: succ -- !0: fail
        */
        static int getInfo(int index, VideoDeviceInfo* outInfo);

        /**
        * Get the video device info
        * @param [in]  devicePath devicePath
        * @param [out] outInfo    info of dev
        * @return 0: succ -- !0: fail
        */
        static int getInfo(const char *devicePath, VideoDeviceInfo* outInfo);
       
        /**
        * Get the Camera Resolution
        * @return 0: succ -- !0: fail
        */
        static int getCameraResolutionCapability(std::string &strJson, std::string &strDeviceId);

        static void release();
    };

    class AAudioDeviceManager : public AutoPtr2<IAudioDeviceManager>
    {
    public:
        AAudioDeviceManager(IRtcEngine* engine)
        {
            queryInterface(engine, TTT_IID_AUDIO_DEVICE_MANAGER);
        }
    };

    class AVideoDeviceManager : public AutoPtr2<IVideoDeviceManager>
    {
    public:
        AVideoDeviceManager(IRtcEngine* engine)
        {
            queryInterface(engine, TTT_IID_VIDEO_DEVICE_MANAGER);
        }
    };

    class TTT_API IRtcPlayerEventHandler
    {
    public:
        virtual ~IRtcPlayerEventHandler() {};
        virtual void onPlayFinished() {};
        virtual void onPlayPosition(uint64_t curPosMS) {};
    };
#if defined(RTMP_PUBLISHER)
    class TTT_API IRtcPlayer
    {
    public:
        virtual ~IRtcPlayer() {}
        virtual int setEventHandler(IRtcPlayerEventHandler *event) = 0;
        virtual int open(const char* url, RtcPlayerConfig* config) = 0;
        virtual int close() = 0;
        virtual int start(int loop_count) = 0;
        virtual int stop() = 0;
        virtual int pause() = 0;
        virtual int resume() = 0;
        virtual int adjustPlayoutVolume(int volume) = 0; // volume of playout
        virtual int getPlayoutVolume() = 0;
        virtual int adjustPublishVolume(int volume) = 0;// 
        virtual int getPublishVolume() = 0;
        virtual int adjustVolume(int volume) = 0;// 
        virtual int duration() = 0; //length in ms
        virtual int position() = 0; //current position in ms
        virtual int seek(uint32_t seek_ms) = 0;

        virtual int audioID() = 0;
        virtual const char* mediaID() = 0;
    };
#endif
    /**
    * EventHandler
	* Observe the event from SDK
    */
    class IRtcEngineEventHandler
    {
    public:
        virtual ~IRtcEngineEventHandler() {}

        /**
        * 
        * @param [in] channel       channelId
        * @param [in] userID        userId
        * @param [in] sessionId     
        * @param [in] elapsed       duration: joinchannel ->> joinSuccess
        */
        virtual void onJoinChannelSuccess(const char* channel, int64_t userID, const char* sessionId, int elapsed) {
            (void)channel;
            (void)userID;
            (void)sessionId;
            (void)elapsed;
        }

        virtual void onRejoinChannelSuccess(const char* channel, int64_t uid, int elapsed) {
            (void)channel;
            (void)uid;
            (void)elapsed;
        }

        /**
        * 
        * @param [in] result        
        */
        virtual void onLeaveChannel(RtcErrorCode reason) {
            (void)reason;
        }

        virtual void onLeaveChannel(const RtcStats& stat) {
            (void)stat;
        }

        /**
        * 
        * @param [in] err       errCode
        * @param [in] msg       errMsg
        */
        virtual void onError(int err, const char* msg) {
            (void)err;
            (void)msg;
        }

        /**
        * 
        * @param [in] warn      errCode
        * @param [in] msg       errMsg
        */
        virtual void onWarning(int warn, const char* msg) {
            (void)warn;
            (void)msg;
        }

        virtual void onFirstLocalAudioFrame(int elapsed) {
            (void)elapsed;
        }

        virtual void onFirstRemoteAudioFrame(int64_t uid, int elapsed) {
            (void)uid;
            (void)elapsed;
        }

        /**
        * 
        * @param [in] mediaID      
        * @param [in] width	       
        * @param [in] height	   
        */
        virtual void onFirstLocalVideoFrame(const char *mediaID, int width, int height, int elapsed) {
            (void)mediaID;
            (void)width;
            (void)height;
        }
        virtual void onFirstLocalVideoFrame(int width, int height, int elapsed) {
            (void)width;
            (void)height;
            (void)elapsed;
        }

        /**
        * 
        * @param [in] mediaID      The id of mediaStream
        * @param [in] width	       video width
        * @param [in] height	   video height
        */
        virtual void onFirstRemoteVideoFrame(const char *mediaID, int width, int height) {
            (void)mediaID;
            (void)width;
            (void)height;
        }
        virtual void onFirstRemoteVideoFrame(int64_t uid, int width, int height, int elapsed) {
            (void)uid;
        }
        virtual void onFirstRemoteVideoDecoded(int64_t uid, int width, int height, int elapsed) {
            (void)uid;
            (void)width;
            (void)height;
            (void)elapsed;
        }

        /**
         * The parameters of encoder changed by bitrate-control
         * @param [in] mediaID      The id of mediaStream
         * @param [in] bitrate      targetBitrate -- kbps
         * @param [in] fps          targetFps
         * */
        virtual void onEncodeParamChanged(const char *mediaID, int bitrate, int fps) {
            (void)mediaID;
            (void)bitrate;
            (void)fps;
        }

        virtual void onAudioVolumeIndication(AudioLevelInfo* info) {
            (void)info;
        }

        virtual void onAudioVolumeIndication(const AudioVolumeInfo* speakers, unsigned int speakerNumber, int totalVolume) {
            (void)speakers;
            (void)speakerNumber;
            (void)totalVolume;
        }

        virtual void onAudioVadIndication(int64_t userID, int active) {
            (void)userID;
            (void)active;
        }
        /**
        * 
        */
        virtual void onConnectionLost() {}

        /**
        * timing trigger -- The RtcStats -- interval: 2s
        *
        * @param [in]  stats -- about communication
        */
        virtual void onRtcStats(const RtcStats& stats) {
            (void)stats;
        }

        /**
        * 
        *
        * @param [in] stats 
        */
        virtual void onLocalVideoStats(const LocalVideoStats& stats) {
            (void)stats;
        }

        /**
        * 
        *
        * @param [in] stats 
        */
        virtual void onRemoteVideoStats(const RemoteVideoStats& stats) {
            (void)stats;
        }


        virtual void onLocalAudioStats(const LocalAudioStats& stats) {
            (void)stats;
        }
        virtual void onRemoteAudioStats(const RemoteAudioStats& stats) {
            (void)stats;
        }

        /**
        * 
        *
        * @param [in] newRole   
        */
        virtual void onClientRoleChanged(const CLIENT_ROLE_TYPE oldRole, const CLIENT_ROLE_TYPE newRole) {
            (void)newRole;
        }

        /**
        * 
        *
        * @param [in] userID     
        * @param [in] elapsed	 
        */
        virtual void onUserJoined(int64_t userID, int elapsed) {
            (void)userID;
            (void)elapsed;
        }
        /**
        * 
        *
        * @param [in] userID    
        * @param [in] reason	
        */
        virtual void onUserOffline(int64_t userID, USER_OFFLINE_REASON_TYPE reason) {
            (void)userID;
            (void)reason;
        }

        /**
        * Some user enable|disable it's Video-Module
		* disable: Cannot render | send it's own video; Cannot render | recv the other's video
        *
        * @param [in] uid     
        * @param [in] enabled	
        */
        virtual void onUserEnableVideo(int64_t uid, bool enabled) {
            (void)uid;
            (void)enabled;
        }

        virtual void onMixerVideoCreate(const char *mediaID, const char * mediaURL) {
            (void)mediaID;
            (void)mediaURL;
        }

        virtual void onMixerVideoRemove(const char *mediaID, const char * mediaURL) {
            (void)mediaID;
            (void)mediaURL;

        }

        virtual void onAudioMixingFinished() {}
        virtual void onAudioMixingPosition(uint64_t curPosMS) {
            (void)curPosMS;
        }
        virtual void onAudioEffectFinished(int id) {
            (void)id;
        }
        /**
        * 
        * @param [in] SEI -- json
        */
        virtual void onSetSEI(const char* SEI) {

        }
        /**
        * 
        * @param [in] err  
        * @param [in] operUserID 
        */
        virtual void onRTMPsenderror(const char * err) {

        }

        /**
        * rtmp pusher status
        * @param [in] roomID   
        * @param [in] szRTMPURL 
        * @param [in] status	
        */
        virtual void onRTMPStatusChange(const char* roomID, const char* szRTMPURL, bool status) {
            (void)roomID;
            (void)szRTMPURL;
            (void)status;
        }

        virtual void onRtmpStreamingStateChanged(const char *url, RTMP_STREAM_PUBLISH_STATE state,
            RTMP_STREAM_PUBLISH_ERROR errCode)
        {
            (void)url;
            (void)state;
            (void)errCode;           
        }

        virtual void onStreamPublished(const char *url, int error) {
            (void)url;
            (void)error;
        }

        virtual void onStreamUnpublished(const char *url) {
            (void)url;
        }

        virtual void onRequestToken() {
        }
        virtual void onUserMuteAudio(int64_t uid, bool muted) {
            (void)uid;
            (void)muted;
        }
        virtual void onUserMuteVideo(int64_t uid, bool muted) {
            (void)uid;
            (void)muted;
        }

        virtual void OnConnectSuccess() {}
        virtual void onDisconnected(const char * uuid) {}
    
        /**
        * 
        * @param [in] deviceType  0: speaker -- 1: mic
        * @param [in] volume -- values: 0~100
        * @param [in] muted -- true: audio muted
        */
        virtual void onAudioDeviceVolumeChanged(uint8_t deviceType, int volume, bool muted){
          (void)deviceType;
          (void)volume;
          (void)muted;
        }

        //virtual void onVideoDeviceStateChanged(VideoDeviceInfo2 *info, RTC_DEVICE_STATE state) {
        //    (void)info;
        //    (void)state;
        //}

        virtual void onVideoDeviceStateChanged(const char* deviceId, int deviceType, int deviceState) {
            (void)deviceId;
        }

        virtual void onVideoSizeChanged(int64_t uid, int width, int height, int rotation) {
            (void)uid;
            (void)width;
            (void)height;
            (void)rotation;
        }

        virtual void onConnectionStateChanged(
            CONNECTION_STATE_TYPE state, CONNECTION_CHANGED_REASON_TYPE reason) {
            (void)state;
            (void)reason;
        }

        virtual void onActiveSpeaker(int64_t uid) {
            (void)uid;
        }

        virtual void onAudioDeviceStateChanged(const char* deviceId, int deviceType, int deviceState) {
            (void)deviceId;
            (void)deviceType;
            (void)deviceState;
        }

        virtual void onNetworkQuality(int64_t uid, int txQuality, int rxQuality) {
            (void)uid;
            (void)txQuality;
            (void)rxQuality;
        }

        virtual void onLastmileQuality(int quality) {
            (void)quality;
        }

        virtual void onChannelMediaRelayStateChanged(CHANNEL_MEDIA_RELAY_STATE state, CHANNEL_MEDIA_RELAY_ERROR code)
        {
            (void)state;
            (void)code;
        }

        virtual void onChannelMediaRelayEvent(CHANNEL_MEDIA_RELAY_EVENT code)
        {
            (void)code;
        }

        virtual void onTakePreEncodeSnapshot(const char* data, int datalen)
        {
            (void)data;
            (void)datalen;
        }

        virtual void onRemoteStreamSubscribeAdvice(const char* channelId, long long uid, int currentStreamType, int suitableStreamType)
        {
            (void)channelId;
            (void)uid;
            (void)currentStreamType;
            (void)suitableStreamType;
        }

        virtual void onVideoBufferingStateChanged(int64_t uid, VIDEO_BUFFERING_STATE state, int64_t timestampInMs) {
            (void)uid;
            (void)state;
            (void)timestampInMs;
        }

        virtual void onAudioBufferingStateChanged(int64_t uid, AUDIO_BUFFERING_STATE state, int64_t timestampInMs) {
            (void)uid;
            (void)state;
            (void)timestampInMs;
        }

        virtual void onStreamMessage(int64_t uid, int streamId, const char* data, size_t length) {
            (void)uid;
            (void)streamId;
            (void)data;
            (void)length;
        }

        virtual void onStreamMessageError(int64_t uid, int streamId, int code, int missed, int cached) {
            (void)uid;
            (void)streamId;
            (void)code;
            (void)missed;
            (void)cached;
        }

        /**
        *
        * @param [in] uid
        * @param [in] state
        * @param [in] reason
        * @param [in] elapsed
        */
        virtual void onRemoteVideoStateChanged(int64_t uid, REMOTE_VIDEO_STATE state, REMOTE_VIDEO_STATE_REASON reason, int elapsed) {
            (void)uid;
            (void)state;
            (void)reason;
            (void)elapsed;
        }
        /**
        *
        * @param [in] uid
        * @param [in] state
        * @param [in] reason
        * @param [in] elapsed
        */
        virtual void onRemoteAudioStateChanged(int64_t uid, REMOTE_AUDIO_STATE state, REMOTE_AUDIO_STATE_REASON reason, int elapsed) {
            (void)uid;
            (void)state;
            (void)reason;
            (void)elapsed;
        }
        /**
        *
        * @param [in] localVideoState
        * @param [in] error
        */
        virtual void onLocalVideoStateChanged(LOCAL_VIDEO_STREAM_STATE localVideoState, LOCAL_VIDEO_STREAM_ERROR error) {
            (void)localVideoState;
            (void)error;
        }
        /**
        *
        * @param [in] state
        * @param [in] error
        */
        virtual void onLocalAudioStateChanged(LOCAL_AUDIO_STREAM_STATE state, LOCAL_AUDIO_STREAM_ERROR error) {
            (void)state;
            (void)error;
        }

        virtual void onUserKicked(int64_t userID, int reason, int kickTime) {
            (void)userID;
            (int)reason;
            (int)kickTime;
        }

        virtual void onAudioSubscribeStateChange(const char* channelId, int64_t uid, STREAM_SUBSCRIBE_STATE oldState, STREAM_SUBSCRIBE_STATE newState, int elapseSinceLastState) {
            (void)channelId;
            (void)uid;
            (void)oldState;
            (void)newState;
            (void)elapseSinceLastState;
        }

        virtual void onVideoSubscribeStateChange(const char* channelId, int64_t uid, STREAM_SUBSCRIBE_STATE oldState, STREAM_SUBSCRIBE_STATE newState, int elapseSinceLastState) {
            (void)channelId;
            (void)uid;
            (void)oldState;
            (void)newState;
            (void)elapseSinceLastState;
        }

        virtual void onAudioPublishStateChange(const char* channelId, STREAM_PUBLISH_STATE oldState, STREAM_PUBLISH_STATE newState, int elapseSinceLastState) {
            (void)channelId;
            (void)oldState;
            (void)newState;
            (void)elapseSinceLastState;
        }

        virtual void onVideoPublishStateChange(const char* channelId, STREAM_PUBLISH_STATE oldState, STREAM_PUBLISH_STATE newState, int elapseSinceLastState) {
            (void)channelId;
            (void)oldState;
            (void)newState;
            (void)elapseSinceLastState;
        }
    };

    class IVideoFrameObserver
    {
    public:
        //TODO: to TVideoFrame
        enum VIDEO_FRAME_TYPE {
            FRAME_TYPE_YUV420 = 0,  //YUV 420 format

            FRAME_TYPE_H264 = 10,
        };
        struct VideoFrameYUV {
            VIDEO_FRAME_TYPE fmt;
            uint8_t *data[3];       //FRAME_TYPE_YUV420 : 0-y, 1-u, 2-v
            int linesize[3];
            int32_t width;
            int32_t height;
            int64_t pts;
            int rotation;
        };
        struct VideoFrameES {
            VIDEO_FRAME_TYPE fmt;   //FRAME_TYPE_H264
            uint8_t *data;
            int len;
            int32_t width;
            int32_t height;
            int64_t pts;
        };

    public:
        virtual ~IVideoFrameObserver() {}

        /* local video */
        virtual void onCaptureVideoFrame(VideoFrameYUV& frame)
        {
            (void)frame;
        }
        virtual void onScaleRenderVideoFrame(VideoFrameYUV& frame)
        {
            (void)frame;
        }
        virtual void onPreEncodeVideoFrame(VideoFrameYUV& frame)
        {
            (void)frame;
        }
        virtual void onEncodeVideoFrame(VideoFrameES& frame)
        {
            (void)frame;
        }

        /* remote/mixer video */
        virtual void onReceiveVideoFrame(int64_t uid, VideoFrameES& frame)
        {
            (void)frame;
        }
        virtual void onRenderVideoFrame(int64_t uid, VideoFrameYUV& frame)
        {
            (void)frame;
        }
        virtual void onVideoStuckEvent(const char* channelId, const char* mediaId, VIDEO_STUCK_EVENT evtStuck, long long dur, long long ts)
        {
            ;
        }
    };

    class IAudioFrameObserver
    {
    public:
        virtual ~IAudioFrameObserver() {}

        // Audio frame captured -- local
        virtual void onRecordAudioFrame(TAudioFrame* frame)
        {
            (void)frame;
        }

        // Audio frame captured --- local + speakerloopback
        virtual void onLocalCaptureMixed(TAudioFrame* frame)
        {
            (void)frame;
        }

        // Remote auido frame -- one-user
        virtual void onPlaybackAudioFrame(TAudioFrame* frame)
        {
            (void)frame;
        }

        // Remote audio frame -- mix-all-user
        virtual void onPlaybackMixedAudioFrame(TAudioFrame* frame)
        {
            (void)frame;
        }

        // Mixed audio frame -- local + remote
        virtual void onMixedAudioFrame(TAudioFrame* frame)
        {
            (void)frame;
        }

        // 
        virtual void onPlaybackAudioFrameBeforeMixing(int64_t userID,
            TAudioFrame* audioFrame) {

            (void)userID;
            (void)audioFrame;
        }
    };

    struct RtcEngineContext
    {
        IRtcEngineEventHandler* eventHandler;
        const char *workPath;
        const char *coreSlb;
        const char *coreSlbbackup;
        const char *coreServerlog;
        const char *imgReportApi;
        RTC_SDK_LOG_LEVEL logLevel;

        RtcEngineContext()
            : eventHandler(NULL)
            , workPath(NULL)
            , coreSlb(NULL)
            , coreSlbbackup(NULL)
            , coreServerlog(NULL)
            , imgReportApi(NULL)
            , logLevel(RTC_LOG_LEVEL_DEBUG)
        {}
    };


    class TTT_API IRtcEngine
    {
    public:
        static IRtcEngine* createInstance();
        static const char *getVersion(int* build);

        virtual ~IRtcEngine() {};
        virtual int initialize(RtcEngineContext& context) = 0;
        virtual void release(bool sync = false) = 0;

        virtual int enableVideo() = 0;
        virtual int disableVideo() = 0; // Should invoke while there's no local|remote video
        virtual int enableAudio() = 0;
        virtual int disableAudio() = 0;
        virtual void enableLowComplexityMode(int ap_complexity) = 0;  // set audio 16k samples ...
        virtual int setAppID(const char *appID) = 0;
        virtual int setLocalUserId(int64_t userId) = 0;              

        // audioOnly: 
        virtual int setChannelProfile(CHANNEL_PROFILE_TYPE profile) = 0;
        virtual int setClientRole(CLIENT_ROLE_TYPE role) = 0; 
        virtual int setServerAddr(const char* addr, uint16_t port) = 0;

        virtual void enableAudioAGC(bool enable, bool exp_enable) = 0;
        virtual void enableAudioEC(bool enable, bool exp_enable) = 0;
        virtual void enableAudioNS(bool enable, bool exp_enable) = 0;

        virtual int joinChannel(const char* token, const char* channelID, const char* info, int64_t userID) = 0;
        virtual int leaveChannel() = 0;

        virtual int startChannelMediaRelay(const ChannelMediaRelayConfiguration &configuration) = 0;
        virtual int updateChannelMediaRelay(const ChannelMediaRelayConfiguration &configuration) = 0;
        virtual int stopChannelMediaRelay() = 0;
        virtual void takePreEncodeSnapshot() = 0;

        virtual ScreenCaptureSourceInfoVector getScreenCaptureSourceVector(const CaptureSize &thumbSize, const CaptureSize &iconSize, const bool includeScreen) = 0;
        virtual int startScreenCaptureByDisplayId(unsigned int displayId, const Rectangle &regionRect, const ScreenCaptureParameters &captureParams) = 0;
        virtual int startScreenCaptureByScreenRect(const Rectangle &screenRect, const Rectangle &regionRect, const ScreenCaptureParameters &captureParams) = 0;
        virtual int startScreenCaptureByWindowId(view_t windowId, const Rectangle &regionRect, const ScreenCaptureParameters &captureParams) = 0;
        virtual int updateScreenCaptureParameters(const ScreenCaptureParameters &captureParams) = 0;
        virtual int updateScreenCaptureRegion(const Rectangle &regionRect) = 0;
        virtual int stopScreenCapture() = 0;

        virtual void startLocalVideoTest(LocalVideoConfig& config) = 0;
        virtual void stopLocalVideoTest() = 0;

		// You must call [setupLocalVideo] before entering room
        //return mediaID
        // !!NON-supported the function while [subs other room]
        virtual int setVideoEncoderConfiguration(const VideoEncoderConfiguration& config) = 0;
        virtual int setupLocalVideo(const VideoCanvas& canvas) = 0;
        virtual int enableLocalVideo(bool enabled) = 0;
        //virtual int enableLocalVideo(bool enabled, const char* channelId) = 0;
        virtual int setLocalRenderMode(VIDEO_RENDER_MODE_TYPE renderMode) = 0;
        virtual int setLocalVideoMirrorMode(VIDEO_MIRROR_MODE_TYPE mirrorMode) = 0;

        virtual const char* createLocalVideo(LocalVideoConfig& config) = 0;
        // ！！NON-supported the function while [subs other room]
        virtual void releaseLocalVideo(const char* mediaID) = 0;
        virtual int reconfigLocalVideo(const char* mediaID, LocalVideoConfig& config) = 0;
        virtual int startPreview(const char *mediaID = NULL) = 0;
        virtual int stopPreview(const char *mediaID = NULL) = 0;
        virtual void inputVideoFrame(const char* mediaID, TVideoFrame *frame) = 0;
        virtual int setExternalVideoSource(bool enable, bool useTexture) = 0;
        virtual int pushVideoFrame(ExternalVideoFrame *frame) = 0;

        //dual stream
        virtual int enableDualStreamMode(bool enabled) = 0;
        virtual int setRemoteSubscribeFallbackOption(STREAM_FALLBACK_OPTIONS option) = 0;
        virtual int setRemoteVideoStreamType(int64_t userId, REMOTE_VIDEO_STREAM_TYPE streamType) = 0;
        virtual int setRemoteDefaultVideoStreamType(REMOTE_VIDEO_STREAM_TYPE streamType) = 0;
        virtual int setDualStreamParameter(int width, int height, int bitrate, int framerate) = 0;
#if defined(RTMP_PUBLISHER)
        virtual IRtcPlayer* createRtcPlayer(int64_t userID) = 0; //TODO: userID???
        virtual void releaseRtcPlayer(IRtcPlayer* player) = 0;
#endif
        virtual int setupRemoteVideo(const VideoCanvas& canvas) = 0;
        virtual int setRemoteRenderMode(int64_t userID, VIDEO_RENDER_MODE_TYPE renderMode) = 0;
        virtual int setRemoteVideoMirrorMode(int64_t userID, VIDEO_MIRROR_MODE_TYPE mirrorMode) = 0;

        virtual int createRemoteVideo(RemoteVideoConfig& config) = 0;
        virtual void releaseRemoteVideo(const char* roomId, int64_t userID, const char* mediaID) = 0;
        virtual void releaseAllRemoteVideo() = 0;//TODO:

        // Customize for XES
        /*
        virtual int createRemoteVideo(RemoteVideoConfig& config, int videoType) = 0;
        virtual void releaseRemoteVideo(int64_t roomId, int64_t userID, int videoType ) = 0;
        virtual int muteRemoteVideoStream(int64_t userID, bool mute, int videoType) = 0;
        */

        virtual int createMixerVideo(MixerVideoConfig& config) = 0;
        virtual void releaseMixerVideo(const char* mediaID) = 0;
        virtual int registerVideoFrameObserver(IVideoFrameObserver* observer) = 0;
        virtual int registerAudioFrameObserver(IAudioFrameObserver* observer) = 0;
        virtual int unRegisterAudioFrameObserver(IAudioFrameObserver* observer) = 0;
        virtual int enableAllAudioMix(bool enable) = 0;

        virtual int configPublisher(const PublisherConfig& config) = 0;
        virtual int addPublishStreamUrl(const char* url, bool transcodingEnabled, bool bAudioOnly = false) = 0;
        virtual int removePublishStreamUrl(const char* url) = 0;
        virtual int updateRtmpUrl(const char* newStreamUrl, const char* oldStreamUrl) = 0;

        virtual void addVideoMixer(int64_t nUserID, const char *mediaID, const char *streamUrl = nullptr) = 0;
        virtual void delVideoMixer(int64_t nUserID, const char *mediaID, const char *streamUrl = nullptr) = 0;
        virtual void addAudioMixer(int64_t nUserID, const char *streamUrl = nullptr) = 0;
        virtual void delAudioMixer(int64_t nUserID, const char *streamUrl = nullptr) = 0;
        virtual void setVideoMixerBackgroundImgUrl(const char *url, const char *streamUrl = nullptr) = 0;
        virtual void setVideoCompositingLayout(const VideoCompositingLayout& layout, const char* streamUrl = nullptr) = 0;
        virtual void setVideoCompositingLayout(const char* sei, const char* seiExt, const char* streamUrl = nullptr) = 0;
        virtual int setVideoMixerParams(int bitrate, int fps, int width, int height, int encodeMode, int gop) = 0;
        virtual int	setAudioMixerParams(int bitrate, int samplerate, int channels) = 0;
        virtual int setLiveTranscoding(const LiveTranscoding &transcoding) = 0;
        virtual int	setPreferAudioCodec(AUDIOCODEC audioCodec, int bitrate, int channels) = 0;
        virtual int setRecordingAudioFrameParameters(int sampleRate, int channel, RAW_AUDIO_FRAME_OP_MODE_TYPE mode, int samplesPerCall) = 0;
        virtual void setSignalTimeout(int signalTimeout) = 0;

        virtual int muteLocalVideoStream(const char* mediaID, bool mute) = 0;
        virtual int muteLocalVideoStream(bool mute) = 0;
        virtual int muteRemoteVideoStream(int64_t userID, const char* mediaID, bool mute) = 0;
        virtual int muteRemoteVideoStream(int64_t userID, bool mute) = 0;
        virtual int muteAllRemoteVideoStreams(bool mute) = 0;
        virtual int muteAllRemoteVideoStreams(const char* channelId, bool mute) = 0;

        virtual bool kickChannelUser(const char* channelID,int64_t userID) = 0;

        // 
        //virtual int setAudioProfile(AUDIO_PROFILE_TYPE profile, AUDIO_SCENARIO_TYPE scenario ) = 0;
        // 
        virtual int enableAudioVolumeIndication(int interval) = 0;
        virtual int enableAudioVolumeIndication(int interval, int smooth, bool report_vad) = 0;

        virtual int enableLocalAudio(bool enable) = 0;
        // mode: 0/1/2/3 -- 3: more stringent
        virtual int enableAudioVad(int64_t userID, bool enable, int mode) = 0;
        virtual int adjustLocalAudioVolume(int volume) = 0;      // Initial: 100 
        virtual int adjustRemoteAudioVolume(int64_t userID, int volume) = 0;
        virtual int adjustRemoteAudioMixedVolume(int volume) = 0;

        virtual int muteLocalAudioStream(bool mute) = 0;
        virtual int muteRemoteAudioStream(int64_t userID, bool mute) = 0;
        virtual int muteRemoteAudioStream(const char* channelId, int64_t userID, bool mute) = 0;
        virtual int muteAllRemoteAudioStreams(bool mute) = 0;
        virtual int muteAllRemoteAudioStreams(const char* channelId, bool mute) = 0;

        virtual int enableEarphoneMonitor(int enable) = 0;
        virtual int adjustEarphoneMonitorVolume(int volume) = 0;

        // AudioDevice Management
        virtual int getNumOfPlayoutDevices() = 0;
        virtual int getNumOfRecordingDevices() = 0;
        virtual int getPlayoutDeviceName(int index, char deviceName[DEVICE_NAME_LENGTH], char devicePath[DEVICE_NAME_LENGTH]) = 0;
        virtual int getRecordingDeviceName(int index, char deviceName[DEVICE_NAME_LENGTH], char devicePath[DEVICE_NAME_LENGTH]) = 0;
        virtual int setPlayoutDevice(int index) = 0;
        virtual int setRecordingDevice(int index) = 0;
        virtual int setSystemVolume(int volume) = 0;
        virtual int getSystemVolume(int* volume) = 0;
        virtual int setSpeakerVolume(int volume) = 0;       // 
        virtual int getSpeakerVolume(int* volume) = 0;
        virtual int setMicrophoneVolume(int volume) = 0;    // 
        virtual int getMicrophoneVolume(int* volume) = 0;

        virtual int startRecordingDeviceTest(int indicationInterval) = 0;
        virtual int stopRecordingDeviceTest() = 0;
        virtual int startPlaybackDeviceTest(const char* testAudioFilePath) = 0;
        virtual int stopPlaybackDeviceTest() = 0;

        // Customized AudioSource
        virtual int createExtAudioSource(ExtAudioConfig* config) = 0;
        virtual int releaseExtAudioSource(int id) = 0;
        virtual int reconfigExtAudio(int id, ExtAudioConfig* config) = 0;
        virtual int pushExtAudioFrame(int id, TAudioFrame* frame) = 0;
        virtual int clearExtAudioCache(int id) = 0;
        virtual int setExtAudioPaused(int id, int paused) = 0;
        virtual int adjustExtAudioVolume(int id,
            int playout_volume,
            int publish_volume) = 0;

        // AudioFile playout & mixing
        // struct AudioMixingConfig
#if defined(RTMP_PUBLISHER)
        virtual int startAudioMixing(AudioMixingConfig* config) = 0;
        virtual int stopAudioMixing() = 0;
        virtual int pauseAudioMixing() = 0;
        virtual int resumeAudioMixing() = 0;
        virtual int adjustAudioMixingVolume(int volume) = 0;
        virtual int adjustAudioMixingPlayoutVolume(int volume) = 0;
        virtual int getAudioMixingPlayoutVolume() = 0;
        virtual int adjustAudioMixingPublishVolume(int volume) = 0;
        virtual int getAudioMixingPublishVolume() = 0;
        virtual int getAudioMixingDuration() = 0;
        virtual int getAudioMixingCurrentPosition() = 0;
        virtual int setAudioMixingPosition(int pos) = 0;
        // AudioEffect Management
        virtual int loadAudioEffect(const char* filePath) = 0;  //0: fail -- valid id: succ
        virtual int unloadAudioEffect(int id) = 0;
        virtual int playAudioEffect(int id, int loop_count, bool publish) = 0;
        virtual int stopAudioEffect(int id) = 0;
        virtual int pauseAudioEffect(int id) = 0;
        virtual int resumeAudioEffect(int id) = 0;
        virtual int setAudioEffectVolume(int id, int volume) = 0;
        virtual int getAudioEffectVolume(int id, int &volume) = 0;

        // Recording
        virtual int startAudioRecording(const char* filepath, AUDIO_RECORDING_QUALITY quality) = 0;
        virtual int stopAudioRecording() = 0;
        //virtual bool StartPlay(const char*  rtmpurl, long long userId, void* hVideoWnd) = 0;
        //virtual void StopPlay() = 0;
        virtual int startPushScreenCaptureRect(int iOffsetX, int iOffsetY, int iWidth, int iHeight, AV_PUSH_TYPE type = AV_PUSH_CDN, const char* mp4FilePath = NULL, const char *mCDNPullAddress = NULL) = 0;
        virtual int stopPushScreenCapture() = 0;
        virtual int startPushLocalVideo(LocalVideoConfig& config, AV_PUSH_TYPE type, const char* mp4FilePath, const char *mCDNPullAddress) = 0;
        virtual int stopPushLocalVideo() = 0;
#endif
        /**
        * Is the video up to videoMixer
        *
        * @param mediaID 
        * @param enable  true | false
        */
        virtual int enableVideoMixer(const char* mediaID, bool enable) = 0;
        /**
        * Update the token
        * You should re-fetch the token while onTokenPrivilegeWillExpire, and then call [renewToken] to update Token, otherwise, SDK will disconnected from server.
        *
        * @param token token
        * @return 0: success; !0: fail。see {@link LocalSDKConstants#FUNCTION_SUCCESS}
        */
        virtual int renewToken(const char *token) = 0;
        virtual int addVideoWatermark(const RtcImage & 	watermark) = 0;
        virtual int clearVideoWatermarks() = 0;
        virtual int getConnectionState() = 0; // Cur connection state: disconnected | reconnecting
        virtual int enableLoopbackRecording(bool enable, const char* deviceName = NULL) = 0;

        virtual int enableLastmileTest() = 0;
        virtual int disableLastmileTest() = 0;

        virtual int createDataStream(int* streamId, bool reliable, bool ordered) = 0;
        virtual int sendStreamMessage(int streamId, const char* data, size_t length) = 0;

        virtual IChannel* createChannel(const char*  channelId) = 0;
        virtual int queryInterface(INTERFACE_ID_TYPE iid, void** inter) = 0;
		
        virtual const char* getChannelSessionId() = 0;

        virtual int enableContentInspect(bool enable, int timeInterval) = 0;
        virtual int contentInspectExtra(const char* arg, int rate_array[], int array_cnt) = 0;

		//typedef void(*sdkLogCallback)(LoggingSeverity loglevel, const char *str); 
		virtual void SetCallback(sdkLogCallback cbk) = 0;

        virtual int setBusinessUserRole(BUSI_USER_ROLE role) = 0;

        // 
        // 自定义
        // parameters - 采用 json 格式，传入自定义参数
        // 例：parameters = {"rtc.min_playout_delay":0}
        virtual int setParameters(const char* parameters) = 0;

        // 
        // 参数
        // bitrate      - 音频编码码率，单位为 kbps
        // processMode  - 音频处理复杂度（按采样率维度）
        virtual int setAudioEncoderConfiguration(int bitrate, AUDIO_PROCESS_MODE processMode) = 0;

        // 

    };
}//namespace TTTRtc
#endif
