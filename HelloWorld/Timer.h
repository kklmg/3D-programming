#pragma once

class CTimer
{
	enum emState 
	{
		eWait,
		eGoing,
		eStop,
		eEnd
	};

public:
	CTimer();
	~CTimer();

	void Begin();
	void Stop();
	void ReCover();

	double GetElapse() const;
	DWORD GetElapse_dw() const;

	//Counting
	bool Timing(float time, bool repeat = false);
	bool Timing_dw(DWORD tick, bool repeat = false);

private:
	void __Update();

private:
	emState m_emState;

	DWORD m_dwBegin;
	DWORD m_dwElapse;

	DWORD m_dwStopBegin;
	DWORD m_dwStopElapse;
};