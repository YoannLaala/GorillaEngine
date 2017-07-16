#ifndef _WEB_WEB_BROWSER_HPP_
#define _WEB_WEB_BROWSER_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>

/******************************************************************************
**	Defines
******************************************************************************/
#if !defined(DEBUG)
	#define LOG_WEB_ENABLED
#endif

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Web
{
	class WebPage;
	class WebBrowserHandle;
}}

namespace Gorilla { namespace Renderer
{
	class Renderer;
	class Texture2D;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Web
{
	class WebBrowser
	{
		friend class WebBrowserHandle;
		friend class WebPage;

		struct EProcessMessage
		{
			enum Type
			{
				CreateCallback = 0,
				ExecuteCallback,
				ExecuteFunction,
			};
		};

	public:
		WebBrowser(Renderer::Renderer* _pRenderer = nullptr);
		~WebBrowser();

		WebPage*		Open					(const char* _szUrl, Renderer::Texture2D* _pTexture = nullptr);
		WebPage*		Open					(const char* _szUrl, uint32 _uiWidth, uint32 _uiHeight, bool _bTransparent);
		WebPage*		Open					(uint32 _uiWidth, uint32 _uiHeight, bool _bTransparent);							// Open blank page
		WebPage*		Open					(Renderer::Texture2D* _pTexture = nullptr, bool _bTransparent = false);						// Open blank page
		void			Close					(WebPage* _pPage);

	private:
		Vector<WebPage*>				m_vPage;
		Vector<Renderer::Texture2D*>	m_vTexture;
		Renderer::Renderer*				m_pRenderer;
		WebBrowserHandle*				m_pHandle;
	};
}}
#endif