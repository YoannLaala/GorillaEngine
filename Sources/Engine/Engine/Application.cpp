/******************************************************************************
**	Includes
******************************************************************************/
#include "Application.hpp"


#include <Windows.h>

#include <Core/Helper/Window.hpp>

#include <Engine/Engine.hpp>
#include <Engine/View.hpp>
#include <Engine/Asset/AssetManager.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	//!	@brief		GetView
	//!	@date		2015-04-08
	Vector<View*> vView;
	View* GetView(Window* _pWindow)
	{
		const uint32 uiElementCount = vView.GetSize();
		for(uint32 uiElement = 0; uiElement < uiElementCount; ++uiElement)
		{
			View* pView = vView[uiElement];
			if(pView->GetWindow() == _pWindow)
			{
				return pView;
			}
		}

		return nullptr;
	}

	void Resize(Window* _pWindow)
	{
		/*uint32 uiOffsetX = _pWindow->GetOffsetX();
		uint32 uiOffsetY = _pWindow->GetOffsetY();*/
		uint32 uiWidth = _pWindow->GetWidth();
		uint32 uiHeight = _pWindow->GetHeight();
		View* pView = GetView(_pWindow);
		pView->Resize(uiWidth, uiHeight);
	}

	//!	@brief		WindowProcess
	//!	@date		2015-04-08
	int64 __stdcall WindowProcess(void* _hWindow, uint32 _uiMessage, uint64 _uiParam, int64 _iParamExt)
	{
		//LOG_PRINT("0x%04x", _uiMessage);
		switch (_uiMessage)
		{
			case WM_DESTROY:
			{
				View* pView = GetView(static_cast<Window*>(_hWindow));
				vView.Remove(pView);
				bool bQuit = pView->GetWindow() == _hWindow;
				SAFE_RELEASE_AND_DELETE(pView);

				// Quit application only if we close the main window
				if(bQuit)
				{
					PostQuitMessage(WM_QUIT);
				}
				break;
			}

			case WM_SIZE:
			{
				switch(_uiParam)
				{
					case SIZE_RESTORED:
					case SIZE_MAXIMIZED:
					{
						Resize(static_cast<Window*>(_hWindow));
						break;
					}
				}
				break;
			}

			case WM_EXITSIZEMOVE:
			{
				Resize(static_cast<Window*>(_hWindow));
				break;
			}

			default:
			{
				return ::DefWindowProc((HWND)_hWindow, _uiMessage, (WPARAM)_uiParam, (LPARAM)_iParamExt);
			}
		}

		return 0;
	}

	//!	@brief		Constructor
	//!	@date		2015-04-08
	Application::Application()
	{		
		Engine::Create();
		Engine* pEngine = GetEngine();
		pEngine->Initialize();
	}

	//!	@brief		Destructor
	//!	@date		2015-04-08
	Application::~Application()
	{
		// Clean all view
		const uint32 uiViewcount = vView.GetSize();
		for(uint32 uiView = 0; uiView < uiViewcount; ++uiView)
		{
			SAFE_RELEASE_AND_DELETE(vView[uiView]);
		}

		GetEngine()->Release();
		Engine::Destroy();
	}

	//!	@brief		Run
	//!	@date		2015-04-08
	void Application::Run()
	{
		// Create the main view
		Engine* pEngine = GetEngine();

		// Main message loop
		MSG kMessage = {0};
		while(WM_QUIT != kMessage.message)
		{
			if(PeekMessage(&kMessage, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&kMessage);
				DispatchMessage(&kMessage);
				continue;
			}

			pEngine->Update();
		}
	}

	//!	@brief		CreateView
	//!	@date		2015-04-08
	View* Application::CreateView(const char* _szName, uint32 _uiWidth, uint32 _uiHeight, uint8 _eFlag /*= 0*/)
	{
		Window* pWindow = Window::Create(_szName, _uiWidth, _uiHeight, WindowProcess);

		// Create View
		View* pView = new View();
		pView->Initialize(this, pWindow, _eFlag);		
		vView.Add(pView);

		return pView;
	}
}}