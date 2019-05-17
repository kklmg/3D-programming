#pragma once

class CObject
{
public:
	CObject();
	~CObject();

	bool Init(std::string str);
	bool Init(ASEData::CASEData& asedata);
	
	bool Draw();
	
	void SetTransform(D3DXMATRIX* TM);

	

private:
	D3DXMATRIX m_mtxWorld;

	std::vector<CObject*>m_pParent;
	std::vector<CObject*>m_vecChild;

	std::vector<OBJ::STMaterial*> m_vecMat;
	//std::vector<OBJ::CGemoObj*> m_vecGemo;

	std::map <std::string, OBJ::CObjNode*>m_mapObjs;

	std::vector<OBJ::CObjNode*>m_vecRoot;
};