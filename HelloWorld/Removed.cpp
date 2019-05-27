#include"stdafx.h"



//Matrix Group
//------------------------------------------------------------------//

//struct STMtxGroup
//{
//	STMtxGroup();
//	STMtxGroup(const STMtxGroup& other);
//	STMtxGroup& operator=(const STMtxGroup& other);
//	STMtxGroup operator*(const STMtxGroup& other);
//
//	void GetInverse(STMtxGroup &Recieve);
//
//
//	D3DXMATRIX com;			//original world matrix
//	D3DXMATRIX trans;		//original translate matrix
//	D3DXMATRIX rotate;		//original rotation matrix
//	D3DXMATRIX scale;		//original scale matrix
//};
//
//STMtxGroup::STMtxGroup() :
//	com(g_IDMATRIX),
//	trans(g_IDMATRIX),
//	rotate(g_IDMATRIX),
//	scale(g_IDMATRIX) {}
//
//STMtxGroup::STMtxGroup(const STMtxGroup& other) :
//	com(other.com),
//	trans(other.trans),
//	rotate(other.rotate),
//	scale(other.scale)
//{}
//
//STMtxGroup& OBJ::STMtxGroup::operator=(const STMtxGroup& other)
//{
//	com = other.com;
//	trans = other.trans;
//	rotate = other.rotate;
//	scale = other.scale;
//
//	return *this;
//}
//
//STMtxGroup OBJ::STMtxGroup::operator*(const STMtxGroup& other)
//{
//	STMtxGroup a;
//
//	a.com = com * other.com;
//	a.trans = trans * other.trans;
//	a.rotate = rotate * other.rotate;
//	a.scale = scale * other.scale;
//
//	return a;
//}
//
//void STMtxGroup::GetInverse(STMtxGroup &Recieve)
//{
//	D3DXMatrixInverse(&Recieve.com, nullptr, &com);
//	D3DXMatrixInverse(&Recieve.trans, nullptr, &trans);
//	D3DXMatrixInverse(&Recieve.rotate, nullptr, &rotate);
//	D3DXMatrixInverse(&Recieve.scale, nullptr, &scale);
//}

//-------------------------------------------------------------------------//