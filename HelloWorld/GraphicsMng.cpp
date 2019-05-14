#include"stdafx.h"
#include"GraphicsMng.h"


CGraphicsMng::CGraphicsMng() :m_pD3D(nullptr)
{
}

CGraphicsMng::~CGraphicsMng()
{
	SAFE_DELETE(m_pD3D);
	SAFE_DELETE(m_pObjMng);
}

bool CGraphicsMng::Init()
{
	m_pD3D = new CDirect3D();
	if (!m_pD3D->Init())return false;

	m_pObjMng = new CObjectMng(m_pD3D->GetDevice());
	if (!m_pObjMng->Init())return false;





	return true;
}


bool CGraphicsMng::Frame()
{
	if (!Render())return false;


	return true;
}
bool CGraphicsMng::Render()
{
	m_pD3D->BeginScene();

	
	m_pObjMng->DrawObject();









	m_pD3D->EndScene();

	return true;
}
