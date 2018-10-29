#include "GlobalData.h"
#include <cassert>

GlobalData * GlobalData::s_instance = nullptr;

GlobalData::GlobalData()
{
}


void GlobalData::Startup()
{
	assert(!s_instance);
	s_instance = new GlobalData();
}


void GlobalData::Shutdown()
{
	assert(s_instance);
	delete s_instance;
	s_instance = nullptr;
}


GlobalData * GlobalData::GetInstance()
{
	return s_instance;
}

void GlobalData::SetMyColor(char mycolor)
{
	m_mycolor = mycolor;
}

void GlobalData::SetBlackStone(char blackstone)
{
	m_blackStone = blackstone;
}

void GlobalData::SetWhiteStone(char whitestone)
{
	m_whiteStone = whitestone;
}

char GlobalData::GetMyColor()
{
	return m_mycolor;
}

char GlobalData::GetBlackStone()
{
	return m_blackStone;
}

char GlobalData::GetWhiteStone()
{
	return m_whiteStone;
}


void GlobalData::SetResult(Result result)
{
	m_result = result;
}


Result GlobalData::GetResult() const
{
	return m_result;
}
