#include"stdafx.h"
#include"Camera.h"

CCamera::CCamera() :
	m_vt3Pos(0, 400, -100),
	m_vt3Up(0, 0, 1),
	m_vt3Right(0, 1, 0),
	m_vt3Look(0, -1, 0)
{
}

CCamera::~CCamera()
{
}

void CCamera::Update() 
{
	__Input();
	__ComputeVM();
}

void CCamera::__Input()
{
	if (::GetAsyncKeyState('W') & 0x8000f)
		Walk(4.0f);

	if (::GetAsyncKeyState('S') & 0x8000f)
		Walk(-4.0f);

	if (::GetAsyncKeyState('A') & 0x8000f)
		Strafe(-1.0f);

	if (::GetAsyncKeyState('D') & 0x8000f)
		Strafe(1.0f);

	if (::GetAsyncKeyState('R') & 0x8000f)
		Fly(1.0f);

	if (::GetAsyncKeyState('F') & 0x8000f)
		Fly(-1.0f);

	//if (::GetAsyncKeyState(VK_UP) & 0x8000f)
	//	Pitch(1.0f);

	//if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
	//	Pitch(-1.0f);

	//if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
	//	Yaw(-1.0f);

	//if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
	//	Yaw(1.0f);

	//if (::GetAsyncKeyState('N') & 0x8000f)
	//	Roll(1.0f);

	//if (::GetAsyncKeyState('M') & 0x8000f)
	//	Roll(-1.0f);

}

void CCamera::__ComputeVM() 
{
	//normalize look vector
	D3DXVec3Normalize(&m_vt3Look, &m_vt3Look);

	//get right vector
	D3DXVec3Cross(&m_vt3Right,&m_vt3Up,&m_vt3Look);
	D3DXVec3Normalize(&m_vt3Right, &m_vt3Right);

	//get up vector
	D3DXVec3Cross(&m_vt3Up, &m_vt3Look, &m_vt3Right);
	D3DXVec3Normalize(&m_vt3Up, &m_vt3Up);

	//get offset 
	float x = D3DXVec3Dot(&m_vt3Right, &m_vt3Pos);
	float y = D3DXVec3Dot(&m_vt3Up, &m_vt3Pos);
	float z = D3DXVec3Dot(&m_vt3Look, &m_vt3Pos);
	
	//set x axis
	m_TMView._11 = m_vt3Right.x;
	m_TMView._21 = m_vt3Right.y;
	m_TMView._31 = m_vt3Right.z;

	//set y axis
	m_TMView._12 = m_vt3Up.x;
	m_TMView._22 = m_vt3Up.y;
	m_TMView._32 = m_vt3Up.z;

	//set z axis
	m_TMView._13 = m_vt3Look.x;
	m_TMView._23 = m_vt3Look.y;
	m_TMView._33 = m_vt3Look.z;


	//set pos
	m_TMView._41 = -x;
	m_TMView._42 = -y;
	m_TMView._43 = -z;

	//
	m_TMView._14 = 0;
	m_TMView._24 = 0;
	m_TMView._34 = 0;
	m_TMView._44 = 1;
}


void CCamera::Strafe(float units) 
{
	m_vt3Pos += units * m_vt3Right;
}
void CCamera::Fly(float units) 
{
	m_vt3Pos += units * m_vt3Up;
}
void CCamera::Walk(float units) 
{
	m_vt3Pos += units * m_vt3Look;
}

void CCamera::Pitch(float angle) 
{
	//get Transform Matrix
	D3DXMATRIX tempTM;
	D3DXMatrixRotationAxis(&tempTM,&m_vt3Right,angle);

	//compute transformed
	D3DXVec3TransformCoord(&m_vt3Up, &m_vt3Up, &tempTM);
	D3DXVec3TransformCoord(&m_vt3Look, &m_vt3Look, &tempTM);
}
void CCamera::Yaw(float angle) 
{
	//get Transform Matrix
	D3DXMATRIX tempTM;
	D3DXMatrixRotationAxis(&tempTM, &m_vt3Up, angle);

	//compute transformed
	D3DXVec3TransformCoord(&m_vt3Right, &m_vt3Right, &tempTM);
	D3DXVec3TransformCoord(&m_vt3Look, &m_vt3Look, &tempTM);
}
void CCamera::Roll(float angle) 
{
	//get Transform Matrix
	D3DXMATRIX tempTM;
	D3DXMatrixRotationAxis(&tempTM, &m_vt3Look, angle);

	//compute transformed
	D3DXVec3TransformCoord(&m_vt3Right, &m_vt3Right, &tempTM);
	D3DXVec3TransformCoord(&m_vt3Up, &m_vt3Up, &tempTM);
}


void CCamera::GetViewMatrix(D3DXMATRIX* V) 
{
	*V = m_TMView;
}

void CCamera::GetPosition(D3DXVECTOR3* pos) 
{
	*pos = m_vt3Pos;
}
void CCamera::SetPosition(D3DXVECTOR3* pos)
{
	m_vt3Pos = *pos;
}

void CCamera::GetRight(D3DXVECTOR3* right) 
{
	*right = m_vt3Right;
}
void CCamera::GetUp(D3DXVECTOR3* up) 
{
	*up = m_vt3Up;
}
void CCamera::GetLook(D3DXVECTOR3* look) 
{
	*look = m_vt3Look;
}