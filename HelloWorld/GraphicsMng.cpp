#include"stdafx.h"
#include"GraphicsMng.h"


CGraphicsMng::CGraphicsMng()
	:m_pD3D(nullptr),
	m_pCamera(nullptr),
	m_pObjMng(nullptr)
{
}

CGraphicsMng::~CGraphicsMng()
{
	SAFE_DELETE(m_pD3D);
	SAFE_DELETE(m_pObjMng);
	SAFE_DELETE(m_pCamera)
}

bool CGraphicsMng::Init()
{
	m_pD3D = new CDirect3D();
	if (!m_pD3D->Init())return false;

	m_pObjMng = new CObjectMng(m_pD3D->GetDevice());
	if (!m_pObjMng->Init())return false;

	m_pCamera = new CCamera;



	return true;
}


bool CGraphicsMng::Frame()
{
	D3DXMATRIX View;
	m_pCamera->Update();
	m_pCamera->GetViewMatrix(&View);
	m_pD3D->SetViewTM(&View);


	if (!Render())return false;


	return true;
}
bool CGraphicsMng::Render()
{
	m_pD3D->BeginScene();

	


	m_refFps->DrawFps();
	
	m_pObjMng->DrawObject();









	m_pD3D->EndScene();

	return true;
}
