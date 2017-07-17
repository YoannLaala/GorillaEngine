///******************************************************************************
//**	Includes
//******************************************************************************/
//#include <Core/Helper/Window.hpp>
//#include <Core/Process/Thread.hpp>
//#include <Renderer/Renderer.hpp>
//#include <Renderer/RenderContext.hpp>
//#include <Renderer/Viewport.hpp>
//#include <Renderer/Scene.hpp>
//#include <Renderer/IRenderable.hpp>
//#include <Renderer/Material.hpp>
//
//// Resource
//#include <Renderer/Resource/RenderTarget.hpp>
//#include <Renderer/Resource/Buffer.hpp>
//#include <Renderer/Resource/SwapChain.hpp>
//#include <Renderer/Resource/Texture2D.hpp>
//#include <Renderer/Resource/Texture3D.hpp>
//#include <Renderer/Resource/Geometry.hpp>
//#include <Renderer/Resource/SamplerState.hpp>
//
//#include <Metal/Metal.H>
//
///******************************************************************************
//**	Class Definition
//******************************************************************************/
//namespace Gorilla { namespace Renderer
//{
//	//!	@brief		Constructor
//	//!	@date		2015-11-01
//	Renderer* Renderer::m_pInstance = NULL;
//	Renderer::Renderer()
//		: m_pDevice(NULL)
//		, m_pImmediateContext(NULL)
//		, m_pGizmo(NULL)
//		, m_bIsRunning(false)
//	{
//		// Set the first instance
//		if(!m_pInstance)
//		{
//			m_pInstance = this;
//		}
//	}
//
//	//!	@brief		Destructor
//	//!	@date		2015-11-01
//	Renderer::~Renderer()
//	{
//		// Nothing to do
//	}
//
//	//!	@brief		Initialize
//	//!	@date		2015-11-01
//	bool Renderer::Initialize()
//	{
//        m_pDevice = new MTLDevice();
//		//UINT uiDeviceFlag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
//		//#ifdef _DEBUG
//			//uiDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
//		//#endif
//
//		// Drivers Available
//		D3D_DRIVER_TYPE eDriverType = D3D_DRIVER_TYPE_NULL;
//		D3D_DRIVER_TYPE aDriverType[] =
//		{
//			D3D_DRIVER_TYPE_HARDWARE,
//			D3D_DRIVER_TYPE_REFERENCE,
//		};
//		UINT uiDriverTypeCount = ARRAYSIZE(aDriverType);
//
//		// Features Available
//		D3D_FEATURE_LEVEL eFeatureLevel = D3D_FEATURE_LEVEL_11_0;
//		D3D_FEATURE_LEVEL aFeatureLevel[] =
//		{
//			D3D_FEATURE_LEVEL_11_0,
//		};
//		UINT uiFeatureLevelCount = ARRAYSIZE(aFeatureLevel);
//
//		// Create Device
//		HRESULT hResult;
//		for(UINT uiDriver = 0; uiDriver < uiDriverTypeCount; ++uiDriver)
//		{
//			eDriverType = aDriverType[uiDriver];
//			hResult = D3D11CreateDevice(NULL, eDriverType, NULL, uiDeviceFlag, aFeatureLevel, uiFeatureLevelCount, D3D11_SDK_VERSION, (ID3D11Device**)&m_pDevice, &eFeatureLevel, (ID3D11DeviceContext**)&m_pImmediateContext);
//			if(SUCCEEDED(hResult))
//			{
//				break;
//			}
//		}
//
//		// Check the last result
//		if(FAILED(hResult))		
//		{
//			LOG_ERROR("[Renderer] Device Creation failed (HRESULT = 0x%0X)", hResult);
//			return false;
//		}
//
//		// Set the Frame Id
//		memset(m_aFrameId, 0, sizeof(uint8) * EThread::Count);
//
//		// Create Default SamplerState
//		m_aSampler[0] = CreateSamplerState(ESampler::Point, EAddressingMode::Wrap, EAddressingMode::Wrap, EAddressingMode::Wrap);
//		m_aSampler[1] = CreateSamplerState(ESampler::Linear, EAddressingMode::Wrap, EAddressingMode::Wrap, EAddressingMode::Wrap);
//		m_aSampler[2] = CreateSamplerState(ESampler::BiLinear, EAddressingMode::Wrap, EAddressingMode::Wrap, EAddressingMode::Wrap);
//		m_aSampler[3] = CreateSamplerState(ESampler::TriLinear, EAddressingMode::Wrap, EAddressingMode::Wrap, EAddressingMode::Wrap);
//		m_aSampler[4] = CreateSamplerState(ESampler::Anisotropic, EAddressingMode::Wrap, EAddressingMode::Wrap, EAddressingMode::Wrap);
//		m_aSampler[5] = CreateSamplerState(ESampler::Shadow, EAddressingMode::Wrap, EAddressingMode::Wrap, EAddressingMode::Wrap, EDepthComparaison::Less);
//
//		m_pGizmo = new Gizmo(this);
//
//		// Create thread for the Rendering
//		m_bIsRunning = true;
//		Thread::Create("RenderThread", &RenderThreadEntry, this);
//
//		return true;
//	}
//
//	//!	@brief		Release
//	//!	@date		2015-11-01
//	void Renderer::Release()
//	{		
//		m_bIsRunning = false;
//
//		SAFE_DELETE(m_pGizmo);
//
//		// SamplerState
//		for(uint32 uiSampler = 0; uiSampler < ESampler::Count; ++uiSampler)
//		{
//			DestroyResource(m_aSampler[uiSampler]);
//		}
//
//		// Clean Immediate Context
//		if(m_pImmediateContext) 
//		{
//			ID3D11DeviceContext* pD3D11ImmediateContext = static_cast<ID3D11DeviceContext*>(m_pImmediateContext);
//			pD3D11ImmediateContext->ClearState();
//			pD3D11ImmediateContext->Flush();
//		}
//
//		// Release Device
//		if(m_pDevice) 
//		{
//			ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
//			pD3D11Device->Release();
//			pD3D11Device = NULL;
//		}
//	}
//
//	//!	@brief		Prepare 
//	//!	@date		2015-11-11
//	void Renderer::Prepare()
//	{		
//		uint8 uiFrameIndex = GetFrameIndex(EThread::Prepare);
//
//		// Prepare rendering for each camera in each scene activated
//		const uint32 uiSceneCount = m_vScene.GetSize();
//		for(uint32 uiScene = 0; uiScene < uiSceneCount; ++uiScene)
//		{
//			Scene* pScene = m_vScene[uiScene];
//			Octree* pTree = pScene->GetTree();
//			
//			// DEBUG
//			/*m_pGizmo->SetOrientation(Math::Quaternion::Identity);
//			m_pGizmo->SetPosition(pTree->m_kRoot.m_vPosition);
//			m_pGizmo->SetScale(pTree->m_kRoot.GetSize());
//			m_pGizmo->PushCube(Color::Red);
//			for(uint32 uiDepth = 0; uiDepth < pTree->m_vChild.GetSize(); ++uiDepth)
//			{
//				OctreeBlock* pBlock = pTree->m_vChild[uiDepth];
//				if(pBlock)
//				{
//					for(uint32 uiChild = 0; uiChild < 8; ++uiChild)
//					{
//						m_pGizmo->SetPosition(pBlock[uiChild].m_vPosition);
//						m_pGizmo->SetScale(pBlock[uiChild].GetSize());
//						m_pGizmo->PushCube(Color::Red);
//					}
//				}
//			}*/
//
//			// Camera Indenpendant
//			const uint32 uiRenderableCount = pScene->GetRenderableCount();
//			for(uint32 uiRenderable = 0; uiRenderable < uiRenderableCount; ++uiRenderable)
//			{
//				IRenderable* pRenderable = pScene->GetRenderable(uiRenderable);
//				pRenderable->PushCommand(GetFrameBuffer());
//			}
//
//			// Each camera in scene
//			const uint32 uiCameraCount = pScene->GetCameraCount();
//			for(uint32 uiCamera = 0; uiCamera < uiCameraCount; ++uiCamera)
//			{
//				Camera* pCamera = pScene->GetCamera(uiCamera);
//
//				// Check if there is a Viewport associated to this camera
//				Viewport* pViewport = pCamera->GetViewport();
//				if(!pViewport)
//				{
//					continue;
//				}
//
//				RenderContext* pContext = pViewport->GetRenderContext();
//				pContext->Prepare(uiFrameIndex, pCamera, pTree);
//			}
//		}
//		m_pGizmo->SetIdentity();
//	}
//
//	//!	@brief		RenderThreadEntry
//	//!	@date		2016-03-26
//	void Renderer::RenderThreadEntry(void* _pData)
//	{
//		Renderer* pRenderer = static_cast<Renderer*>(_pData);
//		while(pRenderer->m_bIsRunning)
//		{
//			pRenderer->Render();
//		}
//	}
//
//	//!	@brief		Render
//	//!	@date		2016-03-26
//	void Renderer::Render()
//	{
//		while (GetFrameDifference() == 0)
//		{
//			m_aCondition[EThread::Render].Wait(m_aMutex[EThread::Render]);
//		}
//
//		uint8 uiFrameIndex = GetFrameIndex(EThread::Render);
//		//LOG_PRINT("[RENDERER] %d",  GetFrameIndex(EThread::Execute));
//
//		// Render each Viewport with a context
//		const uint32 uiViewportCount = m_vViewport.GetSize();
//		for(uint32 uiViewport = 0; uiViewport < uiViewportCount; ++uiViewport)
//		{
//			Viewport* pViewport = m_vViewport[uiViewport];
//
//			// Switch buffer of the RenderContext if the viewport has one
//			RenderContext* pContext = pViewport->GetRenderContext();
//			if(pContext)
//			{
//				// Set default Sampler States
//				for(uint32 uiSampler = 0; uiSampler < ESampler::Count; ++uiSampler)
//				{
//					SamplerState* pSamplerState = m_aSampler[uiSampler];
//					SetVertexShaderSamplerState(pContext, uiSampler, pSamplerState);
//					SetPixelShaderSamplerState(pContext, uiSampler, pSamplerState);
//					SetComputeShaderSamplerState(pContext, uiSampler, pSamplerState);
//				}
//
//				pContext->Render(uiFrameIndex);
//			}
//		}
//
//		m_aFrameBuffer[uiFrameIndex].Clear();
//		++m_aFrameId[EThread::Render];
//	}
//
//	//!	@brief		Display 
//	//!	@date		2015-11-11
//	void Renderer::Display()
//	{		
//		Prepare();
//
//		// Wait the Renderer has finished
//		while(GetFrameDifference() == RENDERER_FRAME_COUNT-1)
//		{
//			//LOG_PRINT("[MAIN] WAIT");
//			m_aCondition[EThread::Render].Signal();
//			Thread::Sleep(1);
//		}
//		++m_aFrameId[EThread::Prepare];
//		m_aCondition[EThread::Render].Signal();
//
//		uint8 uiFrameIndex = GetFrameIndex(EThread::Prepare);
//
//		// Check if we already rendering in this texture via an existing viewport
//		const uint32 uiViewportCount = m_vViewport.GetSize();
//		for(uint32 uiViewport = 0; uiViewport < uiViewportCount; ++uiViewport)
//		{
//			Viewport* pViewport = m_vViewport[uiViewport];
//
//			// Switch buffer of the RenderContext if the viewport has one
//			RenderContext* pContext = pViewport->GetRenderContext();
//			if(pContext)
//			{
//				pContext->Display(uiFrameIndex);
//			}
//
//			// Swap chain
//			IDXGISwapChain* pD3D11SwapChain = static_cast<IDXGISwapChain*>(pViewport->GetSwapChain()->GetHandle());
//			if(pD3D11SwapChain)
//			{
//				pD3D11SwapChain->Present(0, 0);
//			}
//		}
//
//		// Clean resource unused
//		const int32 iRequestCount = m_vDestroyRequest.GetSize();
//		for(int32 iRequest = iRequestCount-1; iRequest >= 0; --iRequest)
//		{
//			DestroyRequest& kRequest = m_vDestroyRequest[iRequest];
//			if(++kRequest.Frame > RENDERER_FRAME_COUNT)
//			{
//				SAFE_RELEASE_AND_DELETE(kRequest.Resource);
//				m_vDestroyRequest.RemoveIndex(iRequest);
//			}
//		}
//	}
//
//	//!	@brief		Display 
//	//!	@date		2015-11-11
//	void Renderer::Display(RenderCommandList* pCommandList)
//	{		
//		ID3D11DeviceContext* pD3D11ImmediateContext = static_cast<ID3D11DeviceContext*>(m_pImmediateContext);
//		ID3D11CommandList* pD3D11CommandList = static_cast<ID3D11CommandList*>(pCommandList->GetHandle());
//		pD3D11ImmediateContext->ExecuteCommandList(pD3D11CommandList, true);
//	}
//
//	//!	@brief		Draw 
//	//!	@date		2015-11-11
//	void Renderer::Draw(RenderContext* _pContext, RenderCommandList* pCommandList)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//
//		// Prepare the new command list
//		pCommandList->Release();
//
//		ID3D11CommandList* pD3D11CommandList = NULL;
//		pD3D11DeferredContext->FinishCommandList(false, &pD3D11CommandList);
//		
//		pCommandList->Initiliaze(pD3D11CommandList);
//	}
//
//	//!	@brief		PushMarker 
//	//!	@date		2015-11-21
//	void* Renderer::PushMarker(RenderContext* _pContext, const char* _szName)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//
//		ID3DUserDefinedAnnotation* pD3D11Annotation;
//		HRESULT hResult = pD3D11DeferredContext->QueryInterface(__uuidof(pD3D11Annotation), reinterpret_cast<void**>(&pD3D11Annotation));
//		if(FAILED(hResult)) 
//		{
//			LOG_ERROR("[Renderer] ID3D11DeviceContext::QueryInterface ID3DUserDefinedAnnotation failed (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		String16 sName;
//		StringHelper::ToString16(_szName, sName);
//		pD3D11Annotation->BeginEvent(sName.GetBuffer());
//
//		return pD3D11Annotation;
//	}
//
//	//!	@brief		PopMarker 
//	//!	@date		2015-11-21
//	void Renderer::PopMarker(RenderContext* /*_pContext*/, void* _pMarker)
//	{
//		ID3DUserDefinedAnnotation* pD3D11Annotation = static_cast<ID3DUserDefinedAnnotation*>(_pMarker);
//		pD3D11Annotation->EndEvent();
//	}
//
//	//!	@brief		CreateScene 
//	//!	@date		2015-11-11
//	Scene* Renderer::CreateScene()
//	{
//		Scene* pScene = new Scene();
//		m_vScene.Add(pScene);
//
//		return pScene;
//	}
//
//	//!	@brief		DestroyScene 
//	//!	@date		2015-11-11
//	void Renderer::DestroyScene(Scene* _pScene)
//	{
//		m_vScene.Remove(_pScene);
//		SAFE_DELETE(_pScene);
//	}
//
//	//!	@brief		CreateViewport 
//	//!	@date		2015-11-11
//	Viewport* Renderer::CreateViewport(Window* _pWindow, EFormat::Type _eFormat, bool _bRenderContext /*= true*/)
//	{
//		uint32 uiWidth = _pWindow->GetWidth();
//		uint32 uiHeight = _pWindow->GetHeight();
//		SwapChain* pSwapChain = CreateSwapChain(_pWindow, uiWidth, uiHeight, _eFormat);
//
//		return CreateViewport(pSwapChain, _bRenderContext);
//	}
//
//	//!	@brief		CreateViewport 
//	//!	@date		2015-11-11
//	Viewport* Renderer::CreateViewport(uint32 _uiWidth, uint32 _uiHeight, EFormat::Type _eFormat, bool _bRenderContext /*= true*/)
//	{
//		Texture2D* pRenderTexture = CreateTexture2D(_uiWidth, _uiHeight, 1, _eFormat, EBind::RenderTarget);
//		SwapChain* pSwapChain = new SwapChain();
//		pSwapChain->Initialize(pRenderTexture);
//
//		return CreateViewport(pSwapChain, _bRenderContext);
//	}
//
//	//!	@brief		AddViewport 
//	//!	@date		2015-11-11
//	Viewport* Renderer::CreateViewport(SwapChain* _pSwapChain, bool _bRenderContext)
//	{
//		// Create the render target for the render context
//		RenderTarget* pRenderTarget = CreateRenderTarget();
//		pRenderTarget->AddTarget(_pSwapChain->GetTexture());
//
//		// New Viewport
//		Viewport* pViewport = new Viewport();
//		pViewport->Initialize(pRenderTarget, _pSwapChain);
//
//		// Create the render context if needed
//		
//		if(_bRenderContext)
//		{
//			ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
//
//			// Create a D3D11 deferred context
//			ID3D11DeviceContext* pD3D11DeferredContext = NULL;
//			HRESULT hResult = pD3D11Device->CreateDeferredContext(0, &pD3D11DeferredContext);
//			if(FAILED(hResult))
//			{
//				LOG_ERROR("[Renderer] CreateDeferredContext failed (HRESULT = 0x%0X)", hResult);
//				return NULL;
//			}
//
//			RenderContext* pRenderContext = new RenderContext();
//			pRenderContext->Initialize(this, pD3D11DeferredContext, pViewport);
//			pViewport->SetRenderContext(pRenderContext);
//			SIGNAL_SEND(RenderContextCreated, pRenderContext, 0);
//		}
//
//		m_vViewport.Add(pViewport);
//
//		return pViewport;
//	}
//
//	//!	@brief		DestroyViewport 
//	//!	@date		2015-11-11
//	void Renderer::DestroyViewport(Viewport* _pViewport)
//	{
//		m_vViewport.Remove(_pViewport);
//		SAFE_RELEASE_AND_DELETE(_pViewport);
//	}
//
//	//!	@brief		CreateSwapChain 
//	//!	@date		2015-11-11
//	SwapChain* Renderer::CreateSwapChain(Window* _pWindow, uint32 _uiWidth, uint32 _uiHeight, EFormat::Type _eFormat)
//	{
//		ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
//
//		// Query Device Interface
//		IDXGIDevice* pD3D11InterfaceDevice = NULL;
//		HRESULT hResult = pD3D11Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&pD3D11InterfaceDevice);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] Failed to query the device interface (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		// Query Adapter Interface
//		IDXGIAdapter* pD3D11InterfaceAdapter = NULL;
//		hResult = pD3D11InterfaceDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pD3D11InterfaceAdapter);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] Failed to query the adapter interface (HRESULT = 0x%0X)", hResult);
//			return false;
//		}
//
//		// Query Factory Interface
//		IDXGIFactory* pD3D11InterfaceFactory = NULL;
//		hResult = pD3D11InterfaceAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pD3D11InterfaceFactory);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] Failed to query the factory interface (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		// Prepare D3D11 Descriptor
//		DXGI_SWAP_CHAIN_DESC kD3D11SwapChainDesc;
//		ZeroMemory(&kD3D11SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
//		kD3D11SwapChainDesc.BufferCount = 1;
//		kD3D11SwapChainDesc.BufferDesc.Width = _uiWidth;
//		kD3D11SwapChainDesc.BufferDesc.Height = _uiHeight;
//		kD3D11SwapChainDesc.BufferDesc.Format = (DXGI_FORMAT)_eFormat;
//		kD3D11SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
//		kD3D11SwapChainDesc.OutputWindow = reinterpret_cast<HWND>(_pWindow);
//		kD3D11SwapChainDesc.SampleDesc.Count = 1;
//		kD3D11SwapChainDesc.SampleDesc.Quality = 0;
//		kD3D11SwapChainDesc.Windowed = true;
//
//		// Create the SwapChain for this viewport
//		IDXGISwapChain* pD3D11SwapChain = NULL;
//		hResult = pD3D11InterfaceFactory->CreateSwapChain(pD3D11Device, &kD3D11SwapChainDesc, &pD3D11SwapChain);
//		
//		// Release interfaces
//		SAFE_RELEASE(pD3D11InterfaceFactory);
//		SAFE_RELEASE(pD3D11InterfaceAdapter);
//		SAFE_RELEASE(pD3D11InterfaceDevice);
//
//		// Check result
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] Failed to create the SwapChain (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		// Get the D3D11 texture from the swap chain
//		ID3D11Texture2D* pD3D11BackBufferTexture = NULL;
//		hResult = pD3D11SwapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (LPVOID*)&pD3D11BackBufferTexture);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] Failed to fetch the back buffer on the SwapChain (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		// Create Texture2D and view
//		Texture2D* pTexture = new Texture2D();
//		pTexture->Initialize(_uiWidth, _uiHeight, pD3D11BackBufferTexture);
//		ID3D11RenderTargetView* pD3D11RenderTargetView = static_cast<ID3D11RenderTargetView*>(CreateRenderTargetView(pD3D11BackBufferTexture));
//		pTexture->SetView(Texture2D::EView::RenderTarget, pD3D11RenderTargetView);
//
//		// Create swap chain
//		SwapChain* pSwapChain = new SwapChain();
//		pSwapChain->Initialize(pTexture);
//		pSwapChain->SetWindow(_pWindow);
//		pSwapChain->SetHandle(pD3D11SwapChain);
//
//		return pSwapChain;
//	}
//
//	//!	@brief		CreateRenderTarget
//	//!	@date		2015-11-11
//	RenderTarget* Renderer::CreateRenderTarget()
//	{
//		RenderTarget* pRenderTarget = new RenderTarget();
//		pRenderTarget->Initiliaze(this);
//		return pRenderTarget;
//	}
//
//	//!	@brief		CreateTexture2D
//	//!	@date		2015-11-11
//	Texture2D* Renderer::CreateTextureCube(uint32 _uiWidth, uint32 _uiHeight, uint32 _uiMipCount, EFormat::Type _eFormat, uint32 _eBindFlag, EUsage::Type _eUsage /*= EUsage::Default*/, const void* _pData /*= NULL*/)
//	{
//		return CreateTexture2D(_uiWidth, _uiHeight, _uiMipCount, 6, _eFormat, _eBindFlag, _eUsage, _pData, D3D11_RESOURCE_MISC_TEXTURECUBE);
//	}
//
//	//!	@brief		CreateTexture2D
//	//!	@date		2015-11-11
//	Texture2D* Renderer::CreateTexture2D(uint32 _uiWidth, uint32 _uiHeight, uint32 _uiMipCount, EFormat::Type _eFormat, uint32 _eBindFlag, EUsage::Type _eUsage /*= EUsage::Default*/, const void* _pData /*= NULL*/)
//	{
//		return CreateTexture2D(_uiWidth, _uiHeight, _uiMipCount, 1, _eFormat, _eBindFlag, _eUsage, _pData, 0);
//	}
//
//	//!	@brief		CreateTexture2D
//	//!	@date		2015-11-11
//	Texture2D* Renderer::CreateTexture2D(uint32 _uiWidth, uint32 _uiHeight, uint32 _uiMipCount, uint32 _uiCount, EFormat::Type _eFormat, uint32 _eBindFlag, EUsage::Type _eUsage, const void* _pData, uint32 _eFlag)
//	{
//		// Prepare D3D11 Descriptor
//		D3D11_TEXTURE2D_DESC kD3D11TextureDesc;
//		ZeroMemory(&kD3D11TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
//		kD3D11TextureDesc.Width = _uiWidth;
//		kD3D11TextureDesc.Height = _uiHeight;
//		kD3D11TextureDesc.BindFlags = _eBindFlag;
//		kD3D11TextureDesc.CPUAccessFlags = (_eUsage == EUsage::Dynamic) ? D3D11_CPU_ACCESS_WRITE : 0;
//		kD3D11TextureDesc.Format = (DXGI_FORMAT)_eFormat;
//		kD3D11TextureDesc.Usage = (D3D11_USAGE)_eUsage;
//		kD3D11TextureDesc.MipLevels = _uiMipCount;
//		kD3D11TextureDesc.ArraySize = _uiCount;
//		kD3D11TextureDesc.SampleDesc.Count = 1;
//		kD3D11TextureDesc.SampleDesc.Quality = 0;
//		kD3D11TextureDesc.MiscFlags = _eFlag;
//
//		// Affect only the subResource pointer in case we have some data in _pData
//		D3D11_SUBRESOURCE_DATA* pD3D11SubResource = NULL;
//		if(_pData)
//		{
//			uint8* pDataStart = (uint8*)_pData;
//			uint8* pDataEnd = NULL;
//
//			uint32 uiIndex = 0;
//			pD3D11SubResource = new D3D11_SUBRESOURCE_DATA[kD3D11TextureDesc.MipLevels * kD3D11TextureDesc.ArraySize];
//			for(uint32 uiArray = 0; uiArray < kD3D11TextureDesc.ArraySize; ++uiArray)
//			{
//				for(uint32 uiMipMap = 0; uiMipMap < kD3D11TextureDesc.MipLevels; ++uiMipMap)
//				{
//					// Retrieve inforamtion on the current mipmap
//					uint32 uiSurfaceByte;
//					memcpy_s(&uiSurfaceByte, sizeof(uint32), pDataStart, sizeof(uint32));
//					pDataStart += sizeof(uint32);
//
//					uint32 uiRowByte;
//					memcpy_s(&uiRowByte, sizeof(uint32), pDataStart, sizeof(uint32));
//					pDataStart += sizeof(uint32);
//					pDataEnd = pDataStart + uiSurfaceByte;
//
//					// Fill SubResources
//					pD3D11SubResource[uiIndex].pSysMem = pDataStart;
//					pD3D11SubResource[uiIndex].SysMemPitch = uiRowByte;
//					pD3D11SubResource[uiIndex].SysMemSlicePitch = uiSurfaceByte;
//
//					// Buffer Overrun
//					pDataStart += uiSurfaceByte;
//					if (pDataStart > pDataEnd)
//					{
//						LOG_ERROR("[Renderer] CreateTexture2D failed - incoherency in MipMap size");
//						return NULL;
//					}
//
//					++uiIndex;
//				}
//			}
//		}
//
//		// Create D3D11 Specific Texture2D
//		ID3D11Texture2D* pD3D11Texture = NULL;
//		ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
//		HRESULT hResult = pD3D11Device->CreateTexture2D(&kD3D11TextureDesc, pD3D11SubResource, &pD3D11Texture);
//
//		// Delete Sub resource in all case
//		if(pD3D11SubResource)
//		{
//			SAFE_DELETE_ARRAY(pD3D11SubResource);
//		}
//
//		// Check result
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] CreateTexture2D failed (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		// Create Texture2D
//		Texture2D* pTexture = new Texture2D();
//		pTexture->Initialize(_uiWidth, _uiHeight, pD3D11Texture);
//
//		// A View will be needed since we want to use it as ShaderResource
//		if((_eBindFlag & EBind::ShaderResource) != 0)
//		{
//			ID3D11ShaderResourceView* pD3D11ShaderResourceView = static_cast<ID3D11ShaderResourceView*>(CreateShaderResourceView(pTexture, _uiMipCount, _uiCount, _eFormat, _eFlag));
//			pTexture->SetView(Texture2D::EView::ShaderResource, pD3D11ShaderResourceView);
//		}
//
//		if((_eBindFlag & EBind::RenderTarget) != 0)
//		{
//			ID3D11RenderTargetView* pD3D11RenderTargetView = static_cast<ID3D11RenderTargetView*>(CreateRenderTargetView(pD3D11Texture));
//			pTexture->SetView(Texture2D::EView::RenderTarget, pD3D11RenderTargetView);
//		}
//
//		if((_eBindFlag & EBind::DepthStencil) != 0)
//		{
//			ID3D11DepthStencilView* pD3D11DepthStencilView = static_cast<ID3D11DepthStencilView*>(CreateDepthStenciltView(pTexture));
//			pTexture->SetView(Texture2D::EView::DepthStencil, pD3D11DepthStencilView);
//		}
//
//		if((_eBindFlag & EBind::UnorderedAccess) != 0)
//		{
//			ID3D11UnorderedAccessView* pD3D11UnorderedAccessView = static_cast<ID3D11UnorderedAccessView*>(CreateUnorderedAccessView(pTexture, _eFormat));
//			pTexture->SetView(Texture2D::EView::UnorderedAccess, pD3D11UnorderedAccessView);
//		}
//
//		return pTexture;
//	}
//
//	//!	@brief		CreateTexture3D
//	//!	@date		2015-11-11
//	Texture3D* Renderer::CreateTexture3D(uint32 _uiWidth, uint32 _uiHeight, uint32 _uiDepth, uint32 _uiMipCount, EFormat::Type _eFormat, uint32 _eBindFlag, EUsage::Type _eUsage /*= EUsage::Default*/, const void* _pData /*= NULL*/)
//	{
//		// Prepare D3D11 Descriptor
//		D3D11_TEXTURE3D_DESC kD3D11TextureDesc;
//		ZeroMemory(&kD3D11TextureDesc, sizeof(D3D11_TEXTURE3D_DESC));
//		kD3D11TextureDesc.Width = _uiWidth;
//		kD3D11TextureDesc.Height = _uiHeight;
//		kD3D11TextureDesc.Depth = _uiDepth;
//		kD3D11TextureDesc.BindFlags = _eBindFlag;
//		kD3D11TextureDesc.CPUAccessFlags = (_eUsage == EUsage::Dynamic) ? D3D11_CPU_ACCESS_WRITE : 0;
//		kD3D11TextureDesc.Format = (DXGI_FORMAT)_eFormat;
//		kD3D11TextureDesc.Usage = (D3D11_USAGE)_eUsage;
//		kD3D11TextureDesc.MipLevels = _uiMipCount;
//		kD3D11TextureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
//
//		// Affect only the subResource pointer in case we have some data in _pData
//		D3D11_SUBRESOURCE_DATA* pD3D11SubResource = NULL;
//		if(_pData)
//		{
//			uint8* pDataStart = (uint8*)_pData;
//			uint8* pDataEnd = NULL;
//			UNUSED(pDataStart)UNUSED(pDataEnd)
//			uint32 uiIndex = 0;
//			pD3D11SubResource = new D3D11_SUBRESOURCE_DATA[kD3D11TextureDesc.MipLevels * 6];
//			for(uint32 uiArray = 0; uiArray < 6; ++uiArray)
//			{
//				for(uint32 uiMipMap = 0; uiMipMap < kD3D11TextureDesc.MipLevels; ++uiMipMap)
//				{
//					// Retrieve inforamtion on the current mipmap
//					uint32 uiSurfaceByte;
//					memcpy_s(&uiSurfaceByte, sizeof(uint32), pDataStart, sizeof(uint32));
//					pDataStart += sizeof(uint32);
//
//					uint32 uiRowByte;
//					memcpy_s(&uiRowByte, sizeof(uint32), pDataStart, sizeof(uint32));
//					pDataStart += sizeof(uint32);
//					pDataEnd = pDataStart + uiSurfaceByte;
//
//					// Fill SubResources
//					pD3D11SubResource[uiIndex].pSysMem = pDataStart;
//					pD3D11SubResource[uiIndex].SysMemPitch = uiRowByte;
//					pD3D11SubResource[uiIndex].SysMemSlicePitch = uiSurfaceByte;
//
//					// Buffer Overrun
//					pDataStart += uiSurfaceByte;
//					if (pDataStart > pDataEnd)
//					{
//						LOG_ERROR("[Renderer] CreateTexture2D failed - incoherency in MipMap size");
//						return NULL;
//					}
//
//					++uiIndex;
//				}
//			}
//		}
//
//		// Create D3D11 Specific Texture2D
//		ID3D11Texture3D* pD3D11Texture = NULL;
//		ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
//		HRESULT hResult = pD3D11Device->CreateTexture3D(&kD3D11TextureDesc, pD3D11SubResource, &pD3D11Texture);
//
//		// Delete Sub resource in all case
//		if(pD3D11SubResource)
//		{
//			SAFE_DELETE_ARRAY(pD3D11SubResource);
//		}
//
//		// Check result
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] CreateTexture3D failed (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		// Create Texture2D
//		Texture3D* pTexture = new Texture3D();
//		pTexture->Initialize(_uiWidth, _uiHeight, pD3D11Texture);
//
//		// A View will be needed since we want to use it as ShaderResource
//		if((_eBindFlag & EBind::ShaderResource) != 0)
//		{
//			ID3D11ShaderResourceView* pD3D11ShaderResourceView = static_cast<ID3D11ShaderResourceView*>(CreateShaderResourceView(pTexture, _uiMipCount, _eFormat));
//			pTexture->SetView(Texture3D::EView::ShaderResource, pD3D11ShaderResourceView);
//		}
//
//		if((_eBindFlag & EBind::RenderTarget) != 0)
//		{
//			ID3D11RenderTargetView* pD3D11RenderTargetView = static_cast<ID3D11RenderTargetView*>(CreateRenderTargetView(pD3D11Texture));
//			pTexture->SetView(Texture3D::EView::RenderTarget, pD3D11RenderTargetView);
//		}
//
//		if((_eBindFlag & EBind::UnorderedAccess) != 0)
//		{
//			ID3D11UnorderedAccessView* pD3D11UnorderedAccessView = static_cast<ID3D11UnorderedAccessView*>(CreateUnorderedAccessView(pTexture, _eFormat));
//			pTexture->SetView(Texture3D::EView::UnorderedAccess, pD3D11UnorderedAccessView);
//		}
//
//		return pTexture;
//	}
//
//	//!	@brief		CreateMaterial 
//	//!	@date		2015-11-21
//	Material* Renderer::CreateMaterial(Shader* _pPixelShader, float32* _pNumber /*= NULL*/, uint32 _uiNumberSize /*= 0*/)
//	{
//		Material* pMaterial = new Material();
//		pMaterial->Initialize(_pPixelShader, _pNumber, _uiNumberSize);
//
//		return pMaterial;
//	}
//
//	//!	@brief		CreateGeometry 
//	//!	@date		2015-11-21
//	Geometry* Renderer::CreateGeometry(const void* _pVertexBuffer, uint32 _uiVertexSize, uint32 _uiVertexCount, const uint32* _pIndexBuffer, uint32 _uiIndexCount, const uint32* _pSectionBuffer /*= NULL*/, uint32 _uiSectionCount /*= 0*/)
//	{
//		// Crete VertexBuffer & IndexBuffer
//		Buffer* pVertexBuffer = CreateBuffer(_uiVertexSize, _uiVertexCount, EBind::VertexBuffer, EUsage::Immutable, 0, 0, _pVertexBuffer);
//		Buffer* pIndexBuffer = CreateBuffer(sizeof(uint32), _uiIndexCount, EBind::IndexBuffer, EUsage::Immutable, 0, 0, _pIndexBuffer);
//
//		// Create default section if needed
//		if(!_pSectionBuffer)
//		{
//			_pSectionBuffer = &_uiIndexCount;
//			_uiSectionCount = 1;
//		}
//
//		// Create Geometry
//		Geometry* pGeometry = new Geometry();
//		pGeometry->Initialize(pVertexBuffer, pIndexBuffer, _pSectionBuffer, _uiSectionCount);
//
//		return pGeometry;
//	}
//
//	//!	@brief		CreateConstantBuffer 
//	//!	@date		2015-11-21
//	Buffer* Renderer::CreateConstantBuffer(uint32 _uiElementSize, uint32 _uiElementCount, const void* _pData /*= NULL*/)
//	{
//		// Force 16 alignement due to D3D11 requirement
//		uint32 uiByteCount = _uiElementSize * _uiElementCount;
//		uint32 uiAlignement = (uiByteCount % D3D11_ALIGNEMENT);
//		if(uiAlignement)
//		{
//			uiAlignement = D3D11_ALIGNEMENT - uiAlignement;
//				
//			// Prefer the element size on case of there is few element
//			if(_uiElementCount < _uiElementSize)
//			{
//				_uiElementSize += uiAlignement;
//			}
//			// Otherwise prefer the element count
//			else
//			{
//				_uiElementCount += uiAlignement / _uiElementSize;
//			}
//		}
//
//		return CreateBuffer(_uiElementSize, _uiElementCount, EBind::ConstantBuffer , EUsage::Dynamic, D3D11_CPU_ACCESS_WRITE, 0, _pData);
//	}
//
//	//!	@brief		CreateShaderResourceBuffer 
//	//!	@date		2015-11-21
//	Buffer* Renderer::CreateShaderResourceBuffer(uint32 _uiElementSize, uint32 _uiElementCount, uint32 _uiBindFlag, EUsage::Type _eUsage /*= EUsage::Default*/, const void* _pData /*= NULL*/)
//	{
//		uint32 uiCpuFlag = 0;
//		switch(_eUsage)
//		{
//			case EUsage::Dynamic:
//			{
//				uiCpuFlag = D3D11_CPU_ACCESS_WRITE;
//				break;
//			}
//
//			case EUsage::Staging:
//			{
//				uiCpuFlag = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
//				break;
//			}
//		}
//
//		return CreateBuffer(_uiElementSize, _uiElementCount, _uiBindFlag , _eUsage, uiCpuFlag, D3D11_RESOURCE_MISC_BUFFER_STRUCTURED, _pData);
//	}
//
//	//!	@brief		CreateIndirectBuffer
//	//!	@date		2015-11-21
//	Buffer* Renderer::CreateIndirectBuffer(uint32 _uiElementSize, uint32 _uiElementCount, const void* _pData /*= NULL*/)
//	{
//		uint32 uiByteCount = _uiElementSize * _uiElementCount;
//		_uiElementSize = sizeof(uint32);
//		_uiElementCount = uiByteCount / _uiElementSize;
//		return CreateBuffer(_uiElementSize, _uiElementCount, EBind::ShaderResource | EBind::UnorderedAccess, EUsage::Default, 0, D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS, _pData);
//	}
//
//	//!	@brief		CreateBuffer 
//	//!	@date		2015-11-21
//	Buffer* Renderer::CreateBuffer(uint32 _uiElementSize, uint32 _uiElementCount, uint32 _eBindFlag, EUsage::Type _eUsage, uint32 _uiCpuFag, uint32 _uiMiscFlag, const void* _pData /*= NULL*/)
//	{
//		// Prepare D3D11 Descriptor
//		D3D11_BUFFER_DESC kD3D11BufferDesc;
//		ZeroMemory(&kD3D11BufferDesc, sizeof(D3D11_BUFFER_DESC));
//		kD3D11BufferDesc.BindFlags = _eBindFlag;
//		kD3D11BufferDesc.Usage = (D3D11_USAGE)_eUsage;
//		kD3D11BufferDesc.CPUAccessFlags = _uiCpuFag;
//		kD3D11BufferDesc.MiscFlags = _uiMiscFlag;
//		kD3D11BufferDesc.ByteWidth = _uiElementSize * _uiElementCount;
//		kD3D11BufferDesc.StructureByteStride = _uiElementSize;
//
//		// Affect only the subResource pointer in case we have some data in _pData
//		D3D11_SUBRESOURCE_DATA* pD3D11SubResourceData = NULL;
//		D3D11_SUBRESOURCE_DATA kD3D11SubResource;
//		if(_pData)
//		{
//			ZeroMemory(&kD3D11SubResource, sizeof(D3D11_SUBRESOURCE_DATA));
//			kD3D11SubResource.pSysMem = _pData;
//			pD3D11SubResourceData = &kD3D11SubResource;
//		}
//
//		// Create D3D11 Specific Buffer
//		ID3D11Buffer* pD3D11Buffer = NULL;
//		ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
//		HRESULT hResult = pD3D11Device->CreateBuffer(&kD3D11BufferDesc, pD3D11SubResourceData, &pD3D11Buffer);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] Failed to Create Buffer (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		Buffer* pBuffer = new Buffer();
//		pBuffer->Initialize(_uiElementSize, _uiElementCount, pD3D11Buffer);
//
//		// Create view needed
//		EFormat::Type eFormat = (_uiMiscFlag & D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS) != 0 ? EFormat::R32_UINT : EFormat::Unknown;
//		if((_eBindFlag & EBind::ShaderResource) != 0)
//		{
//			ID3D11ShaderResourceView* pD3D11ShaderResourceView = static_cast<ID3D11ShaderResourceView*>(CreateShaderResourceView(pBuffer, eFormat));
//			pBuffer->SetView(Buffer::EView::ShaderResource, pD3D11ShaderResourceView);
//		}
//
//		if((_eBindFlag & EBind::UnorderedAccess) != 0)
//		{
//			ID3D11UnorderedAccessView* pD3D11UnorderedAccessView = static_cast<ID3D11UnorderedAccessView*>(CreateUnorderedAccessView(pBuffer, eFormat));
//			pBuffer->SetView(Buffer::EView::UnorderedAccess, pD3D11UnorderedAccessView);
//		}
//
//		return pBuffer;
//	}
//
//	//!	@brief		CreateRenderState 
//	//!	@date		2015-11-21
//	RenderState* Renderer::CreateRenderState(RenderState::Key _kKey)
//	{
//		ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
//		
//		// RasterizerState
//		D3D11_RASTERIZER_DESC kD3D11RasterizerDesc;
//		ZeroMemory(&kD3D11RasterizerDesc, sizeof (D3D11_RASTERIZER_DESC));
//		kD3D11RasterizerDesc.FillMode = _kKey.WireFrame ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
//		kD3D11RasterizerDesc.CullMode = _kKey.Cull ? D3D11_CULL_BACK : D3D11_CULL_NONE;
//		kD3D11RasterizerDesc.FrontCounterClockwise = _kKey.Clockwise;
//		kD3D11RasterizerDesc.DepthBias = 0;
//		kD3D11RasterizerDesc.DepthBiasClamp = 0.0f;
//		kD3D11RasterizerDesc.SlopeScaledDepthBias = 0.0f;
//		kD3D11RasterizerDesc.DepthClipEnable = false;
//		kD3D11RasterizerDesc.ScissorEnable = _kKey.Scissor;
//		kD3D11RasterizerDesc.MultisampleEnable = false;
//		kD3D11RasterizerDesc.AntialiasedLineEnable = false;
//		
//		ID3D11RasterizerState* pD3D11RasterizerState = NULL;
//		HRESULT hResult = pD3D11Device->CreateRasterizerState(&kD3D11RasterizerDesc, &pD3D11RasterizerState);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] Failed to Create RasterizerState (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		// DepthStencilState
//		D3D11_DEPTH_STENCIL_DESC kD3D11DepthStencilDesc;
//		ZeroMemory(&kD3D11DepthStencilDesc, sizeof (D3D11_DEPTH_STENCIL_DESC));
//		kD3D11DepthStencilDesc.DepthEnable = _kKey.Depth;
//		kD3D11DepthStencilDesc.DepthWriteMask = _kKey.DepthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
//		kD3D11DepthStencilDesc.DepthFunc = (D3D11_COMPARISON_FUNC)_kKey.DepthComparaison;
//		kD3D11DepthStencilDesc.StencilEnable = kD3D11DepthStencilDesc.StencilEnable;
//		kD3D11DepthStencilDesc.StencilReadMask = 0xff;
//		kD3D11DepthStencilDesc.StencilWriteMask = 0xff;
//
//		kD3D11DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//		kD3D11DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
//		kD3D11DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//		kD3D11DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//
//		kD3D11DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//		kD3D11DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
//		kD3D11DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//		kD3D11DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//
//		ID3D11DepthStencilState* pD3D11DepthStencilState = NULL;
//		hResult = pD3D11Device->CreateDepthStencilState(&kD3D11DepthStencilDesc, &pD3D11DepthStencilState);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] Failed to Create DepthStencilState (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		// BlendState
//		D3D11_BLEND_DESC kD3D11BlendDesc;
//		ZeroMemory(&kD3D11BlendDesc, sizeof (D3D11_BLEND_DESC));
//		kD3D11BlendDesc.AlphaToCoverageEnable = false;
//		kD3D11BlendDesc.IndependentBlendEnable = false;
//		D3D11_RENDER_TARGET_BLEND_DESC& kD3D11RenderTargetBlendDesc = kD3D11BlendDesc.RenderTarget[0];
//		kD3D11RenderTargetBlendDesc.BlendEnable = _kKey.Blend;
//		kD3D11RenderTargetBlendDesc.SrcBlend = (D3D11_BLEND)_kKey.BlendSource;
//		kD3D11RenderTargetBlendDesc.BlendOp = (D3D11_BLEND_OP)_kKey.BlendOperation;
//		kD3D11RenderTargetBlendDesc.DestBlend = (D3D11_BLEND)_kKey.BlendDestination;
//		kD3D11RenderTargetBlendDesc.SrcBlendAlpha = (D3D11_BLEND)_kKey.BlendSource;
//		kD3D11RenderTargetBlendDesc.BlendOpAlpha = (D3D11_BLEND_OP)_kKey.BlendOperation;
//		kD3D11RenderTargetBlendDesc.DestBlendAlpha = (D3D11_BLEND)_kKey.BlendDestination;
//		kD3D11RenderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
//
//		ID3D11BlendState* pD3D11BlendState = NULL;
//		hResult = pD3D11Device->CreateBlendState(&kD3D11BlendDesc, &pD3D11BlendState);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] Failed to Create BlendState (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		RenderState* pRenderState = new RenderState();
//		pRenderState->Initialize(pD3D11RasterizerState, pD3D11DepthStencilState, pD3D11BlendState);
//
//		return pRenderState;
//	}
//
//	//!	@brief		CreateSamplerState 
//	//!	@date		2015-11-21
//	SamplerState* Renderer::CreateSamplerState(ESampler::Type _eSampler, EAddressingMode::Type _eU, EAddressingMode::Type _eV, EAddressingMode::Type _eW, EDepthComparaison::Type _eComparaison /*= EDepthComparaison::Never*/)
//	{
//		ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
//
//		// SamplerState
//		D3D11_SAMPLER_DESC kD3D11SamplerDesc;
//		ZeroMemory(&kD3D11SamplerDesc, sizeof (D3D11_SAMPLER_DESC));
//		kD3D11SamplerDesc.Filter = (D3D11_FILTER)_eSampler;
//		kD3D11SamplerDesc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)_eU;
//		kD3D11SamplerDesc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)_eV;
//		kD3D11SamplerDesc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)_eW;
//		kD3D11SamplerDesc.ComparisonFunc = (D3D11_COMPARISON_FUNC)_eComparaison;
//		
//		ID3D11SamplerState* pD3D11SamplerState = NULL;
//		HRESULT hResult = pD3D11Device->CreateSamplerState(&kD3D11SamplerDesc, &pD3D11SamplerState);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] Failed to Create SamplerState (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		SamplerState* pSamplerState = new SamplerState();
//		pSamplerState->Initialize(pD3D11SamplerState);
//		
//		return pSamplerState;
//	}
//
//	//!	@brief		CreateEffect 
//	//!	@date		2015-11-21
//	Effect* Renderer::CreateEffect(RenderState::Key _kKey, ETopology::Type _eTopology)
//	{
//		RenderState* pRenderState = CreateRenderState(_kKey);
//
//		Effect* pEffect = new Effect();
//		pEffect->Initialize(pRenderState, _eTopology);
//
//		return pEffect;
//	}
//
//	//!	@brief		CreateVertexShader 
//	//!	@date		2015-11-21
//	Shader* Renderer::CreateVertexShader(uint8* _pBuffer, uint32 _uiSize, Shader::Parameter* _pParameterArray, uint32 _uiParameterCount)
//	{
//		// Create the shader
//		ID3D11VertexShader* pD3D11VertexShader = NULL;
//		ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
//		HRESULT hResult = pD3D11Device->CreateVertexShader(_pBuffer, _uiSize, NULL, &pD3D11VertexShader);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] CreateVertexShader failed (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		// Input Layout Parameters
//		Vector<D3D11_INPUT_ELEMENT_DESC> vD3D11InputElementDesc(_uiParameterCount);
//		for(uint32 uiParameter = 0; uiParameter < _uiParameterCount; ++uiParameter)
//		{
//			Shader::Parameter& kParameter = _pParameterArray[uiParameter];
//
//			// Fill out input element desc
//			D3D11_INPUT_ELEMENT_DESC& kD3D11InputElementDesc = vD3D11InputElementDesc[uiParameter];
//			ZeroMemory(&kD3D11InputElementDesc, sizeof(D3D11_INPUT_ELEMENT_DESC));
//			kD3D11InputElementDesc.SemanticName = kParameter.Name;
//			kD3D11InputElementDesc.SemanticIndex = kParameter.Index;
//			kD3D11InputElementDesc.Format = (DXGI_FORMAT)kParameter.Format;
//			kD3D11InputElementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
//
//			// Per Instance or Per Vertex
//			if(kParameter.Instancing)
//			{
//				kD3D11InputElementDesc.InputSlot = 1;
//				kD3D11InputElementDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
//				kD3D11InputElementDesc.InstanceDataStepRate = 1;
//			}
//			else
//			{
//				kD3D11InputElementDesc.InputSlot = 0;	
//				kD3D11InputElementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
//				kD3D11InputElementDesc.InstanceDataStepRate = 0;
//			}
//		}
//
//		// Create the input layout
//		ID3D11InputLayout* pD3D11InputLayout = NULL;
//		hResult = pD3D11Device->CreateInputLayout(&vD3D11InputElementDesc[0], vD3D11InputElementDesc.GetSize(), _pBuffer, _uiSize, &pD3D11InputLayout);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] CreateInputLayout failed (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		Shader* pShader = new Shader();
//		pShader->Initialize(pD3D11VertexShader, pD3D11InputLayout);
//
//		return pShader;
//	}
//
//	//!	@brief		CreateGeometryShader 
//	//!	@date		2015-11-21
//	Shader* Renderer::CreateGeometryShader(uint8* _pBuffer, uint32 _uiSize)
//	{
//		// Create the shader
//		ID3D11GeometryShader* pD3D11GeometryShader = NULL;
//		ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
//		HRESULT hResult = pD3D11Device->CreateGeometryShader(_pBuffer, _uiSize, NULL, &pD3D11GeometryShader);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] CreateGeometryShader failed (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		Shader* pShader = new Shader();
//		pShader->Initialize(pD3D11GeometryShader);
//
//		return pShader;
//	}
//
//	//!	@brief		CreatePixelShader 
//	//!	@date		2015-11-21
//	Shader* Renderer::CreatePixelShader(uint8* _pBuffer, uint32 _uiSize)
//	{
//		// Create the shader
//		ID3D11PixelShader* pD3D11PixelShader = NULL;
//		ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
//		HRESULT hResult = pD3D11Device->CreatePixelShader(_pBuffer, _uiSize, NULL, &pD3D11PixelShader);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] CreatePixelShader failed (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		Shader* pShader = new Shader();
//		pShader->Initialize(pD3D11PixelShader);
//
//		return pShader;
//	}
//
//	//!	@brief		CreateComputeShader 
//	//!	@date		2015-11-21
//	Shader* Renderer::CreateComputeShader(uint8* _pBuffer, uint32 _uiSize)
//	{
//		// Create the shader
//		ID3D11ComputeShader* pD3D11ComputeShader = NULL;
//		ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
//		HRESULT hResult = pD3D11Device->CreateComputeShader(_pBuffer, _uiSize, NULL, &pD3D11ComputeShader);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] CreateComputeShader failed (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		Shader* pShader = new Shader();
//		pShader->Initialize(pD3D11ComputeShader);
//
//		return pShader;
//	}
//
//	//!	@brief		CreateCommandList 
//	//!	@date		2016-06-18
//	RenderCommandList* Renderer::CreateCommandList(RenderContext* _pContext)
//	{
//		RenderCommandList* pCommandList = new RenderCommandList();
//		Draw(_pContext, pCommandList);
//		return pCommandList;
//	}
//
//	//!	@brief		CopyTexture 
//	//!	@date		2016-06-18
//	void Renderer::CopyTexture(RenderContext* _pContext, Texture2D* _pSource, Texture2D* _pDestination, uint32 _uiOffsetX /*= 0*/, uint32 _uiOffsetY /*= 0*/)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11Texture2D* pD3D11TextureSource = static_cast<ID3D11Texture2D*>(_pSource->GetHandle());
//		ID3D11Texture2D* pD3D11TextureDestination = static_cast<ID3D11Texture2D*>(_pDestination->GetHandle());
//		
//	/*	D3D11_BOX kBox;
//		kBox.left = 0;
//		kBox.right = _pDestination->GetWidth();
//		kBox.top = 0;
//		kBox.bottom = _pDestination->GetHeight();
//		kBox.front = 0;
//		kBox.back = 1;*/
//		pD3D11DeferredContext->CopySubresourceRegion(pD3D11TextureDestination, 0, _uiOffsetX, _uiOffsetY, 0, pD3D11TextureSource, 0, NULL);			
//	}
//
//	//!	@brief		CopyRegion 
//	//!	@date		2016-06-18
//	void Renderer::CopyRegion(RenderContext* _pContext, Texture2D* _pTexture, uint32 /*_uiLeft*/, uint32 /*_uiTop*/, uint32 _uiWidth, uint32 /*_uiHeight*/, const void* _pBuffer)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11Texture2D* pD3D11Texture = static_cast<ID3D11Texture2D*>(_pTexture->GetHandle());
//		pD3D11DeferredContext->UpdateSubresource(pD3D11Texture, 0, NULL, _pBuffer, _uiWidth * 4, _uiWidth * 4);
//
//		// Copy a region into the texture
//		//D3D11_MAPPED_SUBRESOURCE kD3D11SubResource;
//		//HRESULT hResult = pD3D11ImmediateContext->Map(pD3D11Texture, 0, D3D11_MAP_WRITE_DISCARD , 0, &kD3D11SubResource);
//		/*if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] Failed to Copy region (HRESULT = 0x%0X)", hResult);
//			return;
//		}*/
//
//		/*memcpy_s(kD3D11SubResource.pData, _uiWidth * _uiHeight * 4, _pBuffer, _uiWidth * _uiHeight * 4);
//		pD3D11ImmediateContext->Unmap(pD3D11Texture, 0);*/
//	}
//
//	//!	@brief		Map 
//	//!	@date		2016-06-18
//	uint8* Renderer::Map(RenderContext* _pContext, Buffer* _pBuffer, EMap::Type _eMap /*= EMap::WriteDiscard*/)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11Buffer* pD3D11Buffer = static_cast<ID3D11Buffer*>(_pBuffer->GetHandle());
//
//		D3D11_MAPPED_SUBRESOURCE kD3D11SubResource;
//		HRESULT hResult = pD3D11DeferredContext->Map(pD3D11Buffer, 0, (D3D11_MAP)_eMap, 0, &kD3D11SubResource);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] Map failed (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		return static_cast<uint8*>(kD3D11SubResource.pData);
//	}
//	
//	//!	@brief		UnMap 
//	//!	@date		2016-06-18
//	void Renderer::Unmap(RenderContext* _pContext, Buffer* _pBuffer)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11Buffer* pD3D11Buffer = static_cast<ID3D11Buffer*>(_pBuffer->GetHandle());
//		pD3D11DeferredContext->Unmap(pD3D11Buffer, 0);
//	}
//
//	//!	@brief		Synchronize 
//	//!	@date		2016-06-18
//	void Renderer::Synchronize(RenderContext* _pContext, Material* _pMaterial)
//	{
//		_pMaterial->Synchronize(_pContext);
//	}
//
//	//!	@brief		UpdateMipMap 
//	//!	@date		2015-11-21
//	void Renderer::UpdateMipMap(RenderContext* _pContext, Texture3D* _pTexture)
//	{		
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11ShaderResourceView* pD3D11ShaderResourceView = static_cast<ID3D11ShaderResourceView*>(_pTexture->GetView(Texture3D::EView::ShaderResource));
//		pD3D11DeferredContext->GenerateMips(pD3D11ShaderResourceView);
//	}
//
//	//!	@brief		CreateShaderResourceView 
//	//!	@date		2016-06-18
//	void* Renderer::CreateShaderResourceView(Texture2D* _pTexture, uint32 _uiMipCount, uint32 /*_uiCount*/, EFormat::Type _eFormat, uint32 _eFlag)
//	{
//		ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
//		ID3D11Texture2D* pD3D11Texture = static_cast<ID3D11Texture2D*>(_pTexture->GetHandle());
//
//		// Create Shader Resource view Desc
//		D3D11_SHADER_RESOURCE_VIEW_DESC kD3D11ShaderResourceViewDesc;
//		ZeroMemory(&kD3D11ShaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
//
//		// Handle view dimension
//		switch(_eFlag)
//		{
//			case D3D11_RESOURCE_MISC_TEXTURECUBE:
//			{
//				kD3D11ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
//				kD3D11ShaderResourceViewDesc.TextureCube.MipLevels = _uiMipCount;
//				break;
//			}
//
//			default:
//			{
//				kD3D11ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//				kD3D11ShaderResourceViewDesc.Texture2D.MipLevels = _uiMipCount;
//				break;
//			}
//		}
//
//		// Handle format
//		switch(_eFormat)
//		{
//			case EFormat::R32_TYPELESS:
//			{
//				kD3D11ShaderResourceViewDesc.Format = (DXGI_FORMAT)EFormat::R32_FLOAT;
//				break;
//			}
//
//			case EFormat::R24G8_TYPELESS:
//			{
//				kD3D11ShaderResourceViewDesc.Format = (DXGI_FORMAT)EFormat::R24_UNORM_X8_TYPELESS;
//				break;
//			}
//
//			default:
//			{
//				kD3D11ShaderResourceViewDesc.Format = (DXGI_FORMAT)_eFormat;
//				break;
//			}
//		}
//
//		// Create Shader Resource View
//		ID3D11ShaderResourceView* pD3D11ShaderResourceView = NULL;
//		HRESULT hResult = pD3D11Device->CreateShaderResourceView(pD3D11Texture, &kD3D11ShaderResourceViewDesc, &pD3D11ShaderResourceView);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] CreateShaderResourceView failed (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		return pD3D11ShaderResourceView;
//	}
//
//	//!	@brief		CreateShaderResourceView 
//	//!	@date		2016-06-18
//	void* Renderer::CreateShaderResourceView(Texture3D* _pTexture, uint32 _uiMipCount, EFormat::Type _eFormat)
//	{
//		ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
//		ID3D11Texture2D* pD3D11Texture = static_cast<ID3D11Texture2D*>(_pTexture->GetHandle());
//
//		// Create Shader Resource view Desc
//		D3D11_SHADER_RESOURCE_VIEW_DESC kD3D11ShaderResourceViewDesc;
//		ZeroMemory(&kD3D11ShaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
//		kD3D11ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
//		kD3D11ShaderResourceViewDesc.Texture3D.MipLevels = _uiMipCount;
//		switch(_eFormat)
//		{
//			case EFormat::R24G8_TYPELESS:
//			{
//				kD3D11ShaderResourceViewDesc.Format = (DXGI_FORMAT)EFormat::R24_UNORM_X8_TYPELESS;
//				break;
//			}
//
//			default:
//			{
//				kD3D11ShaderResourceViewDesc.Format = (DXGI_FORMAT)_eFormat;
//				break;
//			}
//		}
//
//		// Create Shader Resource View
//		ID3D11ShaderResourceView* pD3D11ShaderResourceView = NULL;
//		HRESULT hResult = pD3D11Device->CreateShaderResourceView(pD3D11Texture, &kD3D11ShaderResourceViewDesc, &pD3D11ShaderResourceView);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] CreateShaderResourceView failed (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		return pD3D11ShaderResourceView;
//	}
//
//	//!	@brief		CreateShaderResourceView 
//	//!	@date		2016-06-18
//	void* Renderer::CreateShaderResourceView(Buffer* _pBuffer, EFormat::Type _eFormat)
//	{
//		ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
//		ID3D11Buffer* pD3D11Buffer = static_cast<ID3D11Buffer*>(_pBuffer->GetHandle());
//
//		// Create D3D11ShaderResourceViewDesc
//		D3D11_SHADER_RESOURCE_VIEW_DESC kD3D11ShaderResourceViewDesc;
//		ZeroMemory(&kD3D11ShaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
//		kD3D11ShaderResourceViewDesc.Format = (DXGI_FORMAT)_eFormat;
//		kD3D11ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
//		kD3D11ShaderResourceViewDesc.Buffer.FirstElement = 0;
//		kD3D11ShaderResourceViewDesc.Buffer.NumElements = _pBuffer->GetElementCount();
//
//		// Create Shader Resource View
//		ID3D11ShaderResourceView* pD3D11ShaderResourceView = NULL;
//		HRESULT hResult = pD3D11Device->CreateShaderResourceView(pD3D11Buffer, &kD3D11ShaderResourceViewDesc, &pD3D11ShaderResourceView);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] CreateShaderResourceView failed (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		return pD3D11ShaderResourceView;
//	}
//
//	//!	@brief		CreateRenderTargetView 
//	//!	@date		2016-06-18
//	void* Renderer::CreateRenderTargetView(void* _pHandle)
//	{
//		ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
//		ID3D11Resource* pD3D11Resource = static_cast<ID3D11Texture2D*>(_pHandle);
//
//		// Create RenderTarget View
//		ID3D11RenderTargetView* pD3D11RenderTargetView = NULL;
//		HRESULT hResult = pD3D11Device->CreateRenderTargetView(pD3D11Resource, NULL, &pD3D11RenderTargetView);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] CreateRenderTargetView failed (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		return pD3D11RenderTargetView;
//	}
//
//	//!	@brief		CreateDepthStenciltView 
//	//!	@date		2016-06-18
//	void* Renderer::CreateDepthStenciltView(Texture2D* _pTexture)
//	{
//		ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
//		ID3D11Texture2D* pD3D11Texture = static_cast<ID3D11Texture2D*>(_pTexture->GetHandle());
//
//		D3D11_TEXTURE2D_DESC desc;
//		pD3D11Texture->GetDesc(&desc);
//
//		// Create DepthStencil view Desc
//		D3D11_DEPTH_STENCIL_VIEW_DESC kD3D11DepthStencilViewDesc;
//		ZeroMemory(&kD3D11DepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
//		kD3D11DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
//		switch(desc.Format)
//		{
//			case EFormat::R32_TYPELESS:
//			{
//				kD3D11DepthStencilViewDesc.Format = (DXGI_FORMAT)EFormat::D32_FLOAT;
//				break;
//			}
//
//			default:
//			{
//				kD3D11DepthStencilViewDesc.Format = (DXGI_FORMAT)EFormat::D24_UNORM_S8_UINT;
//				break;
//			}
//		}
//
//		// Create Depth Stencil View
//		ID3D11DepthStencilView* pD3D11DepthStencilView = NULL;
//		HRESULT hResult = pD3D11Device->CreateDepthStencilView(pD3D11Texture, &kD3D11DepthStencilViewDesc, &pD3D11DepthStencilView);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] CreateDepthStencilView failed (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		return pD3D11DepthStencilView;
//	}
//
//	//!	@brief		CreateUnorderedAccessView 
//	//!	@date		2016-06-18
//	void* Renderer::CreateUnorderedAccessView(Buffer* _pBuffer, EFormat::Type _eFormat)
//	{
//		ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
//		ID3D11Buffer* pD3D11Buffer = static_cast<ID3D11Buffer*>(_pBuffer->GetHandle());
//
//		// Create kD3D11UnorderesAccessViewDesc
//		D3D11_UNORDERED_ACCESS_VIEW_DESC kD3D11UnorderesAccessViewDesc;
//		ZeroMemory(&kD3D11UnorderesAccessViewDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
//		kD3D11UnorderesAccessViewDesc.Format = (DXGI_FORMAT)_eFormat;
//		kD3D11UnorderesAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
//		kD3D11UnorderesAccessViewDesc.Buffer.FirstElement = 0;
//		kD3D11UnorderesAccessViewDesc.Buffer.NumElements = _pBuffer->GetElementCount();
//
//		// Create RenderTarget View
//		ID3D11UnorderedAccessView* pD3D11UnorderdAccessView = NULL;
//		HRESULT hResult = pD3D11Device->CreateUnorderedAccessView(pD3D11Buffer, &kD3D11UnorderesAccessViewDesc, &pD3D11UnorderdAccessView);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] CreateUnorderedAccessView failed (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		return pD3D11UnorderdAccessView;
//	}
//
//	//!	@brief		CreateUnorderedAccessView 
//	//!	@date		2016-06-18
//	void* Renderer::CreateUnorderedAccessView(Texture2D* _pTexture, EFormat::Type _eFormat)
//	{
//		ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
//		ID3D11Resource* pD3D11Resource = static_cast<ID3D11Resource*>(_pTexture->GetHandle());
//
//		// Create kD3D11UnorderesAccessViewDesc
//		D3D11_UNORDERED_ACCESS_VIEW_DESC kD3D11UnorderesAccessViewDesc;
//		ZeroMemory(&kD3D11UnorderesAccessViewDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
//		kD3D11UnorderesAccessViewDesc.Format = (DXGI_FORMAT)_eFormat;
//		kD3D11UnorderesAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
//		kD3D11UnorderesAccessViewDesc.Texture2DArray.ArraySize = 6;
//
//		// Create RenderTarget View
//		ID3D11UnorderedAccessView* pD3D11UnorderdAccessView = NULL;
//		HRESULT hResult = pD3D11Device->CreateUnorderedAccessView(pD3D11Resource, &kD3D11UnorderesAccessViewDesc, &pD3D11UnorderdAccessView);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] CreateUnorderedAccessView failed (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		return pD3D11UnorderdAccessView;
//	}
//
//	//!	@brief		CreateUnorderedAccessView 
//	//!	@date		2016-06-18
//	void* Renderer::CreateUnorderedAccessView(Texture3D* _pTexture, EFormat::Type _eFormat)
//	{
//		ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
//		ID3D11Resource* pD3D11Resource = static_cast<ID3D11Resource*>(_pTexture->GetHandle());
//
//		// Create kD3D11UnorderesAccessViewDesc
//		D3D11_UNORDERED_ACCESS_VIEW_DESC kD3D11UnorderesAccessViewDesc;
//		ZeroMemory(&kD3D11UnorderesAccessViewDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
//		kD3D11UnorderesAccessViewDesc.Format = (DXGI_FORMAT)_eFormat;
//		kD3D11UnorderesAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
//		kD3D11UnorderesAccessViewDesc.Texture3D.WSize = _pTexture->GetWidth();
//
//		// Create RenderTarget View
//		ID3D11UnorderedAccessView* pD3D11UnorderdAccessView = NULL;
//		HRESULT hResult = pD3D11Device->CreateUnorderedAccessView(pD3D11Resource, &kD3D11UnorderesAccessViewDesc, &pD3D11UnorderdAccessView);
//		if(FAILED(hResult))
//		{
//			LOG_ERROR("[Renderer] CreateUnorderedAccessView failed (HRESULT = 0x%0X)", hResult);
//			return NULL;
//		}
//
//		return pD3D11UnorderdAccessView;
//	}
//
//	//!	@brief		SetRenderTarget 
//	//!	@date		2015-11-21
//	void Renderer::SetViewport(RenderContext* _pContext, uint32 _uiOffsetX, uint32 _uiOffsetY, uint32 _uiWidth, uint32 _uiHeight)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//
//		// Create proper D3D11_VIEWPORT struct to set it on the deferred context
//		D3D11_VIEWPORT kD3D11Viewport;
//		ZeroMemory(&kD3D11Viewport, sizeof(D3D11_VIEWPORT));
//		kD3D11Viewport.TopLeftX = (float32)_uiOffsetX;
//		kD3D11Viewport.TopLeftY = (float32)_uiOffsetY;
//		kD3D11Viewport.Width = (float32)_uiWidth;
//		kD3D11Viewport.Height = (float32)_uiHeight;
//		kD3D11Viewport.MinDepth = 0.0f;
//		kD3D11Viewport.MaxDepth = 1.0f;
//		pD3D11DeferredContext->RSSetViewports(1, &kD3D11Viewport);
//	}
//
//	//!	@brief		SetRenderTarget 
//	//!	@date		2015-11-21
//	void Renderer::SetRenderTarget(RenderContext* _pContext, RenderTarget* _pRenderTarget)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//
//		const uint32 uiRenderTargetCount = _pRenderTarget->GetTargetCount();
//		ID3D11RenderTargetView** ppD3D11RenderTargetView = reinterpret_cast<ID3D11RenderTargetView**>(&_pRenderTarget->GetVecTargetView()[0]);
//		ID3D11DepthStencilView* pD3D11DepthStencilView = static_cast<ID3D11DepthStencilView*>(_pRenderTarget->GetDepthStencilView());
//
//		const uint32 uiBufferCount = _pRenderTarget->GetOutputCount();
//		ID3D11UnorderedAccessView** ppD3D11UnorderedAccessView = reinterpret_cast<ID3D11UnorderedAccessView**>(&_pRenderTarget->GetVecOutputView()[0]);
//		pD3D11DeferredContext->OMSetRenderTargetsAndUnorderedAccessViews(uiRenderTargetCount, ppD3D11RenderTargetView, pD3D11DepthStencilView, uiRenderTargetCount, uiBufferCount, ppD3D11UnorderedAccessView, 0);
//	}
//
//	//!	@brief		SetRenderState 
//	//!	@date		2015-11-21
//	void Renderer::SetRenderState(RenderContext* _pContext, RenderState* _pRenderState)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//
//		// RasterizerState
//		ID3D11RasterizerState* pD3D11RasterizerState = static_cast<ID3D11RasterizerState*>(_pRenderState->GetRasterizerState());
//		pD3D11DeferredContext->RSSetState(pD3D11RasterizerState);
//
//		// DepthStencilState
//		ID3D11DepthStencilState* pD3D11DepthStencilState = static_cast<ID3D11DepthStencilState*>(_pRenderState->GetDepthStencilState());
//		pD3D11DeferredContext->OMSetDepthStencilState(pD3D11DepthStencilState, 0);
//
//		// BlendState
//		float32 aBlendFactor[] = {1.0f, 1.0f, 1.0f, 1.0f};
//		ID3D11BlendState* pD3D11BlendState = static_cast<ID3D11BlendState*>(_pRenderState->GetBlendState());
//		pD3D11DeferredContext->OMSetBlendState(pD3D11BlendState, aBlendFactor, 0xffffffff);
//	}
//
//	//!	@brief		SetTopology 
//	//!	@date		2015-11-21
//	void Renderer::SetTopology(RenderContext* _pContext, ETopology::Type _eTopology)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		pD3D11DeferredContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)_eTopology);
//	}
//
//	//!	@brief		SetVertexBuffer 
//	//!	@date		2015-11-21
//	void Renderer::SetVertexBuffer(RenderContext* _pContext, Buffer* _pVertexBuffer)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11Buffer* pD3D11VertexBuffer = static_cast<ID3D11Buffer*>(_pVertexBuffer->GetHandle());
//
//		uint32 uiOffset = 0;
//		uint32 uiElementSize = _pVertexBuffer->GetElementSize();
//		pD3D11DeferredContext->IASetVertexBuffers(0, 1, &pD3D11VertexBuffer, &uiElementSize, &uiOffset);
//	}
//
//	//!	@brief		SetIndexBuffer 
//	//!	@date		2015-11-21
//	void Renderer::SetIndexBuffer(RenderContext* _pContext, Buffer* _pIndexBuffer)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11Buffer* pD3D11IndexBuffer = static_cast<ID3D11Buffer*>(_pIndexBuffer->GetHandle());
//		pD3D11DeferredContext->IASetIndexBuffer(pD3D11IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
//	}
//
//	//!	@brief		SetVertexShader
//	//!	@date		2015-11-21
//	void Renderer::SetVertexShader(RenderContext* _pContext, Shader* _pShader)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//
//		// Input Layout
//		ID3D11InputLayout* pD3D11InputLayout = static_cast<ID3D11InputLayout*>(_pShader->GetInputLayout());
//		pD3D11DeferredContext->IASetInputLayout(pD3D11InputLayout);
//
//		// Shader
//		ID3D11VertexShader* pD3D11VertexShader = static_cast<ID3D11VertexShader*>(_pShader->GetHandle());
//		pD3D11DeferredContext->VSSetShader(pD3D11VertexShader, NULL, 0);
//	}
//
//	//!	@brief		SetVertexShaderConstantBuffer 
//	//!	@date		2015-11-21
//	void Renderer::SetVertexShaderConstantBuffer(RenderContext* _pContext, uint32 _uiIndex, Buffer* _pBuffer)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11Buffer* pD3D11Buffer = static_cast<ID3D11Buffer*>(_pBuffer->GetHandle());
//		pD3D11DeferredContext->VSSetConstantBuffers(_uiIndex, 1, &pD3D11Buffer);
//	}
//
//	//!	@brief		SetVertexShaderResource 
//	//!	@date		2015-11-21
//	void Renderer::SetVertexShaderResource(RenderContext* _pContext, uint32 _uiIndex, Buffer* _pBuffer)
//	{
//		SetVertexShaderResource(_pContext, _uiIndex, _pBuffer->GetView(Buffer::EView::ShaderResource));
//	}
//
//	//!	@brief		SetVertexShaderResource 
//	//!	@date		2015-11-21
//	void Renderer::SetVertexShaderResource(RenderContext* _pContext, uint32 _uiIndex, Texture2D* _pTexture)
//	{
//		SetVertexShaderResource(_pContext, _uiIndex, _pTexture->GetView(Texture2D::EView::ShaderResource));
//	}
//
//	//!	@brief		SetVertexShaderResource 
//	//!	@date		2015-11-21
//	void Renderer::SetVertexShaderResource(RenderContext* _pContext, uint32 _uiIndex, void* _pView)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11ShaderResourceView* pD3D11ShaderResourceView = static_cast<ID3D11ShaderResourceView*>(_pView);
//		pD3D11DeferredContext->VSSetShaderResources(_uiIndex, 1, &pD3D11ShaderResourceView);
//	}
//
//	//!	@brief		SetVertexShaderSamplerState 
//	//!	@date		2015-11-21
//	void Renderer::SetVertexShaderSamplerState(RenderContext* _pContext, uint32 _uiIndex, SamplerState* _pSamplerState)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11SamplerState* pD3D11SamplerState = static_cast<ID3D11SamplerState*>(_pSamplerState->GetHandle());
//		pD3D11DeferredContext->VSSetSamplers(_uiIndex, 1, &pD3D11SamplerState);
//	}
//
//	//!	@brief		SetGeometryShader
//	//!	@date		2015-11-21
//	void Renderer::SetGeometryShader(RenderContext* _pContext, Shader* _pShader)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11GeometryShader* pD3D11GeometryShader = _pShader ? static_cast<ID3D11GeometryShader*>(_pShader->GetHandle()) : NULL;
//		pD3D11DeferredContext->GSSetShader(pD3D11GeometryShader, NULL, 0);
//	}
//
//	//!	@brief		SetGeometryShaderConstantBuffer 
//	//!	@date		2015-11-21
//	void Renderer::SetGeometryShaderConstantBuffer(RenderContext* _pContext, uint32 _uiIndex, Buffer* _pBuffer)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11Buffer* pD3D11Buffer = static_cast<ID3D11Buffer*>(_pBuffer->GetHandle());
//		pD3D11DeferredContext->GSSetConstantBuffers(_uiIndex, 1, &pD3D11Buffer);
//	}
//
//	//!	@brief		SetGeometryShaderResource 
//	//!	@date		2015-11-21
//	void Renderer::SetGeometryShaderResource(RenderContext* _pContext, uint32 _uiIndex, Buffer* _pBuffer)
//	{
//		SetGeometryShaderResource(_pContext, _uiIndex, _pBuffer->GetView(Buffer::EView::ShaderResource));
//	}
//
//	//!	@brief		SetGeometryShaderResource 
//	//!	@date		2015-11-21
//	void Renderer::SetGeometryShaderResource(RenderContext* _pContext, uint32 _uiIndex, Texture2D* _pTexture)
//	{
//		SetGeometryShaderResource(_pContext, _uiIndex, _pTexture->GetView(Texture2D::EView::ShaderResource));
//	}
//
//	//!	@brief		SetGeometryShaderResource 
//	//!	@date		2015-11-21
//	void Renderer::SetGeometryShaderResource(RenderContext* _pContext, uint32 _uiIndex, void* _pView)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11ShaderResourceView* pD3D11ShaderResourceView = static_cast<ID3D11ShaderResourceView*>(_pView);
//		pD3D11DeferredContext->GSSetShaderResources(_uiIndex, 1, &pD3D11ShaderResourceView);
//	}
//
//	//!	@brief		SetPixelShader
//	//!	@date		2015-11-21
//	void Renderer::SetPixelShader(RenderContext* _pContext, Shader* _pShader)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11PixelShader* pD3D11PixelShader = static_cast<ID3D11PixelShader*>(_pShader->GetHandle());
//		pD3D11DeferredContext->PSSetShader(pD3D11PixelShader, NULL, 0);
//	}
//
//	//!	@brief		SetPixelShaderConstantBuffer
//	//!	@date		2015-11-21
//	void Renderer::SetPixelShaderConstantBuffer(RenderContext* _pContext, uint32 _uiIndex, Buffer* _pBuffer)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11Buffer* pD3D11Buffer = static_cast<ID3D11Buffer*>(_pBuffer->GetHandle());
//		pD3D11DeferredContext->PSSetConstantBuffers(_uiIndex, 1, &pD3D11Buffer);
//	}
//
//	//!	@brief		SetPixelShaderResource 
//	//!	@date		2015-11-21
//	void Renderer::SetPixelShaderResource(RenderContext* _pContext, uint32 _uiIndex, Buffer* _pBuffer)
//	{
//		SetPixelShaderResource(_pContext, _uiIndex, _pBuffer->GetView(Buffer::EView::ShaderResource));
//	}
//
//	//!	@brief		SetPixelShaderResource 
//	//!	@date		2015-11-21
//	void Renderer::SetPixelShaderResource(RenderContext* _pContext, uint32 _uiIndex, Texture2D* _pTexture)
//	{
//		SetPixelShaderResource(_pContext, _uiIndex, _pTexture->GetView(Texture2D::EView::ShaderResource));
//	}
//
//	//!	@brief		SetPixelShaderResource 
//	//!	@date		2015-11-21
//	void Renderer::SetPixelShaderResource(RenderContext* _pContext, uint32 _uiIndex, Texture3D* _pTexture)
//	{
//		SetPixelShaderResource(_pContext, _uiIndex, _pTexture->GetView(Texture3D::EView::ShaderResource));
//	}
//
//	//!	@brief		SetPixelShaderResource 
//	//!	@date		2015-11-21
//	void Renderer::SetPixelShaderResource(RenderContext* _pContext, uint32 _uiIndex, void* _pView)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11ShaderResourceView* pD3D11ShaderResourceView = static_cast<ID3D11ShaderResourceView*>(_pView);
//		pD3D11DeferredContext->PSSetShaderResources(_uiIndex, 1, &pD3D11ShaderResourceView);
//	}
//
//	//!	@brief		SetPixelShaderSamplerState 
//	//!	@date		2015-11-21
//	void Renderer::SetPixelShaderSamplerState(RenderContext* _pContext, uint32 _uiIndex, SamplerState* _pSamplerState)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11SamplerState* pD3D11SamplerState = static_cast<ID3D11SamplerState*>(_pSamplerState->GetHandle());
//		pD3D11DeferredContext->PSSetSamplers(_uiIndex, 1, &pD3D11SamplerState);
//	}
//
//	//!	@brief		SetComputeShader
//	//!	@date		2015-11-21
//	void Renderer::SetComputeShader(RenderContext* _pContext, Shader* _pShader)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11ComputeShader* pD3D11ComputeShader = static_cast<ID3D11ComputeShader*>(_pShader->GetHandle());
//		pD3D11DeferredContext->CSSetShader(pD3D11ComputeShader, NULL, 0);
//	}
//
//	//!	@brief		SetComputeShaderSamplerState 
//	//!	@date		2015-11-21
//	void Renderer::SetComputeShaderSamplerState(RenderContext* _pContext, uint32 _uiIndex, SamplerState* _pSamplerState)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11SamplerState* pD3D11SamplerState = static_cast<ID3D11SamplerState*>(_pSamplerState->GetHandle());
//		pD3D11DeferredContext->CSSetSamplers(_uiIndex, 1, &pD3D11SamplerState);
//	}
//
//	//!	@brief		SetComputeShaderConstantBuffer 
//	//!	@date		2015-11-21
//	void Renderer::SetComputeShaderConstantBuffer(RenderContext* _pContext, uint32 _uiIndex, Buffer* _pBuffer)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11Buffer* pD3D11Buffer = static_cast<ID3D11Buffer*>(_pBuffer->GetHandle());
//		pD3D11DeferredContext->CSSetConstantBuffers(_uiIndex, 1, &pD3D11Buffer);
//	}
//
//	//!	@brief		SetComputeShaderResource 
//	//!	@date		2015-11-21
//	void Renderer::SetComputeShaderResource(RenderContext* _pContext, uint32 _uiIndex, Buffer* _pBuffer)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11ShaderResourceView* pD3D11ShaderResourceView = static_cast<ID3D11ShaderResourceView*>(_pBuffer->GetView(Buffer::EView::ShaderResource));
//		pD3D11DeferredContext->CSSetShaderResources(_uiIndex, 1, &pD3D11ShaderResourceView);
//	}
//
//	//!	@brief		SetComputeShaderResource 
//	//!	@date		2015-11-21
//	void Renderer::SetComputeShaderResource(RenderContext* _pContext, uint32 _uiIndex, Texture2D* _pTexture)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11ShaderResourceView* pD3D11ShaderResourceView = static_cast<ID3D11ShaderResourceView*>(_pTexture->GetView(Texture2D::EView::ShaderResource));
//		pD3D11DeferredContext->CSSetShaderResources(_uiIndex, 1, &pD3D11ShaderResourceView);
//	}
//
//	//!	@brief		SetComputeShaderResource 
//	//!	@date		2015-11-21
//	void Renderer::SetComputeShaderResource(RenderContext* _pContext, uint32 _uiIndex, Texture3D* _pTexture)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11ShaderResourceView* pD3D11ShaderResourceView = static_cast<ID3D11ShaderResourceView*>(_pTexture->GetView(Texture3D::EView::ShaderResource));
//		pD3D11DeferredContext->CSSetShaderResources(_uiIndex, 1, &pD3D11ShaderResourceView);
//	}
//
//	//!	@brief		UnsetComputeShaderResource
//	//!	@date		2015-11-21
//	void Renderer::UnsetComputeShaderResource(RenderContext* _pContext, uint32 _uiIndex, uint32 _uiCount)
//	{
//		m_vPointer.Resize(_uiCount, NULL);
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		pD3D11DeferredContext->CSSetShaderResources(_uiIndex, _uiCount, &reinterpret_cast<ID3D11ShaderResourceView*&>(m_vPointer[0]));
//	}
//
//	//!	@brief		SetComputeShaderUnorderedAccess
//	//!	@date		2015-11-21
//	void Renderer::SetComputeShaderUnorderedAccess(RenderContext* _pContext, uint32 _uiIndex, Buffer* _pBuffer)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11UnorderedAccessView* pD3D11UnorderedAccessView = static_cast<ID3D11UnorderedAccessView*>(_pBuffer->GetView(Buffer::EView::UnorderedAccess));
//		pD3D11DeferredContext->CSSetUnorderedAccessViews(_uiIndex, 1, &pD3D11UnorderedAccessView, 0);
//	}
//
//	//!	@brief		SetComputeShaderUnorderedAccess
//	//!	@date		2015-11-21
//	void Renderer::SetComputeShaderUnorderedAccess(RenderContext* _pContext, uint32 _uiIndex, Texture3D* _pTexture)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11UnorderedAccessView* pD3D11UnorderedAccessView = static_cast<ID3D11UnorderedAccessView*>(_pTexture->GetView(Texture3D::EView::UnorderedAccess));
//		pD3D11DeferredContext->CSSetUnorderedAccessViews(_uiIndex, 1, &pD3D11UnorderedAccessView, 0);
//	}
//
//	//!	@brief		UnsetComputeShaderUnorderedAccess
//	//!	@date		2015-11-21
//	void Renderer::UnsetComputeShaderUnorderedAccess(RenderContext* _pContext, uint32 _uiIndex, uint32 _uiCount)
//	{
//		m_vPointer.Resize(_uiCount, NULL);
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		pD3D11DeferredContext->CSSetUnorderedAccessViews(_uiIndex, _uiCount, &reinterpret_cast<ID3D11UnorderedAccessView*&>(m_vPointer[0]), 0);
//	}
//
//	//!	@brief		Compute 
//	//!	@date		2015-11-21
//	void Renderer::Compute(RenderContext* _pContext, Buffer* _pBuffer, uint32 _uiOffset /*= 0*/)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		ID3D11Buffer* pD3D11Buffer = static_cast<ID3D11Buffer*>(_pBuffer->GetHandle());
//		pD3D11DeferredContext->DispatchIndirect(pD3D11Buffer, _uiOffset);
//	}
//
//	//!	@brief		Compute 
//	//!	@date		2015-11-21
//	void Renderer::Compute(RenderContext* _pContext, uint32 _uiThreadGroupCountX, uint32 _uiThreadGroupCountY, uint32 _uiThreadGroupCountZ)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		pD3D11DeferredContext->Dispatch(_uiThreadGroupCountX, _uiThreadGroupCountY, _uiThreadGroupCountZ);
//	}
//
//	//!	@brief		Clear 
//	//!	@date		2015-11-21
//	void Renderer::Clear(RenderContext* _pContext, RenderTarget* _pRenderTarget, uint32 _eBindFlag /*= (uint32)-1*/)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//
//		// Clear RenderTarget
//		if(_eBindFlag & EBind::RenderTarget)
//		{
//			static float32 aColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
//			const uint32 uiRenderTargetCount = _pRenderTarget->GetTargetCount();
//			for(uint32 uiRenderTarget = 0; uiRenderTarget < uiRenderTargetCount; ++uiRenderTarget)
//			{
//				ID3D11RenderTargetView* pD3D11RenderTargetView = static_cast<ID3D11RenderTargetView*>(_pRenderTarget->GetVecTargetView()[uiRenderTarget]);
//				pD3D11DeferredContext->ClearRenderTargetView(pD3D11RenderTargetView, aColor);
//			}
//		}
//		
//		// Clear UnorderedAccess
//		if(_eBindFlag & EBind::UnorderedAccess)
//		{
//			static uint32 test[4] = {0,0,0,0};
//
//			const uint32 uiOutputCount = _pRenderTarget->GetOutputCount();
//			for(uint32 uiOutput = 0; uiOutput < uiOutputCount; ++uiOutput)
//			{
//				ID3D11UnorderedAccessView* pD3D11UnorderesAccessView = static_cast<ID3D11UnorderedAccessView*>(_pRenderTarget->GetVecOutputView()[uiOutput]);
//				pD3D11DeferredContext->ClearUnorderedAccessViewUint(pD3D11UnorderesAccessView, test);
//			}
//		}
//		
//		// Clear DepthStencil
//		if(_eBindFlag & EBind::DepthStencil)
//		{
//			if(_pRenderTarget->GetDepthStencilView())
//			{
//				ID3D11DepthStencilView* pD3D11DepthStencilView = static_cast<ID3D11DepthStencilView*>(_pRenderTarget->GetDepthStencilView());
//				pD3D11DeferredContext->ClearDepthStencilView(pD3D11DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//			}
//		}
//	}
//
//	//!	@brief		Draw 
//	//!	@date		2015-11-21
//	void Renderer::Draw(RenderContext* _pContext, uint32 _uiIndexOffset, uint32 _uiIndexCount, uint32 _uiInstanceCount /*= 1*/)
//	{
//		ID3D11DeviceContext* pD3D11DeferredContext = static_cast<ID3D11DeviceContext*>(_pContext->GetHandle());
//		pD3D11DeferredContext->DrawIndexedInstanced(_uiIndexCount, _uiInstanceCount, _uiIndexOffset, 0, 0);
//	}
//
//	//!	@brief		DestroyResource 
//	//!	@date		2015-11-21
//	void Renderer::DestroyResource(Resource* _pResource)
//	{
//		_pResource->RemoveReference();
//		if(!_pResource->HasReference())
//		{
//			DestroyRequest& kRequest = m_vDestroyRequest.Add();
//			kRequest.Frame = 0;
//			kRequest.Resource = _pResource;
//		}
//	}
//
//	//!	@brief		DestroyHandle 
//	//!	@date		2015-11-21
//	void Renderer::DestroyHandle(void* _pHandle)
//	{
//		ID3D11Resource* pD3D11Resource = static_cast<ID3D11Resource*>(_pHandle);
//		pD3D11Resource->Release();
//	}
//}}
