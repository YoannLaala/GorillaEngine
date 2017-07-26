/******************************************************************************
**	Includes
******************************************************************************/
#include "View.hpp"

#include <Core/Helper/Window.hpp>

#include <Engine/Engine.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/Viewport.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	//!	@brief		Constructor
	//!	@date		2015-04-08
	View::View()
		: m_pApplication(nullptr)
		, m_pWindow(nullptr)
		, m_pViewport(nullptr)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-04-08
	View::~View()
	{
		m_pApplication = nullptr;
		m_pWindow = nullptr;
		m_pViewport = nullptr;
	}

	//!	@brief		Initialize
	//!	@date		2015-04-08
	void View::Initialize(Application* _pApplication, Window* _pWindow, uint8 _eFlag)
	{
		m_pApplication = _pApplication;
		m_pWindow = _pWindow;
		m_pViewport = GetRenderer()->CreateViewport(m_pWindow, Renderer::EFormat::R8G8B8A8_UNORM, _eFlag);
	}

	//!	@brief		Release
	//!	@date		2015-04-08
	void View::Release()
	{
		GetRenderer()->DestroyViewport(m_pViewport);
	}

	//!	@brief		Show
	//!	@date		2015-04-08
	void View::Show()
	{
		m_pWindow->Show();
	}

	//!	@brief		Hide
	//!	@date		2015-04-08
	void View::Hide()
	{
		m_pWindow->Hide();
	}

	//!	@brief		Resize
	//!	@date		2015-04-08
	void View::Resize(uint32 _uiWidth, uint32 _uiHeight)
	{
		m_pViewport->Resize(_uiWidth, _uiHeight);

		/*const uint32 uiViewportCount = m_vViewport.GetSize();
		for(uint32 uiViewport = 0; uiViewport < uiViewportCount; ++uiViewport)
		{
			Renderer::Viewport* pViewport = m_vViewport[uiViewport];
			pViewport->Resize(_uiWidth, _uiHeight);
		}*/
	}
}}