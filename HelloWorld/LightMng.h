//#pragma once
//class CLightMng
//{
//public:
//	CLightMng();
//	~CLightMng();
//
//private:
//	bool Init();
//
//	void GetLight()const;
//
//	void SetDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color) 
//	{
//		::ZeroMemory(&m_d3dLight, sizeof(m_d3dLight));
//		 
//		m_d3dLight.Type = D3DLIGHT_DIRECTIONAL;
//		m_d3dLight.Ambient = *color * 0.6f;
//		m_d3dLight.Diffuse = *color;
//		m_d3dLight.Specular = *color * 0.6f;
//		m_d3dLight.Direction = *direction;	
//	}
//
//	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
//	{
//		
//	}
//
//	D3DLIGHT9 m_d3dLight;
//
//
//};
//
//CLightMng::CLightMng()
//{
//}
//
//CLightMng::~CLightMng()
//{
//}