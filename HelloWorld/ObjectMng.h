#pragma once
class CObjectMng
{
public:
	CObjectMng(IDirect3DDevice9*const Device);
	~CObjectMng();

	bool Init();

	void CreateObject();

	void DrawObject();
private:
	IDirect3DDevice9 *const m_rpDevice;

	

};

