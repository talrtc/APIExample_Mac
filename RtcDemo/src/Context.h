#pragma once

#include <qlist.h>
#include "RtcBase.h"

enum engine {
	TTTEngine = 0,
	XES_ENGIN,
	AGORA_ENGIN,
	TALMEDIA_ENGINE,
	ALI_ENGINE,
	ZegoEngine
};

enum RtcRoleType {
	RtcRole_Broadcaster = 0,
	RtcRole_Audice
};

typedef struct {
	QString userId;
	QString roomId;
	RtcRoleType		role;
    QString token;
}Stru_UserInfo;

typedef struct {
	QString psId;
	engine engineType;
	QString planId;
	QString tokenUrl;
	QString serverIp;
}Stru_LoginInfo;

typedef struct {
	Stru_LoginInfo	loginInfo;
	Stru_UserInfo   myUserInfo;
	QList<Stru_UserInfo> users;
	rtc::Stru_RtmpUplinesInfo rtmpInfos;
}Stru_RtcBaseInfo;

class Context
{
	class ContextPrivate {
	public:
		ContextPrivate();
		~ContextPrivate();
	};

public:
	static Context* instance();
	Stru_RtcBaseInfo &getBaseInfo();
	void cleanBaseInfo();

private:
	Context();
	~Context();

private:
	static Context* m_instance;
	Stru_RtcBaseInfo	m_baseInfo;
};

