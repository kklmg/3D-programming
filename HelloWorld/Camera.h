#pragma once


class CCamera
{
public:
	CCamera();
	~CCamera();

	void Update();

	void Strafe(float units); // left/right
	void Fly(float units);    // up/down
	void Walk(float units);   // forward/backward

	void Pitch(float angle); // rotate on right vector
	void Yaw(float angle);   // rotate on up vector
	void Roll(float angle);  // rotate on look vector

	void GetViewMatrix(D3DXMATRIX* V);
	void GetPosition(D3DXVECTOR3* pos);
	void SetPosition(D3DXVECTOR3* pos);

	void GetRight(D3DXVECTOR3* right);
	void GetUp(D3DXVECTOR3* up);
	void GetLook(D3DXVECTOR3* look);

private:
	void __Input();
	void __ComputeVM();		//update View Matrix

	D3DXVECTOR3 m_vt3Pos;
	D3DXVECTOR3 m_vt3Up;
	D3DXVECTOR3 m_vt3Right;
	D3DXVECTOR3 m_vt3Look;

	D3DXMATRIX m_TMView;
};
