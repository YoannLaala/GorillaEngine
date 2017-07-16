/******************************************************************************
**	Includes
******************************************************************************/
#include <Web/WebBrowser.hpp>

#include <Core/File/FileManager.hpp>
#include <Renderer/Renderer.hpp>

#include <Web/WebPage.hpp>

#pragma warning(push)
#pragma warning(disable : 4100)
	#include <cef/include/cef_app.h>
	#include <cef/include/cef_browser_process_handler.h>
	#include <cef/include/cef_render_process_handler.h>
#pragma warning(pop)

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Web
{
	//!	@brief		WebBrowserHandle
	//!	@date		2015-12-29
	class WebBrowserHandle : public CefApp, public CefRenderProcessHandler
	{
		IMPLEMENT_REFCOUNTING(WebBrowserHandle);

		// CefApp
		virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override { return this; }
		virtual void OnBeforeCommandLineProcessing(const CefString& /*_sProcessType*/, CefRefPtr<CefCommandLine> _hCommandLine) override
		{
			// Cf https://github.com/cztomczak/cefpython/issues/240
			_hCommandLine->AppendSwitch("disable-gpu");
			_hCommandLine->AppendSwitch("disable-d3d11");
			_hCommandLine->AppendSwitch("disable-gpu-compositing");
			_hCommandLine->AppendSwitch("disable-histogram-customizer");
			_hCommandLine->AppendSwitch("enable-begin-frame-scheduling");
			_hCommandLine->AppendSwitchWithValue("off-screen-frame-rate", "60");
		}

		bool GetOrCreateFunction(const String& sFunctionPath, CefV8Handler* pCefHandler, CefRefPtr<CefV8Value>& hCefParentValueOut, CefRefPtr<CefV8Value>& hCefValueOut)
		{
			String sObject;
			bool bContinue = true;
			uint32 uiPositionStart = 0, uiPositionEnd = 0;
			do 
			{
				uiPositionEnd = sFunctionPath.Find(".", uiPositionStart);
				uiPositionEnd = Math::Min<uint32>(uiPositionEnd, sFunctionPath.GetLength());
				sObject.Set(&sFunctionPath.GetBuffer()[uiPositionStart], uiPositionEnd - uiPositionStart);
				uiPositionStart = uiPositionEnd + 1;

				// Last part - create the function
				if(uiPositionEnd == sFunctionPath.GetLength())
				{
					if(pCefHandler) hCefValueOut = CefV8Value::CreateFunction(sObject.GetBuffer(), pCefHandler);
					else hCefValueOut = hCefParentValueOut->GetValue(sObject.GetBuffer());
					hCefParentValueOut->SetValue(sObject.GetBuffer(), hCefValueOut, V8_PROPERTY_ATTRIBUTE_NONE);
					return !hCefValueOut->IsUndefined();
				}
				else
				{
					// Create the object if undefined
					hCefValueOut = hCefParentValueOut->GetValue(sObject.GetBuffer());
					if(hCefValueOut->IsUndefined())
					{
						hCefValueOut = CefV8Value::CreateObject(nullptr, nullptr);
						hCefParentValueOut->SetValue(sObject.GetBuffer(), hCefValueOut, V8_PROPERTY_ATTRIBUTE_NONE);
					}

					if(!hCefValueOut->IsFunction())hCefParentValueOut = hCefValueOut;
					else bContinue = false;
				}

			} while (bContinue);

			return true;
		}		

		// CefRenderProcessHandler
		///
	  // Called for global uncaught exceptions in a frame. Execution of this
	  // callback is disabled by default. To enable set
	  // CefSettings.uncaught_exception_stack_size > 0.
	  ///
	  /*--cef()--*/
	  virtual void OnUncaughtException(CefRefPtr<CefBrowser> browser,
									   CefRefPtr<CefFrame> frame,
									   CefRefPtr<CefV8Context> context,
									   CefRefPtr<CefV8Exception> exception,
									   CefRefPtr<CefV8StackTrace> stackTrace) override
	  {
	  browser;
	  frame;
	  context;
	  exception;
	  stackTrace;
	  }

		virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> _hCefBrowser, CefProcessId /*_uiProcessId*/, CefRefPtr<CefProcessMessage> _hCefMessage) override
		{
			int64 myid= _hCefBrowser->GetMainFrame()->GetIdentifier(); myid;
			
			CefRefPtr<CefListValue> hValueList = _hCefMessage->GetArgumentList();
			WebBrowser::EProcessMessage::Type eMessage = (WebBrowser::EProcessMessage::Type)hValueList->GetInt(0);
			switch(eMessage)
			{
				case WebBrowser::EProcessMessage::CreateCallback:
				{
					String sFunctionPath = hValueList->GetString(1).ToString().c_str();
					CefV8Handler* pCefHandler = reinterpret_cast<CefV8Handler*>((int64)hValueList->GetDouble(2));

					CefString sname = _hCefBrowser->GetMainFrame()->GetName(); sname;
					int64 iid = _hCefBrowser->GetMainFrame()->GetIdentifier(); iid;

					// Enter Context
					CefRefPtr<CefV8Context> hCefContext = _hCefBrowser->GetMainFrame()->GetV8Context();
					hCefContext->Enter();	

						// Create Function
						CefRefPtr<CefV8Value> hCefObject = hCefContext->GetGlobal(), hCefFunction;
						GetOrCreateFunction(sFunctionPath, pCefHandler, hCefObject, hCefFunction);

					hCefContext->Exit();

					break;
				};

				case WebBrowser::EProcessMessage::ExecuteCallback:
				{
					WebValue* pCallback = (WebValue*)((int64)hValueList->GetDouble(1));

					// Enter Context
					CefRefPtr<CefV8Context> hCefContext = _hCefBrowser->GetMainFrame()->GetV8Context();
					hCefContext->Enter();	

						// Execute Function
						CefRefPtr<CefV8Value> hCefFunction = reinterpret_cast<CefV8Value*>(pCallback);
						CefRefPtr<CefV8Value> hCefObject = hCefContext->GetGlobal();
						static CefV8ValueList vArgument; vArgument.clear();
						const uint32 uiArgumentCount = static_cast<uint32>(hValueList->GetSize());
						for(uint32 uiArgument = 2; uiArgument < uiArgumentCount; ++uiArgument)
						{
							switch(hValueList->GetType(uiArgument))
							{
								case VTYPE_BOOL: vArgument.push_back(CefV8Value::CreateBool(hValueList->GetBool(uiArgument))); break;
								case VTYPE_INT: vArgument.push_back(CefV8Value::CreateInt(hValueList->GetInt(uiArgument))); break;
								case VTYPE_DOUBLE: vArgument.push_back(CefV8Value::CreateDouble(hValueList->GetDouble(uiArgument))); break;
								case VTYPE_STRING: vArgument.push_back(CefV8Value::CreateString(hValueList->GetString(uiArgument))); break;
								case VTYPE_LIST: 
								{
									CefRefPtr<CefListValue> hCefValueList = hValueList->GetList(uiArgument);
									const uint32 uiValueCount = static_cast<uint32>(hCefValueList->GetSize());
									CefRefPtr<CefV8Value> hCefV8Array = CefV8Value::CreateArray(uiValueCount);								
									for(uint32 uiValue = 0; uiValue < uiValueCount; ++uiValue)
									{
										switch(hCefValueList->GetType(uiValue))
										{
											case VTYPE_BOOL: hCefV8Array->SetValue(uiValue, CefV8Value::CreateBool(hCefValueList->GetBool(uiValue))); break;
											case VTYPE_INT: hCefV8Array->SetValue(uiValue, CefV8Value::CreateInt(hCefValueList->GetInt(uiValue))); break;
											case VTYPE_DOUBLE: hCefV8Array->SetValue(uiValue, CefV8Value::CreateDouble(hCefValueList->GetDouble(uiValue))); break;
											case VTYPE_STRING: hCefV8Array->SetValue(uiValue, CefV8Value::CreateString(hCefValueList->GetString(uiValue))); break;
										}
									}
									vArgument.push_back(hCefV8Array); break;
									break;
								}
							}
						}
							
						hCefFunction->ExecuteFunction(hCefObject, vArgument);

					hCefContext->Exit();

					break;
				};

				case WebBrowser::EProcessMessage::ExecuteFunction:
				{
					String sFunctionPath = hValueList->GetString(1).ToString().c_str();

					// Enter Context
					CefRefPtr<CefV8Context> hCefContext = _hCefBrowser->GetMainFrame()->GetV8Context();
					hCefContext->Enter();	

						// Execute Function
						CefRefPtr<CefV8Value> hCefObject = hCefContext->GetGlobal(), hCefFunction;
						if(GetOrCreateFunction(sFunctionPath, nullptr, hCefObject, hCefFunction))
						{
							static CefV8ValueList vArgument; vArgument.clear();
							const uint32 uiArgumentCount = static_cast<uint32>(hValueList->GetSize());
							for(uint32 uiArgument = 2; uiArgument < uiArgumentCount; ++uiArgument)
							{
								switch(hValueList->GetType(uiArgument))
								{
									case VTYPE_BOOL: vArgument.push_back(CefV8Value::CreateBool(hValueList->GetBool(uiArgument))); break;
									case VTYPE_INT: vArgument.push_back(CefV8Value::CreateInt(hValueList->GetInt(uiArgument))); break;
									case VTYPE_DOUBLE: vArgument.push_back(CefV8Value::CreateDouble(hValueList->GetDouble(uiArgument))); break;
									case VTYPE_STRING: vArgument.push_back(CefV8Value::CreateString(hValueList->GetString(uiArgument))); break;
								}
							}
							
							hCefFunction->ExecuteFunction(hCefObject, vArgument);
						}

					hCefContext->Exit();

					break;
				};
			}
		
			return true;
		}
	};

	//!	@brief		Constructor
	//!	@date		2015-12-29
	WebBrowser::WebBrowser(Renderer::Renderer* _pRenderer /*= NULL*/)
		: m_pRenderer(_pRenderer)
	{	
		m_pHandle = new WebBrowserHandle();

		// Prepare Cef Settings
		CefMainArgs kCefMainArg;
		CefSettings kCefSetting;		
		kCefSetting.single_process = true;
		kCefSetting.multi_threaded_message_loop = false;
		kCefSetting.windowless_rendering_enabled = true;
		kCefSetting.command_line_args_disabled = true;

	#if defined(LOG_WEB_ENABLED)
		kCefSetting.log_severity = LOGSEVERITY_VERBOSE;
	#else
		kCefSetting.log_severity = LOGSEVERITY_DISABLE;
	#endif

		// User Agent
		cef_string_from_ascii("Gorilla", 7, &kCefSetting.user_agent);

		// Resource Directory
		String sDirectory;
		FileManager::GetDirectory(FileManager::Directory::Executable, sDirectory);
		sDirectory.Append("Resources\\");
		cef_string_from_ascii(sDirectory.GetBuffer(), sDirectory.GetLength(), &kCefSetting.resources_dir_path);

		// Locales Directory
		sDirectory.Append("locales\\");
		cef_string_from_ascii(sDirectory.GetBuffer(), sDirectory.GetLength(), &kCefSetting.locales_dir_path);

		// Initialize CEF
		CefInitialize(kCefMainArg, kCefSetting, m_pHandle, nullptr);
	}

	//!	@brief		Destructor
	//!	@date		2015-12-29
	WebBrowser::~WebBrowser()
	{
		CefShutdown();
		SAFE_DELETE(m_pHandle);
	}

	//!	@brief		Open
	//!	@date		2016-01-30
	WebPage* WebBrowser::Open(const char* _szUrl, Renderer::Texture2D* _pTexture /*= NULL*/)
	{
		WebPage* pPage = Open(_pTexture);
		pPage->Open(_szUrl);
		
		return pPage;
	}

	//!	@brief		Open
	//!	@date		2016-01-30
	WebPage* WebBrowser::Open(const char* _szUrl, uint32 _uiWidth, uint32 _uiHeight, bool _bTransparent)
	{
		WebPage* pPage = Open(_uiWidth, _uiHeight, _bTransparent);
		pPage->Open(_szUrl);

		return pPage;
	}

	//!	@brief		Open
	//!	@date		2016-01-30
	WebPage* WebBrowser::Open(uint32 _uiWidth, uint32 _uiHeight, bool _bTransparent)
	{
		Renderer::Texture2D* pTexture = m_pRenderer->CreateTexture2D(_uiWidth, _uiHeight, 1, Renderer::EFormat::B8G8R8A8_UNORM, Renderer::EBind::ShaderResource);
		return Open(pTexture, _bTransparent);
	}

	//!	@brief		Open
	//!	@date		2015-12-29
	WebPage* WebBrowser::Open(Renderer::Texture2D* _pTexture /*= NULL*/, bool _bTransparent)
	{
		// Create Texture if needed
		if(!_pTexture)
		{	
			_pTexture = m_pRenderer->CreateTexture2D(512, 512, 1, Renderer::EFormat::B8G8R8A8_UNORM, Renderer::EBind::ShaderResource);
			m_vTexture.Add(_pTexture);
		}

		// Create WebPage
		WebPage* pWebPage = new WebPage();
		pWebPage->Initialize(_pTexture, _bTransparent);
		m_vPage.Add(pWebPage);
		
		return pWebPage;
	}

	//!	@brief		Close
	//!	@date		2015-12-29
	void WebBrowser::Close(WebPage* _pPage)
	{
		m_vPage.Remove(_pPage);
		SAFE_RELEASE_AND_DELETE(_pPage);
	}
}}