#include <windows.h>
#include "Application.hpp"

/******************************************************************************
**	Entry Point
******************************************************************************/
int WINAPI wWinMain(HINSTANCE /*_hInstance*/, HINSTANCE /*_hPrevInst*/, LPWSTR /*_lpCmdLine*/, int /*_nCmdShow*/)
{	
	Gorilla::Game::Application kApplication;
	kApplication.Run();

    return 0;
}