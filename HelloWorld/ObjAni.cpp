#include"stdafx.h"
#include"ObjCom.h"
#include"Global.h"


OBJ::CAniTrack::CAniTrack() :
	m_dLastTime(0), 
	m_wKeyPos(1),
	m_wKeyRot(1),
	m_wKeyScale(1)
{
}

OBJ::CAniTrack::~CAniTrack()
{
}


void OBJ::CAniTrack::Update()
{
	if (m_Timer.Timing(m_dLastTime, true))
	{
		m_wKeyPos = 1;
		m_wKeyRot = 1;
		m_wKeyScale = 1;
	}
	double Elapse = m_Timer.GetElapse();

	//compute current keyframe
	while (m_wKeyPos< m_vecPosTrack.size()  
		&&Elapse > m_vecPosTrack[m_wKeyPos].dKeyTime)
	{
		++m_wKeyPos;
	}

	//compute current keyframe
	while (m_wKeyRot< m_vecRotTrack.size()
		&& Elapse > m_vecRotTrack[m_wKeyRot].dKeyTime)
	{
		++m_wKeyRot;
	}

	//compute current keyframe
	while (m_wKeyScale< m_vecScaleTrack.size()
		&& Elapse > m_vecScaleTrack[m_wKeyScale].dKeyTime)
	{
		++m_wKeyScale;
	}
}





bool OBJ::CAniTrack::__Lerp(D3DXMATRIX &__pos, D3DXMATRIX&__rot, D3DXMATRIX&__scale)
{
	Update();

	float TransRate = 1;
	float RotRate = 1;
	float ScaleRate = 1;

	//Translate Lerp
	//---------------------------------------------------------------
	if (m_vecPosTrack.size()>1)
	{
		if (m_wKeyPos > m_vecPosTrack.size() - 1)
		{
			m_wKeyPos = m_vecPosTrack.size() - 1;
		}
		else
		{
			//compute Rate
			TransRate = (m_Timer.GetElapse() - m_vecPosTrack[m_wKeyPos-1].dKeyTime)
				/ (m_vecPosTrack[m_wKeyPos].dKeyTime - m_vecPosTrack[m_wKeyPos-1].dKeyTime);
		}


		D3DXVECTOR3 LerpedPos;

		//lerp 
		D3DXVec3Lerp(&LerpedPos, &m_vecPosTrack[m_wKeyPos-1].Vt3Pos, &m_vecPosTrack[m_wKeyPos].Vt3Pos, TransRate);

		//get final matrix
		D3DXMatrixTranslation(&__pos, LerpedPos.x, LerpedPos.y, LerpedPos.z);
	}

	//Rotation Lerp
	//---------------------------------------------------------------
	if (m_vecRotTrack.size()>1)
	{
		if (m_wKeyRot > m_vecRotTrack.size() - 1)
		{
			m_wKeyRot = m_vecRotTrack.size() - 1;
		}
		else
		{
			//compute Rate
			RotRate = (m_Timer.GetElapse() - m_vecRotTrack[m_wKeyRot - 1].dKeyTime)
				/ (m_vecRotTrack[m_wKeyRot].dKeyTime - m_vecRotTrack[m_wKeyRot - 1].dKeyTime);
		}

		D3DXQUATERNION lerpedQuat;

		//lerp 
		D3DXQuaternionSlerp(&lerpedQuat, 
			&m_vecRotTrack[m_wKeyRot-1].QuatRot, &m_vecRotTrack[m_wKeyRot].QuatRot, RotRate);

		//get final matrix
		D3DXMatrixRotationQuaternion(&__rot, &lerpedQuat);//get rot tm 
	}

	//Scale Lerp
	//---------------------------------------------------------------
	if (m_vecScaleTrack.size()>1)
	{
		if (m_wKeyScale > m_vecScaleTrack.size() - 1)
		{
			m_wKeyScale = m_vecScaleTrack.size() - 1;
		}
		else
		{
			//compute Rate
			ScaleRate = (m_Timer.GetElapse() - m_vecScaleTrack[m_wKeyScale - 1].dKeyTime)
				/ (m_vecScaleTrack[m_wKeyScale].dKeyTime - m_vecScaleTrack[m_wKeyScale - 1].dKeyTime);
		}



		D3DXQUATERNION lerpedAxis;
		D3DXVECTOR3 lerpedScale;

		D3DXMATRIX rotate, rotate_inv;
		D3DXMATRIX scale;

		//lerp 
		D3DXQuaternionSlerp(&lerpedAxis, 
			&m_vecScaleTrack[m_wKeyScale-1].QuatScaleRot, &m_vecScaleTrack[m_wKeyScale].QuatScaleRot, ScaleRate);

		D3DXVec3Lerp(&lerpedScale, 
			&m_vecScaleTrack[m_wKeyScale-1].Vt3Scale, &m_vecScaleTrack[m_wKeyScale].Vt3Scale, ScaleRate);

		//get final matrix
		D3DXMatrixRotationQuaternion(&rotate, &lerpedAxis);//get rotate 
		D3DXMatrixInverse(&rotate_inv, nullptr, &rotate);
		D3DXMatrixScaling(&scale, lerpedScale.x, lerpedScale.y, lerpedScale.z);

		__scale = rotate_inv*scale*rotate;
	}
	return true;
}



void OBJ::CAniTrack::NewPos(double time, D3DXVECTOR3& pos)
{
	m_vecPosTrack.push_back(STPosTrack(time,pos));
}
void OBJ::CAniTrack::NewRot(double time, D3DXQUATERNION& quatrot)
{
	m_vecRotTrack.push_back(STRotTrack(time, quatrot));
}
void OBJ::CAniTrack::NewScale(double time, D3DXVECTOR3& scale, D3DXQUATERNION& quatrot)
{
	m_vecScaleTrack.push_back(STScaleTrack(time, quatrot, scale));
}


void OBJ::CAniTrack::ComputeAbsPos()
{
	//for (int i = 1; i < m_vecPosTrack.size(); ++i) 
	//{
	//	m_vecPosTrack[i].Vt3Pos -= m_vecPosTrack[0].Vt3Pos;
	//}
}

void OBJ::CAniTrack::ComputeAbsRot()
{
	for (int i = 1; i < m_vecRotTrack.size(); ++i)
	{
		//get absolute transform
		D3DXQuaternionMultiply(&m_vecRotTrack[i].QuatRot,
			&m_vecRotTrack[i-1].QuatRot, &m_vecRotTrack[i].QuatRot);
	}
}

D3DXMATRIX OBJ::CAniTrack::GetAniMatrix(D3DXMATRIX LocalTM)
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

