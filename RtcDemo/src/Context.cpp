#include "Context.h"

Context* Context::m_instance = nullptr;
Context * Context::instance()
{
	static ContextPrivate conPrivate;
	if (m_instance == nullptr) {
		m_instance = new Context();
	}
	return m_instance;
}

Stru_RtcBaseInfo & Context::getBaseInfo()
{
	return m_baseInfo;
}

void Context::cleanBaseInfo()
{
}

Context::Context()
{
}

Context::~Context()
{
}

Context::ContextPrivate::ContextPrivate()
{
}

Context::ContextPrivate::~ContextPrivate()
{
	if (m_instance != nullptr) {
		delete m_instance;
		m_instance = nullptr;
	}
}
