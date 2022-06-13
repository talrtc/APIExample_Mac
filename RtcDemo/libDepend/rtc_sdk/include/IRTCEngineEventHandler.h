#ifndef IRTCENGINEEVENTHANDLER_H
#define IRTCENGINEEVENTHANDLER_H


#include "./RtcBase.h"


namespace rtc {
        
    /**
    * @brief  引擎内部事件信息
    */
    struct RTC_RESULT
    {
        RTC_RET_CODE ec;    // 结果码
        const char* msg;    // 结果描述
    };

    /**
    * @brief  RTC 内部事件处理接口，外部需要先将事件处理对象注册到引擎中 
    * @see IRTCEngine::registerEventHandler
    */
    class RTCSDKDLL_API IRTCEngineEventHandler
    {
    public:
        virtual ~IRTCEngineEventHandler() {}
        
        /** @deprecated
        * @brief 登陆成功，自己加入了房间
        * @param [in] MyId 自己在房间的Id（如果用户进入房间时，提供了有效的名字，会优先使用）
        */
        virtual void onJoinedRoom(UserId myId, const char* room) {
            (void)myId;
            (void)room;
        };
        
        /**
        * @brief 自己退出房间
        */
        virtual void onLeaveRoom() {};

        /*
        * @brief 新增回调，自己在房间的角色发生变化，角色类型：主播和观众；该变化由 setClientRole 接口触发
        * @para oldRole 原来的角色
        * @para newRole 新的角色
        */
        virtual void onClientRoleChanged(CLIENT_ROLE_TYPE oldRole, CLIENT_ROLE_TYPE newRole) {
            (void)oldRole;
            (void)newRole;
        }

        /**
        * @brief 有用户加入房间; 本sdk主要提供媒体处理，如果需要从这里得到用户加入房间信息，需要和我们进一步确认
        * @param [in] uid 加入房间的用户
        */
        virtual void onUserJoined(const UserId uid) {
            (void)uid;
        }

        /**
        * @brief when 有用户退出房间; 本sdk主要提供媒体处理，如果需要从这里得到用户退出房间信息，需要和我们进一步确认
        * @param [in] uid 退出房间的用户
        */
        virtual void onUserOffline(const UserId uid) {
            (void)uid;
        }

        
        /**
        *    reason:
        *        "join success";
        *         "connecting";
        *          "network interrupted";
        *         "banned by server";
        *         "join failed";
        *         "leave room";
        */
        virtual void onConnectionStateChanged(RTC_CONNECTION_STATE_TYPE state,const char* reason) {
            (void)state;
            (void)reason;
        }


        /**@desc 通话前网络上下行 last mile 质量报告回调。
        *@detail    该回调描述本地用户在加入频道前的 last mile 网络探测的结果，其中 last mile 是指设备到 媒体服务器的网络状态。
                    在调用 enableLastmileTest 之后，该回调函数每 2 秒触发一次。
        *@param quality  网络上下行 last mile 质量
        */
        virtual void onLastMileQuality(NETWORK_QUALITY_TYPE quality) { 
            (void)quality; 
        }
        

        /**
        * @brief 音频设备状态
        */
        virtual void onAudioDeviceStateChanged(const char* deviceId, RTC_MEDIA_DEVICE_TYPE deviceType, RTC_MEDIA_DEVICE_STATE_TYPE deviceState) {
            (void)deviceId;
            (void)deviceType;
            (void)deviceState;
        }
        /**
        * @brief 视频设备状态
        */
        virtual void onVideoDeviceStateChanged(const char* deviceId, RTC_MEDIA_DEVICE_TYPE deviceType, RTC_MEDIA_DEVICE_STATE_TYPE deviceState) {
            (void)deviceId;
            (void)deviceType;
            (void)deviceState;
        }


        /** 在引擎收到并绘制第一视频帧的时候调用， 只有使用sdk渲染才能收到该回调， 使用自渲染收不到该回调
        @param width (pixels) 第一视频帧的宽度
        @param height (pixels) 第一视频帧的高度        
        */
        virtual void onFirstLocalVideoFrame(int width, int height) {
            (void)width;
            (void)height;        
        }
        /** 发送第一个本地音频帧时调用
        */
        virtual void onFirstLocalAudioFrame() {    
        }

        /**
        * @brief 收到远程用户共享的第一帧视频, 只有使用sdk渲染才能收到该回调， 使用自渲染收不到该回调
        */
        virtual void onFirstRemoteVideoFrame(UserId uid, int width, int height) {
            (void)uid;
            (void)width;
            (void)height;
        }
        /**
        * @brief 收到远程用户共享的第一帧音频
        */
        virtual void onFirstRemoteAudioFrame(UserId uid) {
            (void)uid;
        }

        /**onUserMuteVideo回调将被废弃，使用 onRemoteVideoStateChanged
        * @brief 用户分享音视频的过程中, 暂停、继续发送视频
        * @param [in] userId: 远程用户
        * @param [in] isMuting: true：关闭； false:打开
        */
        virtual void onUserMuteVideo(UserId userId, bool isMuting) {
            (void)userId; 
            (void)isMuting; 
        }
        /** 该接口将被弃用，使用onRemoteAudioStateChanged接口
        远端关闭视频和mute， 都使用onUserEnableVideo回调
        * @brief 用户分享音视频的过程中，将音频静音或者取消静音了
        * @param [in] userId: 远程用户
        * @param [in] isMuting: true：静音； false:取消静音
        */
        virtual void onUserMuteAudio(UserId userId, bool isMuting) {
            (void)userId;
            (void)isMuting;
        }

        /**该接口将被废弃， 使用 onRemoteVideoStateChanged 替换
        * @brief 远程用户是否启用视频服务
        * @param [in] userId: 远程用户
        * @param [in] enabled: true：启用； false:关闭，将不发送也不接受视频
        */
        virtual void onUserEnableVideo(UserId userId, bool enabled) {
            (void)userId;
            (void)enabled;
        }

        //该接口被弃用
        ///**
        //* @brief 远程用户是否启用音频服务
        //* @param [in] userId: 远程流对象
        //* @param [in] enabled: true：启用； false:关闭，将不发送也不接受音频
        //*/
        //virtual void onUserEnableAudio(UserId userId, bool enabled) {
        //    (void)userId;
        //    (void)enabled;
        //}



        /**@brief 报告当前说话声音最大的人
        @note        
        - 检测到的是最近小段时间音量最大的人
        @param uid 当前说话声音最大的人. 0表示是自己
        */
        virtual void onActiveSpeaker(UserId uid) {
            (void)uid;
        }

        /**@brief 报告用户的音量 500ms一次
        @param uid 说话用户, 0表示是自己
        @param volume 音量，范围：[0,255]
        */
        virtual void onAudioVolume(UserId uid, unsigned volume) {
            (void)uid;
            (void)volume;
        }
        

        /**
        * @brief LiveRoom 内部一些处理结果通知 
        * @param：code - 执行结果， res中的结果码，请参考 \ref RTC_RET_CODE        
        */        
        virtual void onError(RTC_RET_CODE code, const char* msg) { 
            (void)code; 
            (void)msg;
        }


        /** @brief每两秒钟报告一次当前呼叫会话的统计信息
        *
        *@param 指向RTC引擎统计信息的stats指针 : RTCStats.
        */
        virtual void onRTCStats(const RTCStats& stats) {
            (void)stats;
        }

        /*新增，本端收到sei时间戳的回调， 单位ms， 接收频率取决于发送端的发送频率， 业务端还可以使用getCurSeiTimestamp获取当前最新时间戳**/
        virtual void onSeiTimestampArrived(UserId uid, uint64_t seiTime) {
            (void)uid;
            (void)seiTime;
        }

        /* 新增，rtmp 旁路推流状态发生变化及原因**/
        virtual void onRtmpStreamingStateChanged(const char *url, rtc::RTMP_STREAM_PUBLISH_STATE state, rtc::RTMP_STREAM_PUBLISH_ERROR errCode) {
            (void)url;
            (void)state;
            (void)errCode;
        }

        /*云录制状态回调， 目前该接口用不到，无需继承**/
        virtual void onStartCloudRecordStateChanged(const std::string &auth, int recordId, 
            int state, int errorCode, const std::string &sid) {
            (void)auth;
            (void)recordId;
            (void)state;
            (void)errorCode;
            (void)sid;
        }

        /*云录制状态回调， 目前该接口用不到，无需继承**/
        virtual void onStopCloudRecordStateChanged(const std::string &auth, int recordId, 
            const std::string &sid, int state, int errorCode) {
            (void)auth;
            (void)recordId;
            (void)sid;
            (void)state;
            (void)errorCode;
        }

        /* 新增，rtmp旁路转推时 http 打点的状态回调，只有当state = 0 && errorCode =0 时属于正确回调，否则属于异常回调**/
        virtual void onHttpMetadataStateStateChanged(const std::string &streamId, int category, int state, int errorCode,
            const std::string &errMsg) {
            (void)streamId;
            (void)category;
            (void)state;
            (void)errorCode;
            (void)errMsg;
        }

        /* rtmp 旁路转推时获取调度的回调，只有state 和 errCode都=0时才是正确回调，否则属于异常回调**/
        virtual void onRtmpUplinesStateChanged(const char* streamId, int state, int errCode, const Stru_RtmpUplinesInfo &info) {
            (void)streamId;
            (void)state;
            (void)errCode;
            (void)info;
        }

        /*远端音频状态的回调*/
        /*
        REMOTE_AUDIO_STATE_STOPPED 和 REMOTE_AUDIO_STATE_FROZEN 是不能收到远端流的状态或者远端未发送音频
        REMOTE_AUDIO_STATE_STARTING 和 REMOTE_AUDIO_STATE_DECODING 是已经接收了远端的音频数据和正在解码
        REMOTE_AUDIO_STATE_FAILED 是收到了远端音频数据但是播放失败
        */
        virtual void onRemoteAudioStateChanged(UserId uid, REMOTE_AUDIO_STATE state, REMOTE_AUDIO_STATE_REASON reason, int elapsed) {
            (void)uid;
            (void)state;
            (void)reason;
            (void)elapsed;
        }

        /*远端视频状态回调*/
        /*新增， 远端音频状态变化的接口， 其中
        REMOTE_AUDIO_STATE_STOPPED 和 REMOTE_AUDIO_STATE_FROZEN 是不能收到远端流的状态或者远端未发送视频
        REMOTE_AUDIO_STATE_STARTING 和 REMOTE_AUDIO_STATE_DECODING 是已经接收了远端的视频数据和正在解码
        REMOTE_AUDIO_STATE_FAILED 是收到了远端视频数据但是播放失败
        */
        virtual void onRemoteVideoStateChanged(UserId uid, rtc::RTC_REMOTE_VIDEO_STATE state, 
            rtc::RTC_REMOTE_VIDEO_STATE_REASON reason, int elapsed) {
            (void)uid;
            (void)state;
            (void)reason;
            (void)elapsed;
        }

		/*新增本端视频状态变化.*/

		virtual void onLocalVideoStateChanged(rtc::RTC_LOCAL_VIDEO_STREAM_STATE localVideoState,
			rtc::RTC_LOCAL_VIDEO_STREAM_ERROR error) {
			(void)localVideoState;
			(void)error;
		}

        /*用户被踢通知*/
        virtual void onEngineChangeNotify() {

        }

        /*网络上下行last mile 质量报告回调*/
        virtual void onNetworkQuality(UserId uid, int txQuality, int rxQuality)
        {
            (void)uid;
            (void)txQuality;
            (void)rxQuality;
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

        virtual void onLocalVideoStats(const LocalVideoStats stats)
        {
            (void)stats;
        }

        virtual void onRemoteVideoStats(const RemoteVideoStats stats)
        {
            (void)stats;
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
        /*
        @brief: #RELAY_STATE_RUNNING and #RELAY_OK, 并且
             onChannelMediaRelayEvent #RELAY_EVENT_PACKET_SENT_TO_DEST_CHANNEL, 
             表示成功推流到了目标房间
        */
        virtual void onChannelMediaRelayStateChanged(CHANNEL_MEDIA_RELAY_STATE state, CHANNEL_MEDIA_RELAY_ERROR code)
        {
            (void)state;
            (void)code;
        }

        virtual void onChannelMediaRelayEvent(CHANNEL_MEDIA_RELAY_EVENT code)
        {
            (void)code;
        }
        /**
         * 建议设置的流类型
         * @param uid 用户Id
         * @param currentStreamType 当前流类型
         * @param suitableStreamType 建议流类型
         */
        virtual void onRemoteStreamSubscribeAdvice(UserId uid, rtc::REMOTE_VIDEO_STREAM_TYPE currentStreamType, rtc::REMOTE_VIDEO_STREAM_TYPE suitableStreamType)
        {
            (void)uid;
            (void)currentStreamType;
            (void)suitableStreamType;
        }

		/*
		@brief: 本地音频设备音量回调
		@param: deviceType:  
						- 0：扬声器
						- 1：麦克风
			    volume: 音量大小，范围： 0-255
				muted: 是否静音
		*/
		virtual void onAudioDeviceVolumeChanged(uint8_t deviceType, int volume, bool muted)
		{
			(void)deviceType;
			(void)volume;
			(void)muted;
		}

        /**
         * 音频发布状态改变回调。
         * @param channel 频道名
         * @param oldState 之前的发布状态
         * @param newState 当前的发布状态
         * @param elapseSinceLastState 两次状态变化时间间隔（毫秒）
         */
        virtual void onAudioPublishStateChange(const char *channelId, rtc::STREAM_PUBLISH_STATE oldState, rtc::STREAM_PUBLISH_STATE newState, int elapseSinceLastState)
        {
            (void)channelId;
            (void)oldState;
            (void)newState;
            (void)elapseSinceLastState;
        }

        /**
        * 视频发布状态改变回调。
        * @param channel 频道名
        * @param oldState 之前的发布状态
        * @param newState 当前的发布状态
        * @param elapseSinceLastState 两次状态变化时间间隔（毫秒）
        */
        virtual void onVideoPublishStateChange(const char *channelId, rtc::STREAM_PUBLISH_STATE oldState, rtc::STREAM_PUBLISH_STATE newState, int elapseSinceLastState)
        {
            (void)channelId;
            (void)oldState;
            (void)newState;
            (void)elapseSinceLastState;
        }

        /**
         * 音频订阅状态发生改变回调。
         * @param channel 频道名
         * @param uid 远端用户的 ID。
         * @param oldState 之前的发布状态
         * @param newState 当前的发布状态
         * @param elapseSinceLastState 两次状态变化时间间隔（毫秒）
         */
        virtual void onAudioSubscribeStateChange(const char *channelId, UserId uid, rtc::STREAM_SUBSCRIBE_STATE oldState, rtc::STREAM_SUBSCRIBE_STATE newState, int elapseSinceLastState)
        {
            (void)channelId;
            (void)uid;
            (void)oldState;
            (void)newState;
            (void)elapseSinceLastState;
        }

        /**
         * 音频订阅状态发生改变回调。
         * @param channel 频道名
         * @param uid 远端用户的 ID。
         * @param oldState 之前的发布状态
         * @param newState 当前的发布状态
         * @param elapseSinceLastState 两次状态变化时间间隔（毫秒）
         */
        virtual void onVideoSubscribeStateChange(const char *channelId, UserId uid, rtc::STREAM_SUBSCRIBE_STATE oldState, rtc::STREAM_SUBSCRIBE_STATE newState, int elapseSinceLastState)
        {
            (void)channelId;
            (void)uid;
            (void)oldState;
            (void)newState;
            (void)elapseSinceLastState;
        }
        
        virtual void onLocalAudioStats(const LocalAudioStats stats)
        {
            (void)stats;
        }
	};
	
}
#endif
