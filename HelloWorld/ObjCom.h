#pragma once
#include"Timer.h"

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

	
		bool InitIBuffer(void* ptr);

		bool LockIDB(void** Address);
		bool LockVTXB(DWORD FVF, DWORD vtxCount, DWORD size,void** Address);


		void SetSource();
		
		bool Draw();
		bool DrawEX(std::vector<STMaterial*>&mat);
		bool DrawFace(WORD Index);
		
		std::vector<STFaceMat>& GetFaceMatVec();
	protected:	
		DWORD m_dwFVF;
		DWORD m_dwVTXSize;
		DWORD m_dwFaceCount;
		DWORD m_dwVTXCount;

		IDirect3DVertexBuffer9* m_pVB;
		IDirect3DIndexBuffer9*  m_pIB;

		std::vector<STFaceMat> m_vecFaceMat;
	};

	struct STAniTrack
	{
		STAniTrack();
		~STAniTrack();

		DWORD dwTick;

		double dKeyTime;			//key

		D3DXVECTOR3 *pVt3Pos;		//position

		D3DXQUATERNION *pQuatRot;	//rotation

		D3DXVECTOR3 *pVt3Scale;		//Scale
		D3DXQUATERNION *pQuatScaleRot;	//scale Axis
	};

	class CObjAni
	{
	public:

		CObjAni();
		~CObjAni();

		void Update();
		D3DXMATRIX GetAniMatrix(D3DXMATRIX LocalTM);

		bool __Lerp(D3DXMATRIX &pos, D3DXMATRIX&rot, D3DXMATRIX&scale);

		bool NewPos(WORD key, double time, D3DXVECTOR3 pos);
		bool NewRot(WORD key, double time, D3DXQUATERNION rot);
		bool NewScale(WORD key, double time, D3DXVECTOR3 scale, D3DXQUATERNION rot);

		void ComputeAbsPos();
		void ComputeAbsRot();

	

		double m_dLastTime;

		CTimer m_Timer;

		bool m_bPosTrack;
		bool m_bRotTrack;
		bool m_bScaleTrack;

		WORD m_wCurKey;
		WORD m_wMaxKey;

		std::vector<STAniTrack*> m_vecTrack;

	};

	class CObjNode
	{
	public:
		//Basic 
		//------------------------------------------------------------
		CObjNode();
		virtual ~CObjNode();


		//Animation
		//------------------------------------------------------------
	public:
		void SetAni(CObjAni*ani);			//Set Animation 

	private:
		CObjAni *m_pAni;					//Animation Data

		//Update Node
		//--------------------------------------------------------------
	public:
		void update();
	private:
		void __updateHelper(CObjNode*node);

		//Draw Node
		//--------------------------------------------------------------
	public:
		virtual bool Draw(std::vector<STMaterial*>&mat) = 0;

		virtual bool DrawAll(std::vector<STMaterial*>&mat);

	private:
		void __DrawHelper(CObjNode*node,std::vector<STMaterial*>&mat);

	
		//Set Hierarchic structure
		//--------------------------------------------------------------
	public:
		void SetParent(CObjNode* Parent);
		void SetChildren(CObjNode* child);

		void InitLocal();

		D3DXMATRIX GetParWorld() const;		//GetParent World Transformed Matrix
	private:
		void __InitLocalHelper(CObjNode*node);

	private:
		CObjNode* m_pParent;					//parent node
		std::vector<CObjNode*>m_vecChildren;	//childeren nodes


		//Basic 
		//--------------------------------------------------------------
	public:
		std::string GetNodeName() const;
		void SetNodeName(std::string& str);
		void SetParNodeName(std::string& str);

		void SetOriginalTM(D3DXMATRIX TM);
		void SetWorldTM(D3DXMATRIX TM); 
		void SetRootTM(D3DXMATRIX TM);

	private:
		

		std::string m_strNodeName;			//Node Name
		std::string m_strParentName;		//Parent Node Name


		D3DXMATRIX m_TMRoot;		
		D3DXMATRIX m_TMLocal;				//Transform Matrix:     Local 
		D3DXMATRIX m_TMOrign;				//Transform Matrix:     Original world 
		D3DXMATRIX m_TMWorld;				//Transform Matrix:     Real World Transform Matrix
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
