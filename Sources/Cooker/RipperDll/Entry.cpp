#include <Core/Core.hpp>
#include <Core/Thread/Thread.hpp>
#include <stdio.h>
#include <Windows.h>
#include <Core/Helper/Window.hpp>

using namespace Gorilla;

//!	@brief		FileWatcherThreadEntry
//!	@date		2015-11-21
void DllThreadEntryTest(void* /*_pData*/)
{
	//bool b = true;
	//while(b)
	{
		//::SetWindowTextA(GetActiveWindow(), "hellooooooooooooo, noob");
		//Thread::Sleep(1000);
		MessageBoxA(nullptr, "a", "b", 0);
	}
	
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	//::SetWindowTextA(GetActiveWindow(), "hellooooooooooooo, noob");
    DisableThreadLibraryCalls(static_cast<HMODULE>(hInstance));
	//MessageBoxA(nullptr, "a", "a", 0);
    switch (dwReason)
    {
		case DLL_PROCESS_ATTACH:
		{
			Gorilla::Thread::Create("GorillaRipper", &DllThreadEntryTest);
			break;
		}
    };

    return TRUE;
}