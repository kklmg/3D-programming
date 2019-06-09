#pragma once
#include"System.h"
#include"DropFile.h"
#include"TimeMng.h"

//windows
//-------------------------------------------------------------
extern HWND g_hWnd;										//Handle

extern HINSTANCE g_hInst;								//Instance


extern const int g_WD_WIDTH;							//Window Width
extern const int g_WD_HEIGHT;							//Window Height


extern std::unique_ptr<CSystem>g_upSystem;				//Game System

extern std::unique_ptr<CDropMng> g_upDropMng;			//File Drop Manager


//DirectX 9
//-------------------------------------------------------------

extern IDirect3DDevice9 * g_pDevice;					//Device

extern const D3DDEVTYPE g_DEVICETYPE;					//Device type



//D3D Color
extern const D3DXCOLOR      g_WHITE;
extern const D3DXCOLOR     g_BLACK;
extern const D3DXCOLOR        g_RED;
extern const D3DXCOLOR      g_GREEN;
extern const D3DXCOLOR       g_BLUE;
extern const D3DXCOLOR     g_YELLOW;
extern const D3DXCOLOR       g_CYAN;
extern const D3DXCOLOR    g_MAGENTA;

//back surface color
extern const D3DXCOLOR	g_BACKCOLOR;




//Game Time Manager
//-------------------------------------------------------------
extern CTimeMng *g_pTimeMng;									//Time Manager



//Common Value
//-------------------------------------------------------------

//Identity matrix
extern const D3DXMATRIX g_IDMATRIX;

//Global Function
//-------------------------------------------------------------

void ComputeNormal(D3DXVECTOR3 *out, D3DXVECTOR3 *v1, D3DXVECTOR3 *v2, D3DXVECTOR3 *v3);

D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);