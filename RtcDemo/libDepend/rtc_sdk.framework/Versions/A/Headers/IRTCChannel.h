#pragma once

#include "RtcBase.h"

/*
* IRTCChannel 类目前(主版本1.6)中主要用于拉取其他房间的音视频流
* 目前存在两种房间： IRTCEngine 中joinroom 和 IRTCChannel
* 两种房间的区别:
* a.    IRTCEngine中的room， 用户可以以主播身份加入房间，可以推拉流
*        IRTCChannel 以观众身份加入该房间，只能拉取该房间的流， 不能向该房间推流
* b.    IRTCChannel的状态回调通过IRTCChannelEventHandler输出
*
* 其他：
* 数据回调：和之前保持一致
*    音频回调：通过 registerAudioFrameObserver(rtc::IRTCAudioFrameObserver* observer) 注册
*    视频回调：通过 registerVideoFrameObserver(rtc::IRTCVideoFrameObserver* observer) 注册
*/

namespace rtc {

    class IRTCChannelEventHandler;

    class RTCSDKDLL_API IRTCChannel {
    public:

        virtual ~IRTCChannel() {}

        /*
        channelId: 返回当前房间的channelId
        */
        virtual const char *channelId() = 0;

        /*
        release: 释放该channel对象的内存
            如果关注onLeaveChannel的回调，需要调用leaveChannel后等待onLeaveChannel收到后再release channel资源
        */
        virtual void release() = 0;

        /*
        joinChannel: 通过token 获取 IRTCChannel 对象后，可以加入房间
        */
        virtual int joinChannel() = 0;

        /*
        leaveChannel: 通过 IRTCChannel 对象离开房间，离开房间后自动不再接收音视频数据
        */
        virtual int leaveChannel() = 0;

        /*
        * @brief 设置channel 中本用户的角色，默认使用观众模式进入房间；该接口可以在加入房间前或者加入房间后设置；在加入房间后重新设置角色会收到 onClientRoleChanged 回调
        * @para roleType 角色类型：CLIENT_ROLE_BROADCASTER 主播，CLIENT_ROLE_AUDIENCE 观众；
        * @return 成功true; 失败false
        */
        virtual bool setClientRole(CLIENT_ROLE_TYPE roleType) = 0;

        /*
        muteRemoteVideoStream: 设置是否接收该房间内某个用户的视频数据
        userId : 需要设置的用户id
        mute: true: 不再接收该uid的视频数据， false:接收该Uid的视频数据
        返回值： 同步返回结果 true:成功； false:失败
        */
        virtual bool muteRemoteVideoStream(UserId userId, bool mute) = 0;

        /*
        muteRemoteAudioStream: 设置是否接收该房间内某个用户的音频数据
        userId : 需要设置的用户id
        mute: true: 不再接收该uid的音频数据， false:接收该Uid的音频数据
        返回值： 同步返回结果 true:成功； false:失败
        */
        virtual bool muteRemoteAudioStream(UserId userId, bool mute) = 0;

        /*
        registerChannelEventHandler: 注册channel对象的回调函数，用于接收该房间内的一些状态
        */
        virtual void registerChannelEventHandler(IRTCChannelEventHandler *eventHandler) = 0;

        /*
            说明：muteAllRemoteViedoStreams 静音channel房间内所有的视频流
            参数：mute=true：不接收  mute=false：接收
            返回值： 同步返回结果 true:成功   false:失败
        */
        virtual bool muteAllRemoteVideoStreams(bool mute) = 0;

        /*
            说明：muteAllRemoteAudioStreams 静音channel房间内所有的音频流
            参数：mute=true：不接收  mute=false：接收
            返回值： 同步返回结果 true:成功   false:失败
        */
        virtual bool muteAllRemoteAudioStreams(bool mute) = 0;

        virtual int muteLocalVideoStream(bool mute) = 0;
        virtual int muteLocalAudioStream(bool mute) = 0;

        virtual bool getCurSeiTimestamp(uint64_t &seiTime) = 0;

        /*
        @brief:设置接收远端用户流类型
        @param: uid: 远端用户ID，streamType: 流类型，
                     REMOTE_VIDEO_STREAM_HIGH 表示高质量流，
                     REMOTE_VIDEO_STREAM_LOW 表示低质量流
        @return - 0: 成功. - < 0: 失败.
        */
        virtual int setRemoteVideoStreamType(UserId uid, rtc::REMOTE_VIDEO_STREAM_TYPE streamType) = 0;
        /**
         * 设置目标拉流的流类型
         * @param uid 目标用户id
         * @param streamType 流类型 0: 大流  1: 小流   2: 音频
         * @return
         */
        virtual int applyRemoteStreamSubscribeAdvice(UserId userId, rtc::SUBSCRIPTION_STREAM_TYPE streamType) = 0;
		/*
		@brief: 开始跨房间推流，需要在加入房间后才可以调用
		@param: srcToken 推流token
				dstToken 目的房间token
				nDstCount 目的房间token数量，最大是4
		@return - 0: 成功. - < 0: 失败.
		*/
		virtual int startChannelMediaRelay(char *srcToken, char **dstToken, int nDstCount) = 0;
		/*
		@brief: 更新跨房间推流，在startChannelMediaRelay后才可以调用
		@param: srcToken 推流token 
		        dstToken 目的房间token
				nDstCount 目的房间token数量，最大是4
		@return - 0: 成功. - < 0: 失败.
		*/
		virtual int updateChannelMediaRelay(char *srcToken, char **dstToken, int nDstCount) = 0;
		/*
		@brief: 停止跨房间推流
		@return - 0: 成功. - < 0: 失败.
		*/
		virtual int stopChannelMediaRelay() = 0;

		virtual int addPublishStreamUrl(const char *url, bool transcodingEnabled) = 0;

		virtual int removePublishStreamUrl(const char *url) = 0;

		virtual int setLiveTranscoding(const Stru_RtmpLiveTranscoding &transcoding) = 0;


		virtual bool sendSeiTimestamp(uint64_t seiTime) = 0;
		virtual int sendStreamMessage(const char* data, size_t length, uint64_t timestamp) = 0;
		virtual void setSubscribeOption(bool autoSubscribeAudio, bool autoSubscribeVideo) = 0;
	};

    class RTCSDKDLL_API IRTCChannelEventHandler {

    public:
        virtual ~IRTCChannelEventHandler() {}

        /*
        onJoinChannelSuccess: 加入房间成功
        */
        virtual void onJoinChannelSuccess(const char *channelId, uint32_t uid) {
            (void)channelId;
            (void)uid;
        }

        /*
        onLeaveChannel: 离开房间成功
        */
        virtual void onLeaveChannel(const char *channelId) {
            (void)channelId;
        }

        /*
        onUserJoined: 收到该房间内主播uid加入的消息
        */
        virtual void onUserJoined(const char *chcannelId, uint32_t uid) {
            (void)chcannelId;
            (void)uid;
        }

        /*
        onUserOffline: 收到该房间内主播uid离开的消息
        */
        virtual void onUserOffline(const char *channelId, uint32_t uid) {
            (void)channelId;
            (void)uid;
        }

        /*
        onChannelWarning: 房间内警告消息, 同 RtcBash.h 中 RTC_RET_CODE
        */
        virtual void onChannelWarning(const char *channelId, int warn, const char* msg) {
            (void)channelId;
            (void)warn;
            (void)msg;
        }

        /*
        onChannelError: 房间内错误消息, 错误类型同 RtcBase.h 中 RTC_RET_CODE
        */
        virtual void onChannelError(const char *channelId, int err, const char* msg) {
            (void)channelId;
            (void)err;
            (void)msg;
        }

		/*
		onFirstRemoteVideoFrame: 收到远端第一帧视频数据
		*/
		virtual void onFirstRemoteVideoFrame(const char *channelId, uint32_t uid,
			int width, int height, int elapsed) {
			(void)channelId;
			(void)uid;
			(void)width;
			(void)height;
			(void)elapsed;
		}


        /*
        onFirstRemoteAudioFrame: 收到远端第一帧音频数据
        */
        virtual void onFirstRemoteAudioFrame(const char *channelId, uint32_t uid, int elapsed) {
            (void)channelId;
            (void)uid;
            (void)elapsed;
        }

        virtual void onRemoteVideoStateChanged(const char *channelId, uint32_t uid,
            RTC_REMOTE_VIDEO_STATE state, RTC_REMOTE_VIDEO_STATE_REASON reason, int elapsed) {
            (void)channelId;
            (void)uid;
            (void)state;
            (void)reason;
            (void)elapsed;
        }

		/*远端音频状态的回调*/
		/*
		REMOTE_AUDIO_STATE_STOPPED 和 REMOTE_AUDIO_STATE_FROZEN 是不能收到远端流的状态或者远端未发送音频
		REMOTE_AUDIO_STATE_STARTING 和 REMOTE_AUDIO_STATE_DECODING 是已经接收了远端的音频数据和正在解码
		REMOTE_AUDIO_STATE_FAILED 是收到了远端音频数据但是播放失败
		*/
		virtual void onRemoteAudioStateChanged(const char* channelId, uint32_t uid,
			REMOTE_AUDIO_STATE state, REMOTE_AUDIO_STATE_REASON reason, int elapsed) {
			(void)channelId;
			(void)uid;
			(void)state;
			(void)reason;
			(void)elapsed;
		}

        virtual void onRtcStats(const char* channelId, const rtc::RTCStats& stats) {
            (void)channelId;
            (void)stats;
        }

        /*
        * @brief 自己在房间的角色发生变化，角色类型：主播和观众；该变化由 setClientRole 接口触发
        * @para oldRole 原来的角色
        * @para newRole 新的角色
        */
        virtual void onClientRoleChanged(const char* channelId, CLIENT_ROLE_TYPE oldRole, CLIENT_ROLE_TYPE newRole) {
            (void)channelId;
            (void)oldRole;
            (void)newRole;
        }

        /*
        *@brief channel中收到远端的sei时间戳，单位ms
        */
        virtual void onSeiTimestampArrived(const char* channelId, UserId uid, uint64_t seiTime) {
            (void)channelId;
            (void)uid;
            (void)seiTime;
        }

		virtual void onConnectionStateChanged(const char* channelId,
			rtc::RTC_CONNECTION_STATE_TYPE state,
			rtc::CONNECTION_CHANGED_REASON_TYPE reason) {
			(void)channelId;
			(void)state;
			(void)reason;
		}
		/*
		@brief: 远端视频卡顿回调
		@param: uid: 远端用户ID，
				state: VIDEO_BUFFERING_STATE_BUFFERING_START 卡顿开始
				VIDEO_BUFFERING_STATE_BUFFERING_END 卡顿结束
				timestampInMs: 时间戳
		*/
		virtual void onVideoBufferingStateChanged(UserId uid,
			rtc::VIDEO_BUFFERING_STATE state,
			int64_t timestampInMs)
		{
			(void)uid;
			(void)state;
			(void)timestampInMs;
		}
        /*
        @brief: 远端音频卡顿回调
        @param: uid: 远端用户ID，
                state: AUDIO_BUFFERING_STATE_BUFFERING_START 卡顿开始
                AUDIO_BUFFERING_STATE_BUFFERING_END 卡顿结束
                timestampInMs: 时间戳
        */
        virtual void onAudioBufferingStateChanged(UserId uid,
            rtc::AUDIO_BUFFERING_STATE state,
            int64_t timestampInMs)
        {
            (void)uid;
            (void)state;
            (void)timestampInMs;
        }
        /*
		@brief: irc信息
		@param: uid: 远端用户ID，
				data: irc数据
				length: data长度
		*/
		virtual void onStreamMessage(UserId uid, const char* data, size_t length)
		{
			(void)uid;
			(void)data;
			(void)length;
		}
		/**
		 * 建议设置的流类型
		 * @param channelId channel Id
		 * @param uid 用户Id
		 * @param currentStreamType 当前流类型
		 * @param suitableStreamType 建议流类型
		 */
		virtual void onRemoteStreamSubscribeAdvice(const char* channelId, UserId uid, rtc::REMOTE_VIDEO_STREAM_TYPE currentStreamType, rtc::REMOTE_VIDEO_STREAM_TYPE suitableStreamType)
		{
			(void)channelId;
			(void)uid;
			(void)currentStreamType;
			(void)suitableStreamType;
		}

		/**
		 * 音频发布状态改变回调。
		 * @param channelId channelId
		 * @param oldState 之前的发布状态
		 * @param newState 当前的发布状态
		 * @param elapseSinceLastState 两次状态变化时间间隔（毫秒）
		 */
		virtual void onAudioPublishStateChange(const char* channelId, rtc::STREAM_PUBLISH_STATE oldState, rtc::STREAM_PUBLISH_STATE newState, int elapseSinceLastState)
		{
			(void)channelId;
			(void)oldState;
			(void)newState;
			(void)elapseSinceLastState;
		}

		/**
		 * 视频发布状态改变回调。
		 * @param channelId channelId
		 * @param oldState 之前的发布状态
		 * @param newState 当前的发布状态
		 * @param elapseSinceLastState 两次状态变化时间间隔（毫秒）
		 */
		virtual void onVideoPublishStateChange(const char* channelId, rtc::STREAM_PUBLISH_STATE oldState, rtc::STREAM_PUBLISH_STATE newState, int elapseSinceLastState)
		{
			(void)channelId;
			(void)oldState;
			(void)newState;
			(void)elapseSinceLastState;
		}

		/**
		 * 音频订阅状态发生改变回调。
		 * @param channelId channelId
		 * @param uid 远端用户的 ID。
		 * @param oldState 之前的发布状态
		 * @param newState 当前的发布状态
		 * @param elapseSinceLastState 两次状态变化时间间隔（毫秒）
		 */
		virtual void onAudioSubscribeStateChange(const char* channelId, UserId uid, rtc::STREAM_SUBSCRIBE_STATE oldState, rtc::STREAM_SUBSCRIBE_STATE newState, int elapseSinceLastState)
		{
			(void)channelId;
			(void)uid;
			(void)oldState;
			(void)newState;
			(void)elapseSinceLastState;
		}

		/**
		 * 音频订阅状态发生改变回调。
		 * @param channelId channe lId
		 * @param uid 远端用户的 ID。
		 * @param oldState 之前的发布状态
		 * @param newState 当前的发布状态
		 * @param elapseSinceLastState 两次状态变化时间间隔（毫秒）
		 */
		virtual void onVideoSubscribeStateChange(const char* channelId, UserId uid, rtc::STREAM_SUBSCRIBE_STATE oldState, rtc::STREAM_SUBSCRIBE_STATE newState, int elapseSinceLastState)
		{
			(void)channelId;
			(void)uid;
			(void)oldState;
			(void)newState;
			(void)elapseSinceLastState;
		}

		virtual void onRtmpStreamingStateChanged(const char* channelId, const char *url, RTMP_STREAM_PUBLISH_STATE state, RTMP_STREAM_PUBLISH_ERROR errCode) {
			(void)channelId;
			(void)url;
			(void)state;
			(void)errCode;
		}
        
        virtual void onRemoteVideoStats(const RemoteVideoStats stats)
        {
            (void)stats;
        }
        
        virtual void onRemoteAudioStats(const RemoteAudioStats stats)
        {
            (void)stats;
        }
        
#if 0
		virtual void onStreamPublished(const char* channelId, const char *url, int error) {
			(void)channelId;
			(void)url;
			(void)error;
		}

		virtual void onStreamUnpublished(const char* channelId, const char *url) {
			(void)channelId;
			(void)url;
		}

		virtual void onTranscodingUpdated(const char* channelId) {
			(void)channelId;
		}
#endif
	};
}; //namespace rtc
