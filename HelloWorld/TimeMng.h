#pragma once

class CTimeMng
{
public:
	CTimeMng();
	~CTimeMng();

	void Init();
	void Stop();


	void Update();

	double GetTime()const;
	DWORD GetTime_DW()const;

private:

	DWORD m_dwBegin;
	DWORD m_dwCur;

	DWORD m_dwElapse;
	DWORD m_dwStop;
};