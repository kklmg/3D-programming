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

void OBJ::CMesh::AddFaceMat(STFaceMat& face)
{
	m_vecFaceMat.push_back(face);
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

std::vector<OBJ::STFaceMat>&
OBJ::CMesh::GetFaceMatVec()
{
	return m_vecFaceMat;
}

bool OBJ::CMesh::InitIBuffer(void* ptr)
{
	m_dwFaceCount = m_vecFaceMat.size();

	//create index buffer
	g_pDevice->CreateIndexBuffer(
		m_vecFaceMat.size() * sizeof(WORD) * 3,//size
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

	int i = 0, j = 0;
	for (; i < m_vecFaceMat.size();++i,j+=3 )
	{
		g_pDevice->SetMaterial(&mat[m_vecFaceMat[i].wMatID]->mat);
		g_pDevice->SetTexture(0, mat[m_vecFaceMat[i].wMatID]->pTexture);
		g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVTXCount, j, 1);
	}
	return true;
}
bool OBJ::CMesh::DrawFace(WORD Index) 
{
	if (Index > m_vecFaceMat.size())return false;
	return	g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVTXCount, Index, Index+1);
}




//-------------------------------------------------------------------
OBJ::CGemoObj::CGemoObj():
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


//Shape Object
//-------------------------------------------------------------------

OBJ::CLineBuffer::CLineBuffer():
	m_bClosed(false),
	m_pLineVB(nullptr),
	m_wCount(0)
{

}

OBJ::CLineBuffer::~CLineBuffer() 
{
	SAFE_RELEASE(m_pLineVB);
}

void OBJ::CLineBuffer::SetBuffer(STVTX_P* vtx, DWORD count, bool isclose) 
{
	SAFE_RELEASE(m_pLineVB);

	m_wCount = count;
	m_bClosed = isclose;
	//create Buffer
	HRESULT hres=
	g_pDevice->CreateVertexBuffer(
		sizeof(STVTX_P)*count, 0, STVTX_P::FVF, D3DPOOL_DEFAULT, &m_pLineVB, nullptr);

	//write memory
	void* pLineVT;
	m_pLineVB->Lock(0, sizeof(STVTX_P)*count, (void**)&pLineVT, 0);
	memcpy(pLineVT, vtx, sizeof(STVTX_P)*count);
	m_pLineVB->Unlock();
}

HRESULT OBJ::CLineBuffer::Draw() 
{
	//Set Source
	g_pDevice->SetStreamSource(0, m_pLineVB, 0, sizeof(STVTX_P));
	g_pDevice->SetFVF(STVTX_P::FVF);

	if (m_bClosed)
		return g_pDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, m_wCount-1);
	else
		return g_pDevice->DrawPrimitive(D3DPT_LINELIST, 0, m_wCount-1);
}


OBJ::CShapeObj::CShapeObj() 
{
}

OBJ::CShapeObj::~CShapeObj() 
{
	for (auto ptr : m_vecLines) 
	{
		SAFE_DELETE(ptr);
	}
}

bool OBJ::CShapeObj::Draw(std::vector<STMaterial*>&mat)
{
	for (auto ptr : m_vecLines)
	{
		ptr->Draw();
	}
	return true;
}

void OBJ::CShapeObj::AddLines(CLineBuffer* linebuff) 
{
	if(linebuff)
	m_vecLines.push_back(linebuff);
}



//Camera Object
//-------------------------------------------------------------------

OBJ::CCameraObj::CCameraObj() 
{
}

OBJ::CCameraObj::~CCameraObj() 
{
}


bool OBJ::CCameraObj::Draw(std::vector<STMaterial*>&mat) 
{
	return true;
}




//Helper Object
//-------------------------------------------------------------------

OBJ::CHelperObj::CHelperObj() 
{
}
OBJ::CHelperObj::~CHelperObj() 
{
}

bool OBJ::CHelperObj::Draw(std::vector<STMaterial*>&mat) 
{
	return true;
}

void OBJ::CHelperObj::SetBound_min(D3DXVECTOR3 &vt3) 
{
	m_vt3bound_min = vt3;
}
void OBJ::CHelperObj::SetBound_max(D3DXVECTOR3 &vt3) 
{
	m_vt3bound_max = vt3;
}


