#include"stdafx.h"
#include"Global.h"

#include"ObjCom.h"



OBJ::STMaterial::STMaterial()
	:
	pTexture(nullptr)
{
}

OBJ::STMaterial::~STMaterial()
{
	for (auto ptr : vecSubMat)
	{
		SAFE_DELETE(ptr);
	}

	SAFE_RELEASE(pTexture);
}




OBJ::CMesh::CMesh() :
	m_dwFVF(0),
	m_dwVTXSize(0),
	m_dwFaceCount(0),
	m_dwVTXCount(0),
	m_pVB(nullptr),
	m_pIB(nullptr)
{
}

OBJ::CMesh::~CMesh()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
}

bool OBJ::CMesh::InitVBuffer(DWORD FVF, DWORD vtxCount, DWORD size, void* ptr)
{
	//init basic attribute
	m_dwFVF = FVF;

	m_dwVTXCount = vtxCount;
	m_dwVTXSize = size;

	//create buffer
	g_pDevice->CreateVertexBuffer(vtxCount*size, D3DUSAGE_WRITEONLY, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, 0);

	//set buffer
	void* address;
	m_pVB->Lock(0, 0, &address, 0);

	//load vtx data
	memcpy(address, ptr, vtxCount*size);

	m_pVB->Unlock();

	return true;
}

void OBJ::CMesh::AddFace(STFace& face)
{
	m_vecFaces.push_back(face);
	m_dwFaceCount++;
}

bool OBJ::CMesh::LockIDB(void** Address) 
{
	return m_pIB->Lock(0, 0, (void**)&Address, 0);
}
bool OBJ::CMesh::LockVTXB(DWORD FVF, DWORD vtxCount, DWORD size,void** GetAddress)
{
	//init basic attribute
	m_dwFVF = FVF;

	m_dwVTXCount = vtxCount;
	m_dwVTXSize = size;

	//create buffer
	g_pDevice->CreateVertexBuffer(vtxCount*size, D3DUSAGE_WRITEONLY, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, 0);

	//lock
	return m_pVB->Lock(0, 0, &*GetAddress, 0);

}


std::vector<OBJ::STFace>&
OBJ::CMesh::GetFaceVec()
{
	return m_vecFaces;
}

bool OBJ::CMesh::InitIBuffer(void* ptr)
{
	m_dwFaceCount = m_vecFaces.size();

	//create index buffer
	g_pDevice->CreateIndexBuffer(
		m_vecFaces.size() * sizeof(WORD) * 3,//size
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		0);

	WORD* indices = 0;
	m_pIB->Lock(0, 0, (void**)&indices, 0);


	//set indicies
	memcpy(indices, ptr, m_dwFaceCount *3*sizeof(WORD));


	m_pIB->Unlock();
	

	return true;
}

void OBJ::CMesh::SetSource() 
{
	g_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVTXSize);
	g_pDevice->SetIndices(m_pIB);
	g_pDevice->SetFVF(m_dwFVF);
}

bool OBJ::CMesh::Draw()
{
	SetSource();
	g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVTXCount, 0, m_dwFaceCount);	
	return true;
}


bool OBJ::CMesh::DrawEX(std::vector<STMaterial*>&mat) 
{
	SetSource();

	for (int i = 0; i < m_vecFaces.size(); ++i) 
	{
		g_pDevice->SetMaterial(&mat[m_vecFaces[i].wMatID]->mat);
		g_pDevice->SetTexture(0, mat[m_vecFaces[i].wMatID]->pTexture);
		g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVTXCount, i, i + 1);	
	}

	return true;
}
bool OBJ::CMesh::DrawFace(WORD Index) 
{
	if (Index > m_vecFaces.size())return false;
	return	g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVTXCount, Index, Index+1);
}

OBJ::CGemoObj::CGemoObj()
	:
	m_pMesh(nullptr),
	m_wMatId(0)
{
}

OBJ::CGemoObj::~CGemoObj()
{
	SAFE_DELETE(m_pMesh);
}

bool OBJ::CGemoObj::Draw(std::vector<STMaterial*>&mat)
{
	if (!m_pMesh)return false;

	if (mat.size() && m_wMatId!=USHRT_MAX)
	{
		if (mat[m_wMatId]->vecSubMat.size())
			m_pMesh->DrawEX(mat[m_wMatId]->vecSubMat);
		else
		{
			g_pDevice->SetMaterial(&mat[m_wMatId]->mat);
			g_pDevice->SetTexture(0, mat[m_wMatId]->pTexture);
			m_pMesh->Draw();
		}
	}
	else 
		m_pMesh->Draw();

	return true;
}
void OBJ::CGemoObj::SetMesh(CMesh* mesh) 
{
	m_pMesh = mesh;
}
void OBJ::CGemoObj::SetMatID(WORD mat) 
{
	m_wMatId = mat;
}
