#include "stdafx.h"
#include "resource.h"
#include <timeapi.h>
#include <shellapi.h>

#include "System.h"
#include "Global.h"


CSystem::CSystem():
	m_pGrapMng(nullptr)
{
}

CSystem::~CSystem()
{
	ShutDown();
}

bool CSystem::Init(HINSTANCE hInstance, int nCmdShow)
{
	g_hInst = hInstance;

	//Init Windows
	if (!InitWindow(nCmdShow))
	{
		::MessageBox(0, L"InitWindow() - FAILED", 0, 0);
		ShutDown();
		return false;
	}

	//Init DirectX
	if (!InitGraphics())
	{
		::MessageBox(0, L"InitD3D() - FAILED", 0, 0);
		ShutDown();
		return false;
	}

	//Init Global
	if (!InitOther())
	{
		::MessageBox(0, L"InitOther() - FAILED", 0, 0);
		ShutDown();
		return false;
	}

	return true;
}

bool CSystem::InitWindow(int nCmdShow)
{
	TCHAR szTitle[255];                  // The title bar text
	TCHAR szWindowClass[255];            // the main window class name

	LoadStringW(g_hInst, IDS_APP_TITLE, szTitle, 255);
	LoadStringW(g_hInst, IDC_HELLOWORLD, szWindowClass, 255);

	//--------------------------------------------------
	//resister class
	//--------------------------------------------------
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = g_hInst;
	wcex.hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_HELLOWORLD));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_HELLOWORLD);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));


	//Register
	if (!RegisterClassExW(&wcex))
	{
		::MessageBox(0, L"RegisterClassExW() - FAILED", 0, 0);
		return false;
	}
	

	//--------------------------------------------------
	//create Window
	//--------------------------------------------------
	g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, g_WD_WIDTH, g_WD_HEIGHT, nullptr, nullptr, g_hInst, nullptr);

	if (!g_hWnd)
	{
		::MessageBox(0, L"CreateWindow() - FAILED", 0, 0);
		return false;
	}


	//--------------------------------------------------
	//ShowWindow
	//--------------------------------------------------
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	return true;
}

bool CSystem::InitGraphics()
{
	m_pGrapMng = new CGraphicsMng();
	return m_pGrapMng->Init();
}

bool CSystem::InitOther()
{
	g_upDropMng = std::make_unique<CDropMng>();

	return true;
}

int CSystem::Run()
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	static float lastTime = (float)timeGetTime();


	HACCEL hAccelTable = LoadAccelerators(g_hInst, MAKEINTRESOURCE(IDC_HELLOWORLD));

	// Main message loop:
	//while (msg.message != WM_QUIT)
	//{
	//	if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	//	{
	//		::TranslateMessage(&msg);
	//		::DispatchMessage(&msg);
	//	}


	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
			float currTime = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;

			lastTime = currTime;

			m_pGrapMng->Frame();
		

	}
	return (int)msg.wParam;

}

void CSystem::ShutDown()
{
	SAFE_DELETE(m_pGrapMng);
}


//call back fun

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{

	}
	break;


	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_DROPFILES:
	{
		if (g_upDropMng)
			g_upDropMng->AcceptFile(wParam);


	}break;


	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
