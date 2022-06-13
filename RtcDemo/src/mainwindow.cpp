#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QStatusBar>
#include <QScrollArea>
#include <QPushButton>
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>


#include <QCoreApplication>
#include <QIODevice>
#include <QMessageBox>
#include <QThread>
#include "ui_mainwindow.h"

#include <QResizeEvent>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonObject>
#include "IRTCEngineEventHandler.h"

#include <mutex>
#include <assert.h>

#include "mainwindow.h"
#include "onevideo.h"
#include "IRTCChannel.h"
#include "xes_rtc_engine.h"
 

rtc::IRTCEngine* rtcEngineInstance = nullptr;
MainWindow*  mpMainWindow = nullptr;

static int getRandom32() {
    long long currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    srand((unsigned)currentTime);
    int val = rand() << 17 | rand() << 3 | rand();
    return val < 0 ? -val : val;

}

void MainWindow::onUserLeave(unsigned int uId, const QString &channelId)
{
    (void)uId;
    (void)channelId;
    {
        std::lock_guard<std::mutex> lock(showListMutex_);
        auto oitr = showList.begin();
        while (showList.end() != oitr) {
            OneVideo* one = *oitr;
            if (one && one->userId() == uId) {
                showList.erase(oitr);
                ui->gridLayout->removeWidget(one);
                one->deleteLater();
                one = nullptr;
                break;
            }
            oitr++;
        }
    }

    layoutChild();
}


void MainWindow::onUserJoined(unsigned int uId, const QString &channelId)
{
    (void)uId;
    (void)channelId;
    qDebug() << "begin begin really setupRemoteVideo : " << uId;


    rtc::WindowIdType remote = localaddWindowSlot(uId, channelId);
 
    if (remote != nullptr)
    {  
        if (uId == 0) { 
            //rtcEngineInstance->setLocalRenderMode(deviceConfig_.isRenderHidden ? rtc::RTC_VIDEO_RENDER_MODE_HIDDEN : rtc::RTC_VIDEO_RENDER_MODE_FIT);
            rtcEngineInstance->setupLocalVideo(remote);
            rtcEngineInstance->startPreview();
        }
        else { // render remote video
            //rtcEngineInstance->setRemoteRenderMode(uId, deviceConfig_.isRemoteRenderHidden ? rtc::RTC_VIDEO_RENDER_MODE_HIDDEN : rtc::RTC_VIDEO_RENDER_MODE_FIT);
            rtcEngineInstance->setupRemoteVideo(uId, remote, channelId.toStdString().c_str());
        }
    }
    else
    { 
        if (m_bSdkRender)
        {
            //rtcEngineInstance->setLocalRenderMode(rtc::RTC_VIDEO_RENDER_MODE_HIDDEN : rtc::RTC_VIDEO_RENDER_MODE_FIT);
            rtcEngineInstance->startPreview();
        }
        else
        {
            qDebug() << "window is null uid " << uId;
        }
    }
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_appID(""),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mpMainWindow = this;


    resize(950, 700);
    setMinimumSize(OneVideo::WIDTH + 20, OneVideo::HEIGHT + 50);


    setStatusBar(new QStatusBar(this));

    ui->appIDText->setText(m_appID);
    connect(ui->initBtn, SIGNAL(clicked()), this, SLOT(onInitBtn()));
    connect(ui->loginRoomBtn, SIGNAL(clicked()), this, SLOT(onAddRoomBtn()));
    connect(ui->leaveRoomBtn, SIGNAL(clicked()), this, SLOT(onLeaveRoomBtn()));

    // by jove
    connect(this, &MainWindow::joinChannelSuccessSignal, this, &MainWindow::onJoinChannelSuccess);
    connect(this, &MainWindow::leaveChannelSuccessSignal, this, &MainWindow::onLeaveChannelSuccess);
    connect(this, &MainWindow::channelWarningSignal, this, &MainWindow::onChannelWarning);
    connect(this, &MainWindow::channelErrorSignal, this, &MainWindow::onChannelError);
    connect(this, &MainWindow::engineErrorSignal, this, &MainWindow::onEngineError);
    connect(this, &MainWindow::rtcStatsSignal, this, &MainWindow::onRtcStats);
    connect(this, &MainWindow::connectionLostSignal, this, &MainWindow::onConnectionLost);
    connect(this, &MainWindow::connectionFailSignal, this, &MainWindow::onConnectionFail);
    connect(this, &MainWindow::lastMileQualitySignal, this, &MainWindow::onLastMileQuality);

    // by jove end
    mprtcHandler = new RtcEventHandler();
    connect(mprtcHandler, &RtcEventHandler::userJoined, this, &MainWindow::onUserJoined);
    connect(mprtcHandler, &RtcEventHandler::userLeave, this, &MainWindow::onUserLeave);
    connect(mprtcHandler, &RtcEventHandler::seiTimestampArrived, this, &MainWindow::onSeiTimestampArrived);
    connect(mprtcHandler, &RtcEventHandler::streamPublished, this, &MainWindow::onStreamPublished);
    connect(mprtcHandler, &RtcEventHandler::streamUnpublished, this, &MainWindow::onStreamUnPublished);
    connect(mprtcHandler, &RtcEventHandler::clientRoleChanged, this, &MainWindow::onClientRoleChanged);
    connect(mprtcHandler, &RtcEventHandler::leaveRoom, this, &MainWindow::onLeaveRoom, Qt::QueuedConnection);
    connect(mprtcHandler, &RtcEventHandler::engineChangeNotify, this, &MainWindow::onEngineChangeNotify);
    connect(mprtcHandler, &RtcEventHandler::audioDeviceChanged, this, &MainWindow::onAudioDeviceChanged);
    connect(mprtcHandler, &RtcEventHandler::videoDeviceChanged, this, &MainWindow::onVideoDeviceChanged);
    connect(mprtcHandler, &RtcEventHandler::rtcStats, this, &MainWindow::onRtcStats);
    connect(mprtcHandler, &RtcEventHandler::keepaliveTimeout, this, &MainWindow::onKeepaliveTimeout);


    m_audioObserver = new audioFrameObserver;
    m_videoFrameObserver = new VideoFrameObserver;
    m_videoFrameObserver->mpMainWindow = this;

    Stru_RtcBaseInfo &baseinfo = Context::instance()->getBaseInfo();
    baseinfo.loginInfo.engineType = XES_ENGIN;
    ui->TabWiget->setCurrentWidget(ui->login);

    int num = getRandom32() % 1000000;
    ui->user->setText(QString::number(num));
}

MainWindow::~MainWindow()
{
    mpMainWindow = nullptr;

    if (rtcEngineInstance) {
        releaseRtcEngineInstance();
    }
    rtcEngineInstance = nullptr;
    qDebug() << Q_FUNC_INFO << __LINE__;
    removeWindowSlot();
    qDebug() << Q_FUNC_INFO << __LINE__;
    release();
    qDebug() << Q_FUNC_INFO << __LINE__;
}



OneVideo* MainWindow::findVideoViewer(unsigned uid)
{
    for (int i = 0; i < showList.size(); ++i) {
        if (showList.at(i)->userId() == uid)
            return showList.at(i);
    }
    return nullptr;
}

void MainWindow::log_func(const char* msg)
{
    qDebug() << "dll msg=" << msg;
    return;
}


void MainWindow::rtcJoinRoom()
{
    if (m_bSdkRender)
    {
        rtc::WindowIdType localView = localaddWindowSlot(0, "");

        if (localView != nullptr)
        {//use sdk render
            rtcEngineInstance->setupLocalVideo(localView);
        }
    }

    if (!rtcEngineInstance) {
        ui->loginRoomBtn->setEnabled(true);
        return;
    }

    if (mIsInRoom)
    {
        ui->loginRoomBtn->setEnabled(true);
        return;
    }

    rtcEngineInstance->enableExternalAudioSource(true, 48000, 2);
    rtcEngineInstance->setConfig("{\"che.audio.enable.agc\":false}");
    rtcEngineInstance->setConfig("{\"che.audio.current.recording.boostMode\":-1}");
    rtcEngineInstance->setConfig("{\"che.audio.specify.codec\":\"NVWA\"}");

    Stru_RtcBaseInfo& baseinfo = Context::instance()->getBaseInfo();
    if (baseinfo.loginInfo.serverIp.length() > 0) {
        QString strConfig = "{\"ttt.serverIp\":\"";
        strConfig.append(baseinfo.loginInfo.serverIp).append("\"}");
        rtcEngineInstance->setConfig(strConfig.toStdString().c_str());
    }

    rtcEngineInstance->joinRoomWithRealToken("", ui->roomTx->toPlainText().toStdString().c_str(), ui->user->toPlainText().toLong());
}


void MainWindow::layoutChild()
{
    int cols = (width() - 20) / OneVideo::WIDTH;

    std::lock_guard<std::mutex> locker(showListMutex_);
    for (int i = 0; i < showList.size(); i++) {
        ui->gridLayout->addWidget(showList.at(i), i / cols, i%cols, 1, 1, Qt::AlignLeft);
        showList.at(i)->update();
    }

    ui->scrollAreaWidgetContents->resize(width() - 20, height() - 10);
    qDebug() << Q_FUNC_INFO << "w:" << ui->scrollAreaWidgetContents->width()
        << ", cols=" << cols;
}


rtc::WindowIdType MainWindow::localaddWindowSlot(unsigned userId, const QString &roomId)
{
    Stru_RtcBaseInfo &baseinfo = Context::instance()->getBaseInfo();
    QString dstRoom = roomId.isEmpty() ? baseinfo.myUserInfo.roomId : roomId;
    OneVideo *one = new OneVideo(this, userId, dstRoom, m_bSdkRender);
    connect(one, &OneVideo::closeSignal, this, &MainWindow::childClosed);
    connect(one, &OneVideo::setRemoteVolume, this, &MainWindow::onSetRemoteVolume);
    connect(one, &OneVideo::muteRemoteAudio, this, &MainWindow::onMuteRemoteAudio);
    connect(one, &OneVideo::muteRemoteVideo, this, &MainWindow::onMuteRemoteVideo);
    connect(one, &OneVideo::mirrorRemoteVideo, this, &MainWindow::onMirrorRemoteVideo);
    int nType = 0;
    if (baseinfo.loginInfo.engineType == AGORA_ENGIN)
        nType = 1;
    else if (baseinfo.loginInfo.engineType == XES_ENGIN)
        nType = 3;
    else if (baseinfo.loginInfo.engineType == TALMEDIA_ENGINE)
        nType = 4;
    else if (baseinfo.loginInfo.engineType == ZegoEngine)
        nType = 6;
    else if (baseinfo.loginInfo.engineType == TTTEngine)
        nType = 7;

    one->setType(nType);
    one->show();
    if (userId == 0)
    {
        one->switchVideoSource(false);
        one->switchAudioSource(false);
        one->setAudioSourceAEC(false);
    }
    {
        std::lock_guard<std::mutex> lock(showListMutex_);
        showList.append(one);
    }

    layoutChild();
    one->repaint();


    return m_bSdkRender ? (rtc::WindowIdType)(one->videoContent_->winId()) : nullptr;
}

void MainWindow::muteReAudio(unsigned int uid)
{
    (void)uid;
}

void MainWindow::onSetRemoteVolume(unsigned uid, int volume, const QString &channelId)
{
    qDebug() << Q_FUNC_INFO << "uid=" << uid << ", volume=" << volume;
    for (OneVideo *one : showList) {
        if (uid == one->userId()) {
            rtcEngineInstance->setRemoteVolume(uid, volume, channelId.toStdString().c_str());
        }
    }
}


void MainWindow::onMuteRemoteVideo(unsigned uid, const QString & room, bool mute)
{
    qDebug() << Q_FUNC_INFO << "uid=" << uid << ", room=" << room << ", mute=" << mute;
    Stru_RtcBaseInfo &baseinfo = Context::instance()->getBaseInfo();
    if (room == baseinfo.myUserInfo.roomId) {
        rtcEngineInstance->muteRemoteVideo(uid, mute);
    }
    else {
        for (auto &m : m_channelMap) {
            if (room.toStdString() == m.second->channelId()) {
                m.second->muteRemoteVideoStream(uid, mute);
            }
        }
    }
}

void MainWindow::onMirrorRemoteVideo(unsigned int uid, const QString& room, bool mirror) {
    (void)room;
    (void)mirror;
    rtcEngineInstance->setRemoteRenderMode(uid, rtc::RTC_VIDEO_RENDER_MODE_HIDDEN);
}

void MainWindow::onMuteRemoteAudio(unsigned uid, const QString & room, bool mute)
{
    qDebug() << Q_FUNC_INFO << "uid=" << uid << ", room=" << room << ", mute=" << mute;
    Stru_RtcBaseInfo &baseinfo = Context::instance()->getBaseInfo();
    if (room == baseinfo.myUserInfo.roomId) {
        rtcEngineInstance->muteRemoteAudio(uid, mute);
    }
    else {
        for (auto &m : m_channelMap) {
            if (room.toStdString() == m.second->channelId()) {
                m.second->muteRemoteAudioStream(uid, mute);
            }
        }
    }
}

void MainWindow::onLeaveRoom(const QString &roomId)
{
    
    {
        std::lock_guard<std::mutex> lock(showListMutex_);

        auto oitr = showList.begin();
        while (showList.end() != oitr) {
            auto one = *oitr;
            if (one && one->roomId() == roomId) {
                showList.erase(oitr);
                ui->gridLayout->removeWidget(one);
                one->deleteLater();
                one = nullptr;
                break;
            }
        }
    }

    layoutChild();
}



void MainWindow::onSeiTimestampArrived(const QString &room, rtc::UserId uid, uint64_t seiTime)
{
    qDebug() << Q_FUNC_INFO << "uid:" << uid << "sei:" << seiTime;

}

void MainWindow::onStreamPublished(const QString &url, int error)
{
    qDebug() << Q_FUNC_INFO;
}

void MainWindow::onStreamUnPublished(const QString &url)
{
    qDebug() << Q_FUNC_INFO;
}

void MainWindow::removeWindowSlot()
{
    {
        std::lock_guard<std::mutex> lock(showListMutex_);
        auto oitr = showList.begin();
        while (showList.end() != oitr) {
            auto one = *oitr;
            if (one) {
                oitr = showList.erase(oitr);
                ui->gridLayout->removeWidget(one);
                one->deleteLater();
                one = nullptr;
            }
            oitr = showList.begin();
        }
    }

    layoutChild();
}

void MainWindow::aboutSlot()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::childClosed(OneVideo* who)
{
    {
        std::lock_guard<std::mutex> lock(showListMutex_);
        auto oitr = showList.begin();
        while (showList.end() != oitr) {
            OneVideo* one = *oitr;
            if (one && one == who && one->userId() == who->userId()) {
                showList.erase(oitr);
                ui->gridLayout->removeWidget(one);
                one->deleteLater();
                one = nullptr;
                break;
            }
            oitr++;
        }
    }

    layoutChild();
}

void MainWindow::OnAcceptButtonclick()
{
    qDebug() << Q_FUNC_INFO;
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    this->resize(event->size());
    qDebug() << Q_FUNC_INFO << event->size();
    layoutChild();
}


void MainWindow::leaveRoom()
{
    qDebug() << Q_FUNC_INFO;

    if (mEnableLastMileCheck) {
        rtcEngineInstance->disableLastmileTest();
    }
    for (auto &m : m_channelMap) {
        m.second->leaveChannel();
        m.second->release();
    }

    m_channelMap.clear();

    mIsInRoom = false;
    if (rtcEngineInstance)
        rtcEngineInstance->leaveRoom();
}


void MainWindow::onRecordLocalAudio()
{
    m_audioObserver->startRecordLocalAudio();
}


void MainWindow::SendYuvData(unsigned int uid, uchar *y, uchar *u, uchar *v, int yStride, int uStride, int vStride, uint width, uint height)
{

    onYuvData(uid, y, u, v, yStride, uStride, vStride, width, height);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    (void)event;

    if (rtcEngineInstance) {
        onLeaveRoomBtn();
    }

}

void MainWindow::channelWarning(const QString &channelId, int warn, const QString &msg)
{
    emit channelWarningSignal(channelId, warn, msg);
}

void MainWindow::channelError(const QString &channelId, int warn, const QString &msg)
{
    emit channelErrorSignal(channelId, warn, msg);
}

void MainWindow::engineError(int code, const QString &msg)
{
    emit engineErrorSignal(code, msg);
}

void MainWindow::rtcStats(const rtc::RTCStats& stats)
{
    emit rtcStatsSignal(stats);
}

void MainWindow::connectionLost()
{
    emit connectionLostSignal();
}

void MainWindow::connectionFail()
{
    emit connectionFailSignal();
}

void MainWindow::lastMileQuality(int quality) {
    emit lastMileQualitySignal(quality);
}

void MainWindow::joinChannelSuccess(const QString& channelId)
{
    emit joinChannelSuccessSignal(channelId);
}

void MainWindow::leaveChannelSuccess(const QString &channelId)
{
    emit leaveChannelSuccessSignal(channelId);
}

void MainWindow::onChannelWarning(const QString &channelId, int warn, const QString &msg)
{
    qDebug() << Q_FUNC_INFO << "<onChannelWarning> channelId: " << channelId << " warn: " << warn << " msg: " << msg;
}

void MainWindow::onChannelError(const QString &channelId, int warn, const QString &msg)
{
    qDebug() << Q_FUNC_INFO << "<onChannelError> channelId: " << channelId << " warn: " << warn << " msg: " << msg;
}

void MainWindow::onEngineError(int code, const QString &msg)
{
    qDebug() << Q_FUNC_INFO << "code: " << code << " msg: " << msg;

    // 
    if (code == rtc::RTC_ERR_ROOM_KICKED)
    {
         
        close();
    }
}

void MainWindow::onRtcStats(const rtc::RTCStats& stats)
{
    qDebug() << "<onRtcStats> rxBytes: " << stats.rxBytes << " txBytes: " << stats.txBytes
        << " rxAudioKBitRate: " << stats.rxAudioKBitRate << " txAudioKBitRate: " << stats.txAudioKBitRate;
    return;
}

void  MainWindow::onKeepaliveTimeout()
{
    qDebug() << Q_FUNC_INFO << "";
    close();
}

void MainWindow::onConnectionFail()
{
    qDebug() << Q_FUNC_INFO << "";
}

void MainWindow::onConnectionLost()
{
    qDebug() << Q_FUNC_INFO << "";
}

void MainWindow::onLastMileQuality(int quality)
{
    qDebug() << Q_FUNC_INFO << "quality=" << quality;
    mLastMileQuality = quality;
}
 
void MainWindow::onJoinChannelSuccess(const QString channelId)
{
    qDebug() << Q_FUNC_INFO << "channelId: " << channelId;

    Stru_RtcBaseInfo &baseinfo = Context::instance()->getBaseInfo();
    if (baseinfo.myUserInfo.role == RtcRole_Audice)
        return;
    rtc::WindowIdType remote = localaddWindowSlot(0, channelId);

}

void MainWindow::onLeaveChannelSuccess(const QString channelId) {
    qDebug() << Q_FUNC_INFO << "channelid=" << channelId;
    for (auto &m : showList) {
        qDebug() << Q_FUNC_INFO << "roomid:" << m->roomId();
        if (m->roomId() == channelId) {
            onUserLeave(m->userId(), channelId);
        }
    }

    auto prtChannel = m_channelMap.find(channelId.toStdString());
    if (prtChannel != m_channelMap.end()) {
        prtChannel->second->release();
        delete prtChannel->second;
        prtChannel->second = nullptr;
        m_channelMap.erase(channelId.toStdString());

    }
}


void MainWindow::OnUserQuality(unsigned int uid, int txQuality, int rxQuality)
{
    std::lock_guard<std::mutex> lock(showListMutex_);
    OneVideo* viewer = findVideoViewer(uid);
    if (viewer)
    {
        viewer->setTxQuality(txQuality);
        viewer->setRxQuality(rxQuality);
    }
}

void MainWindow::onYuvData(unsigned int uid, uchar *y, uchar *u, uchar *v, int yStride, int uStride, int vStride, uint width, uint height)
{
    std::lock_guard<std::mutex> lock(showListMutex_);
    OneVideo* viewer = findVideoViewer(uid);
    if (viewer) {

        viewer->onYuvData((unsigned char*)(y), (unsigned char*)(u), (unsigned char*)(v),
            yStride, uStride, vStride,
            width, height);
    }
}

void MainWindow::onAddRoomBtn()
{
    rtcJoinRoom();
    ui->loginRoomBtn->setDisabled(true);
    ui->leaveRoomBtn->setDisabled(false);
}
 
void MainWindow::onLeaveRoomBtn()
{
    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }
    g_startTime = 0;
    statusBar()->showMessage("");

    leaveRoom();
    removeWindowSlot();
    ui->initBtn->setDisabled(false);
    ui->leaveRoomBtn->setDisabled(true);
    if (rtcEngineInstance) {
        releaseRtcEngineInstance();
    }
    rtcEngineInstance = nullptr;
    hasInit_ = false;

}

void MainWindow::onInitBtn()
{
    init();

    ui->initBtn->setEnabled(false);
    ui->loginRoomBtn->setEnabled(true);
}
 
void MainWindow::onClientRoleChanged(const QString & room, RtcRoleType oldType, RtcRoleType newType)
{
    (void)room;
    (void)oldType;
}


void MainWindow::onEngineChangeNotify() {
    qDebug() << Q_FUNC_INFO << "onEngine switch.....";
}

void MainWindow::init() {
    // 
    releaseRtcEngineInstance();


    rtcEngineInstance = getRtcEngineInstance();
    rtcEngineInstance->registerLogFunc(MainWindow::log_func, rtc::RTCLOGTYPE_INFO);
    rtcEngineInstance->registerEventHandler(mprtcHandler);
    rtcEngineInstance->registerVideoFrameObserver(m_videoFrameObserver);
    rtcEngineInstance->registerAudioFrameObserver(m_audioObserver);

    //memset(&mTranscodingConfig, 0, sizeof(mTranscodingConfig));
    mTranscodingConfig.width = 1280;
    mTranscodingConfig.height = 720;
    mTranscodingConfig.videoBitrate = 1080;
    mTranscodingConfig.videoFramerate = 15;
    mTranscodingConfig.videoGop = 15;
    // mTranscodingConfig.videoCodecProfile = rtc::VIDEO_CODEC_PROFILE_BASELINE;
    mTranscodingConfig.backgroundColor = 0x00;
    // mTranscodingConfig.videoCodecType = rtc::VIDEO_CODEC_H264_TRANSCODING;
    mBackgroudImage.height = 540;
    mBackgroudImage.width = 960;
    mBackgroudImage.x = 100;
    mBackgroudImage.y = 100;
    // strcpy(mBackgroudImage.url, "http://3ttech.cn/res/tpl/default/images/bk.png");
    mBackgroudImage.url = "http://3ttech.cn/res/tpl/default/images/bk.png";
    mTranscodingConfig.backgroundImage = &mBackgroudImage;
    mTranscodingConfig.audioBitrate = 192;
    mTranscodingConfig.audioChannels = 1;
    // mTranscodingConfig.audioSampleRate = rtc::AUDIO_SAMPLE_RATE_48000;
    // mTranscodingConfig.audioCodecProfile = rtc::AUDIO_CODEC_PROFILE_HE_AAC;

    m_appID = ui->appIDText->toPlainText();



    m_engineType = 7;

    hasInit_ = true;
    string strRoomId = ui->roomTx->toPlainText().toStdString();
    string strAppid = m_appID.toStdString();
    string strToken = "";
    rtc::EnterConfig config;
    config.roomId = strRoomId.c_str();
    config.uid = ui->user->toPlainText().toLong();
    config.engineType = (rtc::RTCEngineType)m_engineType;
    config.appId = strAppid.c_str();
    config.token = strToken.c_str();
    int init_result = rtcEngineInstance->init(config);
    rtcEngineInstance->getVideoDeviceManager()->setCurDevice(0);
}


void MainWindow::release()
{
    if (mprtcHandler)
    {
        delete mprtcHandler;
        mprtcHandler = nullptr;
    }

    qDebug() << Q_FUNC_INFO << __LINE__;

    if (m_audioObserver)
    {
        delete m_audioObserver;
        m_audioObserver = nullptr;
    }
    if (m_videoFrameObserver)
    {
        delete m_videoFrameObserver;
        m_videoFrameObserver = nullptr;
    }
    hasInit_ = false;
}

void MainWindow::onAudioDeviceChanged()
{
    rtc::IMicManager* p = rtcEngineInstance->getNewMicManager();
    int micCount = p->getDeviceCount();

    for (int i = 0; i < micCount; ++i) {
        char name[rtc::MAX_DEVICE_ID_LENGTH] = { 0 };
        char id[rtc::MAX_DEVICE_ID_LENGTH] = { 0 };
        if (!p->getDevice(i, name, id))
            continue;
        qDebug() << Q_FUNC_INFO << name << ", id=" << id;
    }
}

void MainWindow::onVideoDeviceChanged()
{
    qDebug() << Q_FUNC_INFO;
}


void MainWindow::onDestoryButton()
{
    qDebug() << Q_FUNC_INFO;
}


