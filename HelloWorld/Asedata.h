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
	enum enLineType
	{ 
		VERTEX_KNOT, 
		VERTEX_INTERP 
	};






	//------------------------------------------------------------------
	//Scene Data
	//------------------------------------------------------------------
	struct STScene
	{
		DWORD m_dwFirstFrame;
		DWORD m_dwLastFrame;
		DWORD m_dwFrameSpeed;
		DWORD m_dwTicksPerFrame;

		D3DXVECTOR3 m_vt3BACKGROUND_STATIC;
		D3DXVECTOR3 m_vt3AMBIENT_STATIC;

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
	//Line Vertex Data
	//-----------------------------------------------------------------
	struct STVertex_Line
	{
		enLineType VertexType;
		D3DXVECTOR3 Vertex;
	};

	struct STLine
	{
		STLine();
		~STLine();

		bool m_bClosed;
		DWORD m_dwVertexCount;
		STVertex_Line *m_pLineVertex;
	};

	//-----------------------------------------------------------------
	//Matrix Data
	//-----------------------------------------------------------------
	struct STNodeTM
	{
	public:
		STNodeTM();
		~STNodeTM();

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
	//Animation Data
	//-----------------------------------------------------------------
	struct STPosCAniTrack
	{
		float Time;
		D3DXVECTOR3 Pos;
	};

	struct STRotCAniTrack
	{
		float Time;
		D3DXVECTOR3 RotAxis;
		float RotAng;
	};

	struct STScaleCAniTrack
	{
		float Time;
		D3DXVECTOR3 Scale;
		D3DXVECTOR3 ScaleAxis;
		float Angle;
	};

	struct STAniTMData
	{
		std::vector<STPosCAniTrack*> m_vecPos;
		std::vector<STRotCAniTrack*> m_vecrot;
		std::vector<STScaleCAniTrack*> m_vecScaleCAniTrack;
	};

	struct STMeshBlendWeight
	{
		DWORD BoneID;
		float Weight;
	};

	struct CMeshWeightData
	{
		std::vector<STMeshBlendWeight>m_vecMeshWeight;
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


		bool RecieveVTX_INDEX(void **Getvtx,WORD &vtxcount,WORD **Indicies);

		bool CreateVI_P(void **Getvtx, WORD &vtxcount, WORD **Indicies);
		bool CreateVI_PN(void **Getvtx, WORD &vtxcount, WORD **Indicies);
		bool CreateVI_PNT(void **Getvtx, WORD &vtxcount, WORD **Indicies);
		bool CreateVI_PT(void **Getvtx, WORD &vtxcount, WORD **Indicies);


		DWORD CheckFormat();
		OBJ::CMesh* CreateMesh();



		WORD m_wNumVertex;
		WORD m_wNumFace;
		WORD m_wNumTexture;
		WORD m_wNumTextureFace;
		WORD m_wNumBone;
		WORD m_wNumSkinWeight;
		DWORD m_dwFVF;
		WORD m_wPerVtxSize;
		bool m_bNormal;



		//Data Container
		std::vector<D3DXVECTOR3> m_vecVertex;	//pos Data
		std::vector<D3DXVECTOR2> m_vecTex;		//Texture Data	
	//	std::vector<STFloat3> m_vecNormal;		//Texture Data	
		

		std::vector<CFace*>m_vecFace;			//Face Data


		//CMeshWeightData *m_pWeightList;

		//char**m_ppBoneNameList;
	};

	struct STObject
	{
		std::string m_strNodeName;
		std::string m_strNodeParent;

		STNodeTM m_NodeTM;
		STAniTMData m_AniTM;
	};

	struct STGEOMObject :public STObject
	{
		STGEOMObject() 
			:
			m_wMatRef(USHRT_MAX)
		{	
		}

		CMesh m_Mesh;
		WORD  m_wMatRef;

		OBJ::CGemoObj* CreateGemoObj()
		{
			OBJ::CGemoObj* newobj = new OBJ::CGemoObj;
		
			newobj->SetMesh(m_Mesh.CreateMesh());
			newobj->SetMatID(m_wMatRef);

			return newobj;
		}
		
	};

	struct STShapeObject :public STObject
	{
	public:
		//STShapeObject();
		//~STShapeObject();

		int LineCount;
	//	CLineData *Line;
	};

	struct STHelperObject
	{
	public:
		//STHelperObject();
		//~STHelperObject();
	};

	class CASEData
	{
	public:
		CASEData();
		~CASEData();

		bool ParsingAll(LPCSTR FileName);

		bool CreateD3DMat(std::vector<OBJ::STMaterial*>&vecd3Mat,LPCSTR FileDir) 
		{
			vecd3Mat.resize(m_vecMaterial.size(),nullptr);

			for (int i = 0; i < m_vecMaterial.size(); ++i)
			{
				m_vecMaterial[i]->CreateD3DMat(FileDir, &vecd3Mat[i], m_vecMaterial[i]);
			}

			return true;
		}


		//CObject* CreateObject();


		/*{
			LoadSceneInfo(&ASEdata->m_SceneInfo);
			LoadMaterialAll(ASEdata);
		}*/
		
	private:
		bool ParseMaterial();

	

		//bool ParseMaterialHelper(STMaterial *mat);

		bool ParseNodeTM(STNodeTM *NodeTM);

		void ParseGemoObject();

		void ParseObject()
		{
			
		}

		CMYLexer *m_plexer;
public:
	
		STScene m_SceneInfo;
		std::vector<STGEOMObject*> m_vecGemoObj;
		std::vector<STHelperObject*> m_vecHelpObj;
		std::vector<STShapeObject*> m_vecShapeObj;
		std::vector<STMaterial*>m_vecMaterial;
	};
}