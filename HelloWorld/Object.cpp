#include"stdafx.h"
#include"Global.h"
#include"ObjCom.h"
#include"Asedata.h"
#include"Object.h"
#include<time.h>


CObject::CObject():m_mtxWorld(g_IDMATRIX)
{
}

CObject::~CObject()
{
	for (auto Pair : m_mapObjs)
	{
		SAFE_DELETE(Pair.second);
	}
	for (auto ptrmat : m_vecMat)
	{
		SAFE_DELETE(ptrmat);
	}
}

bool CObject::Init(std::string str)
{
	//check file type
	std::string postfix;
	WORD index = str.find('.');
	postfix.append(str, index + 1, str.size());

	if (postfix == "ase" || postfix == "ASE")
	{
		ASEData::CASEData data;

		data.ParsingAll(str.data());

		Init(data);
		return true;
	}
	else
		return false;
}

bool CObject::Init(ASEData::CASEData& asedata)
{
	//Material Setting
	asedata.CreateD3DMat(m_vecMat, "ASEFile");

	OBJ::CObjNode* newObj;
	
	//Create Geom object
	//----------------------------------------------------------
	
	for (auto ptr : asedata.m_vecGemoObj)
	{
		newObj = ptr->CreateGemoObj();

		//set root
		if (asedata.m_InheritData.IsRoot(newObj->m_strNodeName))
		{
			m_vecRoot.push_back(newObj);
		}

		//insert data
		m_mapObjs[newObj->m_strNodeName] = newObj;
	}

	
	//find and set children
	std::list<std::string>*Childlist;


	for (auto Iter : m_mapObjs)
	{
		//get children list
		if (asedata.m_InheritData.FindChildren(Iter.first, &Childlist)) 
		{
			for (auto ListIter : *Childlist) 
			{
				Iter.second->SetChildren(m_mapObjs.find(ListIter)->second);	
			}
		}
	}

	m_vecRoot[0]->InitLocal();


	return true;
}

void CObject::SetTransform(D3DXMATRIX* TM) 
{
	if(TM)
	m_mtxWorld = *TM;

}


bool CObject::Draw()
{
	for (auto ptr : m_vecRoot)
	{
		ptr->DrawAll(m_vecMat,&m_mtxWorld);
	}

	return true;
}

