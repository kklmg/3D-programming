#include"stdafx.h"
#include"Vertex.h"




const DWORD STVTX_P::FVF = D3DFVF_XYZ;
const DWORD STVTX_PN::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;
const DWORD STVTX_PNT::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
const DWORD STVTX_PT::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

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
//const DWORD STVTX_P::FVF = D3DFVF_XYZ;


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
//const DWORD STVTX_PN::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;



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
//const DWORD STVTX_PNT::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;


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
//const DWORD STVTX_PT::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

//bool CreateVTXB(DWORD &GetFVF, ASEData::CMesh& meshdata, IDirect3DVertexBuffer9** ppVB,DWORD &GetSize)
//{
//	//Check vertex Format
//	DWORD FVF = 0;
//	DWORD vtxCount = 0;
//	if (!meshdata.m_vecVertex.size())return false;
//	else FVF = (FVF | D3DFVF_XYZ);
//	if (!meshdata.m_SetFace.size())return false;
//
//	if (meshdata.m_bNormal) FVF = (FVF | D3DFVF_NORMAL);
//	if (meshdata.m_dwNumTexture)FVF = (FVF | D3DFVF_TEX1);
//
//	GetFVF = FVF;
//	vtxCount = meshdata.m_vecVertex.size();
//
//	switch (FVF)
//	{
//	case STVTX_P::FVF:
//	{
//		GetSize = sizeof(STVTX_P);
//		g_pDevice->CreateVertexBuffer(
//			vtxCount * sizeof(STVTX_P),	//size
//			D3DUSAGE_WRITEONLY,
//			FVF,				//flexible format
//			D3DPOOL_MANAGED,
//			&(*ppVB),
//			0);
//	}break;
//	case STVTX_PN::FVF:
//	{
//		GetSize = sizeof(STVTX_PN);
//		g_pDevice->CreateVertexBuffer(
//			vtxCount * sizeof(STVTX_PN),	//size
//			D3DUSAGE_WRITEONLY,
//			FVF,				//flexible format
//			D3DPOOL_MANAGED,
//			&(*ppVB),
//			0);
//
//	}break;
//	case STVTX_PNT::FVF:
//	{
//		GetSize = sizeof(STVTX_PNT);
//		g_pDevice->CreateVertexBuffer(
//			vtxCount * sizeof(STVTX_PNT),	//size
//			D3DUSAGE_WRITEONLY,
//			FVF,				//flexible format
//			D3DPOOL_MANAGED,
//			&(*ppVB),
//			0);
//
//		// define unique vertices:
//		STVTX_PNT* vertices;
//		(*ppVB)->Lock(0, 0, (void**)&vertices, 0);
//		int i = 0;
//		for (auto data : meshdata.m_vecVertex)
//		{
//			vertices[i].Pos = data.GetPos();
//			vertices[i].Normal = data.GetNormal();
//			vertices[i].Tex = data.GetTexture();
//			++i;
//		}
//		(*ppVB)->Unlock();
//	}break;
//	case STVTX_PT::FVF:
//	{
//		GetSize = sizeof(STVTX_PT);
//		g_pDevice->CreateVertexBuffer(
//			vtxCount * sizeof(STVTX_PT),	//size
//			D3DUSAGE_WRITEONLY,
//			FVF,				//flexible format
//			D3DPOOL_MANAGED,
//			&(*ppVB),
//			0);
//	}
//	break;
//	default:
//		break;
//	}
//
//
//	return true;
//}

bool CreateIDB(ASEData::CMesh& meshdata, IDirect3DIndexBuffer9** ppIB)
{
	//if (!meshdata.m_SetFace.size())return false;

	//g_pDevice->CreateIndexBuffer(
	//	meshdata.m_SetFace.size() * sizeof(WORD) * 3,//size
	//	D3DUSAGE_WRITEONLY,
	//	D3DFMT_INDEX16,
	//	D3DPOOL_MANAGED,
	//	&(*ppIB),
	//	0);

	//WORD* indices = 0;
	//(*ppIB)->Lock(0, 0, (void**)&indices, 0);

	//int i = 0;
	//for (auto face : meshdata.m_SetFace)
	//{
	//	indices[i] = face[ASEData::eA];
	//	indices[i + 1] = face[ASEData::eB];
	//	indices[i + 2] = face[ASEData::eC];
	//	i += 3;
	//}

	//(*ppIB)->Unlock();
	return true;
}

