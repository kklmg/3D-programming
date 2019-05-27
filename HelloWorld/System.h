#pragma once
#include "GraphicsMng.h"
#include "FpsMng.h"
class CSystem
{
public:
	CSystem();
	~CSystem();

	bool Init(HINSTANCE hInstance, int nCmdShow);
	int  Run();
	void ShutDown();

	void SetFps(WORD fixfps);
private:
	bool InitWindow(int nCmdShow);
	bool InitGraphics();
	bool InitOther();
	bool InitTimeMng();
	bool InitFpsMng();

private:
	CFpsMng *m_pFpsMng;
	CGraphicsMng *m_pGrapMng;
};




//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);