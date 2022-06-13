#ifndef ONEVIDEO_H
#define ONEVIDEO_H

#include <QFrame>
#include <QImage>
#include <QMenu>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTime>
#include <QLineEdit>

#include "IRTCEngine.h"

class GLYuvWidget;
class IconButton;
class QLabel;
class QMacNativeWidget;
class QSpinBox;

class AudioLevelDB : public QWidget {

	Q_OBJECT
public:
	explicit AudioLevelDB(QWidget *parent = 0);
	~AudioLevelDB();
	void setVoulme(int voulme);
protected:
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
private:
	int voulme_;
	bool bPaint_ = false;;

};

class OneVideo : public QFrame
{
	Q_OBJECT
public:
    explicit OneVideo(QWidget *parent = 0, 
					  unsigned userId = 0, 
					  const QString &roomId = "",
					  bool sdkRender = false);
	~OneVideo();

	static const int WIDTH;
	static const int HEIGHT;

public:	
	unsigned userId() const { return userId_; }
	bool isLocalUser() const { return userId_ == 0; }
	QString &roomId() { return m_roomId; }

	bool useSdkRenderer() const { return useSdkRenderer_; }
	bool publishOnce() const { return publishOnce_; }
	void publishVideoStream();
	void unpublishVideoStream();
	void publishAudioStream();
	void unpublishAudioStream();
	

	void switchVideoSource(bool useExternalVideoSource);
	void switchAudioSource(bool useExternalAudioSource);
	void setAudioSourceAEC(bool useExternalAudioSourceAEC);
	void onYuvData(uchar *y, uchar *u, uchar *v, int yStride, int uStride, int vStride, uint width, uint height);
	void setType(int type);
	void setVoulme(int voulme);
    void setTxQuality(int txQuality);
    void setRxQuality(int rxQuality);
	void setSdkRender(bool sdkRender);
	void startCaptureTimer();
	void stopCaptureTimer();
	void startAudioCollectionTimer();
	void stopAudioCollectionTimer();
	void startAudioAECTimer();
	void stopAudioAECTimer();
private slots:
	void publishAudioBtnSlot();
	void publishVideoBtnSlot();
	void muteVideoBtnSlot();
	void muteAudioBtnSlot();	
	void mirrorRemoteVideoBtnSlot();
	void updateImage(QImage image);

	void onDumpVideoYUV();
	
	void timerPlayeData();
	void timerPlayAudio();
	void timerPlayAudioAEC();
	void slotShowYuv(uchar *ptr, uint width, uint height);

	void unSubcribeStream();

	void onSpinValueChanged(int value);
    void onEnableLocalAudio();

    void onAutoMuteAudio();
    void onAutoMuteVideo();

    void autoAutoMuteAudio();
    void autoAutoMuteVideo();

signals:
	void showYuv(uchar *ptr, uint width, uint height);
	void closeSignal(OneVideo *who);
	void signalPublish();
	void setRemoteVolume(unsigned uid, int volume, const QString &channelId);
	void muteRemoteAudio(unsigned uid, const QString &room, bool mute);
	void muteRemoteVideo(unsigned uid, const QString &room, bool mute);
	void mirrorRemoteVideo(unsigned uid, const QString& room, bool mirror);

protected:
	void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private:
	void createToolButtons();
    void releaseToolButtons();

private:
    QPushButton *closeBtn=nullptr;
    QPushButton *publishAudioBtn =nullptr;
    QPushButton *publishVideoBtn=nullptr;
    QPushButton *muteVideoBtn=nullptr;
    QPushButton *muteAudioBtn=nullptr;
    QPushButton* mirrorBtn = nullptr;
    QPushButton * qtestButton=nullptr;
    QPushButton *enableLocalAudioBtn = nullptr;
	QPushButton *dumpVideoBtn = nullptr;
    //QHBoxLayout *buttonsLayout = nullptr;
    QPushButton *audioMuteAudioBtn = nullptr;
    QPushButton *audioMuteVideoBtn = nullptr;

    QLineEdit *EditTimeIntervalleAudio = nullptr;
    QLineEdit *EditTimeIntervalleVideo = nullptr;

    bool			useSdkRenderer_ = false;
    GLYuvWidget*	videoShow=nullptr;
	AudioLevelDB*   audioLevelDb_ = nullptr;

    //QVBoxLayout *qualityBox = nullptr;
    QLabel *labelTxQuality = nullptr;
    QLabel *labelRxQuality = nullptr;

	uint64_t recvYuvDataCnt = 0;
	uint64_t drawYuvDataCnt = 0;

	uint64_t currentIndex = 0;

	static uint64_t ovCounter;

    QMenu menu;
public:
    //QFrame * qFrameView;

	//显示的图像
public:
	QImage centralImage;
	QWidget *videoContent_ = nullptr;
	//按钮工具栏显示状态
	bool isButtonsShow;
	//是否正在运行
	bool isPublishAudio_ = true;


	unsigned userId_ = 0;
	QString  roomId_;

	bool isPublishing_ = true;
	bool isLocalAudioMute_ = false;
	bool isLocalVideoMute_ = false;
    bool useExternalVideoSouce_ = false;
	bool useExternalAudioSouce_ = false;
	bool useExternalAudioSouceAEC_ = false;
	bool useExternalVideoTem_ = false;
	bool useExternalAudioTem_ = false;

	bool isVideoMirrored_ = false;

	bool publishOnce_ = false;		// if have use publish
	int type_ = 3;
    bool    m_bEnabelLocalAudio = true;

    bool m_bStartAutoMuteVideo = false;
    bool m_bStartAutoMuteAudio = false;
    bool m_bCurrentMuteAudioStatus = false;
    bool m_bCurrentMuteVideoStatus = false;

    QTimer *autoMuteVideo = nullptr;
    QTimer *autoMuteAudio = nullptr;

private:
	QTimer *mediaTimer = nullptr;
	QTimer *mAudioTimer = nullptr;
	QTimer *mAudioAECTimer = nullptr;
	char* myuvBuf = NULL;
	char* mPCMBuf = NULL;
	char* mAECBuf = NULL;
	FILE *mpfile = NULL;
	FILE *mpPCMFile = NULL;
	FILE *mpAECFile = NULL;
	int moffset = 0;
	int mAECoffset = 0;
	int  mfilesize = 0;
	int  mAECfilesize = 0;
	int  mcurLen;
	QString mVideoFileName;
	QString mAudioFileName;
	QString mAudioFileAECName;
	QSpinBox	*m_spinVolume = nullptr;
	QString		m_roomId;

	// 
	bool dumpVideo_ = false;
	FILE *yuvFile_ = nullptr;

	int mVideoFileWidth = 0;
	int mVideoFileHeight = 0;

	// 

};

#endif // ONEVIDEO_H
