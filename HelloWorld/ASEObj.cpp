#include"stdafx.h"
#include"ObjCom.h"

#include"Asedata.h"


//Node Data
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
	matrix._44 = 1;


	SWAP(matrix._21, matrix._31);
	SWAP(matrix._22, matrix._32);
	SWAP(matrix._23, matrix._33);
	//SWAP(matrix._24, matrix._34);


	return true;
}

bool ASEData::STNodeTM::GetWorldTMEx
(D3DXMATRIX &_World, D3DXMATRIX& _Trans, D3DXMATRIX&_Rotate, D3DXMATRIX& _Scale)
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



//Inherit Data
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




ASEData::STObject::STObject() 
{
}



//Geom Object
//----------------------------------------------------------------------------
ASEData::STGEOMObject::STGEOMObject() : m_wMatRef(USHRT_MAX)
{
}

void ASEData::STGEOMObject::Parse(CMYLexer *lexer)
{
	lexer->MovetoBlockStart();

	std::unordered_set<DWORD> Memo;
	Memo.insert(TOKENR_NODE_NAME);
	Memo.insert(TOKENR_NODE_PARENT);
	Memo.insert(TOKENR_MATERIAL_REF);
	Memo.insert(TOKENR_NODE_TM);
	Memo.insert(TOKENR_TM_ANIMATION);
	Memo.insert(TOKENR_MESH);
	Memo.insert(TOKEND_BLOCK_START);


	while (lexer->FindToken_UseTable_Until(TOKEND_BLOCK_END, Memo))
	{
		switch (CMYLexer::s_CurToken_Dw)
		{
		case TOKENR_NODE_NAME:		lexer->GetString(m_strNodeName); break;
		case TOKENR_NODE_PARENT:	lexer->GetString(m_strNodeParent); break;
		case TOKENR_MATERIAL_REF:	lexer->GetWORD(m_wMatRef);  break;
		case TOKENR_NODE_TM:		m_NodeTM.parse(lexer); break;
		case TOKENR_TM_ANIMATION:	m_AniData.Parse(lexer); break;
		case TOKENR_MESH:			m_Mesh.ParseAll(lexer); break;
		case TOKEND_BLOCK_START:	lexer->SkipCurBlock(); break;
		}
	}

}

OBJ::CGemoObj* ASEData::STGEOMObject::CreateGemoObj()
{
	OBJ::CGemoObj* newobj = new OBJ::CGemoObj;

	D3DXMATRIX WorldTM, WorldTM_inv;

	//Get World tranform matrix
	m_NodeTM.GetWorldTM(WorldTM);
	
	//Get Inverserd world transform Matrix
	D3DXMatrixInverse(&WorldTM_inv, nullptr, &WorldTM);

	//Set Matrix
	newobj->SetOriginalTM(WorldTM);


	//Set Name	
	newobj->SetNodeName(m_strNodeName);
	newobj->SetParNodeName(m_strNodeParent);	

	//Create Mesh
	newobj->SetMesh(m_Mesh.CreateMesh(&WorldTM_inv));
	newobj->SetMatID(m_wMatRef);

	return newobj;
}






//Shape Object
//----------------------------------------------------------------------------

ASEData::STLines::STLines() :
	bClosed(false), 
	pVertex(nullptr),
	wCount(0)
{
}

ASEData::STLines::~STLines() 
{
	SAFE_DELETE_ARRAY(pVertex);
}

void ASEData::STLines::parse(CMYLexer *lexer)
{
	lexer->MovetoBlockStart();

	std::unordered_set<DWORD> Memo;
	Memo.insert(TOKENR_SHAPE_CLOSED);
	Memo.insert(TOKENR_SHAPE_VERTEXCOUNT);
	Memo.insert(TOKENR_SHAPE_VERTEX_KNOT);
	Memo.insert(TOKENR_SHAPE_VERTEX_INTERP);

	WORD wd;
	while (lexer->FindToken_UseTable_Until(TOKEND_BLOCK_END, Memo))
	{
		switch (CMYLexer::s_CurToken_Dw)
		{
			case TOKENR_SHAPE_CLOSED:	bClosed = true;	break;
			case TOKENR_SHAPE_VERTEXCOUNT: 
			{
				lexer->GetWORD(wCount);
				pVertex = new STVTX_P[wCount];
			}break;
			case TOKENR_SHAPE_VERTEX_KNOT:
			case TOKENR_SHAPE_VERTEX_INTERP:
			{
				lexer->GetWORD(wd);
				lexer->GetFloat(pVertex[wd].Pos.x);
				lexer->GetFloat(pVertex[wd].Pos.z);
				lexer->GetFloat(pVertex[wd].Pos.y);
			}break;
		}
	}
}

void ASEData::STLines::Transform(D3DXMATRIX * TM) 
{
	if (!TM)return;

	STVTX_P* pSlot = pVertex;

	for (int i = 0; i < wCount; ++i, ++pSlot)
	{
		D3DXVec3TransformCoord(&pSlot->Pos, &pSlot->Pos, TM);
	}
}




ASEData::STShapeObject::STShapeObject() 
{
}

ASEData::STShapeObject::~STShapeObject() 
{
}

void ASEData::STShapeObject::Parse(CMYLexer *lexer) 
{
	lexer->MovetoBlockStart();

	std::unordered_set<DWORD> Memo;
	Memo.insert(TOKENR_NODE_NAME);
	Memo.insert(TOKENR_NODE_PARENT);
	Memo.insert(TOKENR_NODE_TM);
	Memo.insert(TOKENR_TM_ANIMATION);
	Memo.insert(TOKENR_SHAPE_LINECOUNT);
	Memo.insert(TOKENR_SHAPE_LINE);

	Memo.insert(TOKEND_BLOCK_START);

	WORD wd;
	while (lexer->FindToken_UseTable_Until(TOKEND_BLOCK_END, Memo))
	{
		switch (CMYLexer::s_CurToken_Dw)
		{
		case TOKENR_NODE_NAME:		lexer->GetString(m_strNodeName); break;
		case TOKENR_NODE_PARENT:	lexer->GetString(m_strNodeParent); break;		
		case TOKENR_NODE_TM:		m_NodeTM.parse(lexer); break;
		case TOKENR_TM_ANIMATION:	m_AniData.Parse(lexer); break;
		case TOKENR_SHAPE_LINECOUNT: 
		{
			lexer->GetWORD(wd);
			vecLines.resize(wd);	
		}  break;
		case TOKENR_SHAPE_LINE: 
		{
			lexer->GetWORD(wd);
			vecLines[wd].parse(lexer);
		}break;
		case TOKEND_BLOCK_START:	lexer->SkipCurBlock(); break;
		}
	}
}


OBJ::CShapeObj* ASEData::STShapeObject::CreateShapeObj() 
{
	OBJ::CShapeObj* NewShapeObj = new OBJ::CShapeObj();

	D3DXMATRIX WorldTM, WorldTM_inv;

	//Get World tranform matrix
	m_NodeTM.GetWorldTM(WorldTM);

	//Get Inverserd world transform Matrix
	D3DXMatrixInverse(&WorldTM_inv, nullptr, &WorldTM);

	//Set Matrix
	NewShapeObj->SetOriginalTM(WorldTM);


	//Set Name	
	NewShapeObj->SetNodeName(m_strNodeName);
	NewShapeObj->SetParNodeName(m_strNodeParent);


	for (int i = 0; i < vecLines.size(); ++i) 
	{
		OBJ::CLineBuffer* linebuff = new OBJ::CLineBuffer();

		//get local position
		vecLines[i].Transform(&WorldTM_inv);

		linebuff->SetBuffer(vecLines[i].pVertex,
			vecLines[i].wCount,vecLines[i].bClosed);

		NewShapeObj->AddLines(linebuff);	
	}
	return NewShapeObj;
}



ASEData::STHelperObject::STHelperObject()
{
}

ASEData::STHelperObject::~STHelperObject()
{
}

void ASEData::STHelperObject::Parse(CMYLexer *lexer)
{
	lexer->MovetoBlockStart();

	std::unordered_set<DWORD> Memo;
	Memo.insert(TOKENR_NODE_NAME);
	Memo.insert(TOKENR_NODE_PARENT);
	Memo.insert(TOKENR_NODE_TM);
	Memo.insert(TOKENR_TM_ANIMATION);
	Memo.insert(TOKENR_BOUNDINGBOX_MIN);
	Memo.insert(TOKENR_BOUNDINGBOX_MAX);


	Memo.insert(TOKEND_BLOCK_START);

	WORD wd;
	while (lexer->FindToken_UseTable_Until(TOKEND_BLOCK_END, Memo))
	{
		switch (CMYLexer::s_CurToken_Dw)
		{
		case TOKENR_NODE_NAME:			lexer->GetString(m_strNodeName); break;
		case TOKENR_NODE_PARENT:		lexer->GetString(m_strNodeParent); break;
		case TOKENR_NODE_TM:			m_NodeTM.parse(lexer); break;
		case TOKENR_TM_ANIMATION:		m_AniData.Parse(lexer); break;
		case TOKENR_BOUNDINGBOX_MIN:
		{
			lexer->GetFloat(m_vt3bound_min.x);
			lexer->GetFloat(m_vt3bound_min.z);
			lexer->GetFloat(m_vt3bound_min.y);
		} break;
		case TOKENR_BOUNDINGBOX_MAX: 
		{
			lexer->GetFloat(m_vt3bound_max.x);
			lexer->GetFloat(m_vt3bound_max.z);
			lexer->GetFloat(m_vt3bound_max.y);
		} break;
	
		case TOKEND_BLOCK_START:	lexer->SkipCurBlock(); break;
		}
	}
}

OBJ::CHelperObj* ASEData::STHelperObject::CreateHelperObj()
{
	OBJ::CHelperObj* NewHelperObj = new OBJ::CHelperObj();

	D3DXMATRIX WorldTM, WorldTM_inv;

	//Get World tranform matrix
	m_NodeTM.GetWorldTM(WorldTM);

	//Get Inverserd world transform Matrix
	D3DXMatrixInverse(&WorldTM_inv, nullptr, &WorldTM);

	//Set Matrix
	NewHelperObj->SetOriginalTM(WorldTM);


	//Set Name	
	NewHelperObj->SetNodeName(m_strNodeName);
	NewHelperObj->SetParNodeName(m_strNodeParent);

	//set bound
	NewHelperObj->SetBound_min(m_vt3bound_min);
	NewHelperObj->SetBound_min(m_vt3bound_max);


	return NewHelperObj;
}


ASEData::STCameraObj::STCameraObj()
{
}

ASEData::STCameraObj::~STCameraObj() 
{
}


void ASEData::STCameraObj::Parse(CMYLexer *lexer) 
{
	lexer->MovetoBlockStart();

	std::unordered_set<DWORD> Memo;
	Memo.insert(TOKENR_NODE_NAME);
	Memo.insert(TOKENR_NODE_PARENT);
	Memo.insert(TOKENR_NODE_TM);
	Memo.insert(TOKENR_TM_ANIMATION);
	Memo.insert(TOKENR_CAMERA_SETTINGS);
	Memo.insert(TOKENR_CAMERA_ANIMATION);
	Memo.insert(TOKEND_BLOCK_START);

	WORD wd;
	bool parsed_ntm = false, parsed_nani = false;
	DWORD tempDw; float tempf;

	while (lexer->FindToken_UseTable_Until(TOKEND_BLOCK_END, Memo))
	{
		switch (CMYLexer::s_CurToken_Dw)
		{
		case TOKENR_NODE_NAME:			lexer->GetString(m_strNodeName); break;
		case TOKENR_NODE_PARENT:		lexer->GetString(m_strNodeParent); break;
		case TOKENR_NODE_TM:
		{
			if (parsed_ntm) 
			{
				lexer->MovetoBlockStart();
				lexer->SkipCurBlock();
			}
			else			
				m_NodeTM.parse(lexer);					
		}break;
		case TOKENR_CAMERA_SETTINGS:
		{
			lexer->MovetoBlockStart();
			lexer->FindToken(TOKENR_CAMERA_FOV);
			lexer->GetFloat(m_fFOV);	
			lexer->MovetoBlockEnd();
		}
		case TOKENR_CAMERA_ANIMATION: 
		{
			lexer->MovetoBlockStart();

			while (lexer->FindToken_Until(TOKENR_CAMERA_SETTINGS, TOKEND_BLOCK_END))
			{
				lexer->MovetoBlockStart();

				//parse time
				lexer->FindToken(TOKENR_TIMEVALUE);
				lexer->GetDWORD(tempDw);
				
				//parse fov
				lexer->FindToken(TOKENR_CAMERA_FOV);
				lexer->GetFloat(tempf);

				m_listCameraAni.push_back({ tempDw,tempf });

				lexer->MovetoBlockEnd();
			}
		}
		case TOKENR_TM_ANIMATION: 
		{
			if (parsed_nani)
			{
				lexer->MovetoBlockStart();
				lexer->SkipCurBlock();
			}
			else
				m_AniData.Parse(lexer);	
		} break;
		case TOKEND_BLOCK_START:	lexer->SkipCurBlock(); break;
		}
	}

}

OBJ::CCameraObj* ASEData::STCameraObj::CreateCameraObj() 
{
	OBJ::CCameraObj* camera = new OBJ::CCameraObj;



	return camera;
}






