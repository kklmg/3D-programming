#include"stdafx.h"
#include"ObjectMng.h"
#include"Global.h"
#include<time.h>
#include"Asedata.h"
#include"Object.h"
#include"Trie.h"


struct Vertex
{
	Vertex() {}
	Vertex(D3DXVECTOR3 *pos, D3DXVECTOR3 *Normal, D3DXVECTOR2 *tex)
	{
		_pos = *pos;
		_Normal = *Normal;
		_Tex = *tex;
	}
	D3DXVECTOR3 _pos;
	D3DXVECTOR3 _Normal;
	D3DXVECTOR2 _Tex;
	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1;


struct GridVertex
{
	float _x, _y, _z;
	DWORD _color;

	GridVertex()
	{
		_x = 0.0f;
		_y = 0.0f;
		_z = 0.0f;
		_color = 0xffffffff;
	}
};

#define FVF_GRID_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

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

D3DLIGHT9 d3dLight;
D3DXVECTOR3 dir(0.0f, -1.0f, -0.25f);






CObject obj00;


CObjectMng::CObjectMng(IDirect3DDevice9*const Device)
	:m_rpDevice(Device)
{
}

CObjectMng::~CObjectMng()
{
}


bool CObjectMng::Init()
{
	CreateObject();
	return true;
}

void CObjectMng::CreateObject() 
{
	HRESULT hr = 0;

	

	obj00.Init("ASEFile/03IK-Joe.ASE");
	//obj00.Init("ASEFile/Dragon2.ASE");

	

	//obj00.Init("ASEFile/MaterialBox.ASE");
	
	
	//obj00.Init("ASEFile/P38.ASE");
	
	//obj00.Init("ASEFile/Dragon2.ASE");
	//obj00.Init("ASEFile/TigerTank.ASE");
	//obj00.Init("ASEFile/teapot.ASE");

	

	
	//obj00.Init("ASEFile/dynamic_walk.ASE");

	//obj00.Init("ASEFile/dynamic_walk.ASE");
	//obj00.Init("ASEFile/FlyingSpaceFighter06.ASE");
	


	//-----------------------------------------------------------------------------
	//Set Light
	//-----------------------------------------------------------------------------
	d3dLight.Type = D3DLIGHT_DIRECTIONAL;
	d3dLight.Ambient = D3DXCOLOR(D3DCOLOR_XRGB(255, 255, 255)) * 0.6f;
	d3dLight.Diffuse = D3DXCOLOR(D3DCOLOR_XRGB(255, 255, 255));
	d3dLight.Specular = D3DXCOLOR(D3DCOLOR_XRGB(255, 255, 255)) * 0.6f;
	d3dLight.Direction = dir;

	m_rpDevice->SetLight(0, &d3dLight);
	
	
	//-----------------------------------------------------------------------------
	// Set Camera.
	//-----------------------------------------------------------------------------

	D3DXVECTOR3 position(0.0f, 250.0f, -50.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);

	m_rpDevice->SetTransform(D3DTS_VIEW, &V);

	//-----------------------------------------------------------------------------
	// Set the projection matrix.
	//-----------------------------------------------------------------------------

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)g_WD_WIDTH / (float)g_WD_HEIGHT,
		1.0f,
		1000.f);
	m_rpDevice->SetTransform(D3DTS_PROJECTION, &proj);

	//-----------------------------------------------------------------------------
	// Set Render State
	//-----------------------------------------------------------------------------
	//m_rpDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//m_rpDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_rpDevice->LightEnable(0, true);
	m_rpDevice->SetRenderState(D3DRS_LIGHTING, true);
	m_rpDevice->SetRenderState(D3DRS_SPECULARENABLE, false);
	m_rpDevice->SetRenderState(D3DRS_FILLMODE, _D3DFILLMODE::D3DFILL_SOLID);
	//m_rpDevice->SetRenderState(D3DRS_FILLMODE, _D3DFILLMODE::D3DFILL_WIREFRAME);
	//m_rpDevice->SetRenderState(D3DRS_CULLMODE, _D3DCULL::D3DCULL_CW);
	D3DMATERIAL9 defaultmat = InitMtrl(g_WHITE, g_WHITE, g_BLACK, g_BLACK, 10);
	m_rpDevice->SetMaterial(&defaultmat);
}


void CObjectMng::DrawObject() 
{
	D3DXMATRIX Rx, Ry;
	D3DXMATRIX Move;
	D3DXMATRIX Scale;
	D3DXMatrixTranslation(&Move, 0, 0, 100);
	D3DXMatrixScaling(&Scale, 3, 3, 0);
	

	static float y = 0.0f;
	static float x = 0.0f;

	if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
		y -= 0.1f;
	if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		y += 0.1f;
	if (::GetAsyncKeyState(VK_UP) & 0x8000f)
		x += 0.1f;
	if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
		x += 0.1f;


	D3DXMatrixRotationY(&Ry, y);
	D3DXMatrixRotationX(&Rx, x);
	
	


	
	if (y >= 6.28f)
		y = 0.0f;
	if (x >= 6.28f)
		x = 0.0f;


	D3DXMATRIX p = Rx * Ry;
	D3DXMATRIX q =   Move*Scale;



	//m_rpDevice->SetTransform(D3DTS_WORLD, &p);

	obj00.SetTransform(&p);

	obj00.Draw();
}

