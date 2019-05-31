#include"stdafx.h"
#include"D3DClass.h"
#include"Global.h"

CDirect3D::CDirect3D():
	m_pDevice(nullptr)
{
}

CDirect3D::~CDirect3D()
{
	SAFE_RELEASE(m_pDevice);
}

bool CDirect3D::Init()
{
	HRESULT hr;

	// Step 1: Create the IDirect3D9 object.

	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d9)
	{
		::MessageBox(0, L"Direct3DCreate9() - FAILED", 0, 0);
		return false;
	}

	// Step 2: Check for hardware vp.

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, g_DEVICETYPE, &caps);

	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.

	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = g_WD_WIDTH;
	d3dpp.BackBufferHeight = g_WD_HEIGHT;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = g_hWnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Step 4: Create the device.

	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,	 // primary adapter
		g_DEVICETYPE,        // device type
		g_hWnd,              // window associated with device
		vp,					 // vertex processing
		&d3dpp,				 // present parameters
		&m_pDevice);         // return created device

	if (FAILED(hr))
	{
		// try again using a 16-bit depth buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		hr = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			g_DEVICETYPE,
			g_hWnd,
			vp,
			&d3dpp,
			&m_pDevice);

		if (FAILED(hr))
		{
			d3d9->Release(); // done with d3d9 object
			::MessageBox(0, L"CreateDevice() - FAILED", 0, 0);
			return false;
		}
	}

	d3d9->Release(); // done with d3d9 object

	g_pDevice = m_pDevice;
	return true;
}


void CDirect3D::BeginScene() 
{
	m_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, g_BACKCOLOR, 1.0f, 0);
	m_pDevice->BeginScene();
}
void CDirect3D::EndScene() 
{
	m_pDevice->EndScene();
	m_pDevice->Present(0, 0, 0, 0);
}

IDirect3DDevice9* CDirect3D::GetDevice()const
{
	return m_pDevice;
}


void CDirect3D::SetViewTM(D3DXMATRIX* V) 
{
	m_pDevice->SetTransform(D3DTS_VIEW, V);
}


