#pragma once

class CFpsMng
{
public:
	CFpsMng();
	~CFpsMng();

	bool Init();

	WORD GetFrmameCount() const;

	void FixFps(WORD fps);

	void Adjust();

	bool DrawFps();

	void Update();

private:
	bool __InitFont();

	double m_dBeginTime;
	double m_dFrameDelay;
	double m_dPerFrameDelay;


	WORD m_wFrameCount;
	WORD m_wLastFrame;
	WORD m_wFixedFrame;

	DWORD m_dwBeginTick;
	DWORD m_dwTick;

	//UI
	LPD3DXFONT m_pftFps;
	
	RECT m_rect;

	std::string m_strFPS;

};