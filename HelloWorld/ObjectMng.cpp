#include"stdafx.h"
#include"Object.h"
#include"ObjectMng.h"
#include"Global.h"
#include<time.h>
#include"Asedata.h"

#include"Trie.h"


CObjectMng::CObjectMng(IDirect3DDevice9*const Device)
	:m_rpDevice(Device)
{
}

CObjectMng::~CObjectMng()
{
	for (auto ptr : m_listObj)
	{
		SAFE_DELETE(ptr);
	}
}


bool CObjectMng::Init()
{
	CreateObject("ASEFile/Dragon2.ASE",&g_IDMATRIX);

	CreateObject("ASEFile/03IK-Joe.ASE", &g_IDMATRIX);
	//CreateObject("ASEFile/P38.ASE", &g_IDMATRIX);
	//CreateObject("ASEFile/MaterialBox.ASE", &g_IDMATRIX);
	//CreateObject("ASEFile/TigerTank.ASE", &g_IDMATRIX);
	//CreateObject("ASEFile/teapot.ASE", &g_IDMATRIX);
	//CreateObject("ASEFile/dynamic_walk.ASE", &g_IDMATRIX);
	//CreateObject("ASEFile/FlyingSpaceFighter06.ASE", &g_IDMATRIX);

	return true;
}

bool CObjectMng::CreateObject(LPCSTR FileName,const D3DXMATRIX *World)
{
	CObject* NewObj = new CObject();

	NewObj->Init(FileName);
	NewObj->SetTransform(World);
	
	if (NewObj) 
	{	
		m_listObj.push_back(NewObj);
		return true;
	}
	else return false;
}


void CObjectMng::DrawObject() 
{
	for (auto ptr : m_listObj) 
	{	
		ptr->Draw();
	}
}





