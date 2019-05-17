// HelloWorld.cpp : Defines the entry point for the application.
//
#include "stdafx.h"

#include "HelloWorld.h"
#include "Global.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//_CrtSetBreakAlloc(251);
    //_CrtSetBreakAlloc(1399);
	//_CrtSetBreakAlloc(1369);



	//Init System
	g_upSystem = std::make_unique<CSystem>();

	if (!g_upSystem->Init(hInstance, nCmdShow))
		return 0;

	//Run System 
	g_upSystem->Run();

	return 0;
}