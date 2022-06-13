#ifndef __TTTRTCCHANNEL_H__
#define __TTTRTCCHANNEL_H__

#include "TTTstruct.h"

namespace TTTRtc {

struct ChannelMediaOptions {
    bool autoSubscribeAudio;
    bool autoSubscribeVideo;
    ChannelMediaOptions()
    : autoSubscribeAudio(true)
    , autoSubscribeVideo(true)
    {}
};

class IChannel;
class IChannelEventHandler
{
public:
    virtual ~IChannelEventHandler() {}
    
    virtual void onChannelWarning(IChannel *channel, int warn, const char* msg) {
        (void)channel;
        (void)warn;
        (void)msg;
    }
    
    virtual void onChannelError(IChannel *channel, int err, const char* msg) {
        (void)channel;
        (void)err;
        (void)msg;
    }
    
    virtual void onJoinChannelSuccess(IChannel *channel, int64_t uid, int elapsed) {
        (void)channel;
        (void)uid;
        (void)elapsed;
    }
    
    virtual void onRejoinChannelSuccess(IChannel *channel, int64_t uid, int elapsed) {
        (void)channel;
        (void)uid;
        (void)elapsed;
    }
    
    virtual void onLeaveChannel(IChannel *channel, const RtcStats& stats) {
        (void)channel;
        (void)stats;
    }
    
    virtual void onClientRoleChanged(IChannel *channel, CLIENT_ROLE_TYPE oldRole,
        CLIENT_ROLE_TYPE newRole) {
        (void)channel;
        (void)oldRole;
        (void)newRole;
    }
    
    virtual void onUserJoined(IChannel *channel, int64_t uid, int elapsed) {
        (void)channel;
        (void)uid;
        (void)elapsed;
    }
    
    virtual void onUserOffline(IChannel *channel, int64_t uid, USER_OFFLINE_REASON_TYPE reason) {
        (void)channel;
        (void)uid;
        (void)reason;
    }
     
    virtual void onFirstRemoteVideoFrame(IChannel *channel, int64_t uid,
        int width, int height, int elapsed) {
        (void)channel;
        (void)uid;
        (void)width;
        (void)height;
        (void)elapsed;
    }
    
    virtual void onRemoteAudioStateChanged(IChannel *channel, int64_t uid,
        REMOTE_AUDIO_STATE state, REMOTE_AUDIO_STATE_REASON reason, int elapsed) {
        (void)channel;
        (void)uid;
        (void)state;
        (void)reason;
        (void)elapsed;
    }

    virtual void onRemoteVideoStateChanged(IChannel *channel, int64_t uid,
        REMOTE_VIDEO_STATE state, REMOTE_VIDEO_STATE_REASON reason, int elapsed) {
        (void)channel;
        (void)uid;
        (void)state;
        (void)reason;
        (void)elapsed;
    }
    
    virtual void onFirstRemoteAudioFrame(IChannel *channel, int64_t uid, int elapsed) {
        (void)channel;
        (void)uid;
        (void)elapsed;
    }

    virtual void onFirstRemoteAudioDecoded(IChannel *rtcChannel, int64_t uid, int elapsed) {
        (void)rtcChannel;
        (void)uid;
        (void)elapsed;
    }

    
    virtual void onRtcStats(IChannel *channel, const RtcStats& stats) {
        (void)channel;
        (void)stats;
    }

    virtual void onChannelMediaRelayStateChanged(IChannel *rtcChannel, CHANNEL_MEDIA_RELAY_STATE state, CHANNEL_MEDIA_RELAY_ERROR code)
    {
        (void)state;
        (void)code;
    }

    virtual void onChannelMediaRelayEvent(IChannel *rtcChannel, CHANNEL_MEDIA_RELAY_EVENT code)
    {
        (void)code;
    }

    virtual void onRemoteStreamSubscribeAdvice(IChannel *rtcChannel, const char* channelId, long long uid, int currentStreamType, int suitableStreamType)
    {
        (void)channelId;
        (void)uid;
        (void)currentStreamType;
        (void)suitableStreamType;
    }

    virtual void onVideoBufferingStateChanged(IChannel *rtcChannel, int64_t uid, VIDEO_BUFFERING_STATE state, int64_t timestampInMs) {
        (void)rtcChannel;
        (void)uid;
        (void)state;
        (void)timestampInMs;
    }

    virtual void onAudioBufferingStateChanged(IChannel *rtcChannel, int64_t uid, AUDIO_BUFFERING_STATE state, int64_t timestampInMs) {
        (void)rtcChannel;
        (void)uid;
        (void)state;
        (void)timestampInMs;
    }


    virtual void onStreamMessage(IChannel* channel, int64_t uid, int streamId, const char* data, size_t length) {
        (void)uid;
        (void)streamId;
        (void)data;
        (void)length;
    }

    virtual void onStreamMessageError(IChannel* channel, int64_t uid, int streamId, int code, int missed, int cached) {
        (void)uid;
        (void)streamId;
        (void)code;
        (void)missed;
        (void)cached;
    }

    virtual void onConnectionStateChanged(IChannel *rtcChannel,
        CONNECTION_STATE_TYPE state,
        CONNECTION_CHANGED_REASON_TYPE reason) {
        (void)rtcChannel;
        (void)state;
        (void)reason;
    }

    virtual void onRequestToken(IChannel *rtcChannel)
    {
        (void)rtcChannel;
    }

    virtual void onUserKicked(int64_t userID, int reason, int kickTime)
    {
        (void)userID;
        (int)reason;
        (int)kickTime;
    }

    virtual void onRemoteVideoStats(IChannel* channel, const RemoteVideoStats& stats)
    {
        (void)stats;
    }

    virtual void onRemoteAudioStats(IChannel* channel, const RemoteAudioStats& stats)
    {
        (void)stats;
    }

    virtual void onAudioPublishStateChange(IChannel *channel, STREAM_PUBLISH_STATE oldState, STREAM_PUBLISH_STATE newState, int elapseSinceLastState) {
        (void)channel;
        (void)oldState;
        (void)newState;
        (void)elapseSinceLastState;
    }

    virtual void onVideoPublishStateChange(IChannel *channel, STREAM_PUBLISH_STATE oldState, STREAM_PUBLISH_STATE newState, int elapseSinceLastState) {
        (void)channel;
        (void)oldState;
        (void)newState;
        (void)elapseSinceLastState;
    }
    
    virtual void onRtmpStreamingStateChanged(IChannel *channel, const char *url, RTMP_STREAM_PUBLISH_STATE state, RTMP_STREAM_PUBLISH_ERROR errCode){
        (void)channel; 
        (void)url;
        (void)state;
        (void)errCode;  
    }
    
    virtual void onAudioSubscribeStateChange(IChannel *channel, int64_t uid, STREAM_SUBSCRIBE_STATE oldState, STREAM_SUBSCRIBE_STATE newState, int elapseSinceLastState) {
        (void)channel;
        (void)uid;
        (void)oldState;
        (void)newState;
        (void)elapseSinceLastState;
    }

    virtual void onVideoSubscribeStateChange(IChannel *channel, int64_t uid, STREAM_SUBSCRIBE_STATE oldState, STREAM_SUBSCRIBE_STATE newState, int elapseSinceLastState) {
        (void)channel;
        (void)uid;
        (void)oldState;
        (void)newState;
        (void)elapseSinceLastState;
    }
};

class IChannel
{
public:
    virtual ~IChannel() {}
    
    virtual int release() = 0;

    virtual int setChannelEventHandler(IChannelEventHandler *chHandler) = 0;

    //virtual int joinChannel(const char* token,
    //                        const char* info,
    //                        int64_t uid,
    //                        const ChannelMediaOptions& options) = 0;
    virtual int joinChannel(const char* token,
        const char* channelID,
        const char* info,
        int64_t uid,
        const ChannelMediaOptions& options) = 0;
    
    virtual int leaveChannel() = 0;

    // 
    virtual int startChannelMediaRelay(const ChannelMediaRelayConfiguration &configuration) = 0;
    
    virtual int updateChannelMediaRelay(const ChannelMediaRelayConfiguration &configuration) = 0;
    
    virtual int stopChannelMediaRelay() = 0;
    // 

    virtual int renewToken(const char* token) = 0;

    virtual const char* channelId() = 0;
    virtual const char* channelIdStr() = 0;
    virtual const char* sessionId()=0;
    
    virtual int setClientRole(CLIENT_ROLE_TYPE role) = 0;

    virtual int muteRemoteVideoStream(int64_t uid, bool mute) = 0;
    virtual int muteRemoteAudioStream(int64_t uid, bool mute) = 0;

    virtual int muteAllRemoteVideoStreams(bool mute) = 0;
    virtual int muteAllRemoteAudioStreams(bool mute) = 0;
    virtual int muteLocalVideoStream(bool mute) = 0;
    virtual int muteLocalAudioStream(bool mute) = 0;

    virtual int setRemoteVideoStreamType(int64_t userId, REMOTE_VIDEO_STREAM_TYPE streamType) = 0;
    virtual int setRemoteDefaultVideoStreamType(REMOTE_VIDEO_STREAM_TYPE streamType) = 0;

    virtual int createDataStream(int* streamId, bool reliable, bool ordered) = 0;
    virtual int sendStreamMessage(int streamId, const char* data, size_t length) = 0;

    virtual int setLiveTranscoding(const LiveTranscoding &transcoding) = 0;
    virtual int addPublishStreamUrl(const char* url, bool transcodingEnabled, bool bAudioOnly = false) = 0;
    virtual int removePublishStreamUrl(const char* url) = 0;

    virtual int setRemoteVideoMirrorMode(int64_t userID, VIDEO_MIRROR_MODE_TYPE mirrorMode) = 0;

};

}


#endif /* __TTTRTCCHANNEL_H__ */
