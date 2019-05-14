#pragma once

class CObject
{
public:
	CObject();
	~CObject();

	bool Init(std::string str);
	bool Init(ASEData::CASEData& asedata);
	
	bool Draw();
	
	std::vector<OBJ::STMaterial*> m_vecMat;
	std::vector<OBJ::CGemoObj*> m_vecGemo;
};