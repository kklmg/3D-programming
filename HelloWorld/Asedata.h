#pragma once
#include"MyLexer.h"
#include"Convert.h"
#include<unordered_map>
#include<set>
#include<algorithm>
#include"ObjCom.h"
#include"mytemp.h"
#include"mystruct.h"


namespace ASEData
{
	//----------------------------------------------------------
	//Enum 
	//----------------------------------------------------------
	//triangle point A,B,C
	enum emTriangle
	{
		eA = 0,
		eB = 1,
		eC = 2,
	};
	enum emMatType
	{	
		eMatDefault,
		eStandard,
		eMulti 
	};	



	//------------------------------------------------------------------
	//Scene Data
	//------------------------------------------------------------------
	struct STScene
	{
		WORD m_wFirstFrame;
		WORD m_wLastFrame;
		WORD m_wFrameSpeed;
		WORD m_wTicksPerFrame;

		D3DXVECTOR3 m_vt3BACKGROUND_STATIC;
		D3DXVECTOR3 m_vt3AMBIENT_STATIC;

		WORD GetTPS() const
		{
			return m_wFrameSpeed*m_wTicksPerFrame;
		}
		double GetEndTime() const 
		{
			return (double)(m_wLastFrame) / (double)m_wFrameSpeed;
		}


		bool parse(CMYLexer *lexer);
	};

	//-----------------------------------------------------------------
	//Material Data
	//-----------------------------------------------------------------
	struct STMaterial
	{
		STMaterial();
		~STMaterial();

		bool parse(CMYLexer *lexer);

		bool CreateD3DMat(LPCSTR FileDir, OBJ::STMaterial**result, STMaterial*help);


		std::string m_strMatName;
		emMatType  m_emMatType;
		std::string m_strMatType;

		D3DCOLORVALUE   m_d3dcAmbient;        /* Ambient color RGB */
		D3DCOLORVALUE   m_d3dcDiffuse;        /* Diffuse color RGBA */
		D3DCOLORVALUE   m_d3dcSpecular;       /* Specular 'shininess' */
		D3DCOLORVALUE   m_d3dcEmissive;       /* Emissive color RGB */

		float m_fShine;
		float m_fShineStrength;/* Sharpness if specular highlight */
		float m_fSelfillum;
		float m_fTransparency;


		std::string m_strTexFile; //texture file name

		std::vector<STMaterial*> m_vecSubMat;
	};



	//-----------------------------------------------------------------
	//Node Transform Matrix 
	//-----------------------------------------------------------------
	struct STNodeTM
	{
	public:
		STNodeTM();
		~STNodeTM();

		bool parse(CMYLexer *lexer);

		bool GetWorldTM(D3DXMATRIX &matrix);
		bool GetWorldTMEx(D3DXMATRIX &_World, D3DXMATRIX& _Trans, D3DXMATRIX&_Rotate, D3DXMATRIX& _Scale);


		D3DXVECTOR3	m_vt3INHERIT_POS;
		D3DXVECTOR3 m_vt3INHERIT_ROT;
		D3DXVECTOR3 m_vt3INHERIT_SCL;

		D3DXVECTOR3 m_vt3TM_ROW0;
		D3DXVECTOR3 m_vt3TM_ROW1;
		D3DXVECTOR3 m_vt3TM_ROW2;
		D3DXVECTOR3 m_vt3TM_ROW3;
		D3DXVECTOR3 m_vt3TM_POS;
		D3DXVECTOR3 m_vt3TM_ROTAXIS;
		float m_fTM_ROTANGLE;
		D3DXVECTOR3 m_vt3TM_SCALE;
		D3DXVECTOR3 m_vt3TM_SCALEAXIS;
		float m_fTM_SCALEAXISANG;
	};



	//-----------------------------------------------------------------
	//vertex Data
	//-----------------------------------------------------------------
	struct STVtxData
	{
		STVtxData() :wTex(USHRT_MAX), wPos(USHRT_MAX){}

		bool operator<(const STVtxData other)const
		{		
			if (wTex != other.wTex) return wTex < other.wTex;
			else if (f3Normal != f3Normal)	return f3Normal < other.f3Normal;
			else return false;
		}

		WORD wPos;
		WORD wTex;
		STFloat3 f3Normal;
	};

	//used in check duplicated  vertex
	struct CompN
	{
		bool operator()(const STVtxData &A, const STVtxData &B)const;
	};

	struct CompT
	{
		bool operator()(const STVtxData &A, const STVtxData &B)const;
	};

	struct CompNT
	{
		bool operator()(const STVtxData &A, const STVtxData &B)const;
	};


	//-----------------------------------------------------------------
	//face Data
	//-----------------------------------------------------------------
	class CFace
	{
	public:
		CFace();
		~CFace();

		void Decompose(STVtxData arr[]);	

		STIndex3 m_Pos;	
		STIndex3 m_TexId;
		STFloat3 m_Normal[3];
	
		DWORD RefTex;
		DWORD RefMat;
	};


	//-----------------------------------------------------------------
	//Weight Data
	//-----------------------------------------------------------------
	struct _Weight
	{
		_Weight();
		_Weight(WORD _id, float _wt);

		WORD wID;
		float fWeight;
	};

	bool CompareWeight(_Weight& a, _Weight& b);

	struct STWeight 
	{
		void Add(WORD id, float weight);
		void Clip();

		void Export(STVTX_PWBNT& vtx);

		std::vector<_Weight>vecWeight;
	};



	//-----------------------------------------------------------------
	//Mesh Data
	//-----------------------------------------------------------------
	class CMesh
	{
	public:
		CMesh();
		~CMesh();

		bool ParseAll(CMYLexer *lexer);

		bool ParseVertex(CMYLexer *lexer);
		bool ParseFace(CMYLexer *lexer);
		bool ParseTexture(CMYLexer *lexer);
		bool ParseTextureFace(CMYLexer *lexer);
		bool ParseNormal(CMYLexer *lexer);
		bool ParseBone(CMYLexer *lexer);
		bool ParseWeight(CMYLexer *lexer);
		

		bool RecieveVTX_INDEX(void **Getvtx,WORD &vtxcount,WORD **Indicies, const D3DXMATRIX *Transform);
	//private:
		//create vertex buffer & index buffer
		bool CreateVI_P(void **Getvtx, WORD &vtxcount, WORD **Indicies, const D3DXMATRIX *Transform);
		bool CreateVI_PN(void **Getvtx, WORD &vtxcount, WORD **Indicies, const D3DXMATRIX *Transform);
		bool CreateVI_PNT(void **Getvtx, WORD &vtxcount, WORD **Indicies, const D3DXMATRIX *Transform);
		bool CreateVI_PT(void **Getvtx, WORD &vtxcount, WORD **Indicies, const D3DXMATRIX *Transform);
		bool CreateVI_PWBNT(void **Getvtx, WORD &vtxcount, WORD **Indicies, const D3DXMATRIX *Transform);

		DWORD CheckFormat();
		OBJ::CMesh* CreateMesh(const D3DXMATRIX *Transform = nullptr);



		WORD m_wNumVertex;
		WORD m_wNumFace;
		WORD m_wNumTexture;
		WORD m_wNumTextureFace;
		DWORD m_dwFVF;
		WORD m_wPerVtxSize;
		bool m_bNormal;

		

		//Data Container
		std::vector<D3DXVECTOR3> m_vecVertex;	//pos Data
		std::vector<D3DXVECTOR2> m_vecTex;		//Texture Data	
		std::vector<CFace*>m_vecFace;			//Face Data
		std::vector<std::string>m_vecBone;		//Bone Data
		std::vector<STWeight>m_vecWeight;		//Weight Data

	};


	//-----------------------------------------------------------------
	//Animation Data
	//-----------------------------------------------------------------
	struct STTrackPos
	{
		STTrackPos();
		D3DXMATRIX* CreateMtx(D3DXMATRIX& Receive);

		DWORD dwTick;
		D3DXVECTOR3 vt3Pos;
	};

	struct STTrackRot
	{
		STTrackRot();
		D3DXMATRIX* CreateMtx(D3DXMATRIX& Receive);
		D3DXQUATERNION* CreateQuaternion(D3DXQUATERNION& Receive);

		DWORD dwTick;
		D3DXVECTOR3 vt3RotAxis;
		float fRotAng;
	};

	struct STTrackScale
	{
		STTrackScale();
		D3DXMATRIX* CreateMtx(D3DXMATRIX& Receive);
		D3DXQUATERNION* CreateQuaternion(D3DXQUATERNION& Receive);

		DWORD dwTick;
		D3DXVECTOR3 vt3Scale;
		D3DXVECTOR3 vt3ScaleAxis;
		float fAngle;
	};

	class CAniData
	{
	public:
		CAniData();
		~CAniData();
	
		void Parse(CMYLexer *lexer);

		OBJ::CAniTrack* CreateAni(STScene* Data);
		
	private:
		void __ParsePos(CMYLexer *lexer);
		void __ParseRot(CMYLexer *lexer);
		void __ParseScale(CMYLexer *lexer);
		
	private:
		std::list<STTrackPos*> m_listPos;
		std::list<STTrackRot*> m_listRot;
		std::list<STTrackScale*> m_listScale;	
	};



	//-----------------------------------------------------------------
	//Object Data
	//-----------------------------------------------------------------

	struct STObject
	{
	public:
		STObject();
		
		std::string m_strNodeName;
		std::string m_strNodeParent;

		OBJ::CAniTrack* CreateAni(STScene* Data);

		virtual void Parse(CMYLexer *lexer) = 0;

		STNodeTM m_NodeTM;
		CAniData m_AniData;
	};

	struct STGEOMObject :public STObject
	{
		STGEOMObject();

		virtual void Parse(CMYLexer *lexer);

		OBJ::CGemoObj* CreateGemoObj(); 
		OBJ::CSkinObj* CreateSkinObj();

		CMesh m_Mesh;
		WORD  m_wMatRef;	
	};



	struct STLines
	{
		STLines();
		~STLines();

		virtual void parse(CMYLexer *lexer);
		void Transform(D3DXMATRIX * TM);

		WORD wCount;
		bool bClosed;
		STVTX_P* pVertex;
	};

	//Line Object
	struct STShapeObject :public STObject
	{
		STShapeObject();
		~STShapeObject();
		
		virtual void Parse(CMYLexer *lexer);
		OBJ::CShapeObj* CreateShapeObj();

		std::vector<STLines> vecLines;
	};

	struct STHelperObject :public STObject
	{
		STHelperObject();
		~STHelperObject();


		D3DXVECTOR3 m_vt3bound_min;
		D3DXVECTOR3 m_vt3bound_max;

		virtual void Parse(CMYLexer *lexer);
		OBJ::CHelperObj* CreateHelperObj();

	};

	struct STCameraAni 
	{
		DWORD dwTrack;
		float fFOV;
	};


	struct STCameraObj :public STObject
	{
	public:
		STCameraObj();
		~STCameraObj();

		float m_fFOV;
		std::list<STCameraAni>m_listCameraAni;
		
		virtual void Parse(CMYLexer *lexer);
		OBJ::CCameraObj* CreateCameraObj();

	};


	class CInheritData 
	{
	public:
		void InsertData(std::string &parent, std::string &child);

		bool IsRoot(std::string NodeName);

		bool FindChildren(const std::string &Parent,std::list<std::string>**Receive);

	private:
	//private:
		std::map<std::string, std::list<std::string>>m_mapInherit;
		std::set<std::string>m_SetRoot;
		
	};

	//-----------------------------------------------------------------
	//ASE Data
	//-----------------------------------------------------------------
	class CASEData
	{
	public:
		CASEData();
		~CASEData();

		bool ParsingAll(LPCSTR FileName);

		bool CreateD3DMat(std::vector<OBJ::STMaterial*>&vecd3Mat, LPCSTR FileDir);

		

		//parsing 
		bool ParseMaterial();
		void ParseGemoObject();
		void ParseShapeObject();
		void ParseHelperObject();
	
	
		CMYLexer *m_plexer;

		STScene* GetSceneData();
		STScene m_SceneInfo;

		
		std::vector<STGEOMObject*> m_vecGemoObj;
		std::vector<STGEOMObject*> m_vecSkinObj;
		std::vector<STHelperObject*> m_vecHelpObj;
		std::vector<STShapeObject*> m_vecShapeObj;
		std::vector<STMaterial*>m_vecMaterial;

		CInheritData m_InheritData;
	};
}