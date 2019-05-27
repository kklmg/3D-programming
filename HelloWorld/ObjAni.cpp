#include"stdafx.h"
#include"ObjCom.h"
#include"Global.h"


OBJ::STAniTrack::STAniTrack() :
	dKeyTime(0),
	pVt3Pos(nullptr),
	pQuatRot(nullptr),
	pVt3Scale(nullptr),
	pQuatScaleRot(nullptr)
{
}

OBJ::STAniTrack::~STAniTrack()
{
	SAFE_DELETE(pVt3Pos);
	SAFE_DELETE(pQuatRot);
	SAFE_DELETE(pVt3Scale);
	SAFE_DELETE(pQuatScaleRot);
}



OBJ::CObjAni::CObjAni():
	m_wCurKey(0),
	m_wMaxKey(0),
	m_bPosTrack(false),
	m_bRotTrack(false),
	m_bScaleTrack(false)
{
}

OBJ::CObjAni::~CObjAni() 
{
	for (auto ptr : m_vecTrack) 
	{
		SAFE_DELETE(ptr);
	}
}


void OBJ::CObjAni::Update() 
{
	if (m_Timer.Timing(m_dLastTime, true)) 
	{
		m_wCurKey = 0;
	}
	while (m_wCurKey < m_vecTrack.size()-1
		&&	m_Timer.GetElapse() > m_vecTrack[m_wCurKey+1]->dKeyTime)
	{
		++m_wCurKey;	
	}
}


D3DXMATRIX OBJ::CObjAni::GetAniMatrix(D3DXMATRIX LocalTM)
{
	D3DXVECTOR3 Trans;
	D3DXQUATERNION quat;
	D3DXVECTOR3 Scale;
	

	D3DXMATRIX TempPos, TempRotate, TempScale;

	//Decompose Local Matrix
	//--------------------------------------------------------------------
	D3DXMatrixDecompose(&Scale, &quat, &Trans,&LocalTM);

	//Get Transform Matrix
	D3DXMatrixTranslation(&TempPos, Trans.x, Trans.y, Trans.z);
	D3DXMatrixRotationQuaternion(&TempRotate, &quat);
	D3DXMatrixScaling(&TempScale, Scale.x, Scale.y, Scale.z);

	//animation lerp
	__Lerp(TempPos, TempRotate, TempScale);


	return TempScale *TempRotate* TempPos;
}

bool OBJ::CObjAni::__Lerp(D3DXMATRIX &__pos, D3DXMATRIX&__rot, D3DXMATRIX&__scale)
{
	Update();
	
	float Rate = 1;

	if (m_wCurKey >= m_vecTrack.size() - 1) 
	{
		m_wCurKey = m_vecTrack.size() - 2;
	}
	else
	{
		//compute Rate
		Rate = (m_Timer.GetElapse() - m_vecTrack[m_wCurKey]->dKeyTime)
			/ (m_vecTrack[m_wCurKey + 1]->dKeyTime - m_vecTrack[m_wCurKey]->dKeyTime);
	}
	

	//Translate Lerp
	//---------------------------------------------------------------
	if (m_bPosTrack)
	{
		D3DXVECTOR3 LerpedPos;

		//lerp 
		D3DXVec3Lerp(&LerpedPos, m_vecTrack[m_wCurKey]->pVt3Pos, m_vecTrack[m_wCurKey + 1]->pVt3Pos, 1);

		//get final matrix
		D3DXMatrixTranslation(&__pos, LerpedPos.x, LerpedPos.y, LerpedPos.z);
	}

	//Rotation Lerp
	//---------------------------------------------------------------
	if (m_bRotTrack)
	{
		D3DXQUATERNION lerpedQuat;

		//lerp 
		D3DXQuaternionSlerp(&lerpedQuat, m_vecTrack[m_wCurKey]->pQuatRot, m_vecTrack[m_wCurKey + 1]->pQuatRot, Rate);

		//get final matrix
		D3DXMatrixRotationQuaternion(&__rot, &lerpedQuat);//get rot tm 
	}

	//Scale Lerp
	//---------------------------------------------------------------
	if (m_bScaleTrack)
	{
		D3DXQUATERNION lerpedAxis;
		D3DXVECTOR3 lerpedScale;

		D3DXMATRIX rotate, rotate_inv;
		D3DXMATRIX scale;
		//lerp 
		D3DXQuaternionSlerp(&lerpedAxis, m_vecTrack[m_wCurKey]->pQuatScaleRot, m_vecTrack[m_wCurKey + 1]->pQuatScaleRot, Rate);

		D3DXVec3Lerp(&lerpedScale, m_vecTrack[m_wCurKey]->pVt3Scale, m_vecTrack[m_wCurKey + 1]->pVt3Scale, Rate);

		//get final matrix
		D3DXMatrixRotationQuaternion(&rotate, &lerpedAxis);//get rotate 
		D3DXMatrixInverse(&rotate_inv, nullptr, &rotate);
		D3DXMatrixScaling(&scale, lerpedScale.x, lerpedScale.y, lerpedScale.z);

		__scale = rotate_inv*scale*rotate;
	}
	return true;
}



bool OBJ::CObjAni::NewPos(WORD key,double time,D3DXVECTOR3 pos)
{
	if (key > m_vecTrack.size())return false;

	if (!m_vecTrack[key])m_vecTrack[key] = new STAniTrack();

	//get key Time
	m_vecTrack[key]->dKeyTime = time;

	//setting
	if (m_vecTrack[key]->pVt3Pos) SAFE_DELETE(m_vecTrack[key]->pVt3Pos);
	m_vecTrack[key]->pVt3Pos = new D3DXVECTOR3(pos);
}
bool OBJ::CObjAni::NewRot(WORD key, double time, D3DXQUATERNION rot)
{
	if (key > m_vecTrack.size())return false;

	if (!m_vecTrack[key])m_vecTrack[key] = new STAniTrack();

	//get key Time
	m_vecTrack[key]->dKeyTime = time;


	//setting
	if (m_vecTrack[key]->pQuatRot) SAFE_DELETE(m_vecTrack[key]->pQuatRot);
	m_vecTrack[key]->pQuatRot = new D3DXQUATERNION(rot);

}
bool OBJ::CObjAni::NewScale(WORD key, double time, D3DXVECTOR3 scale, D3DXQUATERNION rot) 
{
	if (key > m_vecTrack.size())return false;
	if (!m_vecTrack[key])m_vecTrack[key] = new STAniTrack();

	//get key Time
	m_vecTrack[key]->dKeyTime = time;

	
	//Setting
	if (m_vecTrack[key]->pVt3Scale) SAFE_DELETE(m_vecTrack[key]->pVt3Scale);
	m_vecTrack[key]->pVt3Scale = new D3DXVECTOR3(scale);

	if (m_vecTrack[key]->pQuatScaleRot) SAFE_DELETE(m_vecTrack[key]->pQuatScaleRot);
	m_vecTrack[key]->pQuatScaleRot = new D3DXQUATERNION(rot);

}

void OBJ::CObjAni::ComputeAbsPos() 
{ 
	if (!m_bPosTrack) return;
	int size = m_vecTrack.size();
	int i = 1;
	for (; i < size; ++i)
	{
		//get absolute transform
		*m_vecTrack[i]->pVt3Pos -= *m_vecTrack[0]->pVt3Pos;
	}

}

void OBJ::CObjAni::ComputeAbsRot()
{
	if (!m_bRotTrack)return;

	int size = m_vecTrack.size() - 1;
	int i = 0;
	for (; i < size; ++i)
	{
		//get absolute transform
		D3DXQuaternionMultiply(m_vecTrack[i + 1]->pQuatRot, 
			m_vecTrack[i]->pQuatRot, m_vecTrack[i + 1]->pQuatRot);
	}
}
