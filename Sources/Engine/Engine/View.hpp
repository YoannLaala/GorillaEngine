#ifndef _ENGINE_VIEW_HPP_
#define _ENGINE_VIEW_HPP_

/******************************************************************************
**	Include
******************************************************************************/
#include <Core/Core.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla 
{
	class Window;
}

namespace Gorilla { namespace Engine
{
	class Engine;
	class Application;
}}

namespace Gorilla { namespace Renderer
{
	class Viewport;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class View
	{
		friend class Application;

	public:
		View();
		~View();

		void Initialize(Application* _pApplication, Window* _pWindow, bool _bRenderContext);
		void Release();

	public:
		void Show();
		void Hide();

		void Resize(uint32 _uiWidth, uint32 _uiHeight);

		inline Window* GetWindow() { return m_pWindow; }
		inline Application* GetApplication() { return m_pApplication; }
		inline Renderer::Viewport* GetViewport() { return m_pViewport; }

	protected:
		Application*			m_pApplication;
		Window*					m_pWindow;
		Renderer::Viewport*		m_pViewport;
	};
}}

#endif