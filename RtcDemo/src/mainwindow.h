#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <mutex>

#include <QMainWindow>
#include <QList>
#include <QMessageBox>
#include <map>

#include "IRTCEngine.h"
#include "Context.h"

namespace Ui {
    class MainWindow;
}


class OneVideo;
class QPushButton;
class RtcEventHandler;
class VideoRenderImpl;
class IRTCChannel;
class RtcChannelEventHandler;
class audioFrameObserver;
class VideoFrameObserver;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow() override;

 
    
    void layoutChild();
    void rtcJoinRoom();
    static void log_func(const char* msg);
    OneVideo* findVideoViewer(unsigned uid);
    std::mutex& getShowListMutex() { return showListMutex_; }

    void SendYuvData(unsigned int uid, uchar *y, uchar *u, uchar *v, int yStride, int uStride, int vStride, uint width, uint height);
    virtual void closeEvent(QCloseEvent *event) override;
    void leaveChannelSuccess(const QString &channelId);
    void joinChannelSuccess(const QString &channelId);
    void init();
    void release();
    void channelWarning(const QString &channelId, int warn, const QString &msg);
    void channelError(const QString &channelId, int warn, const QString &msg);
    void engineError(int code, const QString &msg);
    void rtcStats(const rtc::RTCStats& stats);
    void connectionLost();
    void connectionFail();
    void lastMileQuality(int quality);
     
signals:
    void remoteUserOfflineSignal(unsigned int userId, const QString &);
    void userAudioVolume(unsigned int uid, unsigned int volume);
    void yuvData(unsigned int uid, uchar *y, uchar *u, uchar *v, int yStride, int uStride, int vStride, uint width, uint height);
    void muteRemoteAudio(unsigned int uid);
    void leaveChannelSuccessSignal(const QString channelId);
    void joinChannelSuccessSignal(const QString channelId);
    void channelErrorSignal(const QString &channelId, int warn, const QString &msg);
    void channelWarningSignal(const QString &channelId, int warn, const QString &msg);
    void engineErrorSignal(int code, const QString &msg);
    void rtcStatsSignal(const rtc::RTCStats& stats);
    void connectionLostSignal();
    void connectionFailSignal();
    void lastMileQualitySignal(int quality);
     
public slots:
    void onUserJoined(unsigned int userId, const QString &channelId);
    void onUserLeave(unsigned int uId, const QString &channelId);

    rtc::WindowIdType localaddWindowSlot(unsigned userId, const QString &roomId);
    void muteReAudio(unsigned int uid);
    void onSetRemoteVolume(unsigned uid, int volume, const QString &channelId);
    void onMuteRemoteVideo(unsigned uid, const QString &room, bool mute);
    void onMuteRemoteAudio(unsigned uid, const QString &room, bool mute);
    void onLeaveRoom(const QString &roomId);
    void onMirrorRemoteVideo(unsigned int uid, const QString& room, bool mirror);
    void leaveRoom();
    void onRecordLocalAudio();
    void removeWindowSlot();

    //关于
    void aboutSlot();

    //定时器
     
    void childClosed(OneVideo *who);
    void OnAcceptButtonclick();
    void OnUserQuality(unsigned int uid, int txQuality, int rxQuality);
    void onYuvData(unsigned int uid, uchar *y, uchar *u, uchar *v, int yStride, int uStride, int vStride, uint width, uint height);
    void onSeiTimestampArrived(const QString &room, rtc::UserId uid, uint64_t seiTime);
    void onStreamPublished(const QString& url, int error);
    void onStreamUnPublished(const QString& url);
    void onAddRoomBtn();
    void onLeaveRoomBtn();
    void onInitBtn();
    void onClientRoleChanged(const QString & room, RtcRoleType oldType, RtcRoleType newType);
    void onEngineChangeNotify();
    void onJoinChannelSuccess(const QString channelId);
    void onLeaveChannelSuccess(const QString channelId);
    void onChannelWarning(const QString &channelId, int warn, const QString &msg);
    void onChannelError(const QString &channelId, int warn, const QString &msg);
    void onEngineError(int code, const QString &msg);
    void onRtcStats(const rtc::RTCStats& stats);
    void onConnectionLost();
    void onConnectionFail();
    void onLastMileQuality(int quality);
    void onAudioDeviceChanged();
    void onVideoDeviceChanged();
    void onKeepaliveTimeout();
    void onDestoryButton();

protected:
    void resizeEvent(QResizeEvent *event) override;
private:
     
    QList<OneVideo*>	showList;
    std::mutex			showListMutex_;
    engine baseinfo;
    QString  realtoken_;

    int mTranscodingConfigUserCount = 0;
    rtc::Stru_RtmpLiveTranscoding mTranscodingConfig;
    rtc::Stru_RtmpRtcImage mBackgroudImage;
    rtc::Stru_RtmpTranscodingUser mTransUser[50];
    
public:
    //定时器
    QTimer *timer = nullptr;

    

    qint64 g_startTime = 0;
    VideoRenderImpl*	viewRender = nullptr;
    QString				logContent_;
    rtc::IRTCChannel		*m_channel = nullptr;
    std::map<std::string, rtc::IRTCChannel*> m_channelMap;
    bool     hasInit_ = false;

    bool mTranscoding_ = false;
    bool mRtmpStarted = false;
    
private:
    Ui::MainWindow *ui;
    RtcEventHandler* mprtcHandler = nullptr;
    audioFrameObserver *m_audioObserver = nullptr;
    VideoFrameObserver *m_videoFrameObserver = nullptr;

    QString m_appID;

    bool m_bSdkRender = false;
    bool mIsInRoom = false;
    bool mEnableLastMileCheck = false;
    int mLastMileQuality = 0;

    bool channelVideoMuteStatus = true;
    bool channelAudioMuteStatus = true;
    bool channelBroadcasterStatus = false;
    int m_engineType = 7;

};

#endif // MAINWINDOW_H
