#pragma once
//#include"Asedata.h"
//#include"Vertex.h"

namespace OBJ 
{
	struct STFaceMat
	{
		WORD wMatID;
		WORD wTexID;
	};


	struct STMaterial 
	{
		STMaterial();
		~STMaterial();


		D3DMATERIAL9 mat;
		IDirect3DTexture9* pTexture;
		std::vector<STMaterial*> vecSubMat;
	};

	class CMesh
	{
	public:
		CMesh();
		~CMesh();

		bool InitVBuffer(DWORD FVF, DWORD vtxCount,DWORD size, void* ptr);
	
		void AddFaceMat(STFaceMat& face);

		std::vector<STFaceMat>& GetFaceMatVec();
		bool InitIBuffer(void* ptr);

		bool LockIDB(void** Address);
		bool LockVTXB(DWORD FVF, DWORD vtxCount, DWORD size,void** Address);


		void SetSource();
		
		bool Draw();
		bool DrawEX(std::vector<STMaterial*>&mat);
		bool DrawFace(WORD Index);
		

	protected:	
		DWORD m_dwFVF;
		DWORD m_dwVTXSize;
		DWORD m_dwFaceCount;
		DWORD m_dwVTXCount;

		IDirect3DVertexBuffer9* m_pVB;
		IDirect3DIndexBuffer9*  m_pIB;

		std::vector<STFaceMat> m_vecFaceMat;
	};

	class CGemoObj
	{
	public:
		CGemoObj();
		virtual ~CGemoObj();

		void SetMesh(CMesh* mesh);
		void SetMatID(WORD mat);
		bool Draw(std::vector<STMaterial*>&mat);
		
	private:
		CMesh* m_pMesh;
		WORD m_wMatId;
	};



}
