#include"stdafx.h"
#include"Timer.h"
#include"Global.h"


CTimer::CTimer() 
	:
	m_emState(emState::eWait),
	m_dwBegin(0),
	m_dwElapse(0),
	m_dwStopBegin(0),
	m_dwStopElapse(0)
{
}
CTimer::~CTimer() 
{
}


void CTimer::Begin()
{
	m_dwBegin = g_pTimeMng->GetTime_DW();
	m_dwElapse = 0;
	m_emState = emState::eGoing;
}

void CTimer::ReCover() 
{
	m_dwBegin += m_dwStopElapse;
	m_dwStopElapse = 0;
	m_emState = emState::eGoing;
}

double CTimer::GetElapse() const
{
	return m_dwElapse * 0.001f;
}

DWORD CTimer::GetElapse_dw() const
{
	return m_dwElapse;
}

void CTimer::Stop() 
{
	m_dwStopBegin = g_pTimeMng->GetTime_DW();
}

void CTimer::__Update()
{
	switch (m_emState)
	{
		//Timer Going
		case CTimer::eGoing: 
		{
			//Compute Delay Time
			double test = g_pTimeMng->GetTime_DW();

			m_dwElapse = g_pTimeMng->GetTime_DW() - m_dwBegin;

			int a = 0;
		}break;

		//Timer Stop
		case CTimer::eStop:
		{			
			//Compute Stop Delay Time
			m_dwStopElapse = g_pTimeMng->GetTime_DW() - m_dwStopBegin;
		}break;

		//Timer End || Timer Wait
		default:
			break;
	}
}

bool CTimer::Timing(float time, bool repeat)
{
	if (m_emState == emState::eEnd) return false;

	//Init Timer
	if (m_emState == emState::eWait) Begin();
	
	//Update Timer
	__Update();

	//Check Interval
	if (m_emState != emState::eStop	&& m_dwElapse > time*1000)
	{
		if (repeat)
			Begin();
		else
			m_emState = emState::eEnd;

		return true;
	}
	
	return false;
}

bool CTimer::Timing_dw(DWORD tick, bool repeat) 
{
	if (m_emState == emState::eEnd) return false;

	//Init Timer
	if (m_emState == emState::eWait) Begin();

	//Update Timer
	__Update();

	//Check Interval
	if (m_emState != emState::eStop	&& m_dwElapse > tick)
	{
		if (repeat)
			Begin();
		else
			m_emState = emState::eEnd;

		return true;
	}

	return false;
}