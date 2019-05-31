#pragma once
#include"Timer.h"
#include"Vertex.h"


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


	class CAniTrack
	{
		struct STPosTrack
		{
			STPosTrack(double keytime, D3DXVECTOR3 &pos)
				:dKeyTime(keytime), Vt3Pos(pos) {}

			double dKeyTime;			//key
			D3DXVECTOR3 Vt3Pos;		//position
		};
		struct STRotTrack
		{
			STRotTrack(double keytime, D3DXQUATERNION &quatrot)
				:dKeyTime(keytime), QuatRot(quatrot) {}

			double dKeyTime;			//key
			D3DXQUATERNION QuatRot;	//rotation
		};
		struct STScaleTrack
		{
			STScaleTrack(double keytime, D3DXQUATERNION &quatrot, D3DXVECTOR3 &vt3)
				:dKeyTime(keytime), QuatScaleRot(quatrot),Vt3Scale(vt3) {}

			double dKeyTime;			//key
			D3DXVECTOR3 Vt3Scale;		//Scale
			D3DXQUATERNION QuatScaleRot;	//scale Axis
		};

	public:
		CAniTrack();
		~CAniTrack();

		void Update();

		D3DXMATRIX GetAniMatrix(D3DXMATRIX LocalTM);

		bool __Lerp(D3DXMATRIX &__pos, D3DXMATRIX&__rot, D3DXMATRIX&__scale);


		CTimer m_Timer;

		double m_dLastTime;



		void NewPos(double time, D3DXVECTOR3& pos);
		void NewRot(double time, D3DXQUATERNION& rot);
		void NewScale(double time, D3DXVECTOR3& scale, D3DXQUATERNION& rot);


		void ComputeAbsPos();
		void ComputeAbsRot();

	private:
		WORD m_wKeyPos;
		WORD m_wKeyRot;
		WORD m_wKeyScale;

		std::vector<STPosTrack> m_vecPosTrack;
		std::vector<STRotTrack> m_vecRotTrack;
		std::vector<STScaleTrack> m_vecScaleTrack;
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
		void SetAni(CAniTrack*ani);			//Set Animation 

	private:
		CAniTrack *m_pAni;					//Animation Data

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

	class CLineBuffer
	{
	public:
		CLineBuffer();
		virtual ~CLineBuffer();

		void SetBuffer(STVTX_P* vtx, DWORD count, bool isclose);
		HRESULT Draw();
	private:
		bool m_bClosed;
		LPDIRECT3DVERTEXBUFFER9 m_pLineVB;
		WORD m_wCount;
	};


	class CShapeObj : public CObjNode
	{	
	public:
		CShapeObj();
		virtual ~CShapeObj();

		void AddLines(CLineBuffer* linebuff);

		virtual bool Draw(std::vector<STMaterial*>&mat);
	private:
		std::vector<CLineBuffer*>m_vecLines;
	};

	class CCameraObj :public CObjNode 
	{
	public:
		CCameraObj();
		virtual ~CCameraObj();
		

		virtual bool Draw(std::vector<STMaterial*>&mat);
	private:
		std::vector<CLineBuffer*>m_vecLines;
	};

	class CHelperObj :public CObjNode
	{
	public:
		CHelperObj();
		virtual ~CHelperObj();

		virtual bool Draw(std::vector<STMaterial*>&mat);

		void SetBound_min(D3DXVECTOR3 &vt3);
		void SetBound_max(D3DXVECTOR3 &vt3);

	private:
		D3DXVECTOR3 m_vt3bound_min;
		D3DXVECTOR3 m_vt3bound_max;	
	};


}


