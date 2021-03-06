#include"stdafx.h"
#include"MyMath.h"
#include"Global.h"

D3DXVECTOR3 SMath::Transform(D3DXVECTOR3 *vt3, const D3DXMATRIX *TM)
{
	if (!vt3)return { 1,1,1 };

	if (!TM) return *vt3;

	D3DXVECTOR4 tempvt4;
	D3DXVECTOR4 result;

	D3DXVec3Transform(&tempvt4, vt3, TM);

	result.x = tempvt4.x;
	result.y = tempvt4.y;
	result.z = tempvt4.z;

	return result;
}

//D3DXVECTOR3 Rotate(D3DXVECTOR3 *vt3, const D3DXMATRIX *TM) 
//{
//	if (!TM)return *vt3;
//
//	D3DXQUATERNION tempQ;
//	D3DXMATRIX tempM;
//	D3DXMatrixDecompose(nullptr, &tempQ, nullptr, TM);
//
//	D3DXMatrixRotationQuaternion(&tempM,&tempQ);
//
//	D3DXVec3TransformCoord(vt3,);
//	
//	return Rotate**vt3;
//
//}