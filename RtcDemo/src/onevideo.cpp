#include "onevideo.h"
//#include "iconbutton.h"

#include "GlYuvWidget.h"

#include <QHBoxLayout>
#include <QDebug>
#include <QPainter>
#include <QLinearGradient>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QTimer>
#include <QImage>
#include <QLabel>
#include <QDateTime>
#include <QFile>
#include <QMessageBox>
#include <QCoreApplication>
#include <QSpinBox>
#include <QLineEdit>

#include <memory>

//使用QICON有内存泄漏，目前暂时不知道怎么释放QICON的内存泄漏
//#define USE_QICON_BTN 

const int OneVideo::WIDTH = 850; //480
const int OneVideo::HEIGHT = 550; //320
const int kOffsetwidth = 5;

extern rtc::IRTCEngine* rtcEngineInstance;

uint64_t OneVideo::ovCounter = 0;

AudioLevelDB::AudioLevelDB(QWidget *parent /*= 0*/):QWidget(parent)
{

}

AudioLevelDB::~AudioLevelDB()
{

}

void AudioLevelDB::setVoulme(int voulme)
{
	voulme_ = voulme;
	bPaint_ = true;
	update();
}

void AudioLevelDB::paintEvent(QPaintEvent *event)
{
	(void)event;

    
    
    QPainter painter(this);
	if (bPaint_)
	{
		painter.setRenderHint(QPainter::Antialiasing, true);
		painter.eraseRect(0, 0, 10, height());

		QLinearGradient linearGradient(0, height(), 0, 0);
		linearGradient.setColorAt(0.0, QColor(31, 116, 182));
		linearGradient.setColorAt(0.6, QColor(47, 161, 190));
		linearGradient.setColorAt(1.0, QColor(95, 223, 255));
		painter.setPen(QPen(QBrush(linearGradient), 10));
		painter.drawLine(0, height(), 0, height() - (height()* (float)voulme_ / 255));
		bPaint_ = false;
	}
}


const int FILEVIDEOLIST[][2] = { {3840, 2160}, {1920, 1080}, {1280,720}, {960,540}, {640,360}, {320,240} };

OneVideo::OneVideo(QWidget *parent, unsigned userId, const QString &roomId, bool sdkRender) 
	: QFrame(parent)
	, useSdkRenderer_(sdkRender)
	, currentIndex(ovCounter++)
	, centralImage(":/images/icon.png")
	, userId_(userId)
	, m_roomId(roomId)
{
    setFixedSize(WIDTH, HEIGHT);
	createToolButtons();	
	setFrameShape(QFrame::Box);
	setBackgroundRole(QPalette::Light);
	setAutoFillBackground(true);
	setMouseTracking(true);
	useExternalVideoSouce_ = false;
	
	connect(this, SIGNAL(signalPublish()), this, SLOT(publishStream()));

	QString runPath = QCoreApplication::applicationDirPath(); // get app path
	//QString sub = runPath.mid(0, runPath.count() - 34);

    mVideoFileWidth = 320;
    mVideoFileHeight = 240;
    mVideoFileName = runPath + "/foreman_320x240.yuv"; // default

	for (int i = 0; i < sizeof(FILEVIDEOLIST) / sizeof(FILEVIDEOLIST[0]); ++i) {
		QString tmpFileName = QString("%1/extvideo_%2x%3.yuv").arg(runPath).arg(FILEVIDEOLIST[i][0]).arg(FILEVIDEOLIST[i][1]);// +QString::sprintf(buf, "extvideo_%dx%d.yuv", , FILEVIDEOLIST[i][1]);
		QFile tmpFile(tmpFileName);
		if (tmpFile.exists()) {
			mVideoFileWidth = FILEVIDEOLIST[i][0];
			mVideoFileHeight = FILEVIDEOLIST[i][1];
			mVideoFileName = tmpFileName;
			break;
		}
	}

	mAudioFileName = runPath + "/test.pcm";
	mAudioFileAECName = runPath + "/testAEC.pcm";
    autoMuteVideo = new QTimer(this);
    autoMuteAudio = new QTimer(this);
    connect(autoMuteAudio, SIGNAL(timeout()), SLOT(autoAutoMuteAudio()));
    connect(autoMuteVideo, SIGNAL(timeout()), SLOT(autoAutoMuteVideo()));
}

OneVideo::~OneVideo()
{
#if !defined(__APPLE__)
	// test for onevideo mem leak
	char buf[1204] = { 0 };
	sprintf(buf, "== destroy onevideo %lld, recv:%lld, draw:%lld, lost:%lld\n",
		currentIndex, recvYuvDataCnt, drawYuvDataCnt, recvYuvDataCnt - drawYuvDataCnt);
	OutputDebugStringA(buf);
#endif

    //releaseToolButtons();

    if(mAudioTimer) {
		mAudioTimer->stop();
		delete mAudioTimer;
		mAudioTimer = nullptr;
	}
	if (mAudioAECTimer) {
		mAudioAECTimer->stop();
		delete mAudioAECTimer;
		mAudioAECTimer = nullptr;
	}
	if (mediaTimer) {
		mediaTimer->stop();
		delete mediaTimer;
		mediaTimer = nullptr;
	}
    if (mPCMBuf) {
        delete mPCMBuf;
        mPCMBuf = nullptr;
    }
	if (mAECBuf) {
		delete mAECBuf;
		mAECBuf = nullptr;
	}
	if (mpPCMFile)
	{
		fclose(mpPCMFile);
        mpPCMFile = nullptr;
	}
	if (mpAECFile)
	{
		fclose(mpAECFile);
		mpAECFile = nullptr;
	}
	if (myuvBuf) {
		delete myuvBuf;
		myuvBuf = nullptr;
	}
    if (autoMuteVideo) {
        autoMuteVideo->stop();
        delete autoMuteVideo;
        autoMuteVideo = nullptr;
    }
    if (autoMuteAudio) {
        autoMuteAudio->stop();
        delete autoMuteAudio;
        autoMuteAudio = nullptr;
    }
}

void OneVideo::releaseToolButtons()
{
    //if (this->buttonsLayout)
    {
        if (isLocalUser()) {
            if (publishAudioBtn)
            {
                publishAudioBtn->setParent(nullptr);
                publishAudioBtn->setIcon(QIcon());
                delete publishAudioBtn;
                publishAudioBtn = nullptr;
            }

            if (publishVideoBtn)
            {
                publishVideoBtn->setParent(nullptr);
                publishVideoBtn->setIcon(QIcon());
                delete publishVideoBtn;
                publishVideoBtn = nullptr;
            }

            if (enableLocalAudioBtn)
            {
                enableLocalAudioBtn->setParent(nullptr);
                enableLocalAudioBtn->setIcon(QIcon());
                delete enableLocalAudioBtn;
                enableLocalAudioBtn = nullptr;
            }
        }
        else {
            if (mirrorBtn)
            {
                mirrorBtn->setParent(nullptr);
                mirrorBtn->setIcon(QIcon());
                delete mirrorBtn;
                mirrorBtn = nullptr;
            }

            if (m_spinVolume)
            {
                m_spinVolume->setParent(nullptr);
                delete m_spinVolume;
                m_spinVolume = nullptr;
            }

        }

        if (muteVideoBtn)
        {
            muteVideoBtn->setParent(nullptr);
            muteVideoBtn->setIcon(QIcon());
            delete muteVideoBtn;
            muteVideoBtn = nullptr;
        }

        if (muteAudioBtn)
        {
            muteAudioBtn->setParent(nullptr);
            muteAudioBtn->setIcon(QIcon());
            delete muteAudioBtn;
            muteAudioBtn = nullptr;
        }

        if (dumpVideoBtn)
        {
            dumpVideoBtn->setParent(nullptr);
            delete dumpVideoBtn;
            dumpVideoBtn = nullptr;
        }

        if (closeBtn)
        {
            closeBtn->setParent(nullptr);
            closeBtn->setIcon(QIcon());
            delete closeBtn;
            closeBtn = nullptr;
        }

        if (videoShow)
        {
            videoShow->setParent(nullptr);
            delete videoShow;
            videoShow = nullptr;
        }

        if (videoContent_)
        {
            videoContent_->setParent(nullptr);
            delete videoContent_;
            videoContent_ = nullptr;
        }

        if (audioLevelDb_)
        {
            audioLevelDb_->setParent(nullptr);
            delete audioLevelDb_;
            audioLevelDb_ = nullptr;
        }

        if (labelTxQuality)
        {
            labelTxQuality->setParent(nullptr);
            delete labelTxQuality;
            labelTxQuality = nullptr;
        }
        if (labelRxQuality)
        {
            labelRxQuality->setParent(nullptr);
            delete labelRxQuality;
            labelRxQuality = nullptr;
        }


        //buttonsLayout->setParent(nullptr);
        //delete buttonsLayout;
        //buttonsLayout = nullptr;
    }

    //QVBoxLayout *infoBox = new QVBoxLayout(this);
    //if (!m_roomId.isEmpty()) {
    //    QLabel *labelRoom = new QLabel(this);
    //    labelRoom->setText(QString("room:") + m_roomId);
    //    buttonsLayout->addWidget(labelRoom);
    //    infoBox->addWidget(labelRoom);
    //}

    //if (!isLocalUser()) {
    //    QLabel *label = new QLabel(this);
    //    label->setText((std::string(" Id:") + std::to_string(userId_)).c_str());
    //    buttonsLayout->addWidget(label);
    //    infoBox->addWidget(label);
    //}

    //buttonsLayout->addLayout(infoBox);

    //if (!useSdkRenderer_) {
    //    videoShow = new GLYuvWidget(this);
    //    videoShow->resize(WIDTH - kOffsetwidth, HEIGHT - 50);
    //    videoShow->move(kOffsetwidth, 0);
    //    videoShow->show();

    //    // TODO : 此处目前采用了 DirectConnection ，以避免内存泄漏，但会导致回调处理时间过长，阻滞SDK层 -- TODO 优化待定
    //    // 
    //    auto ret = connect(this, SIGNAL(showYuv(uchar*, uint, uint)), this, SLOT(slotShowYuv(uchar*, uint, uint)));
    //}
    //else
    //{
    //    //QGridLayout*    m_gridLayout = new QGridLayout(this);
    //    //m_gridLayout->setMargin(0);
    //    //setLayout(m_gridLayout);

    //    videoContent_ = new QWidget(this);
    //    videoContent_->resize(WIDTH - kOffsetwidth, HEIGHT - 50);
    //    videoContent_->move(kOffsetwidth, 0);
    //    //		videoContent_->backgroundRole();
    //    // 		QPalette pal(videoContent_->palette());
    //    // 		//videoContent_->setGeometry(0, 0, 100, 100);
    //    // 		//设置背景黑色
    //    // 		pal.setColor(QPalette::Background, Qt::darkGray);
    //    // 		videoContent_->setAutoFillBackground(true);
    //    // 		videoContent_->setPalette(pal);

    //            //m_gridLayout->addWidget(videoContent_);

    //    videoContent_->show();
    //}

    //audioLevelDb_ = new AudioLevelDB(this);
    //audioLevelDb_->resize(kOffsetwidth, HEIGHT - 50);
    //audioLevelDb_->show();
}

void OneVideo::createToolButtons()
{
    QVBoxLayout *bottomLayout = new QVBoxLayout(this);
    bottomLayout->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);

	QHBoxLayout *buttonsLayout = new QHBoxLayout(this);
	//buttonsLayout = new QHBoxLayout(this);
	buttonsLayout->setSpacing(2);
    QHBoxLayout *buttonsLayout2 = new QHBoxLayout(this);

	if (isLocalUser())
    {
		publishAudioBtn = new QPushButton(this);
#ifdef USE_QICON_BTN
		publishAudioBtn->setIcon(QIcon(":/images/un_play.png"));
#else
        publishAudioBtn->setText("unPubA");
#endif
		publishAudioBtn->setToolTip("unpublish audio stream");
		buttonsLayout->addWidget(publishAudioBtn);
		connect(publishAudioBtn, SIGNAL(clicked(bool)), SLOT(publishAudioBtnSlot()));


		publishVideoBtn = new QPushButton(this);
#ifdef USE_QICON_BTN
        publishVideoBtn->setIcon(QIcon(":/images/un_camera.png"));
#else
        publishVideoBtn->setText("UnPubV");
#endif
		publishVideoBtn->setToolTip("unpublish video stream");
		buttonsLayout->addWidget(publishVideoBtn);
		connect(publishVideoBtn, SIGNAL(clicked(bool)), SLOT(publishVideoBtnSlot()));

        enableLocalAudioBtn = new QPushButton(this);
        enableLocalAudioBtn->setToolTip("disable local audio");
        buttonsLayout->addWidget(enableLocalAudioBtn);
        connect(enableLocalAudioBtn, SIGNAL(clicked(bool)), this, SLOT(onEnableLocalAudio()));
        enableLocalAudioBtn->setText("LAudio");
	}
	else
    {
        /*
		mirrorBtn = new IconButton(this);
#ifdef USE_QICON_BTN
        mirrorBtn->setIcon(QIcon(":/images/un_camera.png"));
#else
#endif
		mirrorBtn->setToolTip("mirror video stream");
		buttonsLayout->addWidget(mirrorBtn);
		connect(mirrorBtn, SIGNAL(clicked(bool)), SLOT(mirrorRemoteVideoBtnSlot()));
        */
	}

	muteVideoBtn = new QPushButton(this);
#ifdef USE_QICON_BTN
	muteVideoBtn->setIcon(QIcon(":/images/record.png"));
#else
    muteVideoBtn->setText("MuteV");
#endif
	muteVideoBtn->setToolTip("mute video");
	buttonsLayout->addWidget(muteVideoBtn);
	connect(muteVideoBtn, SIGNAL(clicked(bool)), SLOT(muteVideoBtnSlot()));

	muteAudioBtn = new QPushButton(this);
#ifdef USE_QICON_BTN
	muteAudioBtn->setIcon(QIcon(":/images/fullscreen.png"));
#else
    muteAudioBtn->setText("MuteA");
#endif
	muteAudioBtn->setToolTip("mute audio");
	buttonsLayout->addWidget(muteAudioBtn);
	connect(muteAudioBtn, SIGNAL(clicked(bool)), SLOT(muteAudioBtnSlot()));

	// 
	dumpVideoBtn = new QPushButton(this);
	dumpVideoBtn->setToolTip("Start - dump video yuv");
	buttonsLayout->addWidget(dumpVideoBtn);
	connect(dumpVideoBtn, SIGNAL(clicked(bool)), this, SLOT(onDumpVideoYUV()));
	dumpVideoBtn->setText("DumpV");
	// 

	closeBtn = new QPushButton(this);
#ifdef USE_QICON_BTN
	closeBtn->setIcon(QIcon(":/images/close.png"));
#else
    closeBtn->setText("Close");
#endif
	//closeBtn->move(WIDTH - closeBtn->width() - 8, 8);
	buttonsLayout->addWidget(closeBtn);
	if (!isLocalUser())
    {
		connect(closeBtn, SIGNAL(clicked(bool)), SLOT(unSubcribeStream()));
		connect(closeBtn, SIGNAL(clicked(bool)), SLOT(close()));
		connect(closeBtn, SIGNAL(clicked(bool)), SLOT(deleteLater()));
		
	}
    else
    {
		closeBtn->setVisible(false);
	}
	
	if (!isLocalUser())
    {
		m_spinVolume = new QSpinBox(this);
		m_spinVolume->setRange(0, 100);
		m_spinVolume->setValue(100);
		buttonsLayout->addWidget(m_spinVolume);
		connect(m_spinVolume, SIGNAL(valueChanged(int)), this, SLOT(onSpinValueChanged(int)));
	}
	
    closeBtn->raise();    
	if (isLocalUser())
    {
		publishAudioBtn->raise();
		publishVideoBtn->raise();
	}
    muteVideoBtn->raise();
	muteAudioBtn->raise();
    
	QVBoxLayout *infoBox = new QVBoxLayout(this);
	if (!m_roomId.isEmpty())
    {
		QLabel *labelRoom = new QLabel(this);
		labelRoom->setText(QString("room:") + m_roomId);
        labelRoom->setToolTip(QString("room:") + m_roomId);
		//buttonsLayout->addWidget(labelRoom);
		infoBox->addWidget(labelRoom);
	}
	if (!isLocalUser())
    {
		QLabel *label = new QLabel(this);
		label->setText((std::string(" Id:") + std::to_string(userId_)).c_str());
        label->setToolTip((std::string(" Id:") + std::to_string(userId_)).c_str());
		//buttonsLayout->addWidget(label);
		infoBox->addWidget(label);
	}
    buttonsLayout->addLayout(infoBox);

    QVBoxLayout *qualityBox = new QVBoxLayout(this);
    labelTxQuality = new QLabel(this);
    labelTxQuality->setText((std::string(" txQuality:")).c_str());
    labelTxQuality->setToolTip((std::string(" txQuality:")).c_str());
    //buttonsLayout->addWidget(labelTxQuality);
    qualityBox->addWidget(labelTxQuality);

    labelRxQuality = new QLabel(this);
    labelRxQuality->setText((std::string(" rxQuality:")).c_str());
    labelRxQuality->setToolTip((std::string(" rxQuality:")).c_str());
    //buttonsLayout->addWidget(labelRxQuality);
    qualityBox->addWidget(labelRxQuality);
    buttonsLayout->addLayout(qualityBox);

    QLabel* labTimeIntervalleAudio = new QLabel(this);
    labTimeIntervalleAudio->setText("timeInterval");
    labTimeIntervalleAudio->setToolTip("自动测试muteLocalAudio时间间隔");
    buttonsLayout2->addWidget(labTimeIntervalleAudio);

    EditTimeIntervalleAudio = new QLineEdit(this);
    EditTimeIntervalleAudio->setText("15");
    EditTimeIntervalleAudio->resize(50, 30);
    buttonsLayout2->addWidget(EditTimeIntervalleAudio);
   
    audioMuteAudioBtn = new QPushButton(this);
    audioMuteAudioBtn->setText("StartAutoMuteAudio");
    audioMuteAudioBtn->setToolTip("自动mute音频");
    buttonsLayout2->addWidget(audioMuteAudioBtn);
    connect(audioMuteAudioBtn, SIGNAL(clicked(bool)), this, SLOT(onAutoMuteAudio()));

    QLabel* labTimeIntervalleVideo = new QLabel(this);
    labTimeIntervalleVideo->setText("timeInterval");
    labTimeIntervalleVideo->setToolTip("自动测试muteLocalAudio时间间隔");
    buttonsLayout2->addWidget(labTimeIntervalleVideo);

    EditTimeIntervalleVideo = new QLineEdit(this);
    EditTimeIntervalleVideo->setText("15");
    EditTimeIntervalleVideo->resize(50, 30);
    buttonsLayout2->addWidget(EditTimeIntervalleVideo);

    audioMuteVideoBtn = new QPushButton(this);
    audioMuteVideoBtn->setText("StartAutoMuteVideo");
    audioMuteVideoBtn->setToolTip("自动mute视频");
    buttonsLayout2->addWidget(audioMuteVideoBtn);
    connect(audioMuteVideoBtn, SIGNAL(clicked(bool)), this, SLOT(onAutoMuteVideo()));

    bottomLayout->addLayout(buttonsLayout);
    bottomLayout->addLayout(buttonsLayout2);


	if (!useSdkRenderer_)
    {
		videoShow = new GLYuvWidget(this);
		videoShow->resize(WIDTH - kOffsetwidth, HEIGHT - 100);
		videoShow->move(kOffsetwidth,0);
		videoShow->show();
		// TODO : 此处目前采用了 DirectConnection ，以避免内存泄漏，但会导致回调处理时间过长，阻滞SDK层 -- TODO 优化待定
		// 
		auto ret = connect(this, SIGNAL(showYuv(uchar*, uint, uint)), this, SLOT(slotShowYuv(uchar*, uint, uint)));
	}
	else
	{
        //QGridLayout*    m_gridLayout = new QGridLayout(this);
        //m_gridLayout->setMargin(0);
        //setLayout(m_gridLayout);

		videoContent_ = new QWidget(this);
		videoContent_->resize(WIDTH - kOffsetwidth, HEIGHT - 100);
		videoContent_->move(kOffsetwidth, 0);
//		videoContent_->backgroundRole();
// 		QPalette pal(videoContent_->palette());
// 		//videoContent_->setGeometry(0, 0, 100, 100);
// 		//设置背景黑色
// 		pal.setColor(QPalette::Background, Qt::darkGray);
// 		videoContent_->setAutoFillBackground(true);
// 		videoContent_->setPalette(pal);
		
        //m_gridLayout->addWidget(videoContent_);
		videoContent_->show();
	}

	audioLevelDb_ = new AudioLevelDB(this);
	audioLevelDb_->resize(kOffsetwidth, HEIGHT - 100);
	audioLevelDb_->show();
}

void OneVideo::onYuvData(uchar *y, uchar *u, uchar *v, int yStride, int uStride, int vStride, uint width, uint height)
{
	if (!useSdkRenderer_ && y && u && v && yStride > 0 && uStride > 0 && vStride > 0 && width > 0 && height > 0) {
		uint32_t len = width * height * 3 / 2;
		uchar* yuvPtr = new uchar[len];

		if (yuvPtr) {
			int nYUVBufsize = 0;
			int nVOffset = 0;
			for (unsigned i = 0; i < height; ++i) {
				memcpy(yuvPtr + nYUVBufsize, y + i * yStride, width);
				nYUVBufsize += width;
			}
			for (unsigned i = 0; i < (height >> 1); ++i) {
				memcpy(yuvPtr + nYUVBufsize, u + i * uStride, width >> 1);
				nYUVBufsize += (width >> 1);
				memcpy(yuvPtr + width * height * 5 / 4 + nVOffset, v + i * vStride, width >> 1);
				nVOffset += (width >> 1);
			}

			// from work tread sync to main thread
			recvYuvDataCnt++;
			emit showYuv(yuvPtr, width, height);
		}

	}
}

void OneVideo::setType(int type)
{
	type_ = type;
}

void OneVideo::setVoulme(int voulme)
{
	if (audioLevelDb_) {
		audioLevelDb_->setVoulme(voulme);
	}
}

void OneVideo::setTxQuality(int txQuality)
{
    if (labelTxQuality) {
        labelTxQuality->setText((std::string(" txQuality:") + std::to_string(txQuality)).c_str());
    }
}

void OneVideo::setRxQuality(int rxQuality)
{
    if (labelRxQuality) {
        labelRxQuality->setText((std::string(" rxQuality:") + std::to_string(rxQuality)).c_str());
    }
}

void OneVideo::slotShowYuv(uchar *ptr, uint width, uint height)
{
	// in main thread
	if (dumpVideo_)
	{
		char szFile[256];
		sprintf(szFile, "./%u-%d-%d.yuv", userId_, width, height);
		if (yuvFile_ == nullptr)
		{
			yuvFile_ = fopen(szFile, "ab+");
		}
		if (yuvFile_)
		{
			fwrite(ptr, 1, width * height * 3 / 2, yuvFile_);
			fclose(yuvFile_);
			yuvFile_ = nullptr;
		}
	}

	drawYuvDataCnt++;

	videoShow->showYuv(ptr, width, height);

	delete[] ptr;
}


void OneVideo::unSubcribeStream()
{
	rtcEngineInstance->muteRemoteVideo(userId_, true);
	rtcEngineInstance->muteRemoteAudio(userId_, true);
	/* TODO : ??? merge from ttt-git
	emit muteRemoteVideo(userId_, roomId(), true);
	emit muteRemoteAudio(userId_, roomId(), true);
	*/
}

void OneVideo::onSpinValueChanged(int value)
{
	qDebug() << Q_FUNC_INFO << "new value=" << value;
    emit setRemoteVolume(userId_, value, m_roomId);
}

void OneVideo::onEnableLocalAudio()
{
    m_bEnabelLocalAudio = !m_bEnabelLocalAudio;
    if (m_bEnabelLocalAudio) {
        enableLocalAudioBtn->setToolTip("disable local audio");
    } else {
        enableLocalAudioBtn->setToolTip("enabel local audio");
    }

    rtcEngineInstance->enableLocalAudio(m_bEnabelLocalAudio);
}

void OneVideo::onAutoMuteAudio()
{
    m_bStartAutoMuteAudio = !m_bStartAutoMuteAudio;

    if (m_bStartAutoMuteAudio)
    {
        autoAutoMuteAudio();
        autoMuteAudio->start(EditTimeIntervalleAudio->text().toInt() * 1000);
        audioMuteAudioBtn->setText("StopAutoMuteAudio");
    }
    else
    {
        autoMuteAudio->stop();
        m_bStartAutoMuteAudio = false;
        if (isLocalUser())
            rtcEngineInstance->muteLocalAudio(m_bStartAutoMuteAudio);
        else
            emit muteRemoteAudio(userId(), roomId(), m_bStartAutoMuteAudio);
        audioMuteAudioBtn->setText("StartAutoMuteAudio");

    }
}

void OneVideo::onAutoMuteVideo()
{
    m_bStartAutoMuteVideo = !m_bStartAutoMuteVideo;

    if (m_bStartAutoMuteVideo)
    {
        autoAutoMuteVideo();
        autoMuteVideo->start(EditTimeIntervalleVideo->text().toInt() * 1000);
        audioMuteVideoBtn->setText("StopAutoMuteVideo");
    }
    else
    {
        autoMuteVideo->stop();
        m_bCurrentMuteVideoStatus = false;
        if (isLocalUser())
            rtcEngineInstance->muteLocalVideo(m_bCurrentMuteVideoStatus);
        else
            emit muteRemoteVideo(userId(), roomId(), m_bCurrentMuteVideoStatus);
        audioMuteVideoBtn->setText("StartAutoMuteVideo");

    }
}

void OneVideo::autoAutoMuteAudio()
{
    m_bCurrentMuteAudioStatus = !m_bCurrentMuteAudioStatus;
    if (isLocalUser())
        rtcEngineInstance->muteLocalAudio(m_bCurrentMuteAudioStatus);
    else
        emit muteRemoteAudio(userId(), roomId(), m_bCurrentMuteAudioStatus);
    
}

void OneVideo::autoAutoMuteVideo()
{
    m_bCurrentMuteVideoStatus = !m_bCurrentMuteVideoStatus;
    if (isLocalUser())
        rtcEngineInstance->muteLocalVideo(m_bCurrentMuteVideoStatus);
    else
        emit muteRemoteVideo(userId(), roomId(), m_bCurrentMuteVideoStatus);
}

void OneVideo::publishAudioBtnSlot()
{
	if(isPublishAudio_){
		isPublishAudio_ = false;
		unpublishAudioStream();
	}
	else{		
		isPublishAudio_ = true;
		publishAudioStream();
	}

	publishAudioBtn->setToolTip(isPublishAudio_ ? "unpublish audio stream" : "publish audio stream");
#ifdef USE_QICON_BTN
    publishAudioBtn->setIcon(QIcon(isPublishAudio_ ? ":/images/un_play.png" : ":/images/play.png"));
#else
    publishAudioBtn->setText((isPublishAudio_ ? "UnPubA" : "PubA"));
#endif

	update();
}

void OneVideo::timerPlayAudio()
{
	if (rtcEngineInstance == nullptr)
		return;
	if (mpPCMFile == NULL)
	{
		QFile bfilePath(mAudioFileName);
		if (!bfilePath.exists()) 
			return;
		
		mpPCMFile = fopen(mAudioFileName.toLocal8Bit().toStdString().c_str(), "rb");
		//获取数据总长度
		fseek(mpPCMFile, 0, SEEK_END);
		mfilesize = ftell(mpPCMFile);
	}

	if (!mpPCMFile) {
		QMessageBox::information(this, "info", "open audio file failed");
		mAudioTimer->stop();
		return;
	}

	if (mpPCMFile && feof(mpPCMFile))
	{
		qDebug() << "timerPlayeAudio eof";
		fseek(mpPCMFile, 0, SEEK_SET);
	}
	unsigned len = 480 * 2 * 2;
	if (mPCMBuf == NULL)
	{
		mPCMBuf = new char[len];
	}

	if (mpPCMFile)
	{
		fseek(mpPCMFile, moffset, SEEK_SET);
		int bufLen = fread(mPCMBuf, 1, len, mpPCMFile);
		rtc::ExternalAudioFrame Aframe;
		Aframe.type = rtc::ExternalAudioFrame::FRAME_TYPE_PCM16;
		Aframe.bytesPerSample = 2;
		Aframe.channels = 2;
		Aframe.samples = 480;
		Aframe.samplesPerSec = 48000;
		Aframe.buffer = mPCMBuf;
		Aframe.renderTimeMs = 0;
		Aframe.avsync_type = 0;
		rtcEngineInstance->pushExternalAudioFrame(&Aframe);
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
}

void OneVideo::timerPlayeData()
{
	//获取yuv 数据发送
	if (rtcEngineInstance == nullptr)
		return;
	if (mpfile == NULL){
		QFile bfilePath(mVideoFileName);
		if (!bfilePath.exists()){
			return;
		}
		mpfile = fopen(mVideoFileName.toLocal8Bit().toStdString().c_str(), "rb");
	}

	if (!mpfile)
	{
		QMessageBox::information(this,"info","open video file failed");
		mediaTimer->stop();
		return;
	}

	if (feof(mpfile)){
		qDebug() << "timerPlayeData eof";
		fseek(mpfile, 0, SEEK_SET);
	}
	if (mpfile){
		qDebug() << "timerPlayeData sucess";
	}
	else{
		qDebug() << "timerPlayeData failed";
	}

	if (myuvBuf == NULL){
		myuvBuf = new char[mVideoFileWidth * mVideoFileHeight * 2];
	}
	if (mpfile != NULL){
		int iread = fread(myuvBuf, 1, mVideoFileWidth * mVideoFileHeight * 3 / 2, mpfile);
        if (iread == 0)
            return;

		qint64 timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch(); //毫秒级

		qDebug() << "timerPlayeData len:" << iread << "timestatmp:" << timestamp;

		rtc::ExternalVideoFrame eframe;
		memset(&eframe, 0, sizeof(rtc::ExternalVideoFrame));
		eframe.type = rtc::ExternalVideoFrame::VIDEO_BUFFER_RAW_DATA;
		eframe.format = rtc::ExternalVideoFrame::VIDEO_PIXEL_I420;
		eframe.stride = mVideoFileWidth;
		eframe.height = mVideoFileHeight;
		eframe.buffer = (char*)myuvBuf;
		eframe.timestamp = timestamp;
		eframe.rotation = 0;
		rtcEngineInstance->pushExternalVideoFrame(&eframe);
		
		if (!videoShow) {
			videoShow = new GLYuvWidget(this);
			videoShow->resize(WIDTH - kOffsetwidth, HEIGHT - 100);
			videoShow->move(kOffsetwidth, 0);
			videoShow->show();
		}
		//分辨率不一样的时候，导致画面闪烁
		slotShowYuv((uchar*)myuvBuf, mVideoFileWidth, mVideoFileHeight);
		myuvBuf = NULL; //released in slotShowYuv
	}
}

void OneVideo::switchVideoSource(bool useExternalVideoSource)
{
	useExternalVideoSouce_ = useExternalVideoSource;
	if(useExternalVideoSouce_)
		publishVideoStream();
}

void OneVideo::switchAudioSource(bool useExternalAudioSource)
{
	useExternalAudioSouce_ = useExternalAudioSource;
	if(useExternalAudioSouce_)
		publishAudioStream();
}

void OneVideo::setAudioSourceAEC(bool useExternalAudioSourceAEC)
{
	useExternalAudioSouceAEC_ = useExternalAudioSourceAEC;
	if (useExternalAudioSouceAEC_)
	{
		if (!mAudioAECTimer)
		{
			mAudioAECTimer = new QTimer(this);
			connect(mAudioAECTimer, SIGNAL(timeout()), SLOT(timerPlayAudioAEC()));
			mAudioAECTimer->start(10);
		}
		else
			mAudioAECTimer->start(10);
	}
}

void OneVideo::publishVideoBtnSlot()
{
	if (!rtcEngineInstance) {
		return;
	}
	isPublishing_ = !isPublishing_;
	publishVideoBtn->setToolTip(isPublishing_ ? "unpublish video stream" : "publish video stream");
#ifdef USE_QICON_BTN
    publishVideoBtn->setIcon(QIcon(isPublishing_ ? ":/images/un_camera.png" : ":/images/camera.png"));
#else
    publishVideoBtn->setText((isPublishing_ ? "UnCamera" : "Camera"));
#endif

	if (isPublishing_) {
		publishOnce_ = true;
		publishVideoStream();

		//防止拖动窗口时闪烁
		if(videoContent_)
			videoContent_->setUpdatesEnabled(false);
	}
	else
		unpublishVideoStream();
}

void OneVideo::unpublishVideoStream()
{
	if (useExternalVideoSouce_) {
		if (mediaTimer)
			mediaTimer->stop();
	}
	rtcEngineInstance->enableLocalVideo(false);
}

void OneVideo::publishAudioStream()
{
	rtcEngineInstance->enableLocalAudio(true);
	if (useExternalAudioSouce_)
	{
		if (!mAudioTimer)
		{
			mAudioTimer = new QTimer(this);
			connect(mAudioTimer, SIGNAL(timeout()), SLOT(timerPlayAudio()));
			mAudioTimer->start(10);
		}
		else
			mAudioTimer->start(10);
	}
}

void OneVideo::unpublishAudioStream()
{
	if (useExternalAudioSouce_) {
		if (mediaTimer)
			mAudioTimer->stop();
	}
	rtcEngineInstance->enableLocalAudio(false);
}

void OneVideo::publishVideoStream()
{
	rtcEngineInstance->enableLocalVideo(true);
	if (useExternalVideoSouce_){	
		//rtcEngineInstance->setVideoResolution(rtc::RTC_VIDEO_RESOLUTION_320x240);
		//rtcEngineInstance->setVideoResolution(mVideoFileWidth, mVideoFileHeight);
		if (!mediaTimer){
			mediaTimer = new QTimer(this);
			connect(mediaTimer, SIGNAL(timeout()), SLOT(timerPlayeData()));
			mediaTimer->start(1000/15);
		} else {
			mediaTimer->start(1000/15);
		}
	}

	if(useExternalVideoSouce_ && type_ == 1) //声网使用外部视频，必须设置fps
		rtcEngineInstance->setVideoFps(15);

#if 0
	/////////////////////////////////////////////////////////////
	string vfile = strUtil::toString(videoFileName_);

	rtcEngineInstance->muteLocalVideo(false);
	rtcEngineInstance->enableExternalVideoSource(useExternalVideoSouce_);
	if (!useExternalVideoSouce_) {
		auto vp = rtc::getVideoProfile()[videoFormatCtl_.GetCurSel()];
		rtcEngineInstance->setVideoResolution(vp.profile);
		rtcEngineInstance->setupLocalVideo(localVideo_.GetSafeHwnd());
		rtcEngineInstance->startPreview();
		rtcEngineInstance->enableLocalVideo(true);
	}
	else {
		SetTimer(VIDEO_FILE_TIMER_INDEX, FILE_VIDEO_GAP, NULL);

		//rtcEngineInstance->enableLocalVideo(true);

		/// cur file resolution...
		if (!yuvData_) {
			localSelfDraw_.StartRenderer();
			yuvData_.reset(new YuvData_f());
			yuvData_->fileOpen(_T("./foreman_320x240.yuv"));
			yuvData_->setFormat(YUV420, 320, 240);
		}

		rtcEngineInstance->setVideoResolution(rtc::RTC_VIDEO_RESOLUTION_320x240);
	}

	rtcEngineInstance->muteLocalAudio(false);
	rtcEngineInstance->enableExternalAudioSource(useAudioFile_, 16000, 1);
	if (useAudioFile_) {
		SetTimer(AUDIO_FILE_TIMER_INDEX, FILE_AUDIO_GAP, NULL);

		if (!audioData_) {
			audioData_.reset(new CWaveFile());
			if (!audioData_->read("./test.wav")) {
				LOG_WARN_("can not open wav file.");
			}
		}
	}
	else {
		rtcEngineInstance->enableLocalAudio(true);
	}
	rtcEngineInstance->setVideoFps(_ttoi(localVideoFps_));
	/*
	rtc::RTC_BEAUTY_STYLE st = rtc::RTC_BEAUTY_STYLE_SMOOTH;
	if (beautyMode_ == _T("光滑"))
	st = rtc::RTC_BEAUTY_STYLE_SMOOTH;
	else if (beautyMode_ == _T("自然"))
	st = rtc::RTC_BEAUTY_STYLE_NATURE;
	else if (beautyMode_ == _T("朦胧"))
	st = rtc::RTC_BEAUTY_STYLE_BLUR;
	///pusher_->setBeautyStyle(st, beautyLevel_, beautyWhite_);
	///pusher_->setRenderMode(rtc::RTC_VIDEO_RENDER_MODE_FILL);
	*/
	bool isChanged;
	rtc::RTC_VIDEO_BITRATE bt;
	getComboBitrate(isChanged, bt);
	rtcEngineInstance->setVideoBitrate(bt);

#ifdef TEST_SPEAKER_VOLUME
	for (int i = 0; i < 100; ++i) {
		/// test setVolume
		int v = rtcEngineInstance->getPlayoutManager()->getVolume();
		qDebug() << Q_FUNC_INFO << ("current speaker volume: " << v);
		if (v >= 0) {
			unsigned av = v * 0.3;
			rtcEngineInstance->getPlayoutManager()->setVolume(av);
			qDebug() << Q_FUNC_INFO << ("set current speaker volume: " << av);
			rtcEngineInstance->getPlayoutManager()->setVolume(80);
			if (!rtcEngineInstance->getPlayoutManager()->setVolume(40)) {
				qDebug() << Q_FUNC_INFO << ("error to set speaker volume");
			}
		}
	}
#endif

#endif
}

void OneVideo::onDumpVideoYUV()
{
	dumpVideo_ = !dumpVideo_;

	// 
	if (dumpVideo_)
		dumpVideoBtn->setToolTip("Stop - dump video yuv");
	else
		dumpVideoBtn->setToolTip("Start - dump video yuv");
}

void OneVideo::muteAudioBtnSlot()
{
	if (!rtcEngineInstance) {
		return;
	}

	qDebug() << Q_FUNC_INFO << ((isLocalAudioMute_ ? "unmute audio" : "mute audio"));

	isLocalAudioMute_ = !isLocalAudioMute_;

	if (isLocalUser())
		rtcEngineInstance->muteLocalAudio(isLocalAudioMute_);
	else {
		//rtcEngineInstance->muteRemoteAudio(userId(), isLocalAudioMute_);
		emit muteRemoteAudio(userId(), roomId(), isLocalAudioMute_);
	}

	muteAudioBtn->setToolTip(isLocalAudioMute_ ? "unmute audio" : "mute audio");
#ifdef USE_QICON_BTN
    muteAudioBtn->setIcon(QIcon(isLocalAudioMute_ ? ":/images/un_fullscreen.png" : ":/images/fullscreen.png"));
#else
    muteAudioBtn->setText((isLocalAudioMute_ ? "UnmuteA" : "MuteA"));
#endif
}

void OneVideo::mirrorRemoteVideoBtnSlot() {
	if (!rtcEngineInstance) {
		return;
	}

	qDebug() << Q_FUNC_INFO << ((isVideoMirrored_ ? "unmirror video" : "mirror video"));

	isVideoMirrored_ = !isVideoMirrored_;

	emit mirrorRemoteVideo(userId(), roomId(), isVideoMirrored_);
}

void OneVideo::muteVideoBtnSlot()
{
	if (!rtcEngineInstance) {
		return;
	}

	qDebug() << Q_FUNC_INFO << ((isLocalAudioMute_ ? "unmute audio" : "mute audio"));

	isLocalVideoMute_ = !isLocalVideoMute_;

	if(isLocalUser())
		rtcEngineInstance->muteLocalVideo(isLocalVideoMute_);
	else {
		//rtcEngineInstance->muteRemoteVideo(userId(),isLocalVideoMute_);
		emit muteRemoteVideo(userId(), roomId(), isLocalVideoMute_);
	}

	muteVideoBtn->setToolTip(isLocalVideoMute_ ? "unmute video" : "mute video");
#ifdef USE_QICON_BTN
    muteVideoBtn->setIcon(QIcon(isLocalVideoMute_ ? ":/images/un_record.png" : ":/images/record.png"));
#else
    muteVideoBtn->setText((isLocalVideoMute_ ? "UnMuteV" : "MuteV"));
#endif
}



void OneVideo::updateImage(QImage image)
{
	(void)image;
// 	if(isPreview){
// 		centralImage = image;
// 		update();
// 	}
}


void OneVideo::closeEvent(QCloseEvent *event)
{
	QFrame::closeEvent(event);
	emit closeSignal(this);
}

void OneVideo::paintEvent(QPaintEvent *event)
{
	QFrame::paintEvent(event);
	QPainter painter(this);
	painter.drawImage(width() / 2 - centralImage.width() / 2,
				height() / 2 - centralImage.height() / 2,
				centralImage);	
}

void OneVideo::mouseMoveEvent(QMouseEvent *event)
{	
	if(event->y() > rect().bottom() - 50 || event->y() < 50){		
		isButtonsShow = true;
		// ...
	}else{		
		isButtonsShow = false;
	}
}

void OneVideo::leaveEvent(QEvent *event)
{

	Q_UNUSED(event);
	if(isButtonsShow){
		isButtonsShow = false;
	}
}

void OneVideo::setSdkRender(bool sdkRender) 
{
	useSdkRenderer_ = sdkRender;
}

void OneVideo::startCaptureTimer()
{
	if (!mediaTimer) {
		mediaTimer = new QTimer(this);
		connect(mediaTimer, SIGNAL(timeout()), SLOT(timerPlayeData()));
		mediaTimer->start(40);
	}
	else {
		mediaTimer->start(40);
	}
}

void OneVideo::stopCaptureTimer()
{
	if (mediaTimer)
		mediaTimer->stop();
}
void OneVideo::startAudioCollectionTimer()
{
	if (!mAudioTimer)
	{
		mAudioTimer = new QTimer(this);
		connect(mAudioTimer, SIGNAL(timeout()), SLOT(timerPlayAudio()));
		mAudioTimer->start(10);
	}
	else
		mAudioTimer->start(10);
}
void OneVideo::stopAudioCollectionTimer()
{
	if (mAudioTimer)
		mAudioTimer->stop();
}

void OneVideo::startAudioAECTimer()
{
	if (!mAudioAECTimer)
	{
		mAudioAECTimer = new QTimer(this);
		connect(mAudioAECTimer, SIGNAL(timeout()), SLOT(timerPlayAudioAEC()));
		mAudioAECTimer->start(10);
	}
	else
		mAudioAECTimer->start(10);
}

void OneVideo::stopAudioAECTimer()
{
	if (mAudioAECTimer)
		mAudioAECTimer->stop();
}

void OneVideo::timerPlayAudioAEC()
{
	if (rtcEngineInstance == nullptr)
		return;
	if (mpAECFile == NULL)
	{
		QFile bfilePath(mAudioFileAECName);
		if (!bfilePath.exists())
			return;

		mpAECFile = fopen(mAudioFileAECName.toLocal8Bit().toStdString().c_str(), "rb");
		//获取数据总长度
		fseek(mpAECFile, 0, SEEK_END);
		mAECfilesize = ftell(mpAECFile);
	}

	if (!mpAECFile) {
		QMessageBox::information(this, "info", "open audio file failed");
		mAudioAECTimer->stop();
		return;
	}

	if (mpAECFile && feof(mpAECFile))
	{
		qDebug() << "timerPlayeAudio eof";
		fseek(mpAECFile, 0, SEEK_SET);
	}
	unsigned len = 480 * 2 * 2;
	if (mAECBuf == NULL)
	{
		mAECBuf = new char[len];
	}

	if (mpAECFile)
	{
		fseek(mpAECFile, mAECoffset, SEEK_SET);
		int bufLen = fread(mAECBuf, 1, len, mpAECFile);
		rtc::ExternalAudioFrame Aframe;
		Aframe.type = rtc::ExternalAudioFrame::FRAME_TYPE_PCM16;
		Aframe.bytesPerSample = 2;
		Aframe.channels = 2;
		Aframe.samples = 480;
		Aframe.samplesPerSec = 48000;
		Aframe.buffer = mAECBuf;
		Aframe.renderTimeMs = 0;
		Aframe.avsync_type = 0;
        //rtcEngineInstance->pushAudioFrame(&Aframe, 0);
		mAECoffset += bufLen;
		qDebug() << "timerPlayAEC sucess";
	}
	if ((mAECoffset >= mAECfilesize) && (mAECfilesize > 0))
	{
		printf(" read the pcm file is end============\n");
		mAECoffset = 0;
		mAECfilesize = 0;
		fclose(mpAECFile);
		mpAECFile = nullptr;
	}
}
