#include"stdafx.h"
#include"TokenDefine.h"
#include"Vertex.h"

#include"Asedata.h"


#include<time.h>

ASEData::CMesh::CMesh() 
	:
	m_wNumVertex(0),
	m_wNumFace(0),
	m_wNumTexture(0),
	m_wNumTextureFace(0),
	m_wNumBone(0),
	m_wNumSkinWeight(0),
	m_dwFVF(0),
	m_wPerVtxSize(0),
	m_bNormal(false)
{
}

ASEData::CMesh::~CMesh()
{
	for (auto ptr : m_vecFace)
	{
		SAFE_DELETE(ptr);
	}
}

bool ASEData::CMesh::ParseAll(CMYLexer *lexer)
{
	lexer->MovetoBlockStart();

	std::unordered_set<DWORD> Memo;
	Memo.insert(TOKENR_MESH_NUMVERTEX);
	Memo.insert(TOKENR_MESH_NUMFACES);
	Memo.insert(TOKENR_MESH_NUMTVERTEX);
	Memo.insert(TOKENR_MESH_NUMTVFACES);
	Memo.insert(TOKENR_MESH_NUMBONE);
	Memo.insert(TOKENR_MESH_NUMSKINWEIGHT);
	Memo.insert(TOKENR_MESH_VERTEX_LIST);
	Memo.insert(TOKENR_MESH_FACE_LIST);
	Memo.insert(TOKENR_MESH_TVERTLIST);
	Memo.insert(TOKENR_MESH_TFACELIST);
	Memo.insert(TOKENR_MESH_NORMALS);
	Memo.insert(TOKENR_BONE_LIST);
	Memo.insert(TOKENR_MESH_WVERTEXS);
	Memo.insert(TOKEND_BLOCK_START);


	double __b = clock();

	while ((lexer->FindToken_UseTable_Until(TOKEND_BLOCK_END,Memo)))
	{
		switch (CMYLexer::s_CurToken_Dw)
		{
		case TOKENR_MESH_NUMVERTEX:		lexer->GetWORD(m_wNumVertex); break;
		case TOKENR_MESH_NUMFACES:		lexer->GetWORD(m_wNumFace); break;
		case TOKENR_MESH_NUMTVERTEX:	lexer->GetWORD(m_wNumTexture); break;
		case TOKENR_MESH_NUMTVFACES:	lexer->GetWORD(m_wNumTextureFace); break;
		case TOKENR_MESH_NUMBONE:		lexer->GetWORD(m_wNumBone); break;
		case TOKENR_MESH_NUMSKINWEIGHT: lexer->GetWORD(m_wNumSkinWeight); break;
		case TOKENR_MESH_VERTEX_LIST:	ParseVertex(lexer); break;
		case TOKENR_MESH_FACE_LIST:		ParseFace(lexer); break;
		case TOKENR_MESH_TVERTLIST:		ParseTexture(lexer); break;
		case TOKENR_MESH_TFACELIST:		ParseTextureFace(lexer); break;
		case TOKENR_MESH_NORMALS:		ParseNormal(lexer); break;
			//case TOKENR_BONE_LIST:			 LoadBone(_MeshInfo); break;
			//case TOKENR_MESH_WVERTEXS:		 LoadWeight(_MeshInfo); break;
		case TOKEND_BLOCK_START:		 lexer->SkipCurBlock(); break;
		}
	}
	double __e = clock();

	double delta = __e - __b;


	//check vertex type
	m_dwFVF = CheckFormat();
	switch (m_dwFVF)
	{
		case D3DFVF_XYZ:
			m_wPerVtxSize = sizeof(STVTX_P); break;

		case D3DFVF_XYZ | D3DFVF_NORMAL:
			m_wPerVtxSize = sizeof(STVTX_PN); break;

		case D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1:
			m_wPerVtxSize = sizeof(STVTX_PNT); break;

		case D3DFVF_XYZ | D3DFVF_TEX1:
			m_wPerVtxSize = sizeof(STVTX_PT); break;
	}

	return TRUE;
}

bool ASEData::CMesh::ParseVertex(CMYLexer *lexer)
{
	lexer->MovetoBlockStart();

	//no position data
	if (!m_wNumVertex)
	{
		lexer->MovetoBlockEnd();
		return FALSE;
	}
	//malloc memory 
	m_vecVertex.resize(m_wNumVertex);

	WORD _ID;
	while (lexer->FindToken_Until(TOKENR_MESH_VERTEX, TOKEND_BLOCK_END))
	{
		//Parse vertex ID
		lexer->GetWORD(_ID);

		//Parse Vertex
		lexer->GetFloat(m_vecVertex[_ID].x);
		lexer->GetFloat(m_vecVertex[_ID].z);
		lexer->GetFloat(m_vecVertex[_ID].y);
	}
	return TRUE;
}

bool ASEData::CMesh::ParseFace(CMYLexer *lexer)
{
	lexer->MovetoBlockStart();
	if (!m_wNumFace)
	{
		lexer->MovetoBlockEnd();
		return FALSE;
	}

	m_vecFace.resize(m_wNumFace, nullptr);

	WORD _ID;
	WORD GET_WORD;
	while (lexer->FindToken_Until(TOKENR_MESH_FACE, TOKEND_BLOCK_END))
	{
		lexer->GetWORD(_ID);
		m_vecFace[_ID] = new CFace();

		//Vertex A
		lexer->FindToken(TOKEND_NUMBER);
		m_vecFace[_ID]->m_Pos[eA] = Convert::SzToDWord(CMYLexer::s_CurToken_Str.data());

		//Vertex C
		lexer->FindToken(TOKEND_NUMBER);
		m_vecFace[_ID]->m_Pos[eC] = Convert::SzToDWord(CMYLexer::s_CurToken_Str.data());

		//Vertex B
		lexer->FindToken(TOKEND_NUMBER);
		m_vecFace[_ID]->m_Pos[eB] = Convert::SzToDWord(CMYLexer::s_CurToken_Str.data());

		//Parse Smoothing
		//lexer->FindToken(TOKENR_MESH_SMOOTHING);
		//lexer->GetWORD(Get_Dword);
		//m_vecFace[_ID].Smooth = GET_WORD;


		//parse material reference
		lexer->FindToken(TOKENR_MESH_MTLID);
		lexer->GetWORD(GET_WORD);
		m_vecFace[_ID]->RefMat = GET_WORD;

	}
	return TRUE;
}

bool ASEData::CMesh::ParseTexture(CMYLexer *lexer)
{
	lexer->MovetoBlockStart();
	if (!m_wNumTexture)
	{
		lexer->MovetoBlockEnd();
		return FALSE;
	}


	WORD _ID;
	m_vecTex.resize(m_wNumTexture);
	while (lexer->FindToken_Until(TOKENR_MESH_TVERT, TOKEND_BLOCK_END))
	{
		//texture ID
		lexer->GetWORD(_ID);

		//Parse texture
		lexer->GetFloat(m_vecTex[_ID].x);
		lexer->GetFloat(m_vecTex[_ID].y);
		m_vecTex[_ID].y = 1.0f - m_vecTex[_ID].y;
	}

	return TRUE;
}

bool ASEData::CMesh::ParseTextureFace(CMYLexer *lexer)
{
	lexer->MovetoBlockStart();
	if (!m_wNumTextureFace)
	{
		lexer->MovetoBlockEnd();
		return FALSE;
	}


	WORD faceID;
	STIndex3 texId3;
	while (lexer->FindToken_Until(TOKENR_MESH_TFACE, TOKEND_BLOCK_END))
	{
		//Parse face ID
		lexer->GetWORD(faceID);

		//Parse texture index
		lexer->GetWORD(m_vecFace[faceID]->m_TexId[0]);
		lexer->GetWORD(m_vecFace[faceID]->m_TexId[2]);
		lexer->GetWORD(m_vecFace[faceID]->m_TexId[1]);
	}
	return TRUE;
}

bool ASEData::CMesh::ParseNormal(CMYLexer *lexer)
{
	lexer->MovetoBlockStart();
	if (!m_wNumFace)
	{
		lexer->MovetoBlockEnd();
		return FALSE;
	}
	//m_vecNormal.resize(m_wNumFace);
	m_bNormal = true;


	WORD vtxID_A, vtxID_B, vtxID_C;
	WORD FaceID;

	while (lexer->FindToken_Until(TOKENR_MESH_FACENORMAL, TOKEND_BLOCK_END))
	{
		lexer->GetWORD(FaceID);

		//Parse Normal A
		//-------------------------------------------------------------------
		lexer->FindToken(TOKENR_MESH_VERTEXNORMAL);

		//Parse vertex ID
		lexer->GetWORD(vtxID_A);

		//parse vector3
		lexer->GetFloat(m_vecFace[FaceID]->m_Normal[0][0]);
		lexer->GetFloat(m_vecFace[FaceID]->m_Normal[0][2]);
		lexer->GetFloat(m_vecFace[FaceID]->m_Normal[0][1]);


		//Parse Normal C
		//-------------------------------------------------------------------
		lexer->FindToken(TOKENR_MESH_VERTEXNORMAL);

		//Parse vertex ID
		lexer->GetWORD(vtxID_C);

		//parse vector3
		lexer->GetFloat(m_vecFace[FaceID]->m_Normal[2][0]);
		lexer->GetFloat(m_vecFace[FaceID]->m_Normal[2][2]);
		lexer->GetFloat(m_vecFace[FaceID]->m_Normal[2][1]);



		//Parse Normal B
		//-------------------------------------------------------------------
		lexer->FindToken(TOKENR_MESH_VERTEXNORMAL);

		//Parse vertex ID
		lexer->GetWORD(vtxID_B);

		//parse vector3
		lexer->GetFloat(m_vecFace[FaceID]->m_Normal[1][0]);
		lexer->GetFloat(m_vecFace[FaceID]->m_Normal[1][2]);
		lexer->GetFloat(m_vecFace[FaceID]->m_Normal[1][1]);

	}

	return TRUE;
}

bool ASEData::CMesh::RecieveVTX_INDEX(void **Getvtx, WORD &vtxcount, WORD **Indicies)
{
	switch (m_dwFVF)
	{
	case D3DFVF_XYZ:
		return CreateVI_P(Getvtx, vtxcount, Indicies); break;

	case D3DFVF_XYZ | D3DFVF_NORMAL:
		return 	CreateVI_PN(Getvtx, vtxcount, Indicies); break;

	case D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1:
		return 	CreateVI_PNT(Getvtx, vtxcount, Indicies); break;

	case D3DFVF_XYZ | D3DFVF_TEX1:
		return 	CreateVI_PT(Getvtx, vtxcount, Indicies); break;
	case 0:return false; break;

	}



}

bool ASEData::CMesh::CreateVI_P(void **Getvtx, WORD &vtxcount, WORD **Indicies) 
{
	//Init vertex Atrray
	*Getvtx = new STVTX_P[m_vecVertex.size()];
	STVTX_P *CurVTXSlot = (STVTX_P*)*Getvtx;

	//Init Index Array
	*Indicies = new WORD[m_vecFace.size() * 3];
	WORD *CurIDSlot = *Indicies;
	WORD MAXID = 0;

	//Decompose Face
	STVtxData vtxData[3];
	int i;

	int curid = 0;
	//for each face
	for (auto face : m_vecFace)
	{
		//decompose face to three point
		face->Decompose(vtxData);

		//for each vertex
		for (i = 0; i < 3; ++i)
		{
			//fill vertex slot			
			CurVTXSlot->Pos = m_vecVertex[vtxData[i].wPos];

			++CurVTXSlot;

			//fill index slot	
			*CurIDSlot = MAXID;
			++MAXID;
		}

	}
	return true;
}
bool ASEData::CMesh::CreateVI_PN(void **Getvtx, WORD &vtxcount, WORD **Indicies) 
{
	//duplicate checker
	std::map<STVtxData, WORD,CompN>*dplChecker
		= new std::map<STVtxData, WORD,CompN>[m_vecVertex.size()];

	//Init vertex Atrray
	*Getvtx = new STVTX_PN[m_vecFace.size() * 3];
	STVTX_PN *CurVTXSlot = (STVTX_PN*)*Getvtx;

	//Init Index Array
	*Indicies = new WORD[m_vecFace.size() * 3];
	WORD *CurIDSlot = *Indicies;
	WORD MAXID = 0;

	//Decompose Face
	STVtxData vtxData[3];
	int i;

	int curid = 0;
	//for each face
	for (auto face : m_vecFace)
	{
		//decompose face to three point
		face->Decompose(vtxData);

		//for each vertex
		for (i = 0; i < 3; ++i)
		{
			//find if the vertex has already exist
			auto iter = dplChecker[vtxData[i].wPos].find(vtxData[i]);

			//case: not found
			if (iter == dplChecker[vtxData[i].wPos].end())
			{
				dplChecker[vtxData[i].wPos].insert(std::make_pair(vtxData[i], MAXID));

				//fill vertex slot			
				CurVTXSlot->Pos = m_vecVertex[vtxData[i].wPos];
				CurVTXSlot->Normal = vtxData[i].f3Normal.vt3();		
				++vtxcount;
				++CurVTXSlot;


				//fill index slot	
				*CurIDSlot = MAXID;
				++MAXID;
			}

			//case:found
			else
			{
				//fill index slot
				*CurIDSlot = iter->second;
			}
			++CurIDSlot;
		}
	}
	delete[]dplChecker;
	return true;
}
bool ASEData::CMesh::CreateVI_PNT(void **Getvtx, WORD &vtxcount, WORD **Indicies) 
{
	//duplicate checker
	std::map<STVtxData, WORD,CompNT>*dplChecker 
		= new std::map<STVtxData, WORD, CompNT>[m_vecVertex.size()];

	//Init vertex Atrray
	*Getvtx = new STVTX_PNT[m_vecFace.size() * 3];
	STVTX_PNT *CurVTXSlot = (STVTX_PNT*)*Getvtx;

	//Init Index Array
	*Indicies = new WORD[m_vecFace.size() * 3];
	WORD *CurIDSlot = *Indicies;
	WORD MAXID = 0;

	//Decompose Face
	STVtxData vtxData[3];
	int i;

	int curid = 0;
	//for each face
	for (auto face : m_vecFace)
	{
		//decompose face to three point
		face->Decompose(vtxData);

		//for each vertex
		for (i = 0; i < 3; ++i)
		{
			//find if the vertex has already exist
			auto iter = dplChecker[vtxData[i].wPos].find(vtxData[i]);

			//case: not found
			if (iter == dplChecker[vtxData[i].wPos].end())
			{
				dplChecker[vtxData[i].wPos].insert(std::make_pair(vtxData[i], MAXID));

				//fill vertex slot			
				CurVTXSlot->Pos = m_vecVertex[vtxData[i].wPos];
				CurVTXSlot->Normal = vtxData[i].f3Normal.vt3();
				CurVTXSlot->Tex = m_vecTex[vtxData[i].wTex];

				//next slot
				++CurVTXSlot;
				++vtxcount;

				//fill index slot	
				*CurIDSlot = MAXID;
				++MAXID;
			}

			//case:found
			else
			{
				//fill index slot
				*CurIDSlot = iter->second;
			}
			++CurIDSlot;
		}
	}

	delete[]dplChecker;
	return true;
}
bool ASEData::CMesh::CreateVI_PT(void **Getvtx, WORD &vtxcount, WORD **Indicies) 
{
	//duplicate checker
	std::map<STVtxData, WORD,CompT>*dplChecker 
		= new std::map<STVtxData, WORD, CompT>[m_vecVertex.size()];

	//Init vertex Atrray
	*Getvtx = new STVTX_PT[m_vecFace.size() * 3];
	STVTX_PT *CurVTXSlot = (STVTX_PT*)*Getvtx;

	//Init Index Array
	*Indicies = new WORD[m_vecFace.size() * 3];
	WORD *CurIDSlot = *Indicies;
	WORD MAXID = 0;

	//Decompose Face
	STVtxData vtxData[3];
	int i;

	int curid = 0;
	//for each face
	for (auto face : m_vecFace)
	{
		//decompose face to three point
		face->Decompose(vtxData);

		//for each vertex
		for (i = 0; i < 3; ++i)
		{
			//find if the vertex has already exist
			auto iter = dplChecker[vtxData[i].wPos].find(vtxData[i]);

			//case: not found
			if (iter == dplChecker[vtxData[i].wPos].end())
			{
				dplChecker[vtxData[i].wPos].insert(std::make_pair(vtxData[i], MAXID));

				//fill vertex slot			
				CurVTXSlot->Pos = m_vecVertex[vtxData[i].wPos];			
				CurVTXSlot->Tex = m_vecTex[vtxData[i].wTex];
				++vtxcount;
				++CurVTXSlot;

				//fill index slot	
				*CurIDSlot = MAXID;
				++MAXID;
			}

			//case:found
			else
			{
				//fill index slot
				*CurIDSlot = iter->second;
			}
			++CurIDSlot;
		}
	}
	delete[]dplChecker;
	return true;
}



DWORD ASEData::CMesh::CheckFormat()
{
	DWORD FVF = 0;
	if (m_wNumVertex)
		FVF |= D3DFVF_XYZ;
	if (m_wNumTexture)
		FVF |= D3DFVF_TEX1;
	if (m_bNormal)
		FVF |= D3DFVF_NORMAL;


	return FVF;
}


OBJ::CMesh* ASEData::CMesh::CreateMesh()
{
	WORD vtxcount = 0;
	void *Vertexies;
	WORD *Indicies;

	//get verteies and indicies
	if (!RecieveVTX_INDEX(&Vertexies, vtxcount, &Indicies))
		return nullptr;



	//create new mesh
	OBJ::CMesh *newmesh = new OBJ::CMesh();
	std::vector<OBJ::STFaceMat>& MeshFacies = newmesh->GetFaceMatVec();

	//set vertex
	newmesh->InitVBuffer(m_dwFVF, vtxcount, m_wPerVtxSize, Vertexies);

	//set Face
	WORD facecount = m_vecFace.size();
	MeshFacies.resize(facecount);

	int i = 0, j = 0;
	for (; i < facecount; ++i, j += 3)
	{
		MeshFacies[i].wMatID = m_vecFace[i]->RefMat;
		MeshFacies[i].wTexID = m_vecFace[i]->RefTex;
	}
	newmesh->InitIBuffer(Indicies);

	delete[]Vertexies;
	delete[]Indicies;

	return newmesh;
}






