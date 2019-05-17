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

	class CObjNode
	{
	public:
		CObjNode():m_pParent(nullptr) {}
		virtual ~CObjNode(){}

		//virtual Draw();
		std::string m_strNodeName;
		std::string m_strParentName;

		D3DXMATRIX m_mtxAseWorld;

		D3DXMATRIX m_mtxLocal;
		D3DXMATRIX m_mtxWorld;

		D3DXMATRIX m_mtxScale;
		D3DXMATRIX m_mtxTranslate;
		D3DXMATRIX m_mtxRotate;


		void update() 
		{
			if (m_pParent)
			{
				//m_mtxWorld = m_mtxLocal*m_pParent->m_mtxWorld;
			}
			else
			{

			}
		
		


		}

		void SetChildren(CObjNode* child)
		{
			if(child)
			m_vecChildren.push_back(child);
		}
		virtual bool Draw(std::vector<STMaterial*>&mat)
		{
			return true;
		}
		virtual bool DrawAll(std::vector<STMaterial*>&mat, const D3DXMATRIX* World = nullptr);

		void InitLocal();

	private:
		void __InitLocalHelper(CObjNode*node, const D3DXMATRIX *parLocalTM);
		void __DrawHelper(CObjNode*node, const D3DXMATRIX *ParWorldTM, std::vector<STMaterial*>&mat);
		

		CObjNode* m_pParent;

		std::vector<CObjNode*>m_vecChildren;
	
	};

	class CGemoObj: public CObjNode
	{
	public:
		CGemoObj();
		virtual ~CGemoObj();

		void SetMesh(CMesh* mesh);
		void SetMatID(WORD mat);
		virtual bool Draw(std::vector<STMaterial*>&mat);
		

	private:
		CMesh* m_pMesh;
		WORD m_wMatId;
	};



}
