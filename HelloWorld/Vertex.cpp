#include"stdafx.h"
#include"Vertex.h"

//Vertex
//Position
STVTX_P::STVTX_P() {}
STVTX_P::STVTX_P(float x, float y, float z)
{
	Pos.x = x;  Pos.y = y;  Pos.z = z;
}
STVTX_P::STVTX_P(D3DXVECTOR3 _pos) 
{
	Pos = _pos;
}
const DWORD STVTX_P::FVF = D3DFVF_XYZ;


//Vertex
//Position & Normal 
STVTX_PN::STVTX_PN() 
{
}
STVTX_PN::STVTX_PN(float x, float y, float z, float nx, float ny, float nz) 
{
	Pos.x = x;  Pos.y = y;  Pos.z = z;
	Normal.x = nx;  Normal.y = ny;  Normal.z = nz;
}
STVTX_PN::STVTX_PN(D3DXVECTOR3 _pos, D3DXVECTOR3 _Normal) 
{
	Pos = _pos;
	Normal = _Normal;
}
const DWORD STVTX_PN::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;


//Vertex
//Position & Normal & Texture
STVTX_PNT::STVTX_PNT()
{
}
STVTX_PNT::STVTX_PNT(float x, float y, float z, float nx, float ny, float nz, float u, float v)
{
	Pos.x = x;  Pos.y = y;  Pos.z = z;
	Normal.x = nx;  Normal.y = ny;  Normal.z = nz;
	Tex.x = u; Tex.y = v;
}
STVTX_PNT::STVTX_PNT(D3DXVECTOR3 _pos, D3DXVECTOR3 _Normal, D3DXVECTOR2 _texture)
{
	Pos = _pos;
	Normal = _Normal;
	Tex = _texture;
}
const DWORD STVTX_PNT::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;


//Vertex
//Position & Texture
STVTX_PT::STVTX_PT()
{
}
STVTX_PT::STVTX_PT(float x, float y, float z,  float u, float v)
{
	Pos.x = x;  Pos.y = y;  Pos.z = z;
	Tex.x = u; Tex.y = v;
}
STVTX_PT::STVTX_PT(D3DXVECTOR3 _pos, D3DXVECTOR2 _texture)
{
	Pos = _pos;
	Tex = _texture;
}
const DWORD STVTX_PT::FVF = D3DFVF_XYZ | D3DFVF_TEX1;


//Vertex
//Grid
STVTX_GRID::STVTX_GRID()
{
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
	_color = 0xffffffff;
}
const DWORD STVTX_GRID::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;