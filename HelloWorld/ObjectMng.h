#pragma once

class CObject;

class CObjectMng
{
public:
	CObjectMng(IDirect3DDevice9*const Device);
	~CObjectMng();

	bool Init();

	bool CreateObject(LPCSTR FileName, const D3DXMATRIX *World);

	void DrawObject();
private:
	IDirect3DDevice9 *const m_rpDevice;

	std::list<CObject*>m_listObj;

};

