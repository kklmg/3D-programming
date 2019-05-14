#pragma once

class CDirect3D
{
public:
	CDirect3D();
	~CDirect3D();

	bool Init();

	void BeginScene();
	void EndScene();

	IDirect3DDevice9*  GetDevice()const;

private:
	IDirect3DDevice9* m_pDevice;
	
};
