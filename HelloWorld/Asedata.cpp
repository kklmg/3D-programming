#include"stdafx.h"
#include"TokenDefine.h"
#include"Vertex.h"
#include<unordered_set>
#include<algorithm>
#include"Global.h"
#include"Asedata.h"


using namespace ASEData;

//---------------------------------------------------
//Compare Function
//---------------------------------------------------
//used in check repeated vector
bool ASEData::CompN::operator()(const STVtxData &A, const STVtxData &B) const
{
	return A.f3Normal < B.f3Normal;
};

bool ASEData::CompT::operator()(const STVtxData &A, const STVtxData &B) const
{
	return A.wTex < B.wTex;
};

bool ASEData::CompNT::operator()(const STVtxData &A, const STVtxData &B) const
{
	if (A.wTex != B.wTex) return A.wTex < B.wTex;
	if (A.f3Normal != B.f3Normal) return A.f3Normal < B.f3Normal;
	else return false;
};

	


//---------------------------------------------------
//Material Data
//---------------------------------------------------
STMaterial::STMaterial()
	:
	m_emMatType(emMatType::eStandard), 
	m_fShine(1),
	m_fShineStrength(1),
	m_fSelfillum(0),
	m_fTransparency(0)
{
}
STMaterial::~STMaterial() 
{
	for (auto ptr : m_vecSubMat) 
	{
		SAFE_DELETE(ptr);
	}

}

bool STMaterial::parse(CMYLexer *lexer)
{
	//move to block start
	lexer->MovetoBlockStart();

	std::unordered_set<DWORD> Memo;
	Memo.insert(TOKENR_MATERIAL_NAME);
	Memo.insert(TOKENR_MATERIAL_CLASS);
	Memo.insert(TOKENR_MATERIAL_AMBIENT);
	Memo.insert(TOKENR_MATERIAL_DIFFUSE);
	Memo.insert(TOKENR_MATERIAL_SPECULAR);
	Memo.insert(TOKENR_MATERIAL_SHINE);
	Memo.insert(TOKENR_MATERIAL_SHINESTRENGTH);
	Memo.insert(TOKENR_MATERIAL_TRANSPARENCY);
	Memo.insert(TOKENR_MATERIAL_SELFILLUM);
	Memo.insert(TOKENR_MAP_DIFFUSE);
	Memo.insert(TOKENR_MAP_REFLECT);
	Memo.insert(TOKENR_MAP_SPECULAR);
	Memo.insert(TOKENR_NUMSUBMTLS);
	Memo.insert(TOKENR_SUBMATERIAL);
	Memo.insert(TOKEND_BLOCK_START);



	while (lexer->FindToken_UseTable_Until(TOKEND_BLOCK_END,Memo))
	{
		switch (CMYLexer::s_CurToken_Dw)
		{
			//parse Material Name
		case TOKENR_MATERIAL_NAME:
		{
			lexer->GetString(m_strMatName);
		}break;

		//Parse Material Type
		case TOKENR_MATERIAL_CLASS:
		{
			std::string strTemp;
			lexer->GetString(strTemp);

			if (strTemp == "Standard")m_emMatType = emMatType::eStandard;
			else if (strTemp == "Multi/Sub-Object")m_emMatType = emMatType::eMulti;
			else m_emMatType = emMatType::eMatDefault;
		} break;

		//Parse Ambient
		case TOKENR_MATERIAL_AMBIENT:
		{
			lexer->GetFloat(m_d3dcAmbient.r);
			lexer->GetFloat(m_d3dcAmbient.g);
			lexer->GetFloat(m_d3dcAmbient.b);
		} break;

		//Parse Diffuse
		case TOKENR_MATERIAL_DIFFUSE:
		{
			lexer->GetFloat(m_d3dcDiffuse.r);
			lexer->GetFloat(m_d3dcDiffuse.g);
			lexer->GetFloat(m_d3dcDiffuse.b);
		}break;

		//Parse Specular
		case TOKENR_MATERIAL_SPECULAR:
		{
			lexer->GetFloat(m_d3dcSpecular.r);
			lexer->GetFloat(m_d3dcSpecular.g);
			lexer->GetFloat(m_d3dcSpecular.b);
		}break;

		//Parse Mat Shine
		case TOKENR_MATERIAL_SHINE:
			lexer->GetFloat(m_fShine); break;

			//Parse Shine Strength
		case TOKENR_MATERIAL_SHINESTRENGTH:
			lexer->GetFloat(m_fShineStrength); break;

			//Parse Tranparency
		case TOKENR_MATERIAL_TRANSPARENCY:
			lexer->GetFloat(m_fTransparency); break;

			//Parse Selfillum
		case TOKENR_MATERIAL_SELFILLUM:
			lexer->GetFloat(m_fSelfillum); break;

			//Parse Texture
		case TOKENR_MAP_DIFFUSE:
		case TOKENR_MAP_REFLECT:
		case TOKENR_MAP_SPECULAR:
		{
			lexer->MovetoBlockStart();

			lexer->FindToken(TOKENR_MAP_CLASS);
			lexer->GetString(m_strMatType);

			//parse File Address
			if (m_strMatType == "Bitmap")
			{
				std::string FileAddress;
				lexer->FindToken_InBlock(TOKENR_BITMAP);
				lexer->GetString(FileAddress);
				size_t index = FileAddress.find_last_of("\\");
				m_strTexFile.assign(FileAddress, index + 1, FileAddress.size());
			}
			lexer->MovetoBlockEnd();

		}break;

		//parse Submat count
		case TOKENR_NUMSUBMTLS:
		{
			WORD count = 0;
			lexer->GetWORD(count);
			m_vecSubMat.resize(count);
		}break;

		//Rcursivly parse sub Material
		case TOKENR_SUBMATERIAL:
		{
			for (int i = 0; i <m_vecSubMat.size(); i++)
			{
				m_vecSubMat[i] = new STMaterial();
				m_vecSubMat[i]->parse(lexer);
			}
		} break;

		//skip undefined token
		case TOKEND_BLOCK_START:
			lexer->SkipCurBlock(); 
			break;
		}
	}
	return TRUE;
}

bool STMaterial::CreateD3DMat(LPCSTR FileDir, OBJ::STMaterial**result, STMaterial*help)
{
	//-----------------------------------------------------------
	//Ambient = MATERIAL_AMBIENT
	//Diffuse = MATERIAL_DIFFUSE
	//Specular = MATERIAL_SPECULAR ¡¿ MATERIAL_SHINESTRENGTH
	//Specular Power = MATERIAL_SHINE ¡¿ 127.9961
	//Emissive = MATERIAL_SELFILLUM ¡¿ (MATERIAL_DIFFUSE)
	//Alpha = 1.0f - MATERIAL_TRANSPARENCY
	//-----------------------------------------------------------

	*result = new OBJ::STMaterial();

	//Ambient = MATERIAL_AMBIENT
	(*result)->mat.Ambient = help->m_d3dcAmbient;

	//Diffuse = MATERIAL_DIFFUSE
	(*result)->mat.Diffuse = help->m_d3dcDiffuse;

	//Specular = MATERIAL_SPECULAR ¡¿ MATERIAL_SHINESTRENGTH
	(*result)->mat.Specular.r = help->m_d3dcSpecular.r * help->m_fShineStrength;
	(*result)->mat.Specular.g = help->m_d3dcSpecular.g * help->m_fShineStrength;
	(*result)->mat.Specular.b = help->m_d3dcSpecular.b * help->m_fShineStrength;

	//Specular Power = MATERIAL_SHINE ¡¿ 127.9961
	(*result)->mat.Power = help->m_fShine*127.9961;

	//Emissive = MATERIAL_SELFILLUM ¡¿ (MATERIAL_DIFFUSE)
	(*result)->mat.Emissive = help->m_fSelfillum * help->m_d3dcDiffuse;

	//Alpha = 1.0f - MATERIAL_TRANSPARENCY
	(*result)->mat.Ambient.a = 1.0f - help->m_fTransparency;
	(*result)->mat.Diffuse.a = 1.0f - help->m_fTransparency;
	(*result)->mat.Emissive.a = 1.0f - help->m_fTransparency;
	(*result)->mat.Specular.a = 1.0f - help->m_fTransparency;

	//create d3d Texture
	if (help->m_strTexFile.size())
	{
		std::string Address = FileDir;
		Address+= '/' + help->m_strTexFile;
		D3DXCreateTextureFromFileA(g_pDevice, Address.c_str(), &(*result)->pTexture);
	}

	//create sub material
	//recursive
	WORD submatcount = help->m_vecSubMat.size();
	if (submatcount) 
	{
		(*result)->vecSubMat.resize(submatcount,nullptr);
	
		for (int i = 0; i <submatcount; ++i)
		{
			CreateD3DMat(FileDir, &(*result)->vecSubMat[i], help->m_vecSubMat[i]);
		}
	}	
	return true;
}




bool STScene::parse(CMYLexer *lexer)
{
	lexer->MovetoBlockStart();

	std::unordered_set<DWORD> Memo;
	Memo.insert(TOKENR_SCENE_FIRSTFRAME);
	Memo.insert(TOKENR_SCENE_LASTFRAME);
	Memo.insert(TOKENR_SCENE_FRAMESPEED);
	Memo.insert(TOKENR_SCENE_TICKSPERFRAME);
	Memo.insert(TOKENR_SCENE_BACKGROUND_STATIC);
	Memo.insert(TOKENR_SCENE_AMBIENT_STATIC);
	Memo.insert(TOKEND_BLOCK_START);


	while (lexer->FindToken_UseTable_Until(TOKEND_BLOCK_END,Memo))
	{
		switch (CMYLexer::s_CurToken_Dw)
		{
		case TOKENR_SCENE_FIRSTFRAME:
			lexer->GetWORD(m_wFirstFrame); break;

		case TOKENR_SCENE_LASTFRAME:
			lexer->GetWORD(m_wLastFrame); break;

		case TOKENR_SCENE_FRAMESPEED:
			lexer->GetWORD(m_wFrameSpeed); break;

		case TOKENR_SCENE_TICKSPERFRAME:
			lexer->GetWORD(m_wTicksPerFrame); break;

		case TOKENR_SCENE_BACKGROUND_STATIC:
		{
			lexer->GetFloat(m_vt3BACKGROUND_STATIC.x);
			lexer->GetFloat(m_vt3BACKGROUND_STATIC.y);
			lexer->GetFloat(m_vt3BACKGROUND_STATIC.z);
		} break;

		case TOKENR_SCENE_AMBIENT_STATIC:
		{
			lexer->GetFloat(m_vt3AMBIENT_STATIC.x);
			lexer->GetFloat(m_vt3AMBIENT_STATIC.y);
			lexer->GetFloat(m_vt3AMBIENT_STATIC.z);
		} break;

		case TOKEND_BLOCK_START:
			//lexer->SkipCurBlock(); 
			break;
		}
	}
	return true;
}



//Face Data
//--------------------------------------------------------------------
CFace::CFace() 
{
}

CFace::~CFace()
{
}



void CFace::Decompose(STVtxData arr[])
{
	for (int i = 0; i < 3; ++i)
	{
		//get position
		arr[i].wPos = m_Pos.wIndex[i];

		//get texture
		arr[i].wTex = m_TexId.wIndex[i];

		//get normal		
		arr[i].f3Normal = m_Normal[i];
	}
}



//Ase Data
//--------------------------------------------------------------------

//public function----------------------------------------
CASEData::CASEData()
{
}

CASEData::~CASEData() 
{
	for (auto Iter : m_vecGemoObj)
	{
		SAFE_DELETE(Iter);
	}
	for (auto Iter : m_vecSkinObj)
	{
		SAFE_DELETE(Iter);
	}
	for (auto Iter : m_vecHelpObj)
	{
		SAFE_DELETE(Iter);
	}
	for (auto Iter : m_vecShapeObj)
	{
		SAFE_DELETE(Iter);
	}
	for (auto Iter : m_vecMaterial)
	{
		SAFE_DELETE(Iter);
	}
}



//Parsing All Data
bool CASEData::ParsingAll(LPCSTR FileName)
{
	CToken_ASE3D TokenDefine;
	m_plexer = new CMYLexer(&TokenDefine);
	if (!m_plexer->Open(FileName))return false;
	

	std::unordered_set<DWORD> Memo;

	Memo.insert(TOKENR_SCENE);
	Memo.insert(TOKENR_MATERIAL_LIST);
	Memo.insert(TOKENR_GEOMOBJECT);
	Memo.insert(TOKENR_HELPEROBJECT);
	Memo.insert(TOKENR_SHAPEOBJECT);
	Memo.insert(TOKENR_LIGHTOBJECT);
	Memo.insert(TOKENR_CAMERAOBJECT);

	while (m_plexer->FindToken_UseTable(Memo))
	{
		switch (CMYLexer::s_CurToken_Dw)
		{
			case TOKENR_SCENE:				m_SceneInfo.parse(m_plexer); break;
			case TOKENR_MATERIAL_LIST:		ParseMaterial(); break;
			case TOKENR_GEOMOBJECT:			ParseGemoObject();	break;
			case TOKENR_HELPEROBJECT:		ParseHelperObject(); break;
			case TOKENR_SHAPEOBJECT:		ParseShapeObject(); break;
		}
	}


	SAFE_DELETE(m_plexer);

	return true;
}

bool CASEData::CreateD3DMat(std::vector<OBJ::STMaterial*>&vecd3Mat, LPCSTR FileDir)
{
	vecd3Mat.resize(m_vecMaterial.size(), nullptr);

	for (int i = 0; i < m_vecMaterial.size(); ++i)
	{
		m_vecMaterial[i]->CreateD3DMat(FileDir, &vecd3Mat[i], m_vecMaterial[i]);
	}

	return true;
}

STScene* CASEData::GetSceneData()
{
	return &m_SceneInfo;
}




//private function------------------------------

void CASEData::ParseGemoObject()
{
	STGEOMObject*GeoObj = new STGEOMObject;

	GeoObj->Parse(m_plexer);

	if (GeoObj->m_Mesh.m_vecWeight.size())
		m_vecSkinObj.push_back(GeoObj);
	else
		m_vecGemoObj.push_back(GeoObj);

	//create inheritTable
	m_InheritData.InsertData(GeoObj->m_strNodeParent, GeoObj->m_strNodeName);
}

void CASEData::ParseShapeObject() 
{
	STShapeObject*ShapeObj = new STShapeObject;

	ShapeObj->Parse(m_plexer);

	m_vecShapeObj.push_back(ShapeObj);

	//create inheritTable
	m_InheritData.InsertData(ShapeObj->m_strNodeParent, ShapeObj->m_strNodeName);
}

void CASEData::ParseHelperObject()
{
	STHelperObject*Helper = new STHelperObject;

	Helper->Parse(m_plexer);

	m_vecHelpObj.push_back(Helper);

	//create inheritTable
	m_InheritData.InsertData(Helper->m_strNodeParent, Helper->m_strNodeName);
}

bool CASEData::ParseMaterial()
{
	//move to block start
	m_plexer->MovetoBlockStart();

	//read material count
	DWORD Matcount = 0;
	m_plexer->FindToken(TOKENR_MATERIAL_COUNT);
	m_plexer->GetDWORD(Matcount);
	m_vecMaterial.resize(Matcount, nullptr);

	WORD matID;
	while (m_plexer->FindToken_InBlock(TOKENR_MATERIAL))
	{
		m_plexer->GetWORD(matID);

		m_vecMaterial[matID] = new STMaterial();
		m_vecMaterial[matID]->parse(m_plexer);
	}
	return TRUE;
}

