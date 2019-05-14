#pragma once
#include<shellapi.h>

class CDropMng
{
public:
	CDropMng();
	~CDropMng();
	
	void ActiveDrop(HWND hWnd);
	bool AcceptFile(WPARAM wParam);

	std::vector<std::basic_string<TCHAR>> GetFiles() const;

private:
	void Clear();

	bool m_bActived;
	bool m_bAccepted;
	WORD m_wDropCount;

	std::vector<std::basic_string<TCHAR>> m_vetFilePath;

	HDROP m_hdrop;
};