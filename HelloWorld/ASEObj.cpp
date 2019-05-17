#include"stdafx.h"
#include"Asedata.h"




ASEData::STLine::STLine()
	:
	m_bClosed(false),
	m_dwVertexCount(0),
	m_pLineVertex(nullptr)
{
}

ASEData::STLine::~STLine()
{
}

ASEData::STNodeTM::STNodeTM()
	:
	m_fTM_ROTANGLE(0),
	m_fTM_SCALEAXISANG(0)
{
}

ASEData::STNodeTM::~STNodeTM()
{
}


bool ASEData::STNodeTM::parse(CMYLexer *lexer)
{
	lexer->MovetoBlockStart();

	//Inherit Data
	//---------------------------------------------------
	lexer->FindToken(TOKENR_INHERIT_POS);
	lexer->GetFloat(m_vt3INHERIT_POS.x);
	lexer->GetFloat(m_vt3INHERIT_POS.z);
	lexer->GetFloat(m_vt3INHERIT_POS.y);

	lexer->FindToken(TOKENR_INHERIT_ROT);
	lexer->GetFloat(m_vt3INHERIT_ROT.x);
	lexer->GetFloat(m_vt3INHERIT_ROT.z);
	lexer->GetFloat(m_vt3INHERIT_ROT.y);

	lexer->FindToken(TOKENR_INHERIT_SCL);
	lexer->GetFloat(m_vt3INHERIT_SCL.x);
	lexer->GetFloat(m_vt3INHERIT_SCL.z);
	lexer->GetFloat(m_vt3INHERIT_SCL.y);


	//Matrix Data
	//---------------------------------------------------
	lexer->FindToken(TOKENR_TM_ROW0);
	lexer->GetFloat(m_vt3TM_ROW0.x);
	lexer->GetFloat(m_vt3TM_ROW0.z);
	lexer->GetFloat(m_vt3TM_ROW0.y);

	lexer->FindToken(TOKENR_TM_ROW1);
	lexer->GetFloat(m_vt3TM_ROW1.x);
	lexer->GetFloat(m_vt3TM_ROW1.z);
	lexer->GetFloat(m_vt3TM_ROW1.y);

	lexer->FindToken(TOKENR_TM_ROW2);
	lexer->GetFloat(m_vt3TM_ROW2.x);
	lexer->GetFloat(m_vt3TM_ROW2.z);
	lexer->GetFloat(m_vt3TM_ROW2.y);

	lexer->FindToken(TOKENR_TM_ROW3);
	lexer->GetFloat(m_vt3TM_ROW3.x);
	lexer->GetFloat(m_vt3TM_ROW3.z);
	lexer->GetFloat(m_vt3TM_ROW3.y);


	//Position Data
	//---------------------------------------------------
	lexer->FindToken(TOKENR_TM_POS);
	lexer->GetFloat(m_vt3TM_POS.x);
	lexer->GetFloat(m_vt3TM_POS.z);
	lexer->GetFloat(m_vt3TM_POS.y);


	//Rotation Data
	//---------------------------------------------------
	lexer->FindToken(TOKENR_TM_ROTAXIS);
	lexer->GetFloat(m_vt3TM_ROTAXIS.x);
	lexer->GetFloat(m_vt3TM_ROTAXIS.z);
	lexer->GetFloat(m_vt3TM_ROTAXIS.y);

	lexer->FindToken(TOKENR_TM_ROTANGLE);
	lexer->GetFloat(m_fTM_ROTANGLE);

	//Scale Data
	//---------------------------------------------------
	lexer->FindToken(TOKENR_TM_SCALE);
	lexer->GetFloat(m_vt3TM_SCALE.x);
	lexer->GetFloat(m_vt3TM_SCALE.z);
	lexer->GetFloat(m_vt3TM_SCALE.y);

	lexer->FindToken(TOKENR_TM_SCALEAXIS);
	lexer->GetFloat(m_vt3TM_SCALEAXIS.x);
	lexer->GetFloat(m_vt3TM_SCALEAXIS.z);
	lexer->GetFloat(m_vt3TM_SCALEAXIS.y);



	lexer->FindToken(TOKENR_TM_SCALEAXISANG);
	lexer->GetFloat(m_fTM_SCALEAXISANG);

	lexer->MovetoBlockEnd();

	return TRUE;
}

bool ASEData::STNodeTM::GetWorldTM(D3DXMATRIX &matrix)
{
	matrix._11 = m_vt3TM_ROW0.x;
	matrix._12 = m_vt3TM_ROW0.y;
	matrix._13 = m_vt3TM_ROW0.z;
	matrix._14 = 0;

	matrix._21 = m_vt3TM_ROW1.x;
	matrix._22 = m_vt3TM_ROW1.y;
	matrix._23 = m_vt3TM_ROW1.z;
	matrix._24 = 0;

	matrix._31 = m_vt3TM_ROW2.x;
	matrix._32 = m_vt3TM_ROW2.y;
	matrix._33 = m_vt3TM_ROW2.z;
	matrix._34 = 0;

	matrix._41 = m_vt3TM_ROW3.x;
	matrix._42 = m_vt3TM_ROW3.y;
	matrix._43 = m_vt3TM_ROW3.z;
	matrix._44 = 0;


	SWAP(matrix._21, matrix._31);
	SWAP(matrix._22, matrix._32);
	SWAP(matrix._23, matrix._33);
	//SWAP(matrix._24, matrix._34);


	return true;
}

bool ASEData::STNodeTM::GetWorldTMEx(D3DXMATRIX &_World, D3DXMATRIX& _Trans, D3DXMATRIX&_Rotate, D3DXMATRIX& _Scale)
{
	//Create Translate Matrix
	D3DXMatrixTranslation(&_Trans, m_vt3TM_POS.x, m_vt3TM_POS.y, m_vt3TM_POS.z);

	//Create Rotation Matrix
	D3DXMatrixRotationAxis(&_Rotate, &m_vt3TM_ROTAXIS,m_fTM_ROTANGLE);
	//Create Scale Matrix
	D3DXMATRIX Scale;
	D3DXMatrixScaling(&Scale, m_vt3TM_SCALE.x, m_vt3TM_SCALE.y, m_vt3TM_SCALE.z);

	D3DXMATRIX ScaleAxis;
	D3DXMATRIX ScaleAxis_inv;
	D3DXMatrixRotationAxis(&ScaleAxis, &m_vt3TM_SCALEAXIS, m_fTM_SCALEAXISANG);
	D3DXMatrixInverse(&ScaleAxis_inv, nullptr, &ScaleAxis);

	//Scale Combine
	_Scale = ScaleAxis_inv * Scale* ScaleAxis;
	
	//combine Matrix
	_World = _Scale * _Rotate * _Trans;

	return true;
}




void ASEData::CInheritData::InsertData(std::string &parent, std::string &child)
{
	if (!parent.size())
	{
		m_SetRoot.insert(child);
		return;
	}
	//create Inherit Table
	auto Iter = m_mapInherit.find(parent);

	//not found
	if (Iter == m_mapInherit.end())
	{
		m_mapInherit[parent] = std::list<std::string>();
		m_mapInherit[parent].push_back(child);
	}
	else
		m_mapInherit[parent].push_back(child);



}

bool ASEData::CInheritData::IsRoot(std::string NodeName) 
{
	return m_SetRoot.find(NodeName) != m_SetRoot.end();
}

bool ASEData::CInheritData::FindChildren(const std::string &Parent, std::list<std::string>**Receive) 
{
	auto Iter = m_mapInherit.find(Parent);

	if (Iter != m_mapInherit.end())
	{
		*Receive = &Iter->second;
		return true;
	}
	else
		return false;
}














//Geom Object
ASEData::STGEOMObject::STGEOMObject()
	:
	m_wMatRef(USHRT_MAX)
{
}



OBJ::CGemoObj* ASEData::STGEOMObject::CreateGemoObj()
{
	OBJ::CGemoObj* newobj = new OBJ::CGemoObj;

	D3DXMATRIX WorldTM_inv;

	//Get local tranform matrix
	//m_NodeTM.GetLocalTM(LocalTM);
	m_NodeTM.GetWorldTMEx(newobj->m_mtxAseWorld, newobj->m_mtxTranslate, newobj->m_mtxRotate, newobj->m_mtxScale);



	D3DXMatrixInverse(&WorldTM_inv, nullptr, &newobj->m_mtxAseWorld);


	//Set Name
	newobj->m_strNodeName = m_strNodeName;
	newobj->m_strParentName = m_strNodeParent;

	

	//Create Mesh
	newobj->SetMesh(m_Mesh.CreateMesh(&WorldTM_inv));
	newobj->SetMatID(m_wMatRef);

	return newobj;
}










//ASEData::
//struct STGEOMObject :public STObject
//{
//	STGEOMObject()
//		:
//		m_wMatRef(USHRT_MAX)
//	{
//	}
//
//	CMesh m_Mesh;
//	WORD  m_wMatRef;
//
//	OBJ::CGemoObj* CreateGemoObj()
//	{
//		OBJ::CGemoObj* newobj = new OBJ::CGemoObj;
//
//		newobj->SetMesh(m_Mesh.CreateMesh());
//		newobj->SetMatID(m_wMatRef);
//
//		return newobj;
//	}
//};
//
//struct STShapeObject :public STObject
//{
//public:
//	//STShapeObject();
//	//~STShapeObject();
//
//	int LineCount;
//	//	CLineData *Line;
//};
//
//struct STHelperObject
//{
//public:
//	//STHelperObject();
//	//~STHelperObject();
//};
