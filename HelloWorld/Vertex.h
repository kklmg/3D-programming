#pragma once

struct STVTX_P
{
	STVTX_P();
	STVTX_P(float x, float y, float z);
	STVTX_P(D3DXVECTOR3 _pos);

	D3DXVECTOR3 Pos;
	static const DWORD FVF;
};


struct STVTX_PN
{
	STVTX_PN();
	STVTX_PN(float x, float y, float z, float nx, float ny, float nz);
	STVTX_PN(D3DXVECTOR3 _pos, D3DXVECTOR3 _Normal);

	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Normal;
	static const DWORD FVF;
};


struct STVTX_PNT
{
	STVTX_PNT();
	STVTX_PNT(float x, float y, float z, float nx, float ny, float nz,float u,float v);
	STVTX_PNT(D3DXVECTOR3 _pos,D3DXVECTOR3 _Normal, D3DXVECTOR2 _texture);

	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 Tex;
	static const DWORD FVF;
};


struct STVTX_PT
{
	STVTX_PT();
	STVTX_PT(float x, float y, float z, float u, float v);
	STVTX_PT(D3DXVECTOR3 _pos,D3DXVECTOR2 _texture);

	D3DXVECTOR3 Pos;
	D3DXVECTOR2 Tex;
	static const DWORD FVF;
};

struct STVTX_PWBNT
{
	STVTX_PWBNT();

	D3DXVECTOR3 Pos;
	float Weight[3];
	DWORD  BoneID;

	D3DXVECTOR3 Normal;
	D3DXVECTOR2 Tex;
	static const DWORD FVF;
};


struct STVTX_GRID
{
	STVTX_GRID();

	float _x, _y, _z;
	DWORD _color;

	static const DWORD FVF;
};


extern const D3DVERTEXELEMENT9 SkinVertexDecl[6];