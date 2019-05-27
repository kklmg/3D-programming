#include"stdafx.h"
#include"ObjCom.h"
#include"Asedata.h"



//Line Object
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

OBJ::CObjAni* ASEData::STObject::CreateAni(STScene* Data)
{
	return	m_AniData.CreateAni(Data);
}


//Geom Object
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



//Position Track
//---------------------------------------------------------------------------
ASEData::STTrackPos::STTrackPos():dwTick(0)
{
}

D3DXMATRIX* ASEData::STTrackPos::CreateMtx(D3DXMATRIX& Receive)
{
	return D3DXMatrixTranslation(&Receive, vt3Pos.x, vt3Pos.y, vt3Pos.z);
}



//Rotation Track
//----------------------------------------------------------------------------
ASEData::STTrackRot::STTrackRot() :dwTick(0)
{

}

D3DXMATRIX* ASEData::STTrackRot::CreateMtx(D3DXMATRIX& Receive)
{
	return D3DXMatrixRotationAxis(&Receive, &vt3RotAxis, fRotAng);
}
D3DXQUATERNION* ASEData::STTrackRot::CreateQuaternion(D3DXQUATERNION& Receive)
{
	return D3DXQuaternionRotationAxis(&Receive,&vt3RotAxis,fRotAng);
}


//Scale Track
//------------------------------------------------------------------------------
ASEData::STTrackScale::STTrackScale() :dwTick(0)
{
}

D3DXMATRIX* ASEData::STTrackScale::CreateMtx(D3DXMATRIX& Receive)
{

	D3DXMATRIX ScaleAxis;
	D3DXMATRIX ScaleAxis_inv;
	D3DXMatrixRotationAxis(&ScaleAxis, &vt3ScaleAxis,fAngle);
	D3DXMatrixInverse(&ScaleAxis_inv, nullptr, &ScaleAxis);
	
	return D3DXMatrixTranslation(&Receive, vt3Scale.x, vt3Scale.y, vt3Scale.z);
}

D3DXQUATERNION* ASEData::STTrackScale::CreateQuaternion(D3DXQUATERNION& Receive)
{
	return D3DXQuaternionRotationAxis(&Receive, &vt3ScaleAxis, fAngle);
}


ASEData::CAniData::CAniData() 
{


}
ASEData::CAniData::~CAniData() 
{
	for (auto ptr : m_listPos) 	
		SAFE_DELETE(ptr);
	for (auto ptr : m_listRot)
		SAFE_DELETE(ptr);
	for (auto ptr : m_listScale)
		SAFE_DELETE(ptr);
}


void ASEData::CAniData::Parse(CMYLexer *lexer)
{
	lexer->MovetoBlockStart();

	std::unordered_set<DWORD> Memo;
	Memo.insert(TOKENR_CONTROL_POS_TRACK);
	Memo.insert(TOKENR_CONTROL_ROT_TRACK);
	Memo.insert(TOKENR_CONTROL_SCALE_TRACK);

	while (lexer->FindToken_UseTable_Until(TOKEND_BLOCK_END, Memo))
	{
		switch (CMYLexer::s_CurToken_Dw)
		{
		case TOKENR_CONTROL_POS_TRACK:  __ParsePos(lexer); break;
		case TOKENR_CONTROL_ROT_TRACK:  __ParseRot(lexer); break;
		case TOKENR_CONTROL_SCALE_TRACK:  __ParseScale(lexer); break;
		}
	}
};


OBJ::CObjAni*  ASEData::CAniData::CreateAni(STScene* Data)
{
	WORD Numpos = m_listPos.size();
	WORD NumRot = m_listRot.size();
	WORD NumScale = m_listScale.size();

	WORD size = (Numpos | NumRot | NumScale);

	//case: no Animation Data
	if(!size) return nullptr;
	
	OBJ::CObjAni *NewAni = new OBJ::CObjAni();
	int i;

	//create position track 
	//-----------------------------------------------------
	NewAni->m_vecTrack.resize(size);

	//mark usage
	NewAni->m_bPosTrack = Numpos;
	NewAni->m_bRotTrack = NumRot;
	NewAni->m_bScaleTrack = NumScale;

	
	//------------------------------------------------------------
	auto IterPos = m_listPos.begin();
	auto IterRot = m_listRot.begin();
	auto IterSacle = m_listScale.begin();

	D3DXVECTOR3 tempvt3;
	D3DXQUATERNION tempquat;
	double keyTime;

	//position Track
	for (i = 0; IterPos != m_listPos.end(); ++IterPos, ++i)
	{
		//get key time
		keyTime = (double)(*IterPos)->dwTick / Data->GetTPS();

		//Get Translate
		tempvt3 = (*IterPos)->vt3Pos;

		//setting
		NewAni->NewPos(i, keyTime, tempvt3);
	}
	NewAni->ComputeAbsPos();

	//Rotation track 
	//-----------------------------------------------------
	for (i = 0; IterRot != m_listRot.end(); ++IterRot, ++i)
	{
		//get key time
		keyTime = (double)(*IterRot)->dwTick / Data->GetTPS();

		//Get quaternion
		(*IterRot)->CreateQuaternion(tempquat);

		//setting
		NewAni->NewRot(i, keyTime, tempquat);
	}
	NewAni->ComputeAbsRot();


	// Scale track 
	//-----------------------------------------------------	
	for (i = 0; IterSacle != m_listScale.end(); ++IterSacle, ++i)
	{
		//get key time
		keyTime = (double)(*IterSacle)->dwTick / Data->GetTPS();

		//Get Scale
		tempvt3 = (*IterSacle)->vt3Scale;

		//Get quaternion
		(*IterSacle)->CreateQuaternion(tempquat);
		
		//setting
		NewAni->NewScale(i, keyTime,tempvt3,tempquat);
	}

	NewAni->m_dLastTime = (float)Data->m_wLastFrame / Data->m_wFrameSpeed;
	

	return NewAni;
}
	
void ASEData::CAniData::__ParsePos(CMYLexer *lexer) 
{
	lexer->MovetoBlockStart();

	while (lexer->FindToken_Until(TOKENR_CONTROL_POS_SAMPLE, TOKEND_BLOCK_END))
	{
		STTrackPos *NewTrack = new STTrackPos;

		//read Frame
		lexer->GetDWORD(NewTrack->dwTick);

		//read x 
		lexer->GetFloat(NewTrack->vt3Pos.x);

		//read z
		lexer->GetFloat(NewTrack->vt3Pos.z);

		//read y
		lexer->GetFloat(NewTrack->vt3Pos.y);

		//Save New Track
		m_listPos.push_back(NewTrack);
	}
}
void ASEData::CAniData::__ParseRot(CMYLexer *lexer) 
{
	lexer->MovetoBlockStart();

	while (lexer->FindToken_Until(TOKENR_CONTROL_ROT_SAMPLE, TOKEND_BLOCK_END))
	{
		STTrackRot *NewTrack = new STTrackRot;

		//read Frame
		lexer->GetDWORD(NewTrack->dwTick);

		//read x 
		lexer->GetFloat(NewTrack->vt3RotAxis.x);

		//read z
		lexer->GetFloat(NewTrack->vt3RotAxis.z);

		//read y
		lexer->GetFloat(NewTrack->vt3RotAxis.y);

		//read rot angle
		lexer->GetFloat(NewTrack->fRotAng);
		
		//Save New Track
		m_listRot.push_back(NewTrack);
	}
}
void ASEData::CAniData::__ParseScale(CMYLexer *lexer) 
{
	lexer->MovetoBlockStart();

	while (lexer->FindToken_Until(TOKENR_CONTROL_SCALE_SAMPLE, TOKEND_BLOCK_END))
	{
		STTrackScale *NewTrack = new STTrackScale;

		//read Frame
		lexer->GetDWORD(NewTrack->dwTick);

		//read scale x 
		lexer->GetFloat(NewTrack->vt3Scale.x);

		//read scale z
		lexer->GetFloat(NewTrack->vt3Scale.z);

		//read scale y
		lexer->GetFloat(NewTrack->vt3Scale.y);


		//read scaleAxis x 
		lexer->GetFloat(NewTrack->vt3ScaleAxis.x);

		//read scaleAxis z
		lexer->GetFloat(NewTrack->vt3ScaleAxis.z);

		//read scaleAxis y
		lexer->GetFloat(NewTrack->vt3ScaleAxis.y);

		//read rot angle
		lexer->GetFloat(NewTrack->fAngle);

		//Save New Track
		m_listScale.push_back(NewTrack);
	}
}

