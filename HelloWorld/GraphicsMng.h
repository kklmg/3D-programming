#pragma once
#include"D3DClass.h"
#include"ObjectMng.h"

class CGraphicsMng
{
public:
	CGraphicsMng();
	~CGraphicsMng();

	bool Init();

	bool Frame();

private:
	bool Render();

	CDirect3D *m_pD3D;

	CObjectMng * m_pObjMng;
};

