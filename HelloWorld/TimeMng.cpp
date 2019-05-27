#include"stdafx.h"
#include"TimeMng.h"
#include"Global.h"
#include<timeapi.h>



CTimeMng::CTimeMng():
	m_dwBegin(0),
	m_dwElapse(0),
	m_dwStop(0),
	m_dwCur(0)
{
}

CTimeMng::~CTimeMng()
{
}

void CTimeMng::Init() 
{
	m_dwBegin = timeGetTime();
}

void CTimeMng::Stop() 
{
}

void CTimeMng::Update()
{
	m_dwCur = timeGetTime();
	m_dwElapse = timeGetTime() - m_dwBegin;
}

double CTimeMng::GetTime()const
{
	return (double)m_dwCur*0.001f;
}

DWORD CTimeMng::GetTime_DW()const
{
	return m_dwCur;
}

