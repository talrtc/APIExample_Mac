/*****************************************************************************
*  RTC engine sdk
*  Copyright (C) 2018 xueersi
*                                                                            
*  @brief    实时通讯引擎接口
*  Details.                                                                  
*                                                                            
*  @author   rao
*  @email    
*  @version  
*  @date     
*  @license  
*                                                                            
*----------------------------------------------------------------------------
*  Remark         : 
*----------------------------------------------------------------------------
*  Change History :                                                          
*  <Date>     | <Version> | <Author>        | <Description>                   
*----------------------------------------------------------------------------
*                                                                            
*****************************************************************************/

#ifndef IRTCENGINE_H
#define IRTCENGINE_H

#include "RtcBase.h"
#include "IRTCEngineEventHandler.h"


using namespace std;
static const char* RTC_ENGINE_VERSION = "3.25.0.6";

/**@brief
* 实时通讯统一使用rtc（realtime communicate）作为名字空间
*/
namespace rtc {
    
    class IVideoDeviceManager;
    class IPlayoutManager;
    class IMicManager;
    class IRTCChannel;
    class IScreenCaptureSourceList;

    /// 标准的sdk回调函数； 由于系统中有不少是异步调用方式，需要在后续返回结果
    typedef void(*SDK_CB)(RTC_RET_CODE);
    /// log function
    typedef void(*log_func) (const char* msg);

    /**
    * @brief 实时互动房间接口
    */
    class RTCSDKDLL_API IRTCEngine
    {
    public:
        virtual ~IRTCEngine() {}

        /////////////////////////////////////////////////////////////////////////////////
        //                      （一）设备管理
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 设备管理
        /// @{
        /**@brief 视频设备管理
        */
        virtual IVideoDeviceManager* getVideoDeviceManager() = 0;
        /**@brief 播放器设备管理
        */
        virtual IPlayoutManager* getPlayoutManager() = 0;
        /**@brief 麦克（录音）设备管理
        */
        virtual IMicManager* getMicManager() = 0;
        /// @}


        /////////////////////////////////////////////////////////////////////////////////
        //                      （二）入会控制
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 入会控制
        /// @{
        /**@brief 初始化系统        
        * 通过getRtcEngineInstance得到引擎实例后，在在使用引擎的相关功能之前要先调用此函数初始化引擎
        * @param [in] token : 用户的token （用户通过自己的业务服务器获取），使用jwt编码，里面应该包括：
        *                    type: 引擎的类型
        *                    payload： 数据，包括如下内容
        *                         room: room名字（ID）        
        *                        user: 用户的名字；如果不提供，sdk会自动生成一个
        *                        appid:    app的标志
        *                        token: 如果APP启用的了加密认证，则必须有该用户的入会token
        * @param [in] cb : 结果回调函数; 将返回相关的入会前准备结果；
        *                    如果用户提供的token无法解码，会返回状态码：RET_DECODE_FAILED
        *                    入会情况（是否成功）将通过@see onJoinedRoom 事件反应        
        */
        virtual RTC_RET_CODE initWithToken(const char* token, SDK_CB cb = nullptr) = 0;

        /**@brief 初始化系统
        * 通过getRtcEngineInstance得到引擎实例后，在在使用引擎的相关功能之前要先调用此函数初始化引擎
        * @param [in] EnterConfig : 用于初始化的参数
        */
        virtual int init(const EnterConfig &config) = 0;

        /**@brief 加入房间， 房间的信息在initWithToken里面已经指定了
        */ 
        virtual bool joinRoom() = 0;
                
        /**@brief 登出并退出房间，在用户退出房间的时候，应该调用一下，保证系统资源有序清理
        */
        virtual bool leaveRoom() = 0;

        virtual int joinRoomWithRealToken(const char *token, const char * channelID, long uid) = 0;
        /// @}

        /*
        @brief:开始推流
        @return - 0: 成功. - < 0: 失败.
        */
        virtual int publish() = 0;

        /*
        @brief:停止推流
        @return - 0: 成功. - < 0: 失败.
        */
        virtual int unpublish() = 0;

        /////////////////////////////////////////////////////////////////////////////////
        //                      （三）音视频控制
        /////////////////////////////////////////////////////////////////////////////////

        /*
        * @brief 设置Engine中本用户的角色， 默认使用主播模式进入房间；该接口可以在加入房间前或者加入房间后设置；在加入房间后重新设置角色会收到 onClientRoleChanged 回调
        * @para roleType 角色类型：CLIENT_ROLE_BROADCASTER 主播，CLIENT_ROLE_AUDIENCE 观众；
        * @return 成功true; 失败false
        */
        virtual bool setClientRole(CLIENT_ROLE_TYPE roleType) = 0;
        /// @name 音视频控制
        /// @{
        /**@brief 是否打开本地视频流  包括capturer, renderer, and sender
        * @param enable true：打开；false： 关闭
        */
        virtual int enableLocalVideo(bool enable) = 0;
        /**@brief 是否打开本地音频流  包括capturer, renderer, and sender
        * @param enable true：打开；false： 关闭
        */        
        virtual int enableLocalAudio(bool enable) = 0;


        /**@brief 是否发送视频        
        @param mute 是否发送视频
        */
        virtual void muteLocalVideo(bool mute) = 0;
        /**@brief 是否发送音频
        @param enable 是否发送音频
        */
        virtual void muteLocalAudio(bool mute) = 0;
        

        /**@brief 是否接受远程视频    
        @note 远方用户加入，会自动接收其视频（缺省ummute状态）
        @param uid 远程用户Id
        @param mute true：不接受远程视频；false：接受远程视频
        */
        virtual void muteRemoteVideo(UserId uid, bool mute) = 0;
        /**@brief 是否接受远程音频
        @note 远方用户加入，会自动接收其音频（缺省unmute状态）
        @param uid 远程用户Id
        @param mute true：不接受远程音频；false：接受远程音频
        */
        virtual void muteRemoteAudio(UserId uid, bool mute) = 0;
        /**@brief 是否接受所有远程视频
        @param mute true：不接受远程视频；false：接受远程视频
        */
        virtual void muteAllRemoteVideo(bool mute) = 0;
        /**@brief 是否接受所有远程音频
        @param mute true：不接受远程音频；false：接受远程音频
        */
        virtual void muteAllRemoteAudio(bool mute) = 0;
        /// @}

        /**@brief 控制本地播放远端的音量
        @param volume 取值[0,100]
        */
        virtual void setRemoteVolume(UserId uid, int volume, const char* channelId = nullptr) = 0;
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //                      （四）视频预览，显示
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 视频预览、显示
        /// @{
        /**@brief 设置本地预览的显示窗口
        *  @param view 本地预览的显示窗口，如果为0，就不显示了
        */
        virtual void setupLocalVideo(WindowIdType view) = 0;
        /**@brief 设置本地预览显示的方式
        *  @param mode 显示方式， \ref rtc::RTC_VIDEO_RENDER_MODE
        */
        virtual void setLocalRenderMode(RTC_VIDEO_RENDER_MODE mode) = 0;
        /**@brief 开始本地视频预览        
            在调用本方法之前，应先：
        - 调用 \ref IRTCEngine::setupLocalVideo 设置本地预览窗口
        - 调用 \ref IRTCEngine::setupLocalRenderMode 设置本地预览显示方式        
        */
        virtual void startPreview() = 0;
        /**@brief 结束本地视频预览
        */
        virtual void stopPreview() = 0;


        /**@brief 设置远程视频的显示窗口
        *  @param uid 远程用户Id
        *  @param view 本地预览的显示窗口，如果为0，就不显示了
        */
        virtual void setupRemoteVideo(UserId uid, WindowIdType view, const char* channelId = nullptr) = 0;
        /**@brief 设置远程用户窗口显示方式
        *  @param uid 远程用户Id
        *  @param mode 显示方式， \ref rtc::RTC_VIDEO_RENDER_MODE
        */
        virtual void setRemoteRenderMode(UserId uid, RTC_VIDEO_RENDER_MODE mode) = 0;
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //                      （五）发送流参数设置
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 发送流参数设置
        /// @{
        /**@brief 设置发送视频的比特率
        *  @param videoBitrate 视频的比特率    @see 取值[50, 10000]     
        */

        virtual int setVideoResolution(int w, int h) = 0;
		/**@brief 设置音频采样率
		*  @param bitrate 音频码率 processmode 音频采样率模式
		*/
		virtual int setAudioEncoderConfiguration(int bitrate, int processmode) = 0;

        virtual void setVideoBitrate(int videoBitrate) = 0;
        /**@brief 设置发送视频的帧率
        *  @param videoFps 视频帧率
        */
        virtual void setVideoFps(unsigned videoFps) = 0;
        /**@brief 设置发送视频的分辨率
        *  @param videoResolution 视频的分辨率 \ref RTC_VIDEO_RESOLUTION 
        */
        virtual void setVideoResolution(RTC_VIDEO_RESOLUTION videoResolution) = 0;
        /**@brief 设置本地（待发送）视频是否镜像处理
        *  @param isMirror 是否镜像
        */
        virtual void setMirror(bool isMirror) = 0;

		virtual void setRemoteMirror(unsigned uid, bool isMirror) = 0;
		/**@brief 设置本地录制音频参数 通过onRecordAudioFrame返回
		*  @param sampleRate 设置采集的音频的采样率 {8000，16000，32000，44100,48000}
		*  @param nChannel 设置采集的音频声道数   1 mono,2 stereo
		*  @para  nIntervalTime，取值 [1, 1000], 影响onRecordAudioFrame回调的间隔，单位ms,
					默认 10ms 回调onRecordAudioFrame一次
		*/
		virtual void setAudioRecordedParam(unsigned sampleRate, unsigned nChannel, int nIntervalTime = 0) = 0;

        /////////////////////////////////////////////////////////////////////////////////
        //                      （六）使用外部数据
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 使用外部数据
        /// @{
        /**@brief 是否启用外部音频数据
        * @param enable 是否用外部的音频数据源(系统默认不使用）
        * @param sampleRate 外部音频数据的采样率, 目前可以是： 8000, 16000, 32000, 44100, or 48000.
        * @param channels 外部数据源的通道数量(最多为2).
        * @notice sampleRate和channels必须和pushExternalAudioFrame中数据中指定的格式一致，否则语音无法正常播放
        *  @return
        - true: Success.
        - false: Failure.        
        */
        virtual bool enableExternalAudioSource(bool enable, int sampleRate, int channels) = 0;
        /**@brief 推送外部音频帧， 注意必须在 enableExternalAudioSource(true)成功之后，才能推送外部视频        
        *  @param frame 音频数据帧
        */
        virtual void pushExternalAudioFrame(ExternalAudioFrame* frame) = 0;
        
        /**@brief 设置是否使用外部视频源
        @param enable
        - true: 使用外部视频源
        - false: 不使用外部视频源(系统默认）
        @return
        - true: Success.
        - false: Failure.
        */
        virtual bool enableExternalVideoSource(bool enable) = 0;
        /**@brief 推送外部视频数据，注意必须在 enableExternalVideoSource(true)成功之后，才能推送外部视频        
        * @param [in] frame 外部视频帧， @see ExternalVideoFrame        
        * */
        virtual void pushExternalVideoFrame(ExternalVideoFrame* frame) = 0;
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //                      （七）音视频拦截
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 音视频拦截
        /// @{
        /**@brief 注册音频帧观察对象，当有音频数据时，引擎会调用其中对应的方法
        \ref IRTCAudioFrameObserver::onRecordAudioFrame "onRecordAudioFrame" or \ref IAudioFrameObserver::onPlaybackAudioFrame "onPlaybackAudioFrame" callback.
        * 必须在initWithToken之后调用，才可能成功
        @param observer 音频帧观察对象. 如果为null，则取消观察
        @return
        - true: Success.
        - false: Failure.
        */
        virtual bool registerAudioFrameObserver(rtc::IRTCAudioFrameObserver* observer) = 0;

        /**@brief 注册视频帧观察对象，当有视频数据时，引擎会调用其中对应的方法
        \ref IRTCVideoFrameObserver::onCaptureVideoFrame "onCaptureVideoFrame" or \ref IARTCVideoFrameObserver::onRenderVideoFrame "onRenderVideoFrame" callback.        
        * 必须在initWithToken之后调用，才可能成功
        @param observer 音频帧观察对象. 如果为null，则取消观察    
        @return
        - true: Success.
        - false: Failure.
        */
        virtual bool registerVideoFrameObserver(rtc::IRTCVideoFrameObserver* observer) = 0;

        /** Retrieves the SDK version number.
         @param build Pointer to the build number.
         @return The version of the current SDK in the string format. For example, 2.3.1.
         */
        virtual const char* getVersion(int* build) = 0;


        /**@brief 该接口暂不支持
        */        
        virtual void registerLogFunc(log_func fun, RTCLOGTYPE type = RTCLOGTYPE_INFO) {
            (void)fun;
            (void)type;
        }

        /**@brief 注册事件处理器，处理rtc engine内部发出后的事件
        * 如果要取消监听事件，将参数设置为nullptr即可
        * @param [in] evengHandler : 事件处理对象
        */ 
        virtual void registerEventHandler(IRTCEngineEventHandler *eventHandler) = 0;    


        /**@desc 启动网络测试。
        *@detail 该方法启用网络连接质量测试，用于检测用户目前的网络接入质量（上行网络质量）。默认该功能为关闭状态。
            启用该方法会消耗网络流量，影响通话质量。用户应该在收到 onLastmileQuality 回调后调用 disableLastmileTest 停止测试。
            注意：调用该方法后，在收到 onLastmileQuality 回调前请勿调用其他方法，否则可能由于 API 操作过于频繁导致回调无法执行。
        *@return 返回  0: 方法调用成功；<0: 方法调用失败。
        */
        virtual int enableLastmileTest() = 0;

        /**@desc 停止网络测试。 */
        virtual int disableLastmileTest() = 0;

        /// @}


        /*
        是否开启声卡采集:启用声卡采集功能后，声卡播放的声音会被合到本地音频流中，从而可以发送到远端
        enabled:    true: 开启声卡采集
                    false: （默认）关闭声卡采集
        deviceName:声卡的设备名。默认设为 NULL，即使用当前声卡采集。
                    如果用户使用虚拟声卡，如 “Soundflower”，可以将虚拟声卡名称 “Soundflower” 作为参数，SDK 会找到对应的虚拟声卡设备，并开始采集。
        */
        virtual int enableLoopbackRecording(bool enabled, const char *     deviceName = nullptr) = 0;


        /**初始化token获取仅用于拉流的channel对象
        * @token: 通过token 获取房间对象
        */
        virtual IRTCChannel* initChannelToken(const char* token) = 0;

        /*
        设置私有接口
        */
        virtual int setConfig(const char* config) = 0;

        /*
        *设置日志存储路径和日志大小， 默认2M，该接口需要在initWithToken前调用
        */
        virtual bool setLogFileConfig(const char* path, int logSize = 2*1024) = 0;

        /*
        *设置改变日志存储路径 前提：初始化之后
        */
        virtual int setLogFile(const char* filePath) = 0;

        /*
        *设置改变每个日志文件大小 前提：初始化之后
        */
        virtual int setLogFileSize(unsigned int fileSizeInKBytes) = 0;

        /*
        *设置改变日志级别 前提：初始化之后
        */
        virtual int setLogFilter(unsigned int filter) = 0;

    /////////////////////////////////////////////////////////////////////////////////
    //                      （八）录制功能
        /*
        现在录制部分提供的接口：主动sei/自动sei； http打点；rtmp调度； rtmp转推
        sei功能：在视频中增加sei时间戳，业务可以使用sendSeiTimestamp发送单次sei时间戳，或者开启自动化enableAutoTimestamp
        http打点：通过sendHttpMetadata增加http的打点信息
        rtmp调度：获取rtmp推流的节点及url，接口getRtmpUploadLines
        rtmp转推：rtc直播中转推rtmp流，接口addPublishStreamUrl/removePublishStreamUrl
        */
    /////////////////////////////////////////////////////////////////////////////////

        /*
        @brief 发送sei时间戳，单位毫秒，对端通过onSeiTimestampArrived回调收到sei时间戳,该接口需要在加入房间后调用,
            注意如果不需要发送sei，千万不要调用该接口，否则会变成主播身份
        @return =0 : Success  or <0 Failer
        */
        virtual bool sendSeiTimestamp(uint64_t seiTime) = 0;

        /*
        @brief 获取sei时间戳， sei时间戳是通过 onSeiTimestampArrived 回调来更新的
        @return  =0 Success; or <0 Failer
        */
        virtual bool getCurSeiTimestamp(uint64_t &seiTime) = 0;

        virtual bool setLiveTranscoding(const Stru_RtmpLiveTranscoding &config) = 0;

        /*
        *@brief: 增加rtmp旁路推流功能，当transcodingEnabled=true时，要先调用setLiveTranscoding接口来设置转码参数，
                    接口结果通过异步回调 onRtmpStreamingStateChanged
        *@para: 转推rtmp的地址；transcodingEnabled是否开启转码推流
        *@return: true:该接口执行成功； false:该接口执行失败
        */
        virtual bool addPublishStreamUrl(const char *url, bool transcodingEnabled = true) = 0;

        /*
        *@brief:  删除对应url流的旁路推流功能， 该接口会触发回调 onRtmpStreamingStateChanged
        */
        virtual bool removePublishStreamUrl(const char *url) = 0;
        
        /*自动化发送sei时间戳*/
        /*使用业务设置的时间戳setServerTimestamp， 该接口需要在enableAutoTimestamp之前调用**/
        virtual bool setServerTimestamp(uint64_t timestampMs) = 0;
        /*
        @brief: 开始/停止自动化sei，该接口需要在onJoinChannelSuccess回调后使用， 
            注意如果不需要发送sei，千万不要调用该接口，否则会变成主播身份
        @para enable: true：开始自动化sei； stop:停止自动化sei
        @para freq: 每秒钟发送sei的次数
        @return true： 该接口执行成功； false :该接口执行失败
        */
        virtual bool enableAutoTimestamp(bool enable, float freq) = 0;

        /** 
        @brief:开启/关闭检黄功能
       
        @return 0: Success, < 0: Failure.
        */
        /*
        @brief: 开启/关闭检黄/监课截图
        @param  enabled : true: 开启. false: 关闭.
                extInfo: 自定义数据
                interval: 截图时间间隔
                featureRate 检测类型的频率数组，值为基础频率的倍数，基础频率10s一次
                featureRate[0]是截图的上传频率
                featureRate[1]是AI监课的上传频率, example: featureRate[0] = 2；意味着20s上传一次
                contentInspectType 模式类型 默认填0
                count: 检测类型数量、featureRate数组的大小，featureRate数组最大值为32
        @return - 0: 成功. - < 0: 失败.
        */
        virtual int enableContentInspect(bool enabled, int interval, char* extInfo, int featureRate[], int contentInspectType[], int count) = 0;
        /*
        @brief:双发流端音画同步
        @param channelId: 远端房间ID. uid: 房间中的一个用户.
        @return 0: Success, < 0: Failure.
        */
        virtual int setAVSyncSource(const char *channelId, UserId uid) = 0;
        /*
        @brief:获取最新的扬声器设备管理器，获取后如果再调getPlayoutManager得到的仍然是新设备管理器
        */
        virtual IPlayoutManager* getNewPlayoutManager() = 0;
        /*
        @brief:获取最新的麦克风设备管理器，获取后如果再调getMicManager得到的仍然是新设备管理器
        */
        virtual IMicManager* getNewMicManager() = 0;
        /*
        @brief:获取最新的摄像头设备管理器，获取后如果再调getVideoDeviceManager得到的仍然是新设备管理器
        */
        virtual IVideoDeviceManager* getNewVideoDeviceManager() = 0;
        /*
        @brief:设置SDK层的麦克风音量，建议只设置0和100这两种值，可以和设备IMicManager的setVolume同时调用
        @param:volume音量大小，0表示静音，100表示原始音量（同设备的真实音量相等）
        @return - 0: 成功. - < 0: 失败.
        */
        virtual int adjustRecordingSignalVolume(int volume) = 0;
        /*
        @brief: 开启大小流
        @param: enabled: true 开启，false 不开启
        @return - 0: 成功. - < 0: 失败.
        */
        virtual int enableDualStreamMode(bool enabled) = 0;
        /**
         * 设置弱网条件下订阅的音视频流回退选项。
         * @param option 0: 不回退   1: 小流    2: 声音
         * @return 0 成功
         */
        virtual int setRemoteSubscribeFallbackOption(rtc::STREAM_FALLBACK_OPTIONS option) = 0;
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
        @brief:调节本地播放的所有远端用户音量
        @param: volume: 0  : 关闭
                        100: 原始音量
                        400：4倍原始音量
        @return - 0: 成功. - < 0: 失败.
        */
        virtual int adjustPlaybackSignalVolume(int volume) = 0;
        /*
        @brief:设置本端业务角色
        @param: role: BUSI_USER_ROLE_STUDENT  : 学生
                      BUSI_USER_ROLE_TEACHER  : 主讲
                      BUSI_USER_ROLE_TUTOR    ：辅导
        @return - 0: 成功. - < 0: 失败.
        */
        virtual int setBusinessUserRole(rtc::BUSI_USER_ROLE role) = 0;

        /** 
        @brief:解析token
        @param token : 输入token
               channelId : 房间号
               userId:     用户ID
        @return true: Success, false: Failure.
        */
        virtual bool parseToken(const char* token, uint64_t& channelId, uint64_t& userId) = 0;
        /*
        @brief:设置接收远端用户是否SDK旋转角度, 目前是轻直播专用
        @param: bRotation: true: 需要SDK旋转(默认)
                           false:不需要SDK旋转
        @return - 0: 成功. - < 0: 失败.
        */
        virtual int setRotationApplied(bool bRotation) = 0;
        /*
        @brief:为onPlaybackAudioFrame设置回调数据的格式
        @param: sampleRate: 8000, 16000, 32000, 44100, or 48000 Hz.
                channel   :    1: 单声道,  2: 立体声
                mode      ：请见RAW_AUDIO_FRAME_OP_MODE_TYPE
                samplesPerCall： 每次返回采样点数
        @return - 0: 成功. - < 0: 失败.
        */
        virtual int setPlaybackAudioFrameParameters(int sampleRate, int channel, RAW_AUDIO_FRAME_OP_MODE_TYPE mode, int samplesPerCall) = 0;
        /*
        @brief: 发送irc信息
        @param: 
                data: irc数据
                length: data长度, 最大1000
                timestamp：时间戳
        @return - 0: 成功. - < 0: 失败.
        */
        virtual int sendStreamMessage(const char* data, size_t length, uint64_t timestamp) = 0;
        /*
        @brief: 设置远端用户的优先级。如果将某个用户的优先级设为高，那么发给这个用户的音视频流的优先级就会高于其他用户。
                弱网下 SDK 会优先保证高优先级用户收到的流的质量。
                仅允许将一名远端用户设为高优先级。
                该方法需要在加入频道前调用。
        @param: 
                uid: 用户ID
                userPriority: PRIORITY_HIGH 高优先级
                              PRIORITY_NORMAL 正常
        @return - 0: 成功. - < 0: 失败.
        */
        virtual int setRemoteUserPriority(UserId uid, PRIORITY_TYPE userPriority) = 0;
        /*
        @brief: 引擎是否支持某种能力
        @param: feature 能力
        @return - 1: 支持. - 0: 不支持. - < 0: 错误.
        */
        virtual int checkFeatureSupport(RTCFeature feature) = 0;
        /*
        @brief: 获取引擎类型
        @return - 1: 声网. - 7: 自研. - < 0: 错误.
        */
        virtual int getEngineType() = 0;
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

        virtual IScreenCaptureSourceList* getScreenCaptureSources(const SIZE &thumbSize, const SIZE &iconSize, const bool includeScreen) = 0;
        virtual int startScreenCaptureByDisplayId(unsigned int displayId, const Rectangle &regionRect, const ScreenCaptureParameters &captureParams) = 0;
        virtual int startScreenCaptureByScreenRect(const Rectangle &screenRect, const Rectangle &regionRect, const ScreenCaptureParameters &captureParams) = 0;
        virtual int startScreenCaptureByWindowId(view_t windowId, const Rectangle &regionRect, const ScreenCaptureParameters &captureParams) = 0;
        virtual int updateScreenCaptureParameters(const ScreenCaptureParameters &captureParams) = 0;
        virtual int updateScreenCaptureRegion(const Rectangle &regionRect) = 0;
        virtual int stopScreenCapture() = 0;


		virtual RTC_RET_CODE initWithEngineInfo(char* token, char* appId, char* roomId, unsigned int userId, int engineType) = 0;

		virtual IRTCChannel* initWithChannelInfo(char* token, char* appId, char* channelId, unsigned int userId, int engineType) = 0;
        
        virtual bool enableAudioVolumeIndication(int interval) = 0;
        
        virtual void setCaptureResolution(int w, int h) = 0;

		virtual void setSubscribeOption(bool autoSubscribeAudio, bool autoSubscribeVideo) = 0;
};

    class IScreenCaptureSourceList {
    public:
        virtual ~IScreenCaptureSourceList() {}

        virtual unsigned int getCount() = 0;
        virtual ScreenCaptureSourceInfo getSourceInfo(unsigned int index) = 0;
        virtual void release() = 0;
    };

    class IVideoDeviceManager {
    public:
        virtual ~IVideoDeviceManager() {}

        /**@brief 获取摄像头数量
        * @return 返回摄像头数量
        */
        virtual unsigned getDeviceCount() = 0;

        /**@brief 获取当前使用的摄像头设备Id
        * @devceId 当前摄像头的设备ID
        * @return true：找打当前使用的摄像头； false：没有找到当前使用的摄像头
        */
        virtual bool getCurDeviceID(char* deviceId) = 0;
        /**@brief 设置当前使用的摄像头
        * @param devideIndex 摄像头的编号（从0开始， @see IVideoDeviceManager::getDevice）
        * @return 是否成功设置了
        */
        virtual bool setCurDevice(unsigned deviceIndex) = 0;
        /**@brief 设置当前使用的摄像头
        * @param devideId 摄像头的Id
        * @return 是否成功设置了
        */
        virtual bool setCurDeviceID(const char* deviceId) = 0;

		/**@brief 获取摄像头信息
		* @param nIndex 摄像头的编号（从0开始）
		* @param deviceName 摄像头的名字
		* @param deviceID 摄像头的ID
		* @return 是否成功找到摄像头
		*/
		virtual bool getDevice(unsigned nIndex, char* deviceName, char* deviceID) = 0;
	};


    class IPlayoutManager
    {
    public:    
        virtual ~IPlayoutManager() {};
        
        /**@brief 获取当前系统音量 [0~255]
        * @return 返回当前使用的系统音量, 如果无法获取，则返回-1
        */
        virtual int getVolume() = 0;
        /**@brief 设置系统音量
        * @param nVol 音量值  [0~255]
        * @return 是否成功设置了系统音量
        */
        virtual bool setVolume(unsigned nVol) = 0; 
        /**@brief 获取当前使用的播放器音量 [0~255]
        * @return 返回当前使用的播放器音量, 如果无法获取，则返回-1
        */
        virtual int getAppVolume() = 0;
        /**@brief 设置当前使用的播放器音量
        * @param nVol 音量值  [0~100]
        * @return 是否成功设置了当前使用的播放器音量
        */
        virtual bool setAppVolume(unsigned nVol) = 0;

        /**@brief 获取播放器数量
        * @return 返回播放器数量
        */
        virtual unsigned getDeviceCount() = 0;

        /**@brief 获取当前使用的播放器设备Id
        * @devceId 当前播放器的设备ID
        * @return true：找打当前使用的播放器； false：没有找到当前使用的播放器
        */
        virtual bool getCurDeviceID(char* deviceId) = 0;
        /**@brief 设置当前使用的播放器
        * @param devideIndex 播放器的编号（从0开始， @see IPlayoutManager::getDevice）
        * @return 是否成功设置了
        */
        virtual bool setCurDevice(unsigned deviceIndex) = 0;
        /**@brief 设置当前使用的播放器
        * @param devideId 播放器的Id
        * @return 是否成功设置了
        */
        virtual bool setCurDeviceID(const char* deviceId) = 0;

		/**@brief 获取播放器信息
		* @param nIndex 播放器的编号（从0开始）
		* @param deviceName 播放器的名字
		* @param deviceID 播放器的ID
		* @return 是否成功找到播放器
		*/
		virtual bool getDevice(unsigned nIndex, char* deviceName, char* deviceID) = 0;
		/**@brief 获取所有播放器信息
		* @param deviceName 播放器的名字
		* @param deviceID 播放器的ID
		* @return 播放器的数量
		*/
		virtual int getAllDeviceInfo(char deviceName[rtc::MAX_DEVICE_COUNT][rtc::MAX_DEVICE_ID_LENGTH],
									 char deviceID[rtc::MAX_DEVICE_COUNT][rtc::MAX_DEVICE_ID_LENGTH]) = 0;
		/**@brief 获取当前播放器信息
		* @param deviceName 播放器的名字
		* @param deviceID 播放器的ID
		* @return 是否成功找到播放器
		*/
		virtual bool getCurDeviceInfo(char deviceName[rtc::MAX_DEVICE_ID_LENGTH], char deviceID[rtc::MAX_DEVICE_ID_LENGTH]) = 0;

		virtual int setApplicationVolume(int volume) = 0;
		virtual int getApplicationVolume(int &volume) = 0;
		virtual int setApplicationMute(bool mute) = 0;
		virtual int isApplicationMute(bool &mute) = 0;
		virtual int setPlaybackDeviceMute(bool mute) = 0;
	};
	
	class IMicManager
	{
	public:
		virtual ~IMicManager() {};

        /**@brief 获取当前使用的麦克音量 [0~255]
        * @return 返回当前使用的麦克音量, 如果无法获取，则返回-1
        */
        virtual int getVolume() = 0;
        /**@brief 设置当前使用的麦克音量
        * @param nVol 音量值 [0~255]
        * @return 是否成功设置了当前使用的麦克音量
        */
        virtual bool setVolume(unsigned nVol) = 0;

        /**@brief 获取麦克数量
        * @return 返回麦克数量
        */
        virtual unsigned getDeviceCount() = 0;

        /**@brief 获取当前使用的麦克设备Id
        * @devceId 当前麦克的设备ID
        * @return true：找打当前使用的麦克； false：没有找到当前使用的麦克
        */
        virtual bool getCurDeviceID(char* deviceId) = 0;
        /**@brief 设置当前使用的麦克
        * @param devideIndex 麦克的编号（从0开始， @see IMicManager::getDevice）
        * @return 是否成功设置了
        */
        virtual bool setCurDevice(unsigned deviceIndex) = 0;
        /**@brief 设置当前使用的麦克
        * @param devideId 麦克的Id
        * @return 是否成功设置了
        */
        virtual bool setCurDeviceID(const char* deviceId) = 0;

		/**@brief 获取麦克信息
		* @param nIndex 麦克的编号（从0开始）
		* @param deviceName 麦克的名字
		* @param deviceID 麦克的ID
		* @return 是否成功找到麦克
		*/
		virtual bool getDevice(unsigned nIndex, char* deviceName, char* deviceID) = 0;
		/*
		@brief:开始麦克风检测，检测音频采集设备是否正常工作
			   调用该方法后，SDK会按照设置的时间间隔触发onAudioVolumeIndication报告应用的音量
		@param: indicationInterval: 时间间隔(毫秒)	
		@return - 0: 成功. - < 0: 失败.
		*/
		virtual int startRecordingDeviceTest(int indicationInterval) = 0;
		/*
		@brief:停止麦克风检测
		@return - 0: 成功. - < 0: 失败.
		*/
		virtual int stopRecordingDeviceTest() = 0;
		virtual int setRecordingDeviceMute(bool mute) = 0;
	};
}

extern "C" {
    /**@brief 创建rtcEngine对象，对象是全局唯一的，多次调用返回的结果相同，非多线程安全    
    * @return 返回创建的对象
    */
    RTCSDKDLL_API rtc::IRTCEngine* getRtcEngineInstance();
    /**@brief 释放通过getRtcEngineInstance得到的对象，只需要在应用结束时调用一次
    * @return 返回创建的对象
    */
    RTCSDKDLL_API void releaseRtcEngineInstance();
    /**@brief 绑定用户日志处理函数，系统内部的日志通过这里给出的函数输出
    *@deprecated    
    */
    typedef void (*log_func) (const char* msg);
    RTCSDKDLL_API void registerLogFunc(log_func fun, rtc::RTCLOGTYPE type = rtc::RTCLOGTYPE_INFO);
}
#endif
