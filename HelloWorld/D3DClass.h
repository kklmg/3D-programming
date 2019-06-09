#pragma once

class CDirect3D
{
public:
	CDirect3D();
	~CDirect3D();

	bool Init();

	void BeginScene();
	void EndScene();

	void SetViewTM(D3DXMATRIX* V);

	IDirect3DDevice9*  GetDevice()const;

private:
	void __InitRenderState();

	IDirect3DDevice9* m_pDevice;
	
};
