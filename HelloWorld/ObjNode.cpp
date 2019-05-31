#include"stdafx.h"
#include"Vertex.h"
#include"ObjCom.h"
#include"Global.h"


OBJ::CObjNode::CObjNode() :
	m_pParent(nullptr),
	m_pAni(nullptr),
	m_TMRoot(g_IDMATRIX)
{}
OBJ::CObjNode::~CObjNode()
{
	SAFE_DELETE(m_pAni);
}

void OBJ::CObjNode::InitLocal()
{
	__InitLocalHelper(this);
}

void OBJ::CObjNode::__InitLocalHelper(CObjNode*node)
{
	D3DXMATRIX Parinv;

	//case: not root node
	if (node->m_pParent)
	{
		//compute parent inverse
		D3DXMatrixInverse(&Parinv, nullptr, &node->m_pParent->m_TMOrign);

		//Compute Local
		node->m_TMLocal = node->m_TMOrign * Parinv;

		//Compute World
		node->m_TMWorld = node->m_TMOrign;
	}
	//case: root node
	else
	{
		//local == world == original
		node->m_TMWorld = node->m_TMLocal = node->m_TMOrign;
	}


	//Compute Childern's local Transform matrix
	for (auto ptr : node->m_vecChildren)
	{
		//recursion
		__InitLocalHelper(ptr);
	}
}


std::string OBJ::CObjNode::GetNodeName() const
{
	return m_strNodeName;
}


void OBJ::CObjNode::SetNodeName(std::string& str) 
{
	m_strNodeName = str;
}
void OBJ::CObjNode::SetParNodeName(std::string& str) 
{
	m_strParentName = str;
}


void OBJ::CObjNode::SetOriginalTM(D3DXMATRIX TM) 
{
	m_TMOrign = TM;
}
void OBJ::CObjNode::SetWorldTM(D3DXMATRIX TM) 
{
	m_TMWorld = TM;
}
void OBJ::CObjNode::SetRootTM(D3DXMATRIX TM)
{
	m_TMRoot = TM;
}



void OBJ::CObjNode::__DrawHelper(CObjNode*node, std::vector<STMaterial*>&mat)
{
	//set world Trasform Matrix
	g_pDevice->SetTransform(D3DTS_WORLD, &node->m_TMWorld);

	//Draw
	node->Draw(mat);

	//Draw Children *Recursion
	for (auto ptr : node->m_vecChildren)
	{
		__DrawHelper(ptr, mat);
	}
}

bool OBJ::CObjNode::DrawAll(std::vector<STMaterial*>&mat)
{	
	__DrawHelper(this,mat);
	return true;
}


void OBJ::CObjNode::update()
{
	__updateHelper(this);
}
void OBJ::CObjNode::__updateHelper(CObjNode*node)
{
	if (node->m_pAni)
	{
		//compute real World Transform
		node->m_TMWorld = node->m_pAni->GetAniMatrix(node->m_TMLocal) * node->GetParWorld();
	}
	else
	{
		//compute real World Transform
		node->m_TMWorld = node->m_TMLocal * node->GetParWorld();
	}

	//Compute Children's World Transform
	for (auto ptr : node->m_vecChildren)
	{
		//recursion
		__updateHelper(ptr);
	}
}



D3DXMATRIX OBJ::CObjNode::GetParWorld() const
{
	if (m_pParent)
		return m_pParent->m_TMWorld;
	else
		return m_TMRoot;
}

void OBJ::CObjNode::SetAni(CAniTrack*ani)
{
	m_pAni = ani;
}

void OBJ::CObjNode::SetParent(CObjNode* Parent)
{
	m_pParent = Parent;
}
void OBJ::CObjNode::SetChildren(CObjNode* child)
{
	if (child)
		m_vecChildren.push_back(child);
}
