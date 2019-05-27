#include"stdafx.h"
#include"FpsMng.h"
#include<timeapi.h>
#include"FpsMng.h"
#include"Global.h"

CFpsMng::CFpsMng()
	:m_wFixedFrame(0),
	m_pftFps(nullptr)
{
}

CFpsMng::~CFpsMng()
{
}

bool CFpsMng::Init() 
{
	return __InitFont();
}

bool CFpsMng::__InitFont() 
{
	D3DXFONT_DESC desc;
	ZeroMemory(&desc, sizeof(D3DXFONT_DESC));
	desc.Height = 25;
	desc.Width = 12;
	desc.Weight = 500;
	desc.CharSet = DEFAULT_CHARSET;


	//desc.FaceName = "Times New Roman";

	m_rect.left = 10;
	m_rect.right = 200;
	m_rect.top = 10;
	m_rect.bottom = 100;

	return !D3DXCreateFontIndirect(g_pDevice, &desc, &m_pftFps);
}



void CFpsMng::Adjust()
{
	if (m_wFixedFrame == 0)return;


	DWORD begin = timeGetTime();
	DWORD FrameDelay = 0;

	while (FrameDelay < m_dPerFrameDelay*1000)
	{
		FrameDelay = timeGetTime() - begin;

		//Sleep(1);
	}
}

void CFpsMng::FixFps(WORD fps)
{
	m_wFixedFrame = fps;
	m_dPerFrameDelay = 1.0L / fps;
}



WORD CFpsMng::GetFrmameCount() const
{
	return m_wLastFrame;
}

void CFpsMng::Update()
{
	Adjust();

	++m_wFrameCount;

	if (timeGetTime() - m_dwBeginTick > 1000) 
	{
		m_wLastFrame = m_wFrameCount;
		m_wFrameCount = 0;
		m_dwBeginTick = timeGetTime();
		m_strFPS = "FPS: " + std::to_string(m_wLastFrame);
	}
}

bool CFpsMng::DrawFps() 
{
	return m_pftFps->DrawTextA(nullptr,
		m_strFPS.data(), -1, &m_rect, DT_TOP | DT_LEFT, g_GREEN);
}