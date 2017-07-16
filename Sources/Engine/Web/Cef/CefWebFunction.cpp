/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Stream/MemoryReader.hpp>
#include <Core/Stream/MemoryWriter.hpp>
#include <Web/WebPage.hpp>
#include <Web/WebFunction.hpp>

#pragma warning(push)
#pragma warning(disable : 4100)
	#include <cef/include/cef_v8.h>
#pragma warning(pop)

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Web
{
	//!	@brief		WebFunctionHandle
	//!	@date		2015-12-29
	class WebFunctionHandle : CefV8Handler
	{	
		IMPLEMENT_REFCOUNTING(WebFunctionHandle);
		friend class WebFunction;
		
		//!	@brief		Constructor
		//!	@date		2015-12-29
		WebFunctionHandle(WebPage* _pPage, WebFunction* _pFunction, std::function<void (const Vector<WebValue*>&, WebValueList&)> _pCallback)
			: m_pPage(_pPage)
			, m_pFunction(_pFunction)
			, m_pCallback(_pCallback)
		{
			// Nothing to do
		}

		//!	@brief		Destructor
		//!	@date		2015-12-29
		~WebFunctionHandle()
		{
			// Nothing to do
		}

		//!	@brief		Execute
		//!	@date		2015-12-29
		virtual bool Execute(const CefString& /*_sName*/, CefRefPtr<CefV8Value> /*_pObject*/, const CefV8ValueList& _vArgument, CefRefPtr<CefV8Value>& /*_pValueOut*/, CefString& /*_sException*/) override
		{
			const uint32 uiArgumentCount = (uint32)_vArgument.size();
			m_vArgumentInput.Resize(uiArgumentCount);
			for(uint32 uiArgument = 0; uiArgument < uiArgumentCount; ++uiArgument)
			{
				CefV8Value* pCefV8Value = _vArgument[uiArgument].get();
				pCefV8Value->AddRef();
				m_vArgumentInput[uiArgument] = reinterpret_cast<WebValue*>(pCefV8Value);
			}
			m_pPage->SwitchFunctionThread(m_pFunction, WebPage::EThread::Javascript, WebPage::EThread::Main);
			//m_pCallback(vInput, *reinterpret_cast<WebValueList*>(&vCefV8Value));

			// Convert Output
			/*const uint32 uiValueCount = static_cast<uint32>(vCefV8Value.size());
			if(uiValueCount)
			{
				if(uiValueCount == 1 ) _pValueOut = vCefV8Value[0];
				else
				{
					_pValueOut = CefV8Value::CreateArray(uiValueCount);
					for(uint32 uiValue = 0; uiValue < uiValueCount; ++uiValue) _pValueOut->SetValue(uiValue, vCefV8Value[uiValue]);
				}
			}*/

			return true;
		}

		//!	@brief		PostExecute
		//!	@date		2015-12-29
		void PostExecute()
		{
			static CefV8ValueList vCefV8Value; vCefV8Value.clear();
			m_pCallback(m_vArgumentInput, *reinterpret_cast<WebValueList*>(&vCefV8Value));

			// Release Value
			const uint32 uiArgumentCount = m_vArgumentInput.GetSize();
			for(uint32 uiArgument = 0; uiArgument < uiArgumentCount; ++uiArgument)
			{
				CefV8Value* pCefV8Value = reinterpret_cast<CefV8Value*>(m_vArgumentInput[uiArgument]);
				pCefV8Value->Release();
			}
			m_vArgumentInput.Clear();
			// Convert Output
			/*const uint32 uiValueCount = static_cast<uint32>(vCefV8Value.size());
			if(uiValueCount)
			{
				if(uiValueCount == 1 ) _pValueOut = vCefV8Value[0];
				else
				{
					_pValueOut = CefV8Value::CreateArray(uiValueCount);
					for(uint32 uiValue = 0; uiValue < uiValueCount; ++uiValue) _pValueOut->SetValue(uiValue, vCefV8Value[uiValue]);
				}
			}*/
		}

	private:
		Vector<WebValue*>												m_vArgumentInput;
		std::function<void (const Vector<WebValue*>&, WebValueList&)>	m_pCallback;
		WebPage*														m_pPage;
		WebFunction*													m_pFunction;
	};

	//!	@brief		Constructor
	//!	@date		2015-12-29
	WebFunction::WebFunction(WebPage* _pPage, const char* _szPath, std::function<void (const Vector<WebValue*>&, WebValueList&)> _pCallback)
		: m_sPath(_szPath)
		, m_pHandle(nullptr)
	{
		m_pHandle = new WebFunctionHandle(_pPage, this, _pCallback);
	}

	//!	@brief		Destructor
	//!	@date		2015-12-29
	WebFunction::~WebFunction()
	{
		SAFE_DELETE(m_pHandle);
	}

	//!	@brief		OnExecuted
	//!	@date		2015-12-29
	void WebFunction::Execute()
	{
		m_pHandle->PostExecute();
	}
}}
