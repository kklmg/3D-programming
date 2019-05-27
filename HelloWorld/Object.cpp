#include"stdafx.h"
#include"Global.h"
#include"Timer.h"
#include"ObjCom.h"
#include"Asedata.h"
#include"Object.h"



CObject::CObject()
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
		//create mesh
		newObj = ptr->CreateGemoObj();

		//create animation
		newObj->SetAni(ptr->CreateAni(asedata.GetSceneData()));
		
		//set root
		if (asedata.m_InheritData.IsRoot(newObj->GetNodeName()))
		{
			m_vecRoot.push_back(newObj);
		}

		//insert data
		m_mapObjs[newObj->GetNodeName()] = newObj;
	}

	
	//find and set children
	//---------------------------------------------------------
	std::list<std::string>*Childlist;
	OBJ::CObjNode* child;

	for (auto Iter : m_mapObjs)
	{
		//get children list
		if (asedata.m_InheritData.FindChildren(Iter.first, &Childlist)) 
		{
			for (auto ListIter : *Childlist) 
			{
				child = m_mapObjs.find(ListIter)->second;

				//set child
				Iter.second->SetChildren(child);

				//set parent
				child->SetParent(Iter.second);

			}
		}
	}

	//Init Local Matrix
	for (auto ptr : m_vecRoot) 
	{
		ptr->InitLocal();
	}
	
	return true;
}

void CObject::SetTransform(D3DXMATRIX* TM) 
{
	if (TM)
	{
		for (auto ptr : m_vecRoot) 
		{
			ptr->SetRootTM(*TM);
		}
	}
}

bool CObject::Draw()
{
	for (auto ptr : m_vecRoot)
	{
		ptr->update();
		ptr->DrawAll(m_vecMat);
	}
	return true;
}

bool CObject::PlayAnimation(WORD AniID) 
{
	

	return true;
}

