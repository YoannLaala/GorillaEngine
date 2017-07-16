#ifndef _WEB_WEB_FUNCTION_HPP_
#define _WEB_WEB_FUNCTION_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>
#include <functional>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Web
{	
	class WebBrowser;
	class WebBrowserHandle;
	class WebPage;
	class WebValue;
	class WebValueList;
	class WebFunctionHandle;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Web
{	
	class WebFunction
	{
		friend class WebBrowser;
		friend class WebBrowserHandle;
		friend class WebPage;

	private:
		WebFunction(WebPage* _pPage, const char* _szPath, std::function<void (const Vector<WebValue*>&, WebValueList&)> _pCallback);
		~WebFunction();

		void Execute();

	private:
		inline const String& GetPath() const { return m_sPath; }
		inline WebFunctionHandle* GetHandle() { return m_pHandle; }

	private:
		String				m_sPath;
		WebFunctionHandle*	m_pHandle;
		uint8				m_eFlag;
	};
}}				

#endif