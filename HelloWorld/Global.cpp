#include"stdafx.h"
#include"Global.h"

//window
HWND g_hWnd = nullptr;

HINSTANCE g_hInst = nullptr;

const int g_WD_WIDTH = 800;
const int g_WD_HEIGHT = 800;


//Device type
const D3DDEVTYPE g_DEVICETYPE = D3DDEVTYPE_HAL;

//Deviece
IDirect3DDevice9 * g_pDevice = NULL;

//D3d Common Colors
const D3DXCOLOR      g_WHITE=D3DCOLOR_XRGB(255, 255, 255);
const D3DXCOLOR     g_BLACK=D3DCOLOR_XRGB(0, 0, 0);
const D3DXCOLOR        g_RED=D3DCOLOR_XRGB(255, 0, 0);
const D3DXCOLOR      g_GREEN=D3DCOLOR_XRGB(0, 255, 0);
const D3DXCOLOR       g_BLUE=D3DCOLOR_XRGB(0, 0, 255);
const D3DXCOLOR     g_YELLOW=D3DCOLOR_XRGB(255, 255, 0);
const D3DXCOLOR       g_CYAN=D3DCOLOR_XRGB(0, 255, 255);
const D3DXCOLOR    g_MAGENTA=D3DCOLOR_XRGB(255, 0, 255);

//Back Surface Color
const D3DXCOLOR g_BACKCOLOR = g_BLUE;


//System
std::unique_ptr<CSystem> g_upSystem;

std::unique_ptr<CDropMng> g_upDropMng;

//Time Manager
extern CTimeMng *g_pTimeMng = nullptr;


//Common Value
const D3DXMATRIX g_IDMATRIX =
{
	/*_11, _12, _13, _14*/ 1,0,0,0,
	/*_21, _22, _23, _24*/ 0,1,0,0,
	/*_31, _32, _33, _34*/ 0,0,1,0,
	/*_41, _42, _43, _44*/ 0,0,0,1,
};


//Global Function

void ComputeNormal(D3DXVECTOR3 *out, D3DXVECTOR3 *v1, D3DXVECTOR3 *v2, D3DXVECTOR3 *v3)
{
	D3DXVECTOR3 u = *v2 - *v1;
	D3DXVECTOR3 v = *v3 - *v1;

	D3DXVec3Cross(out, &u, &v);
	D3DXVec3Normalize(out, out);
}

D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;

	return mtrl;
}
