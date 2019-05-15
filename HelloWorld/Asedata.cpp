#include"stdafx.h"
#include"TokenDefine.h"
#include"Vertex.h"
#include<unordered_set>
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
	//Specular = MATERIAL_SPECULAR × MATERIAL_SHINESTRENGTH
	//Specular Power = MATERIAL_SHINE × 127.9961
	//Emissive = MATERIAL_SELFILLUM × (MATERIAL_DIFFUSE)
	//Alpha = 1.0f - MATERIAL_TRANSPARENCY
	//-----------------------------------------------------------

	*result = new OBJ::STMaterial();

	//Ambient = MATERIAL_AMBIENT
	(*result)->mat.Ambient = help->m_d3dcAmbient;

	//Diffuse = MATERIAL_DIFFUSE
	(*result)->mat.Diffuse = help->m_d3dcDiffuse;

	//Specular = MATERIAL_SPECULAR × MATERIAL_SHINESTRENGTH
	(*result)->mat.Specular.r = help->m_d3dcSpecular.r * help->m_fShineStrength;
	(*result)->mat.Specular.g = help->m_d3dcSpecular.g * help->m_fShineStrength;
	(*result)->mat.Specular.b = help->m_d3dcSpecular.b * help->m_fShineStrength;

	//Specular Power = MATERIAL_SHINE × 127.9961
	(*result)->mat.Power = help->m_fShine*127.9961;

	//Emissive = MATERIAL_SELFILLUM × (MATERIAL_DIFFUSE)
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

STLine::STLine()
	:
	m_bClosed(false),
	m_dwVertexCount(0),
	m_pLineVertex(nullptr)
{
}
STLine::~STLine() 
{
}

STNodeTM::STNodeTM() 
	:
	m_fTM_ROTANGLE(0),
	m_fTM_SCALEAXISANG(0)
{
}
STNodeTM::~STNodeTM()
{
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
			lexer->GetDWORD(m_dwFirstFrame); break;

		case TOKENR_SCENE_LASTFRAME:
			lexer->GetDWORD(m_dwLastFrame); break;

		case TOKENR_SCENE_FRAMESPEED:
			lexer->GetDWORD(m_dwFrameSpeed); break;

		case TOKENR_SCENE_TICKSPERFRAME:
			lexer->GetDWORD(m_dwTicksPerFrame); break;

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
			case TOKENR_SCENE: m_SceneInfo.parse(m_plexer); break;
			case TOKENR_MATERIAL_LIST:ParseMaterial(); break;
			case TOKENR_GEOMOBJECT:	ParseGemoObject();	break;
			//case TOKENR_HELPEROBJECT:	LoadHelpObject(ASE); break;
			//case TOKENR_SHAPEOBJECT:	LoadShapeObject(ASE); break;
		}
	}


	SAFE_DELETE(m_plexer);



	return true;
}


//private function------------------------------

void CASEData::ParseGemoObject()
{
	STGEOMObject*GeoObj = new STGEOMObject;

	std::unordered_set<DWORD> Memo;
	Memo.insert(TOKENR_NODE_NAME);
	Memo.insert(TOKENR_NODE_PARENT);
	Memo.insert(TOKENR_MATERIAL_REF);
	Memo.insert(TOKENR_NODE_TM);
	Memo.insert(TOKENR_TM_ANIMATION);
	Memo.insert(TOKENR_MESH);
	Memo.insert(TOKEND_BLOCK_START);


	m_plexer->MovetoBlockStart();

	while (m_plexer->FindToken_UseTable_Until(TOKEND_BLOCK_END,Memo))
	{
		switch (CMYLexer::s_CurToken_Dw)
		{
		case TOKENR_NODE_NAME:		m_plexer->GetString(GeoObj->m_strNodeName); break;
		case TOKENR_NODE_PARENT:	m_plexer->GetString(GeoObj->m_strNodeParent); break;
		case TOKENR_MATERIAL_REF:	m_plexer->GetWORD(GeoObj->m_wMatRef);  break;
		case TOKENR_NODE_TM:		ParseNodeTM(&GeoObj->m_NodeTM); break;
		//case TOKENR_TM_ANIMATION:	LoadAnimationTM(&GeoObj->AniTM); break;
		case TOKENR_MESH:			GeoObj->m_Mesh.ParseAll(m_plexer); break;
		case TOKEND_BLOCK_START:	m_plexer->SkipCurBlock(); break;
		}
	}
	m_vecGemoObj.push_back(GeoObj);
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

//parse Detail
bool CASEData::ParseNodeTM(STNodeTM *NodeTM)
{
	m_plexer->MovetoBlockStart();

	//Inherit Data
	//---------------------------------------------------
	m_plexer->FindToken(TOKENR_INHERIT_POS);
	m_plexer->GetFloat(NodeTM->m_vt3INHERIT_POS.x);
	m_plexer->GetFloat(NodeTM->m_vt3INHERIT_POS.z);
	m_plexer->GetFloat(NodeTM->m_vt3INHERIT_POS.y);

	m_plexer->FindToken(TOKENR_INHERIT_ROT);
	m_plexer->GetFloat(NodeTM->m_vt3INHERIT_ROT.x);
	m_plexer->GetFloat(NodeTM->m_vt3INHERIT_ROT.z);
	m_plexer->GetFloat(NodeTM->m_vt3INHERIT_ROT.y);

	m_plexer->FindToken(TOKENR_INHERIT_SCL);
	m_plexer->GetFloat(NodeTM->m_vt3INHERIT_SCL.x);
	m_plexer->GetFloat(NodeTM->m_vt3INHERIT_SCL.z);
	m_plexer->GetFloat(NodeTM->m_vt3INHERIT_SCL.y);


	//Matrix Data
	//---------------------------------------------------
	m_plexer->FindToken(TOKENR_TM_ROW0);
	m_plexer->GetFloat(NodeTM->m_vt3TM_ROW0.x);
	m_plexer->GetFloat(NodeTM->m_vt3TM_ROW0.z);
	m_plexer->GetFloat(NodeTM->m_vt3TM_ROW0.y);

	m_plexer->FindToken(TOKENR_TM_ROW1);
	m_plexer->GetFloat(NodeTM->m_vt3TM_ROW1.x);
	m_plexer->GetFloat(NodeTM->m_vt3TM_ROW1.z);
	m_plexer->GetFloat(NodeTM->m_vt3TM_ROW1.y);

	m_plexer->FindToken(TOKENR_TM_ROW2);
	m_plexer->GetFloat(NodeTM->m_vt3TM_ROW2.x);
	m_plexer->GetFloat(NodeTM->m_vt3TM_ROW2.z);
	m_plexer->GetFloat(NodeTM->m_vt3TM_ROW2.y);

	m_plexer->FindToken(TOKENR_TM_ROW3);
	m_plexer->GetFloat(NodeTM->m_vt3TM_ROW3.x);
	m_plexer->GetFloat(NodeTM->m_vt3TM_ROW3.z);
	m_plexer->GetFloat(NodeTM->m_vt3TM_ROW3.y);


	//Position Data
	//---------------------------------------------------
	m_plexer->FindToken(TOKENR_TM_POS);
	m_plexer->GetFloat(NodeTM->m_vt3TM_POS.x);
	m_plexer->GetFloat(NodeTM->m_vt3TM_POS.z);
	m_plexer->GetFloat(NodeTM->m_vt3TM_POS.y);


	//Rotation Data
	//---------------------------------------------------
	m_plexer->FindToken(TOKENR_TM_ROTAXIS);
	m_plexer->GetFloat(NodeTM->m_vt3TM_ROTAXIS.x);
	m_plexer->GetFloat(NodeTM->m_vt3TM_ROTAXIS.z);
	m_plexer->GetFloat(NodeTM->m_vt3TM_ROTAXIS.y);

	m_plexer->FindToken(TOKENR_TM_ROTANGLE);
	m_plexer->GetFloat(NodeTM->m_fTM_ROTANGLE);

	//Scale Data
	//---------------------------------------------------
	m_plexer->FindToken(TOKENR_TM_SCALE);
	m_plexer->GetFloat(NodeTM->m_vt3TM_SCALE.x);
	m_plexer->GetFloat(NodeTM->m_vt3TM_SCALE.z);
	m_plexer->GetFloat(NodeTM->m_vt3TM_SCALE.y);

	m_plexer->FindToken(TOKENR_TM_SCALEAXIS);
	m_plexer->GetFloat(NodeTM->m_vt3TM_SCALEAXIS.x);
	m_plexer->GetFloat(NodeTM->m_vt3TM_SCALEAXIS.z);
	m_plexer->GetFloat(NodeTM->m_vt3TM_SCALEAXIS.y);



	m_plexer->FindToken(TOKENR_TM_SCALEAXISANG);
	m_plexer->GetFloat(NodeTM->m_fTM_SCALEAXISANG);

	m_plexer->MovetoBlockEnd();

	return TRUE;
}




//bool CASEParser::LoadBone(CMeshData *_MeshInfo)
//{
//	if (_MeshInfo->NumOfBone == 0)return FALSE;

//	int _ID;
//	_MeshInfo->ppBoneNameList = new char*[_MeshInfo->NumOfBone];

//	FindBlockStart();
//	while (FindTokenEX(TOKENR_BONE, TOKEND_BLOCK_END))
//	{
//		FindNextToken();
//		_ID = StrToInt(g_strToken);
//		FindBlockStart();
//		while (FindTokenEX(TOKENR_BONE_NAME, TOKEND_BLOCK_END))
//		{
//			FindNextToken();
//			//read bone name 
//			_MeshInfo->ppBoneNameList[_ID] = new char[strlen(g_strToken) + 1];
//			strcpy(_MeshInfo->ppBoneNameList[_ID], g_strToken);
//		}
//	}
//	return TRUE;
//}

//bool CASEParser::LoadWeight(CMeshData *_MeshInfo)
//{
//	if (_MeshInfo->NumOfSkinWeight == 0)return FALSE;

//	int _ID;
//	_MeshInfo->pWeightList = new CMeshWeightData[_MeshInfo->NumOfSkinWeight];
//	FindBlockStart();
//	while (FindTokenEX(TOKENR_MESH_WEIGHT, TOKEND_BLOCK_END))
//	{
//		FindNextToken();
//		_ID = StrToInt(g_strToken);
//		FindBlockStart();

//		while (FindTokenEX(TOKENR_BONE_BLENGING_WEIGHT, TOKEND_BLOCK_END))
//		{
//			STMeshBlendWeightData temp;
//			//read bone ref 
//			FindNextToken();
//			temp.BoneID = StrToInt(g_strToken);

//			//read  weight
//			FindNextToken();
//			temp.Weight = StrToFloat(g_strToken);

//			_MeshInfo->pWeightList[_ID].vecMeshWeight.push_back(temp);
//		}

//		//중복 된 웨이트값 합치기_______________________________________________________________
//		for (int i = 0; i < _MeshInfo->pWeightList[_ID].vecMeshWeight.size(); i++)
//		{
//			int tempID = _MeshInfo->pWeightList[_ID].vecMeshWeight[i].BoneID;

//			for (int k = i + 1; k < _MeshInfo->pWeightList[_ID].vecMeshWeight.size();)
//			{
//				//중복 찾음_______________________________________________________
//				if (tempID == _MeshInfo->pWeightList[_ID].vecMeshWeight[k].BoneID)
//				{
//					//중복된 웨이트값 합치기
//					_MeshInfo->pWeightList[_ID].vecMeshWeight[i].Weight += _MeshInfo->pWeightList[_ID].vecMeshWeight[k].Weight;
//					//중복된 데이타 제거
//					_MeshInfo->pWeightList[_ID].vecMeshWeight.erase(_MeshInfo->pWeightList[_ID].vecMeshWeight.begin() + k);
//					continue;
//				}
//				k++;
//			}
//		}
//		//----------------------------------------------------------------------------------------

//		//가중치가 높은 4 개 웨이트값을 저장
//		if (_MeshInfo->pWeightList[_ID].vecMeshWeight.size() > 4)
//		{
//			//정열
//			sort(_MeshInfo->pWeightList[_ID].vecMeshWeight.begin(),
//				_MeshInfo->pWeightList[_ID].vecMeshWeight.end(), CompareWeight);
//			_MeshInfo->pWeightList[_ID].vecMeshWeight.resize(4);
//		}
//	}
//	return TRUE;
//}

//bool CASEParser::LoadShapeLine(CLineData *line)
//{
//	if (line == NULL)
//	{
//		return FALSE;
//	}
//	int lineID, vtID;
//	FindNextToken();
//	lineID = StrToInt(g_strToken);
//	FindBlockStart();
//	while (FindTokenUltra(4, TOKEND_BLOCK_END, TOKENR_SHAPE_CLOSED, TOKENR_SHAPE_VERTEXCOUNT, TOKENR_SHAPE_VERTEX_KNOT, TOKENR_SHAPE_VERTEX_INTERP))
//	{
//		switch (g_iToken)
//		{
//		case TOKENR_SHAPE_CLOSED:  line[lineID].closed = TRUE; break;
//		case TOKENR_SHAPE_VERTEXCOUNT:
//		{
//			FindNextToken();
//			line[lineID].VertexCount = StrToInt(g_strToken);
//			line[lineID].LineVertex = new STLineVertexData[line[lineID].VertexCount];

//		}break;
//		case TOKENR_SHAPE_VERTEX_KNOT:
//		{
//			FindNextToken();
//			vtID = StrToInt(g_strToken);
//			line[lineID].LineVertex[vtID].VertexType = VERTEX_KNOT;
//			FindNextToken();
//			line[lineID].LineVertex[vtID].Vertex.x = StrToFloat(g_strToken);
//			FindNextToken();
//			line[lineID].LineVertex[vtID].Vertex.z = StrToFloat(g_strToken);
//			FindNextToken();
//			line[lineID].LineVertex[vtID].Vertex.y = StrToFloat(g_strToken);
//			break;
//		}
//		case TOKENR_SHAPE_VERTEX_INTERP:
//		{
//			FindNextToken();
//			vtID = StrToInt(g_strToken);
//			line[lineID].LineVertex[vtID].VertexType = VERTEX_INTERP;
//			FindNextToken();
//			line[lineID].LineVertex[vtID].Vertex.x = StrToFloat(g_strToken);
//			FindNextToken();
//			line[lineID].LineVertex[vtID].Vertex.z = StrToFloat(g_strToken);
//			FindNextToken();
//			line[lineID].LineVertex[vtID].Vertex.y = StrToFloat(g_strToken);
//			break;
//		}
//		}
//	}
//	return TRUE;
//}




