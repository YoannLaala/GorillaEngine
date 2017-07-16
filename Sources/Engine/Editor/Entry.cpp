#include <windows.h>
#include <Editor/Application.hpp>

/******************************************************************************
**	Entry Point
******************************************************************************/
int WINAPI wWinMain(HINSTANCE /*_hInstance*/, HINSTANCE /*_hPrevInst*/, LPWSTR /*_lpCmdLine*/, int /*_nCmdShow*/)
{	
	Gorilla::Editor::Application kApplication;
	kApplication.Run();

    return 0;
}