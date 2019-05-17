#pragma once
#include"System.h"
#include"DropFile.h"

//window
extern HWND g_hWnd;

extern HINSTANCE g_hInst;

extern std::unique_ptr<CSystem>g_upSystem;

extern std::unique_ptr<CDropMng> g_upDropMng;

extern const int g_WD_WIDTH;
extern const int g_WD_HEIGHT;

extern IDirect3DDevice9 * g_pDevice;

//D3D
extern const D3DDEVTYPE g_DEVICETYPE;

extern const D3DXCOLOR      g_WHITE;
extern const D3DXCOLOR     g_BLACK;
extern const D3DXCOLOR        g_RED;
extern const D3DXCOLOR      g_GREEN;
extern const D3DXCOLOR       g_BLUE;
extern const D3DXCOLOR     g_YELLOW;
extern const D3DXCOLOR       g_CYAN;
extern const D3DXCOLOR    g_MAGENTA;

extern const D3DXCOLOR	g_BACKCOLOR;


extern const D3DXMATRIX g_IDMATRIX;