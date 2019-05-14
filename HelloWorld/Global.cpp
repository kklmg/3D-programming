#include"stdafx.h"
#include"Global.h"

//window
HWND g_hWnd = nullptr;

HINSTANCE g_hInst = nullptr;

const int g_WD_WIDTH = 800;
const int g_WD_HEIGHT = 800;



//D3D
const D3DDEVTYPE g_DEVICETYPE = D3DDEVTYPE_HAL;

IDirect3DDevice9 * g_pDevice = NULL;

const D3DXCOLOR      g_WHITE=D3DCOLOR_XRGB(255, 255, 255);
const D3DXCOLOR     g_BLACK=D3DCOLOR_XRGB(0, 0, 0);
const D3DXCOLOR        g_RED=D3DCOLOR_XRGB(255, 0, 0);
const D3DXCOLOR      g_GREEN=D3DCOLOR_XRGB(0, 255, 0);
const D3DXCOLOR       g_BLUE=D3DCOLOR_XRGB(0, 0, 255);
const D3DXCOLOR     g_YELLOW=D3DCOLOR_XRGB(255, 255, 0);
const D3DXCOLOR       g_CYAN=D3DCOLOR_XRGB(0, 255, 255);
const D3DXCOLOR    g_MAGENTA=D3DCOLOR_XRGB(255, 0, 255);


const D3DXCOLOR g_BACKCOLOR = g_BLUE;

//System
std::unique_ptr<CSystem>g_upSystem;

std::unique_ptr<CDropMng> g_upDropMng;
