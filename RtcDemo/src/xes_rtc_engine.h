#pragma once
#ifndef XESRTCENGINE_H
#define XESRTCENGINE_H

#include "IRTCEngine.h"
#include "IRTCChannel.h"

#include <QObject>
#include <time.h>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <QFile>
#include <QDebug>
#include <qapplication.h>
#include "mainwindow.h"
#include  "Context.h"
#include <QMetaType>

Q_DECLARE_METATYPE(rtc::UserId);
Q_DECLARE_METATYPE(RtcRoleType);
Q_DECLARE_METATYPE(uint64_t);
//Q_DECLARE_METATYPE(size_t);
Q_DECLARE_METATYPE(rtc::Stru_RtmpUplinesInfo);

extern rtc::IRTCEngine* rtcEngineInstance;
extern MainWindow* mpMainWindow;
class VideoFrameObserver : public rtc::IRTCVideoFrameObserver {

public:
    virtual bool onCaptureVideoFrame(rtc::YuvI420VideoFrame& videoFrame) {
#pragma message("...")

        if (mpMainWindow) {
            mpMainWindow->SendYuvData(0, (unsigned char*)(videoFrame.yBuffer), (unsigned char*)(videoFrame.uBuffer), (unsigned char*)(videoFrame.vBuffer),
                videoFrame.yStride, videoFrame.uStride, videoFrame.vStride,
                videoFrame.width, videoFrame.height);
        }
        return true;
    }

    virtual bool onScaleRenderVideoFrame(rtc::YuvI420VideoFrame& videoFrame)
    {
        return true;
    }

    virtual bool onRenderVideoFrame(rtc::UserId uid, rtc::YuvI420VideoFrame& videoFrame) {



        //qDebug() << Q_FUNC_INFO << "linatest, uid=" << uid << ", w=" << videoFrame.width << ", h=" << videoFrame.height << ", rotation=" << videoFrame.rotation;
        if (mpMainWindow) {
            mpMainWindow->SendYuvData(uid, (unsigned char*)(videoFrame.yBuffer), (unsigned char*)(videoFrame.uBuffer), (unsigned char*)(videoFrame.vBuffer),
                videoFrame.yStride, videoFrame.uStride, videoFrame.vStride,
                videoFrame.width, videoFrame.height);
        }

        /*int nYUVBufsize = 0;
        int nVOffset = 0;
        static char *m_uBuffer = (char*)malloc(videoFrame.width * videoFrame.height * 3/2 + 1);
        memset(m_uBuffer, 0, sizeof(m_uBuffer));

        for (int i = 0; i < videoFrame.height; ++i) {
            memcpy(m_uBuffer + nYUVBufsize, (char*)videoFrame.yBuffer + i * videoFrame.yStride, videoFrame.width);
            nYUVBufsize += videoFrame.width;
        }
        for (int i = 0; i < (videoFrame.height >> 1); ++i) {
            memcpy(m_uBuffer + nYUVBufsize, (char*)videoFrame.uBuffer + i * videoFrame.uStride, videoFrame.width >> 1);
            nYUVBufsize += (videoFrame.width >> 1);
            memcpy(m_uBuffer + videoFrame.width * videoFrame.height * 5 / 4 + nVOffset,
                (char*)videoFrame.vBuffer + i * videoFrame.vStride, videoFrame.width >> 1);
            nVOffset += (videoFrame.width >> 1);
        }



        static int index = 0;
        if (index < 500 && uid > 0) {
            QString name = "D:\\" + QString::number(index) + ".yuv";

            QFile file(name);
            if (file.open(QIODevice::ReadWrite)) {
                file.write(m_uBuffer);
            }

            file.close();
        }
        index += 10;*/

        return true;


        //LOG_INFO_("current thread is: " << GetCurrentThreadId());
        std::lock_guard<std::mutex> guard(capMutex_);
        int64_t wh = userReceives[uid];
        if (wh != 0 && ((wh >> 16) != videoFrame.width || (wh & 0xffff) != videoFrame.height)) {
            qDebug() << Q_FUNC_INFO << "!!!User: " << uid << ", origin:  " << (wh >> 16) << ": "
                << (wh & 0xffff) << ", current: " << videoFrame.width << ": " << videoFrame.height;
        }
        else {
            //LOG_INFO_("origin:  " << (wh >> 16) << ": " << (wh & 0xffff) << ", current: " << videoFrame.width << ": " << videoFrame.height);
        }
        userReceives[uid] = (videoFrame.width << 16) + videoFrame.height;


        return true;
    }

    virtual bool onPreEncodeVideoFrame(rtc::YuvI420VideoFrame& videoFrame) {
        return true;
    }
public:
    MainWindow * mpMainWindow = nullptr;
    std::mutex  capMutex_;
    map<rtc::UserId, int32_t>   userReceives;
    int32_t		userSend = 0;
};


class audioFrameObserver : public rtc::IRTCAudioFrameObserver {

public:
    virtual ~audioFrameObserver() override {}

    /**@brief 本机麦克风采集到的音频数据
    @note: 腾讯有但数据不可修改，声网有且可修改
    Callback occuring once every 10 ms. Retrieves the recorded audio frame.
    @param audioFrame Pointer to AudioFrame.
    @return
    - true: Valid buffer in AudioFrame, and the recorded audio frame is sent out.
    - false: Invalid buffer in AudioFrame, and the recorded audio frame ise discarded.
    */
    virtual bool onRecordAudioFrame(rtc::ExternalAudioFrame& audioFrame) override {
        (void)audioFrame;
        //#ifdef TEST_RECORD_AUDIO
        const unsigned MaxAudioLen = 5000000; // 5MB
        if (recordLocalAudio_) {
            static int len = 0;
            static FILE *pcmFile = nullptr;

            if (pcmFile) {
                len += audioFrame.samples * audioFrame.channels * 2;
                if (len < MaxAudioLen) {
                    fwrite(audioFrame.buffer, audioFrame.bytesPerSample, audioFrame.samples * audioFrame.channels, pcmFile);
                }
                else {
                    fclose(pcmFile);
                    len = 0;
                    pcmFile = nullptr;
                    recordLocalAudio_ = false;
                }
            }
            else {
                char filename[1000] = { 0 };
                std::time_t now_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                std::stringstream ss;
                ss << QCoreApplication::applicationDirPath().toStdString() << "/local_" << std::put_time(std::localtime(&now_c), "%Y-%m-%d_%H-%M-%S") << ".pcm";
                ss >> filename;

                pcmFile = fopen(filename, "wb");
            }
        }
        //#endif
        return true;
    }
    /**@brief 各路音频数据混合后送入扬声器播放的音频数据
    @note: 腾讯有但数据不可修改，声网有且可修改
    Callback occuring once every 10 ms. Retrieves the audio playback frame.
    This callback returns after \ref agora::rtc::RtcEngineParameters::playEffect "playEffect" is successfully called.
    @param audioFrame Pointer to AudioFrame.
    @return
    - true: Valid buffer in AudioFrame, and the audio playback frame is sent out.
    - false: Invalid buffer in AudioFrame, and the audio playback frame is discarded.
    */
    virtual bool onPlaybackAudioFrame(rtc::ExternalAudioFrame& audioFrame)override {
        (void)audioFrame;

        //if (true) {
        //    char szFile[256];
        //    sprintf(szFile, "D:\\onPlaybackAudioFrame-%d-%d.pcm", audioFrame.bytesPerSample, audioFrame.samplesPerSec);

        //    FILE *pfPcm = fopen(szFile, "ab");
        //    if (pfPcm)
        //        fwrite(audioFrame.buffer, audioFrame.bytesPerSample, audioFrame.channels * audioFrame.samples, pfPcm);
        //    if (pfPcm)
        //        fclose(pfPcm);

        //}

        if (!mMixAudio)
            return true;

        QString mAudioFileName = QCoreApplication::applicationDirPath() + "/test.pcm";
        if (mpPCMFile == NULL)
        {
            QFile bfilePath(mAudioFileName);
            if (!bfilePath.exists())
                return true;

            mpPCMFile = fopen(mAudioFileName.toLocal8Bit().toStdString().c_str(), "rb");
            fseek(mpPCMFile, 0, SEEK_END);
            mfilesize = ftell(mpPCMFile);
        }

        if (!mpPCMFile) {
            return true;
        }

        if (mpPCMFile && feof(mpPCMFile))
        {
            qDebug() << "timerPlayeAudio eof";
            fseek(mpPCMFile, 0, SEEK_SET);
        }
        int len = 480 * 2 * 2;
        if (mpPCMFile)
        {
            fseek(mpPCMFile, moffset, SEEK_SET);
            int bufLen = fread(mPCMBuf, 1, len, mpPCMFile);
            memcpy(audioFrame.buffer, mPCMBuf, bufLen);
            moffset += bufLen;
            qDebug() << "timerPlayeAudio sucess";
        }

        if ((moffset >= mfilesize) && (mfilesize > 0))
        {
            printf(" read the pcm file is end============\n");
            moffset = 0;
            mfilesize = 0;
            fclose(mpPCMFile);
            mpPCMFile = nullptr;
        }
        return true;
    }

    /**@brief 各路音频数据 以及 本机麦克采集的 混合后的音频数据
    @note: 腾讯无，声网有且数据可修改
    Callback occuring every 10 ms. Retrieves the mixed recorded and playback audio frame.
    @note This method only returns the single-channel data.
    @param audioFrame Pointer to AudioFrame.
    @return
    - true: Valid buffer in AudioFrame and the mixed recorded and playback audio frame is sent out.
    - false: Invalid buffer in AudioFrame and the mixed recorded and playback audio frame is discarded.
    */
    virtual bool onMixedAudioFrame(rtc::ExternalAudioFrame& audioFrame)override {
        (void)audioFrame;
        return true;
    }

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
        rtc::ExternalAudioFrame& audioFrame)override {

        (void)uid;
        (void)audioFrame;
        return true;
    }

    virtual bool onPlaybackAudioFrameBeforeMixingEx(const char *channelId,
        unsigned int uid, rtc::ExternalAudioFrame& audioFrame) override {
        (void)channelId;
        (void)uid;
        (void)audioFrame;

        //TODO TEST
        if (channelId == nullptr) return false;
        static FILE *pcmFile = fopen("D:/test_engine.pcm", "wb");
        Stru_RtcBaseInfo &info = Context::instance()->getBaseInfo();
        if (info.myUserInfo.userId == uid) {
            if (pcmFile) {
                fwrite(audioFrame.buffer, audioFrame.bytesPerSample, audioFrame.samples * audioFrame.channels, pcmFile);
            }
        }
        return false;
    }

public:
    void startRecordLocalAudio() { recordLocalAudio_ = true; }
    void SetMixAudio(bool bMix) { mMixAudio = bMix; }

private:
    bool recordLocalAudio_ = false; // 录制音频片段
    int mfilesize = 0;
    char mPCMBuf[480 * 2 * 2];
    int moffset = 0;
    FILE *mpPCMFile = NULL;
    bool mMixAudio = false;
};



class RtcEventHandler : public QObject, public rtc::IRTCEngineEventHandler
{
    Q_OBJECT

signals:
    void userJoined(unsigned int, const QString &);
    void userLeave(unsigned int, const QString &);

    void leaveRoom(const QString &room);
    void seiTimestampArrived(const QString &room, rtc::UserId uid, uint64_t seiTime);
    //void streamMessageArrived(rtc::UserId uid, int streamId, const QString &message);
    //void streamMessageError(rtc::UserId uid, int streamId, int code, int missed, int cached);
    void streamPublished(const QString& url, int error);
    void streamUnpublished(const QString &url);
    void clientRoleChanged(const QString &room, RtcRoleType oldRole, RtcRoleType newType);
    void rtmpUplinesStateChanged(const QString& streamId, int state, int errCode, const rtc::Stru_RtmpUplinesInfo &info);
    void engineChangeNotify();
    void rtcStats(const rtc::RTCStats& stats);
    void audioDeviceChanged();
    void videoDeviceChanged();
    void  keepaliveTimeout();
    void RemoteStreamSubscribeAdvice(QString str);
    void onAudioDeviceVolumeChanged(uint8_t deviceType, int volume, bool muted);
    void onAudioPublishStateChange(const char *channelId, rtc::STREAM_PUBLISH_STATE oldState, rtc::STREAM_PUBLISH_STATE newState, int elapseSinceLastState);
    void onAudioSubscribeStateChange(const char *channelId, rtc::UserId uid, rtc::STREAM_SUBSCRIBE_STATE oldState, rtc::STREAM_SUBSCRIBE_STATE newState, int elapseSinceLastState);
    void onChannelMediaRelayEvent(rtc::CHANNEL_MEDIA_RELAY_EVENT code);
    void onHttpMetadataStateStateChanged(const std::string &streamId, int category, int state, int errorCode, const std::string &errMsg);
    void onChannelMediaRelayStateChanged(rtc::CHANNEL_MEDIA_RELAY_STATE state, rtc::CHANNEL_MEDIA_RELAY_ERROR code);
    void onLocalAudioStats(const rtc::LocalAudioStats stats);
    void onLocalVideoStateChanged(rtc::RTC_LOCAL_VIDEO_STREAM_STATE localVideoState, rtc::RTC_LOCAL_VIDEO_STREAM_ERROR error);
    void onLocalVideoStats(const rtc::LocalVideoStats stats);
    void onRemoteAudioStateChanged(rtc::UserId uid, rtc::REMOTE_AUDIO_STATE state, rtc::REMOTE_AUDIO_STATE_REASON reason, int elapsed);
    void onRemoteVideoStateChanged(rtc::UserId uid, rtc::RTC_REMOTE_VIDEO_STATE state, rtc::RTC_REMOTE_VIDEO_STATE_REASON reason, int elapsed);
    void onStartCloudRecordStateChanged(const std::string &auth, int recordId, int state, int errorCode, const std::string &sid);
    void onStopCloudRecordStateChanged(const std::string &auth, int recordId,
        const std::string &sid, int state, int errorCode);
    void onStreamMessage(rtc::UserId uid, const char* data, size_t length);
    void onVideoPublishStateChange(const char *channelId, rtc::STREAM_PUBLISH_STATE oldState, rtc::STREAM_PUBLISH_STATE newState, int elapseSinceLastState);
    void onVideoSubscribeStateChange(const char *channelId, rtc::UserId uid, rtc::STREAM_SUBSCRIBE_STATE oldState, rtc::STREAM_SUBSCRIBE_STATE newState, int elapseSinceLastState);
    void onAudioBufferingStateChanged(rtc::UserId uid, rtc::AUDIO_BUFFERING_STATE state, int64_t timestampInMs);
    void onRemoteVideoStats(const rtc::RemoteVideoStats stats);
public:
    RtcEventHandler(QObject *parent = nullptr)
        : QObject(parent) {

    }
    virtual ~RtcEventHandler() override {

    }

public:
    void onJoinedRoom(rtc::UserId myId, const char* room) override
    {
        qDebug() << Q_FUNC_INFO << "uid:" << myId << " room:" << room;

        /*if (mpMainWindow) {
            mpMainWindow->OnRemoteUserEnableVideo(0, true, QString(room));
        }*/
        emit userJoined(0, QString(room));
    }
    void onLeaveRoom() override {
        Stru_RtcBaseInfo &baseinfo = Context::instance()->getBaseInfo();
        qDebug() << Q_FUNC_INFO << baseinfo.myUserInfo.roomId;
        emit leaveRoom(baseinfo.myUserInfo.roomId);
    }

    virtual void onFirstRemoteVideoFrame(rtc::UserId userId, int width, int height) override {
        qDebug() << Q_FUNC_INFO << "Received the first video frame from user: " << userId
            << ", w:" << width << ",h:" << height;
    }
    virtual void onFirstRemoteAudioFrame(rtc::UserId userId) override {
        qDebug() << Q_FUNC_INFO << "Received the first audio frame from user: " << userId;
    }

    virtual void onUserJoined(const rtc::UserId userId) override
    {
        qDebug() << Q_FUNC_INFO << "user: " << userId << " join";
        Stru_RtcBaseInfo &info = Context::instance()->getBaseInfo();
        QString roomId = info.myUserInfo.roomId;
        emit userJoined(userId, roomId);
    }

    virtual void onUserOffline(const rtc::UserId userId) override
    {
        qDebug() << Q_FUNC_INFO << "user: " << userId << " offline";
        Stru_RtcBaseInfo &info = Context::instance()->getBaseInfo();
        QString roomId = info.myUserInfo.roomId;
        emit userLeave(userId, roomId);
    }

    virtual void onUserMuteVideo(rtc::UserId userId, bool mute) override
    {
        (void)userId;
        (void)mute;
        qDebug() << Q_FUNC_INFO << "user: " << userId << " mute video: " << mute;
        //((CrtcsdkDlg*)theApp.GetMainWnd())->onUserMuteVideo(userId, mute);
    }
    virtual void onUserMuteAudio(rtc::UserId userId, bool isMuting) override
    {
        (void)userId;
        (void)isMuting;
        //((CrtcsdkDlg*)theApp.GetMainWnd())->onUserMuteAudio(userId, isMuting);
        qDebug() << Q_FUNC_INFO << "user: " << userId << " mute audio: " << isMuting;
    }

    virtual void onUserEnableVideo(rtc::UserId userId, bool enable) override
    {
        (void)userId;
        (void)enable;
        qDebug() << Q_FUNC_INFO << "user: " << userId << " enable video: " << enable;
        /*if (mpMainWindow){
            mpMainWindow->OnRemoteUserEnableVideo(userId, enable);
        }	*/

        //TODO  liy 20201024
// 		if (mpMainWindow->mRtmpStarted && mpMainWindow->mTranscoding_ && enable) {
// 			mpMainWindow->addMixerVideo(userId);
// 		}
    }
    virtual void onUserEnableAudio(rtc::UserId userId, bool enable)
    {
        (void)userId;
        (void)enable;
        qDebug() << Q_FUNC_INFO << "user: " << userId << " enable audio:" << enable;

        //((CrtcsdkDlg*)theApp.GetMainWnd())->onUserEnableAudio(userId, enable);
    }

    virtual void onActiveSpeaker(rtc::UserId uid) override {
        (void)uid;
        qDebug() << Q_FUNC_INFO << "uid:" << uid;
    }

    virtual void onAudioVolume(rtc::UserId uid, unsigned volume) override {

        (void)uid;
        (void)volume;

        Stru_RtcBaseInfo &baseinfo = Context::instance()->getBaseInfo();
        QString str = baseinfo.myUserInfo.userId.toUInt() == uid ? "(local)" : "(remote)";
        qDebug() << Q_FUNC_INFO << "uid:" << uid << str << ", volume=" << volume;

        // 		if (mpMainWindow) {
        // 			mpMainWindow->onAudioVolume(uid, volume);
        // 		}
    }

    virtual void onConnectionLost() {
        //theApp.setState(STATE_DISCONNECTED);
        qDebug() << Q_FUNC_INFO;
        // 		if (mpMainWindow) {
        // 			mpMainWindow->connectionLost();
        // 		}
    }
    virtual void onConnectionFail() {
        //theApp.setState(STATE_CONNECT_FAILED);
        qDebug() << Q_FUNC_INFO;
        // 		if (mpMainWindow) {
        // 			mpMainWindow->connectionFail();
        // 		}
    }

    /**@desc 通话前网络上下行 last mile 质量报告回调。
        *@detail	该回调描述本地用户在加入频道前的 last mile 网络探测的结果，其中 last mile 是指设备到 媒体服务器的网络状态。
                    在调用 enableLastmileTest 之后，该回调函数每 2 秒触发一次。
        *@param quality  网络上下行 last mile 质量
        */
    virtual void onLastMileQuality(rtc::NETWORK_QUALITY_TYPE quality) override {
        (void)quality;
        qDebug() << Q_FUNC_INFO << "onLastMileQuality :" << quality;
        // 		if (mpMainWindow) {
        // 			mpMainWindow->lastMileQuality(quality);
        // 		}
    }

    virtual void onError(rtc::RTC_RET_CODE code, const char* msg) override
    {
        (void)code;
        (void)msg;
        qDebug() << Q_FUNC_INFO << "error : " << code << msg;

        string prompt;
        switch (code) {
        case rtc::RTC_ERR_INVALID_TOKEN:
            break;
        case rtc::RTC_ERR_ROOM_KICKED:
            break;
        default:
            break;
        }

        // 		if (mpMainWindow) {
        // 			mpMainWindow->engineError(code, QString(msg));
        // 		}
    }

    virtual void onRTCStats(const rtc::RTCStats& stats) override {
        (void)stats;
        qDebug() << "cpuAppUsage: " << stats.cpuAppUsage << "% cpuTotalUsage: " << stats.cpuTotalUsage << "%";
        /*
        if (mpMainWindow) {
            mpMainWindow->rtcStats(stats);
        }
        */
        emit rtcStats(stats);
    }

    //virtual void onStreamMessage(rtc::UserId uid, int streamId, const char* data, size_t length) override{
    //    (void)uid;
    //    (void)streamId;
    //    (void)data;
    //    (void)length;

    //    qDebug() << Q_FUNC_INFO << "uid:" <<uid <<
    //                ",stream:" << streamId
    //             << "message:" << QString::fromLocal8Bit(data);
    //    emit streamMessageArrived(uid, streamId, QString::fromLocal8Bit(data));

    //}

    virtual void onSeiTimestampArrived(rtc::UserId uid, uint64_t seiTime) override {
        (void)uid;
        (void)seiTime;

        qDebug() << Q_FUNC_INFO << "uid:" << uid << ", time:" << seiTime;
        emit seiTimestampArrived("", uid, seiTime);
    }

    //virtual void onStreamMessageError(rtc::UserId uid, int streamId, int code, int missed, int cached) override{
    //    (void)uid;
    //    (void)streamId;
    //    (void)code;
    //    (void)missed;
    //    (void)cached;

    //    qDebug() << Q_FUNC_INFO << "uid:" <<uid <<
    //                ",stream:" << streamId
    //             << ", missed:" << missed
    //             << ",cached:" << cached;

    //    emit streamMessageError(uid, streamId, code, missed, cached);
    //}


    virtual void onRtmpStreamingStateChanged(const char *url, rtc::RTMP_STREAM_PUBLISH_STATE state,
        rtc::RTMP_STREAM_PUBLISH_ERROR errCode) override {
        (void)url;
        (rtc::RTMP_STREAM_PUBLISH_STATE)state;
        (rtc::RTMP_STREAM_PUBLISH_ERROR)errCode;
        //TODO
        qDebug() << Q_FUNC_INFO << "url:" << url << ", state:" << state << ", errCode:" << errCode;
        if (state == rtc::RTMP_STREAM_PUBLISH_STATE_IDLE) {
            emit streamUnpublished(QString(url));
        }
        else if (state == rtc::RTMP_STREAM_PUBLISH_STATE_RUNNING) {
            emit streamPublished(QString(url), errCode);
        }
    }

    /*virtual void onStreamPublished(const char *url, int error) override{
        (void)url;
        (void)error;

        emit streamPublished(QString(url), error);
    }

    virtual void onStreamUnpublished(const char *url) override {
        (void)url;

        emit streamUnpublished(QString(url));
    }*/

    virtual void onClientRoleChanged(rtc::CLIENT_ROLE_TYPE oldRole, rtc::CLIENT_ROLE_TYPE newRole) override {
        (void)oldRole;
        (void)newRole;

        qDebug() << Q_FUNC_INFO << "new role:" << newRole;
        RtcRoleType rtcOld = oldRole == rtc::CLIENT_ROLE_BROADCASTER ? RtcRole_Broadcaster : RtcRole_Audice;
        RtcRoleType rtcNew = newRole == rtc::CLIENT_ROLE_BROADCASTER ? RtcRole_Broadcaster : RtcRole_Audice;
        emit clientRoleChanged("", rtcOld, rtcNew);
    }

    /* rtmp 旁路转推时获取调度的回调，只有state 和 errCode都=0时才是正确回调，否则属于异常回调**/
    virtual void onRtmpUplinesStateChanged(const char* streamId, int state, int errCode, const rtc::Stru_RtmpUplinesInfo &info) override {
        (void)streamId;
        (void)state;
        (void)errCode;
        (void)info;

        emit rtmpUplinesStateChanged(QString(streamId), state, errCode, info);
    }

    virtual void onConnectionStateChanged(rtc::RTC_CONNECTION_STATE_TYPE state, const char* reason) override {
        if (state == rtc::CONNECTION_STATE_FAILED && strcmp(reason, "keepalive timeout") == 0)
            emit  keepaliveTimeout();

    }

    virtual void onEngineChangeNotify() override {
        qDebug() << Q_FUNC_INFO << "KICK";
        emit engineChangeNotify();
    }

    virtual void onNetworkQuality(rtc::UserId uid, int txQuality, int rxQuality) override {
        qDebug() << Q_FUNC_INFO << "onNetworkQuality: uid=" << uid << ", txQuality=" << txQuality << ", rxQuality=" << rxQuality;
        //         if (mpMainWindow)
        //         {
        //             mpMainWindow->onNetworkQuality(uid, txQuality, rxQuality);
        //         }
    }

    virtual void onAudioDeviceStateChanged(const char* deviceId,
        rtc::RTC_MEDIA_DEVICE_TYPE deviceType,
        rtc::RTC_MEDIA_DEVICE_STATE_TYPE deviceState) override
    {
        (void)deviceId;
        if ((deviceType == rtc::RTC_AUDIO_PLAYOUT_DEVICE ||
            deviceType == rtc::RTC_AUDIO_RECORDING_DEVICE) &&
            (deviceState == rtc::RTC_MEDIA_DEVICE_STATE_ACTIVE ||
                deviceState == rtc::RTC_MEDIA_DEVICE_STATE_NOT_PRESENT ||
                deviceState == rtc::RTC_MEDIA_DEVICE_STATE_UNPLUGGED))
        {
            emit audioDeviceChanged();
        }
    }

    virtual void onVideoDeviceStateChanged(const char* deviceId,
        rtc::RTC_MEDIA_DEVICE_TYPE deviceType,
        rtc::RTC_MEDIA_DEVICE_STATE_TYPE deviceState) override
    {
        (void)deviceId;
        if ((deviceType == rtc::RTC_VIDEO_CAPTURE_DEVICE) &&
            (deviceState == rtc::RTC_MEDIA_DEVICE_STATE_ACTIVE ||
                deviceState == rtc::RTC_MEDIA_DEVICE_STATE_NOT_PRESENT ||
                deviceState == rtc::RTC_MEDIA_DEVICE_STATE_UNPLUGGED))
        {
            emit videoDeviceChanged();
        }
    }

    virtual void onVideoBufferingStateChanged(rtc::UserId uid,
        rtc::VIDEO_BUFFERING_STATE state,
        int64_t timestampInMs) override
    {
        (void)uid;
        (void)state;
        (void)timestampInMs;
        qDebug() << Q_FUNC_INFO << "onVideoBufferingStateChanged";
    }


    virtual void onRemoteStreamSubscribeAdvice(rtc::UserId uid, rtc::REMOTE_VIDEO_STREAM_TYPE currentStreamType, rtc::REMOTE_VIDEO_STREAM_TYPE suitableStreamType) override
    {
        (void)uid;
        (void)currentStreamType;
        (void)suitableStreamType;
        QString str;
        str.sprintf("userid:%d,currentStreamType:%d, suitableStreamType:%d", (int)uid, (int)currentStreamType, (int)suitableStreamType);
        //QMessageBox::information(nullptr, " ", str, QMessageBox::Yes, QMessageBox::Yes);
        emit RemoteStreamSubscribeAdvice(str);

    }

};


class RtcChannelEventHandler : public QObject, public rtc::IRTCChannelEventHandler
{
    Q_OBJECT
signals:
    void seiTimestampArrived(const QString &room, rtc::UserId uid, uint64_t seiTime);
    void userJoined(unsigned int, const QString &);
    void userLeave(unsigned int, const QString &);
    void clientRoleChanged(const QString &channelId, RtcRoleType oldRole, RtcRoleType newRole);
    void RemoteStreamSubscribeAdvice(QString str);

private:
    MainWindow * mpMainWindow = nullptr;

public:
    RtcChannelEventHandler(MainWindow * mainWin) {
        mpMainWindow = mainWin;
    }

    virtual ~RtcChannelEventHandler() {}

    virtual void onJoinChannelSuccess(const char *channelId, uint32_t uid) {

        if (channelId == nullptr) return;
        qDebug() << Q_FUNC_INFO << "channelId:" << channelId << ", uid:" << uid;
        // 		if (mpMainWindow) {
        // 			mpMainWindow->joinChannelSuccess(QString(channelId));
        // 		}
    }
    virtual void onLeaveChannel(const char *channelId) {
        qDebug() << Q_FUNC_INFO << "channelId=" << channelId;

        // 		if (mpMainWindow) {
        // 			mpMainWindow->leaveChannelSuccess(QString(channelId));
        // 		}

    }
    virtual void onUserJoined(const char *channelId, uint32_t uid) {

        qDebug() << Q_FUNC_INFO << "channelId:" << channelId << ", uid:" << uid;
        emit userJoined(uid, QString(channelId));

    }
    virtual void onUserOffline(const char *channelId, uint32_t uid) {
        qDebug() << Q_FUNC_INFO << "channelId:" << channelId << ", user: " << uid << " offline";
        emit userLeave(uid, QString(channelId));
    }
    virtual void onChannelWarning(const char *channelId, int warn, const char* msg) {


        qDebug() << Q_FUNC_INFO << "channelId=" << channelId
            << ", warn=" << warn << ",msg=" << msg;;

        // 		if (mpMainWindow) {
        // 			mpMainWindow->channelWarning(QString(channelId), warn, QString(msg));
        // 		}
    }
    virtual void onChannelError(const char *channelId, int warn, const char* msg) {

        qDebug() << Q_FUNC_INFO << "channelId=" << channelId
            << ", warn=" << warn << ", msg=" << msg;

        // 		if (mpMainWindow) {
        // 			mpMainWindow->channelError(QString(channelId), warn, QString(msg));
        // 		}
    }

    virtual void onFirstRemoteVideoFrame(const char *channelId, uint32_t uid, int width, int height, int elapsed) {
        (void)channelId;
        (void)uid;
        (void)width;
        (void)height;
        (void)elapsed;

        qDebug() << Q_FUNC_INFO << "channelId=" << channelId << ", uid=" << uid << ", w=" << width << ", h=" << height;
    }

    virtual void onRemoteVideoStateChanged(const char *rtcChannel, uint32_t uid,
        rtc::RTC_REMOTE_VIDEO_STATE state, rtc::RTC_REMOTE_VIDEO_STATE_REASON reason, int elapsed) {
        qDebug() << Q_FUNC_INFO << "linatest, state, channelId=" << rtcChannel << ", uid=" << uid << ",state=" << state << ",reason="
            << reason << ", elapsed=" << elapsed;
    }

    virtual void onRtcStats(const char* channelId, const rtc::RTCStats& stats) {

        qDebug() << Q_FUNC_INFO << "channelId=" << channelId << ", cpuAppUse=" << stats.cpuAppUsage
            << ", cpuTotalUse=" << stats.cpuTotalUsage << ", txPacketLossRate=" << stats.txPacketLossRate
            << ", rxPacketLossRate=" << stats.rxPacketLossRate;
    }

    virtual void onAudioVolume(const char* channelId, rtc::UserId uid, unsigned volume) {

        // 		if (mpMainWindow) {
        // 			mpMainWindow->onAudioVolume(uid, volume);
        // 		}

        qDebug() << Q_FUNC_INFO << "channelId:" << channelId << ", uid:" << uid << ", volume=" << volume;
    }


    virtual void onSeiTimestampArrived(const char* channelId, rtc::UserId uid, uint64_t seiTime) {
        (void)channelId;
        (void)uid;
        (void)seiTime;

        emit seiTimestampArrived(QString(channelId), uid, seiTime);
    }
    virtual void onConnectionStateChanged(const char* channelId,
        rtc::RTC_CONNECTION_STATE_TYPE state,
        rtc::CONNECTION_CHANGED_REASON_TYPE reason) {
        (void)channelId;
        (void)state;
        (void)reason;

    }

    virtual void onVideoBufferingStateChanged(rtc::UserId uid,
        rtc::VIDEO_BUFFERING_STATE state,
        int64_t timestampInMs)
    {
        (void)uid;
        (void)state;
        (void)timestampInMs;
        qDebug() << Q_FUNC_INFO << "onVideoBufferingStateChanged";
    }

    virtual void onClientRoleChanged(const char* channelId, rtc::CLIENT_ROLE_TYPE oldRole, rtc::CLIENT_ROLE_TYPE newRole) {
        (void)oldRole;
        (void)newRole;

        qDebug() << Q_FUNC_INFO << "new role:" << newRole;
        RtcRoleType rtcOld = oldRole == rtc::CLIENT_ROLE_BROADCASTER ? RtcRole_Broadcaster : RtcRole_Audice;
        RtcRoleType rtcNew = newRole == rtc::CLIENT_ROLE_BROADCASTER ? RtcRole_Broadcaster : RtcRole_Audice;
        emit clientRoleChanged(channelId, rtcOld, rtcNew);
    }

    virtual void onRemoteStreamSubscribeAdvice(const char* channelId, rtc::UserId uid, rtc::REMOTE_VIDEO_STREAM_TYPE currentStreamType, rtc::REMOTE_VIDEO_STREAM_TYPE suitableStreamType) override
    {
        (void)uid;
        (void)currentStreamType;
        (void)suitableStreamType;
        QString str;
        str.sprintf("userid:%d,currentStreamType:%d, suitableStreamType:%d", (int)uid, (int)currentStreamType, (int)suitableStreamType);
        emit RemoteStreamSubscribeAdvice(str);
    }
};

#endif //XESRTCENGINE_H
