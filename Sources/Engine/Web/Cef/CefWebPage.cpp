/******************************************************************************
**	Includes
******************************************************************************/
#include <Web/WebPage.hpp>
#include <Web/WebBrowser.hpp>
#include <Web/WebValue.hpp>

#include <Core/File/FileManager.hpp>
#include <Core/Input/Input.hpp>
#include <Engine/Engine.hpp>

#include <Renderer/Renderer.hpp>
#include <Renderer/Resource/Texture2D.hpp>

#pragma warning(push)
#pragma warning(disable : 4100)
	#include <cef/include/cef_app.h>
	#include <cef/include/cef_client.h>
	#include <cef/include/cef_life_span_handler.h>
	#include <cef/include/cef_load_handler.h>
	#include <cef/include/cef_render_handler.h>
	#include <cef/include/cef_display_handler.h>
	#include <cef/include/cef_dialog_handler.h>
	#include <cef/include/cef_jsdialog_handler.h>

	#include <cef/include/cef_task.h>
	#include <cef/include/cef_v8.h>
#pragma warning(pop)

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Web
{
	//!	@brief		WebFunctionHandle
	//!	@date		2015-12-29
	class WebFunctionHandle2 : CefV8Handler
	{	
		IMPLEMENT_REFCOUNTING(WebFunctionHandle2);
		friend class WebFunction;
		
	public:
		//!	@brief		Constructor
		//!	@date		2015-12-29
		WebFunctionHandle2(WebPageHandle* _pPage, std::function<void (const WebArgument&, WebValueList&)> _pCallback, bool _bSynchronous)
			: m_pPage(_pPage)
			, m_pCallback(_pCallback)
			, m_bSynchronous(_bSynchronous)
		{
			// Nothing to do
		}

		//!	@brief		Destructor
		//!	@date		2015-12-29
		~WebFunctionHandle2()
		{
			// Nothing to do
		}

		//!	@brief		Execute
		//!	@date		2015-12-29
		bool Execute(const CefString& /*_sName*/, CefRefPtr<CefV8Value> /*_pObject*/, const CefV8ValueList& _vArgument, CefRefPtr<CefV8Value>& /*_pValueOut*/, CefString& /*_sException*/) override;

		//!	@brief		PostExecute
		//!	@date		2015-12-29
		void PostExecute();

	private:
		WebArgument														m_vArgument;
		std::function<void (const WebArgument&, WebValueList&)>			m_pCallback;
		WebPageHandle*													m_pPage;
		bool															m_bSynchronous;
	};

	//!	@brief		WebPageHandle
	//!	@date		2015-12-29
	class WebPageHandle : public CefClient, public CefLifeSpanHandler, public CefLoadHandler, public CefRenderHandler, public CefDisplayHandler, public CefContextMenuHandler, public CefDialogHandler, public CefJSDialogHandler, public IInputListener
	{
		struct EFunction
		{
			enum Type : uint8
			{
				All = 0,
				Execute,
				Count,
			};
		};

		IMPLEMENT_REFCOUNTING(WebPageHandle);
		friend class WebPage;

		//!	@brief		Constructor
		//!	@date		2015-12-29
		WebPageHandle(WebPage* _pPage)
			: m_pPage(_pPage)
		{
			// Nothing to do
		}

		//!	@brief		Destructor
		//!	@date		2015-12-29
		~WebPageHandle()
		{
			// Nothing to do
		}

		//!	@brief		Initialize
		//!	@date		2015-12-29
		void Initialize(bool /*_bTransparent*/)
		{
			// Create Browser
			CefWindowInfo kCefWindowInfo;
			kCefWindowInfo.SetAsWindowless(nullptr, true);
			CefBrowserSettings kCefBrowserSetting;
			CefBrowserHost::CreateBrowserSync(kCefWindowInfo, this, "", kCefBrowserSetting, nullptr);

			// Add Listener for inputs
			Input::GetInstance()->AddListener(this);
		}

		//!	@brief		Release
		//!	@date		2015-12-29
		void Release()
		{
			Input::GetInstance()->RemoveListener(this);
		}

		//CefClient
		virtual CefRefPtr<CefLifeSpanHandler>	GetLifeSpanHandler		() override { return this; }
		virtual CefRefPtr<CefLoadHandler>		GetLoadHandler			() override { return this; }
		virtual CefRefPtr<CefRenderHandler>		GetRenderHandler		() override { return this; }
		virtual CefRefPtr<CefContextMenuHandler>GetContextMenuHandler	() override { return this; }
		virtual CefRefPtr<CefDialogHandler>		GetDialogHandler		() override { return this; }
		virtual CefRefPtr<CefDisplayHandler>	GetDisplayHandler		() override { return this; }
		virtual CefRefPtr<CefJSDialogHandler>	GetJSDialogHandler		() override { return this; }
		
		// CefContextMenuHandler
		  ///
		  // Called to allow custom display of the context menu. |params| provides
		  // information about the context menu state. |model| contains the context menu
		  // model resulting from OnBeforeContextMenu. For custom display return true
		  // and execute |callback| either synchronously or asynchronously with the
		  // selected command ID. For default display return false. Do not keep
		  // references to |params| or |model| outside of this callback.
		  ///
		  /*--cef()--*/
		  virtual bool RunContextMenu(CefRefPtr<CefBrowser> /*browser*/,
									  CefRefPtr<CefFrame> /*frame*/,
									  CefRefPtr<CefContextMenuParams> /*params*/,
									  CefRefPtr<CefMenuModel> /*model*/,
									  CefRefPtr<CefRunContextMenuCallback> /*callback*/) override
		  {
			  return true;
		  }


		inline void Open(const char* _szUrl) 
		{  
			if(_szUrl) m_hCefBrowser->GetMainFrame()->LoadURL(_szUrl); 
		}

		

		Vector<CefRefPtr<CefProcessMessage>> vMessage;
		bool bContextCreated = false;
		void Update()
		{
			if(bContextCreated)
			{
				static Vector<CefRefPtr<WebFunctionHandle2>> vFunction; 

				// sync
				m_mFunction.Lock();
				vFunction = m_aFunction[EFunction::Execute];
				m_aFunction[EFunction::Execute].Clear();
				m_mFunction.Unlock();

				// Execute
				const uint32 uiFunctionCount = vFunction.GetSize();
				for(uint32 uiFunction = 0; uiFunction < uiFunctionCount; ++uiFunction)
				{
					CefRefPtr<WebFunctionHandle2> hFunction = vFunction[uiFunction];
					hFunction->PostExecute();
				}
				vFunction.Clear();

				uint32 count = vMessage.GetSize();
				for(uint32 i = 0; i < count; ++i)
				{
					m_hCefBrowser->SendProcessMessage(PID_RENDERER, vMessage[i]);
					vMessage.Clear();
				}
			}
			CefDoMessageLoopWork();
		}

		CefRefPtr<CefProcessMessage> PushMessage(uint32 _eProcessMessage)
		{
			CefRefPtr<CefProcessMessage> hCefProcessMessage = CefProcessMessage::Create("Generic");
			CefRefPtr<CefListValue> hCefValueList = hCefProcessMessage->GetArgumentList();
			hCefValueList->SetInt(0, _eProcessMessage);		
			vMessage.Add(hCefProcessMessage);

			return hCefProcessMessage;
		}

		/*void PushMessage(CefRefPtr<CefProcessMessage> hCefProcessMessage)
		{
			m_hCefBrowser->SendProcessMessage(PID_RENDERER, hCefProcessMessage);
		}*/

		// CefLifeSpanHandler
		virtual void OnAfterCreated(CefRefPtr<CefBrowser> _hBrowser) override
		{
			m_hCefBrowser = _hBrowser;
		}

		virtual bool OnBeforePopup(CefRefPtr<CefBrowser> /*browser*/,
                             CefRefPtr<CefFrame> /*frame*/,
                             const CefString& /*target_url*/,
                             const CefString& /*target_frame_name*/,
                             WindowOpenDisposition /*target_disposition*/,
                             bool /*user_gesture*/,
                             const CefPopupFeatures& /*popupFeatures*/,
                             CefWindowInfo& /*windowInfo*/,
                             CefRefPtr<CefClient>& /*client*/,
                             CefBrowserSettings& /*settings*/,
                             bool* /*no_javascript_access*/) 
		{
			return false;
		}

		// CefRenderHandler
		//virtual bool GetScreenInfo(CefRefPtr<CefBrowser> _kBrowser, CefScreenInfo& _kScreenInfo) override;
		virtual bool GetViewRect(CefRefPtr<CefBrowser> _kBrowser, CefRect& _kRect) override
		{
			_kRect.Set(0, 0, m_pPage->m_pTexture->GetWidth(), m_pPage->m_pTexture->GetHeight()); 
			return true;
		};

		virtual void OnPaint(CefRefPtr<CefBrowser> /*_kBrowser*/, PaintElementType /*_ePaint*/, const RectList& /*_vRect*/, const void* _pBuffer, int /*_iWidth*/, int /*_iHeight*/) override
		{
			m_pPage->m_pBuffer = _pBuffer;
			m_pPage->SetFlag(WebPage::EFlag::Changed);
		}

		//!	@brief		OnCursorChange
		//!	@detail		Apply new cursor
		//!	@date		2015-04-04
		virtual void OnCursorChange(CefRefPtr<CefBrowser> /*_hCefBrowser*/, CefCursorHandle _hCursor, CursorType /*_eType*/, const CefCursorInfo& /*_kCefCursorInfo*/) override
		{
			::SetCursor(_hCursor);
		}


		virtual bool OnFileDialog(CefRefPtr<CefBrowser> /*browser*/, CefDialogHandler::FileDialogMode mode, const CefString& title, const CefString& default_file_path, const std::vector<CefString>& accept_filters, int selected_accept_filter, CefRefPtr<CefFileDialogCallback> callback) 
		{
			mode;
			title;
			default_file_path;
			accept_filters;
			selected_accept_filter;
			/*CefRefPtr<FileDialogCallback> hFileDialogCallback = new FileDialogCallback(nullptr, nullptr);
			String sDirectory;
			FileManager::GetDirectory(FileManager::Directory::Executable, sDirectory);
			m_hCefBrowser->GetHost()->RunFileDialog(FILE_DIALOG_OPEN_FOLDER, "test", sDirectory.GetBuffer(), accept_filters, selected_accept_filter, hFileDialogCallback);*/

			return false;
		}

		virtual bool OnJSDialog(CefRefPtr<CefBrowser> /*browser*/,
                          const CefString& /*origin_url*/,
                          const CefString& /*accept_lang*/,
                          JSDialogType /*dialog_type*/,
                          const CefString& /*message_text*/,
                          const CefString& /*default_prompt_text*/,
                          CefRefPtr<CefJSDialogCallback> /*callback*/,
                          bool& /*suppress_message*/) 
		{
			return false;
		}

		
	  virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> /*_hCefBrowser*/, const CefString& _sMessage, const CefString& /*_sSource*/, int /*_iLine*/) override
	  { 
		  _sMessage;
		  LOG_ERROR(_sMessage.ToString().c_str())
		  return false; 
	  }

	  virtual void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transition_type)override
	  {
	    browser;
		  frame;
		  transition_type;
		    bContextCreated = true;
	  }

  ///
  // Called when the browser is done loading a frame. The |frame| value will
  // never be empty -- call the IsMain() method to check if this frame is the
  // main frame. Multiple frames may be loading at the same time. Sub-frames may
  // start or continue loading after the main frame load has ended. This method
  // will not be called for same page navigations (fragments, history state,
  // etc.) or for navigations that fail or are canceled before commit. For
  // notification of overall browser load status use OnLoadingStateChange
  // instead.
  ///
  /*--cef()--*/
  virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                         CefRefPtr<CefFrame> frame,
                         int httpStatusCode) override
  
  {
  browser;
		  frame;
		  httpStatusCode;
		 
  }

	  virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           ErrorCode errorCode,
                           const CefString& errorText,
                           const CefString& failedUrl) override 
	  
	  {
	   browser;
		  frame;
		  errorCode;
		  errorText;
		  failedUrl;
	  }

		// IInputListener
		virtual void OnMouseChanged(const Mouse* _pMouse, EEvent::Type _eEvent, Mouse::Type _eButton) override
		{
			if(m_hCefBrowser)
			{
				m_kCefMouseEvent.x = (uint32)_pMouse->GetPosition().GetX();
				m_kCefMouseEvent.y = (uint32)_pMouse->GetPosition().GetY();
				switch(_eEvent)
				{
					case IInputListener::EEvent::Wheel:
					{
						m_hCefBrowser->GetHost()->SendMouseWheelEvent(m_kCefMouseEvent, 0, (uint32)_pMouse->GetWheel());
						break;
					}

					case IInputListener::EEvent::Move:
					{
						m_hCefBrowser->GetHost()->SendMouseMoveEvent(m_kCefMouseEvent, false);
						break;
					}

					case IInputListener::EEvent::Button:
					{
						static CefBrowserHost::MouseButtonType aButton[Mouse::Count] = 
						{
							CefBrowserHost::MouseButtonType::MBT_LEFT,
							CefBrowserHost::MouseButtonType::MBT_RIGHT,
							CefBrowserHost::MouseButtonType::MBT_MIDDLE,
						};

						m_hCefBrowser->GetHost()->SendMouseClickEvent(m_kCefMouseEvent, aButton[_eButton], _pMouse->IsReleased(_eButton), 1);
						break;
					}
				}
			}
		}
		
		virtual void OnKeyboardChanged(Keyboard::Type _eKey, Keyboard::EKeyState::Type _eState) override
		{
			CefKeyEvent event;
			event.windows_key_code = _eKey;
			event.native_key_code = 0;
			event.is_system_key = 0;

			// Modifiers
			event.modifiers = 0;
			Input* pInput = Input::GetInstance();
			if (pInput->IsKeyPressed(Keyboard::Shift)) event.modifiers |= EVENTFLAG_SHIFT_DOWN;
			if (pInput->IsKeyPressed(Keyboard::Control)) event.modifiers |= EVENTFLAG_CONTROL_DOWN;
			if (pInput->IsKeyPressed(Keyboard::Alternative)) event.modifiers |= EVENTFLAG_ALT_DOWN;
			if (::GetKeyState(VK_NUMLOCK) & 1) event.modifiers |= EVENTFLAG_NUM_LOCK_ON;
			if (::GetKeyState(VK_CAPITAL) & 1) event.modifiers |= EVENTFLAG_CAPS_LOCK_ON;

			// Type
			switch(_eState)
			{
				case Keyboard::EKeyState::Down:
				{
					event.type = KEYEVENT_KEYDOWN;

					bool bIsCharacater = !((_eKey >= Keyboard::BackSpace && _eKey <= Keyboard::Down) || (_eKey >= Keyboard::F1 && _eKey <= Keyboard::F12)) || _eKey == Keyboard::Space;
					if (bIsCharacater && (!pInput->IsKeyPressed(Keyboard::Control) || pInput->IsKeyPressed(Keyboard::Alternative)))
					{
						// Convert to ascii character
						static unsigned char aKeyboardState[256];
						::GetKeyboardState(aKeyboardState);
						::ToAscii(_eKey, ::MapVirtualKey(_eKey, MAPVK_VK_TO_VSC), aKeyboardState, (unsigned short*)&event.windows_key_code, 0);
						event.type = KEYEVENT_CHAR;
					}
					break;
				}

				case Keyboard::EKeyState::Up: event.type = KEYEVENT_KEYUP; break;
			}
			m_hCefBrowser->GetHost()->SendKeyEvent(event);
		}

		class FileDialogCallback : public CefRunFileDialogCallback
		{
			IMPLEMENT_REFCOUNTING(FileDialogCallback);
		public:

			inline void SetCallback(std::function<void (const Vector<String>&)> _pCallback) { m_pCallback = _pCallback; }

			virtual void OnFileDialogDismissed(int /*_iFilter*/, const std::vector<CefString>& _vFile) override
			{
				m_vFile.Clear();

				// Build list of file
				const uint32 uiFileCount = (uint32)_vFile.size();
				m_vFile.Reserve(uiFileCount);
				for(uint32 uiFile = 0; uiFile < uiFileCount; ++uiFile)
				{
					String& sFilePath = m_vFile.Add();
					sFilePath.Set(_vFile[uiFile].ToString().c_str());
					if(FileManager::IsDirectoryExist(sFilePath.GetBuffer()))
					{
						sFilePath.Append(REPERTORY_SEPARATOR);
					}
				}
				m_pCallback(m_vFile);
			}
		private:
			Vector<String> m_vFile;
			std::function<void (const Vector<String>&)> m_pCallback;
		};

		void CreateCallback(const char* _szFunctionPath, std::function<void (const WebArgument&, WebValueList&)> _pCallback, bool _bSynchronous)
		{
			WebFunctionHandle2* pFunction = new WebFunctionHandle2(this, _pCallback, _bSynchronous);
			m_aFunction[EFunction::All].Add(pFunction);

			CefRefPtr<CefProcessMessage> hCefProcessMessage = PushMessage(0);
			CefRefPtr<CefListValue> hCefValueList = hCefProcessMessage->GetArgumentList();
			hCefValueList->AddRef();
			hCefValueList->SetString(1, _szFunctionPath);
			hCefValueList->SetDouble(2, (float64)(int64)pFunction);
		}

	public:
		void OnJavascriptTrigered(WebFunctionHandle2* _pFunction)
		{
			Vector<CefRefPtr<WebFunctionHandle2>>& vFunction = m_aFunction[EFunction::Execute];
			m_mFunction.Lock();
			vFunction.Add(_pFunction);
			m_mFunction.Unlock();
		}

		//!	@brief		OpenFileDialog
		//!	@date		2015-12-05
		void OpenFileDialog(WebPage::EDialog::Type _eDialog, std::function<void (const Vector<String>&)> _pCallback, const char* _szTitle /*= nullptr*/, const char* _szFilter /*= nullptr*/, const char* _szDefaultPath /*= nullptr*/)
		{
			static FileDialogCallback kFileDialogCallback;
			static std::vector<CefString> vExtension; vExtension.clear();
			if(_szFilter)
			{
				const char* pCursor = _szFilter;
				const char* pStart = _szFilter;
				const char* pEnd = _szFilter + strlen(_szFilter);
				while (pCursor != pEnd)
				{
					++pCursor;
					if(*pCursor == ',' || pCursor == pEnd)
					{
						static String sExtension;
						sExtension.Set(pStart, (uint32)(pCursor - pStart));
						vExtension.push_back(sExtension.GetBuffer());
						pStart = pCursor + 1;
					}
				}
			}

			kFileDialogCallback.AddRef();
			kFileDialogCallback.SetCallback(_pCallback);
			m_hCefBrowser->GetHost()->RunFileDialog((CefBrowserHost::FileDialogMode)_eDialog, _szTitle, _szDefaultPath, vExtension, 0, &kFileDialogCallback);
		}

	private:
		WebPage*								m_pPage;
		CefMouseEvent							m_kCefMouseEvent;
		CefRefPtr<CefBrowser>					m_hCefBrowser;
		Vector<CefRefPtr<CefProcessMessage>>	m_vCefMessage;
		Vector<CefRefPtr<WebFunctionHandle2>>	m_aFunction[EFunction::Count];
		Mutex									m_mFunction;
	};



	//!	@brief		Execute
	//!	@date		2015-12-29
	static CefV8ValueList vCefV8Value;
	bool WebFunctionHandle2::Execute(const CefString& /*_sName*/, CefRefPtr<CefV8Value> /*_pObject*/, const CefV8ValueList& _vArgument, CefRefPtr<CefV8Value>& _pValueOut, CefString& /*_sException*/)
	{
		m_vArgument.Clear();
		const uint32 uiArgumentCount = (uint32)_vArgument.size();
		for(uint32 uiArgument = 0; uiArgument < uiArgumentCount; ++uiArgument)
		{
			CefV8Value* pCefV8Value = _vArgument[uiArgument].get();
			if(pCefV8Value->IsBool()) m_vArgument.SetBool(uiArgument, pCefV8Value->GetBoolValue());
			else if(pCefV8Value->IsInt()) m_vArgument.SetInt32(uiArgument, pCefV8Value->GetIntValue());
			else if(pCefV8Value->IsUInt()) m_vArgument.SetUInt32(uiArgument, pCefV8Value->GetUIntValue());
			else if(pCefV8Value->IsDouble()) m_vArgument.SetFloat64(uiArgument, pCefV8Value->GetDoubleValue());
			else if(pCefV8Value->IsString()) m_vArgument.SetString(uiArgument, pCefV8Value->GetStringValue().ToString().c_str());
			else if(pCefV8Value->IsFunction())
			{
				pCefV8Value->AddRef();
				m_vArgument.SetCallback(uiArgument, pCefV8Value);
			}
			else ASSERT(false, "not supported");
		}
		
		if(m_bSynchronous)
		{
			PostExecute();
			
			// Convert Output
			const uint32 uiValueCount = static_cast<uint32>(vCefV8Value.size());
			if(uiValueCount)
			{
				if(uiValueCount == 1 ) _pValueOut = vCefV8Value[0];
				else
				{
					_pValueOut = CefV8Value::CreateArray(uiValueCount);
					for(uint32 uiValue = 0; uiValue < uiValueCount; ++uiValue) _pValueOut->SetValue(uiValue, vCefV8Value[uiValue]);
				}
			}
		}
		else m_pPage->OnJavascriptTrigered(this);
		/*const uint32 uiArgumentCount = (uint32)_vArgument.size();
		m_vArgumentInput.Resize(uiArgumentCount);
		for(uint32 uiArgument = 0; uiArgument < uiArgumentCount; ++uiArgument)
		{
			CefV8Value* pCefV8Value = _vArgument[uiArgument].get();
			pCefV8Value->AddRef();
			m_vArgumentInput[uiArgument] = reinterpret_cast<WebValue*>(pCefV8Value);
		}
		m_pPage->SwitchFunctionThread(m_pFunction, WebPage::EThread::Javascript, WebPage::EThread::Main);*/
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
	void WebFunctionHandle2::PostExecute()
	{		
		vCefV8Value.clear();
		m_pCallback(m_vArgument, *reinterpret_cast<WebValueList*>(&vCefV8Value));
	}

	//!	@brief		Constructor
	//!	@date		2015-12-29
	WebPage::WebPage()
		: m_pTexture(nullptr)
		, m_szUrl(nullptr)
		, m_eFlag(0)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-12-29
	WebPage::~WebPage()
	{
		// Nothing to do
	}

	//!	@brief		Initialize
	//!	@date		2015-12-29
	void WebPage::Initialize(Renderer::Texture2D* _pTexture, bool _bTransparent)
	{		
		m_pTexture = _pTexture;
		m_pHandle = new WebPageHandle(this);
		m_pHandle->Initialize(_bTransparent);
	}

	//!	@brief		Release
	//!	@date		2015-12-29
	void WebPage::Release()
	{
		SAFE_RELEASE_AND_DELETE(m_pHandle);
	}

	//!	@brief		Open
	//!	@date		2015-12-05
	void WebPage::Open(const char* _szUrl)
	{
		m_szUrl = _szUrl;
		m_pHandle->Open(_szUrl);
	}	

	//!	@brief		Update
	//!	@date		2015-12-05
	void WebPage::Update()
	{
		m_pHandle->Update();
	}

	//!	@brief		Resize
	//!	@date		2015-12-05
	void  WebPage::Resize(uint32 _uiWidth, uint32 _uiHeight) 
	{ 
		if(m_pTexture)
		{
			if(m_pTexture->GetWidth() == _uiWidth && m_pTexture->GetHeight() == _uiHeight) return;
			Renderer::Renderer::GetInstance()->DestroyResource(m_pTexture);
		}
		m_pTexture = Renderer::Renderer::GetInstance()->CreateTexture2D(_uiWidth, _uiHeight, 1, Renderer::EFormat::B8G8R8A8_UNORM, Renderer::EBind::ShaderResource);
		m_pHandle->m_hCefBrowser->GetHost()->WasResized();
	}

	//!	@brief		CreateCallback
	//!	@date		2015-12-05
	void WebPage::CreateCallback(const char* _szPath, std::function<void (const WebArgument&, WebValueList&)> _pCallback, bool _bSynchronous /*= false*/)
	{
		//WebFunction* pFunction = new WebFunction(this, _szPath, _pCallback);

		m_pHandle->CreateCallback(_szPath, _pCallback, _bSynchronous);

		/*m_mFunction.Lock();
		m_vFunction[EThread::Javascript].Add(pFunction);
		m_mFunction.Unlock();*/

		
		//m_pHandle->PushMessage(hCefProcessMessage);
	}

	WebPage::ArgumentList* WebPage::ExecuteFunctionInternal(const char* _szPath)
	{
		CefRefPtr<CefProcessMessage> hCefProcessMessage = m_pHandle->PushMessage(WebBrowser::EProcessMessage::ExecuteFunction);		
		CefRefPtr<CefListValue> hCefValueList = hCefProcessMessage->GetArgumentList();
		hCefValueList->AddRef();
		hCefValueList->SetString(1, _szPath);
		return reinterpret_cast<WebPage::ArgumentList*>(hCefValueList.get());
	}

	WebPage::ArgumentList* WebPage::ExecuteCallbackInternal(void* _pCallback)
	{
		CefRefPtr<CefProcessMessage> hCefProcessMessage = m_pHandle->PushMessage(WebBrowser::EProcessMessage::ExecuteCallback);		
		CefRefPtr<CefListValue> hCefValueList = hCefProcessMessage->GetArgumentList();
		hCefValueList->AddRef();
		hCefValueList->SetDouble(1, (float64)(int64)(_pCallback));
		return reinterpret_cast<WebPage::ArgumentList*>(hCefValueList.get());
	}

	uint32 WebPage::ArgumentList::GetSize() const
	{
		return (uint32)((CefListValue*)this)->GetSize();
	}

	void WebPage::ArgumentList::SetBool(uint32 _uiIndex, bool _bValue)
	{
		reinterpret_cast<CefListValue*>(this)->SetBool(_uiIndex, _bValue);
	}

	void  WebPage::ArgumentList::SetArrayString(uint32 _uiIndex, const Vector<String>& _vValue)
	{
		CefRefPtr<CefListValue> pCefArgumentValue = CefListValue::Create();
		const uint32 uiValueCount = _vValue.GetSize();
		pCefArgumentValue->SetSize(uiValueCount);
		for(uint32 uiValue = 0; uiValue < uiValueCount; ++uiValue)
		{
			pCefArgumentValue->SetString(uiValue, _vValue[uiValue].GetBuffer());
		}
		reinterpret_cast<CefListValue*>(this)->SetList(_uiIndex, pCefArgumentValue);
	}

	//!	@brief		ExecuteFunction
	//!	@date		2015-12-05
	void WebPage::ExecuteFunction(const char* _szPath, const char* _szValue)
	{
		CefRefPtr<CefProcessMessage> hCefProcessMessage = m_pHandle->PushMessage(WebBrowser::EProcessMessage::ExecuteFunction);		
		CefRefPtr<CefListValue> hCefValueList = hCefProcessMessage->GetArgumentList();
		hCefValueList->SetString(1, _szPath);
		hCefValueList->SetString(2, _szValue);

		//m_pHandle->PushMessage(hCefProcessMessage);
	}

	//!	@brief		OpenFileDialog
	//!	@date		2015-12-05
	void WebPage::OpenFileDialog(WebPage::EDialog::Type _eDialog, std::function<void (const Vector<String>&)> _pCallback, const char* _szTitle /*= nullptr*/, const char* _szFilter /*= nullptr*/, const char* _szDefaultPath /*= nullptr*/)
	{
		m_pHandle->OpenFileDialog(_eDialog, _pCallback, _szTitle, _szFilter, _szDefaultPath);
	}
}}
