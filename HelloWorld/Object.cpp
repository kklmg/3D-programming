#include"stdafx.h"
#include"ObjCom.h"
#include"Asedata.h"
#include"Object.h"
#include<time.h>


CObject::CObject()
{
}

CObject::~CObject()
{
	for (auto ptrgemo : m_vecGemo)
	{
		SAFE_DELETE(ptrgemo);
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


	//Mesh Setting
	//----------------------------------------------------------
	m_vecGemo.reserve(asedata.m_vecGemoObj.size());
	for (auto ptr : asedata.m_vecGemoObj)
	{
		m_vecGemo.push_back(ptr->CreateGemoObj());
	}
	return true;
}


bool CObject::Draw()
{
	for (auto ptr : m_vecGemo)
	{
		ptr->Draw(m_vecMat);
	}

	return true;
}


