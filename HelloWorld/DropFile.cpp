#include"stdafx.h"
#include"DropFile.h"

CDropMng::CDropMng() :m_bActived(FALSE), m_bAccepted(FALSE), m_wDropCount(0)
{
}

CDropMng::~CDropMng()
{
	Clear();
}

void CDropMng::ActiveDrop(HWND hWnd)
{
	m_bActived = TRUE;
	DragAcceptFiles(hWnd, TRUE);
}

bool CDropMng::AcceptFile(WPARAM wParam)
{
	if (!m_bActived)return false;

	//get Drop File buffer
	m_hdrop = (HDROP)wParam;

	//Get Drop File Count
	m_wDropCount = DragQueryFile(m_hdrop, 0xFFFFFFFF, NULL, 0);
	m_vetFilePath.reserve(m_wDropCount);

	//FileName buffer
	TCHAR sDropFilePath[MAX_PATH + 1];

	//Get File Name
	for (int i = 0; i<m_wDropCount; ++i)
	{
		DragQueryFile(m_hdrop, i, sDropFilePath, sizeof(sDropFilePath));
		m_vetFilePath.push_back(sDropFilePath);
	}
	return true;
}

std::vector<std::basic_string<TCHAR>> CDropMng::GetFiles() const
{
	return m_vetFilePath;
}

void CDropMng::Clear()
{
	m_bActived = FALSE;
	m_bAccepted = FALSE;
	m_wDropCount = 0;

	DragFinish(m_hdrop);
}