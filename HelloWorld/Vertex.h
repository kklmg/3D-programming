#pragma once
#include"Asedata.h"
#include"Global.h"

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

struct STVTX_Pos
{

};



















//
//struct STVXGroup
//{
//	bool CreateMesh(ASEData::CMesh& mesh)
//	{
//		std::map<ASEData::STVtxData, WORD>map;
//
//		ASEData::STIndex3 indicies;
//		ASEData::STVtxData vtxData[3];
//
//		int i = 0;
//		for (auto face : mesh.m_vecFace)
//		{
//			DWORD FVF = face.CheckFormat();
//
//			face.GetVtx(vtxData);
//		}
//		return true;
//	}
//
//	std::vector<STVTX_P>vecVTX_P;
//
//
//	std::vector<STVTX_PN>vecVTX_PN;
//	std::vector<STVTX_PNT>vecVTX_PNT;
//	std::vector<STVTX_PT>vecVTX_PT;
//
//
//
//};
//////g_pDevice->SetTransform(D3DTS_WORLD, &q);
//////g_pDevice->SetTransform(D3DTS_WORLD, &q);
////
////
////std::vector<LPDIRECT3DTEXTURE9>vectex;
////
////
////void ResetDefaultRenderState() 
////{
////	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
////	//g_pDevice->SetMaterial(&material);
////
////
////	//g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
////	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
////	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
////
////	// set blending factors so that alpha component determines transparency
////	g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
////	g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
////
////	g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
////	g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
////
////
////}
//
//
////namespace mesh
////{
////
////	struct STFace
////	{
////		WORD wMatID;
////		WORD wTexID;
////		WORD wFaceID;
////	};
////	}
//
//	//class CMeshBase
//	//{
//	//public:
//	//	CMeshBase();
//	//	~CMeshBase();
//
//	///*	bool Init(ASEData::CMesh& meshdata) 
//	//	{
//	//		return true;
//	//	}
//	//	*/
//
//
//
//	//	bool InitBuffer(DWORD Sizevtxbuff,WORD sizeidbuff,DWORD fvf,D3DPOOL pool= D3DPOOL_MANAGED)
//	//	{
//	//		m_dwFVF = fvf;
//
//	//		g_pDevice->CreateVertexBuffer(
//	//			Sizevtxbuff,
//	//			D3DUSAGE_WRITEONLY,
//	//			m_dwFVF,
//	//			pool,
//	//			&m_pVB,
//	//			0);
//
//	//		g_pDevice->CreateIndexBuffer(
//	//			sizeidbuff,
//	//			D3DUSAGE_WRITEONLY,
//	//			D3DFMT_INDEX16,
//	//			pool,
//	//			&m_pIB,
//	//			0);
//
//	//		return true;
//	//	}
//
//	//	bool Draw(std::vector<D3DMATERIAL9>& mat, std::vector<LPDIRECT3DTEXTURE9>& tex)
//	//	{
//	//		//set Source
//	//		g_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVTXSize);
//
//	//		//set indicies
//	//		g_pDevice->SetIndices(m_pIB);
//
//	//		//set format
//	//		g_pDevice->SetFVF(m_dwFVF);
//
//	//		//Render
//	//		int i = 0;
//	//		for (auto face : m_vecFacies) 
//	//		{
//	//			//set Material
//	//			g_pDevice->SetMaterial(&mat[face.wMatID]);
//
//	//			//set texture
//	//			g_pDevice->SetTexture(0, tex[face.wTexID]);
//	//			
//	//			//render
//	//			g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVTXCount, i, i+1);
//
//	//			++i;
//	//		}
//	//	}
//
//
//	//	std::vector<STFace>m_vecFacies;
//
//
//	//	DWORD m_dwFVF;
//	//	DWORD m_dwVTXSize;
//	//	DWORD m_dwFaceCount;
//	//	DWORD m_dwVTXCount;
//
//
//	//	IDirect3DVertexBuffer9* m_pVB;
//	//	IDirect3DIndexBuffer9*  m_pIB;
//
//	//	//std::vector<STFace> m_vecFaces;
//	//private:
//	//	
//	//};
//
//
//	//class CObject
//	//{
//	//	CObject();
//	//	~CObject();
//	//
//	//	//bool Init(ASEData::CFace&face) 
//	//	//{
//	//	//
//	//	//}
//
//
//
//
//	//	std::vector<D3DMATERIAL9> mat;
//	//	std::vector<LPDIRECT3DTEXTURE9> tex;
//	//
//	//	CMeshBase sub[2];
//
//	//	D3DXMATRIX m_matrWorld;
//
//
//
//	//};
//
//
//
//	//CMeshBase::CMeshBase()
//	//{
//	//}
//
//	//CMeshBase::~CMeshBase()
//	//{
//	//}
//
//
//
//}



//bool CreateVTXB(DWORD &GetFVF, ASEData::CMesh& meshdata, IDirect3DVertexBuffer9** ppVB, DWORD &GetSize);

//bool CreateIDB(ASEData::CMesh& meshdata, IDirect3DIndexBuffer9** ppIB);