#include"stdafx.h"
#include"Asedata.h"

OBJ::CAniTrack* ASEData::STObject::CreateAni(STScene* Data)
{
	return	m_AniData.CreateAni(Data);
}

//Position Track
//---------------------------------------------------------------------------
ASEData::STTrackPos::STTrackPos() :dwTick(0)
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
	return D3DXQuaternionRotationAxis(&Receive, &vt3RotAxis, fRotAng);
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
	D3DXMatrixRotationAxis(&ScaleAxis, &vt3ScaleAxis, fAngle);
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


OBJ::CAniTrack*  ASEData::CAniData::CreateAni(STScene* Data)
{

	if ((!m_listPos.size()) && (!m_listRot.size()) && (!m_listScale.size())) return nullptr;

	OBJ::CAniTrack *NewAni = new OBJ::CAniTrack();


	//------------------------------------------------------------
	auto IterPos = m_listPos.begin();
	auto IterRot = m_listRot.begin();
	auto IterSacle = m_listScale.begin();

	D3DXVECTOR3 tempvt3;
	D3DXQUATERNION tempquat;
	double keyTime;

	//position Track
	for (; IterPos != m_listPos.end(); ++IterPos)
	{
		//get key time
		keyTime = (double)(*IterPos)->dwTick / Data->GetTPS();

		//Get Translate
		tempvt3 = (*IterPos)->vt3Pos;

		//setting
		NewAni->NewPos(keyTime, tempvt3);
	}
	NewAni->ComputeAbsPos();

	//Rotation track 
	//-----------------------------------------------------
	for (; IterRot != m_listRot.end(); ++IterRot)
	{
		//get key time
		keyTime = (double)(*IterRot)->dwTick / Data->GetTPS();

		//Get quaternion
		(*IterRot)->CreateQuaternion(tempquat);

		//setting
		NewAni->NewRot(keyTime, tempquat);
	}
	NewAni->ComputeAbsRot();


	// Scale track 
	//-----------------------------------------------------	
	for (; IterSacle != m_listScale.end(); ++IterSacle)
	{
		//get key time
		keyTime = (double)(*IterSacle)->dwTick / Data->GetTPS();

		//Get Scale
		tempvt3 = (*IterSacle)->vt3Scale;

		//Get quaternion
		(*IterSacle)->CreateQuaternion(tempquat);

		//setting
		NewAni->NewScale(keyTime, tempvt3, tempquat);
	}

	NewAni->m_dLastTime = Data->GetEndTime();


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

