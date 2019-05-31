#pragma once
#include"D3DClass.h"
#include"ObjectMng.h"
#include"FpsMng.h"
#include"Camera.h"

class CGraphicsMng
{
public:
	CGraphicsMng();
	~CGraphicsMng();

	bool Init();

	bool Frame();

	CFpsMng* m_refFps;

private:
	bool Render();

	CDirect3D *m_pD3D;

	CCamera *m_pCamera;

	CObjectMng * m_pObjMng;
};

