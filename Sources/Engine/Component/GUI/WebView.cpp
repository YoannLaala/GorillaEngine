/******************************************************************************
**	Includes
******************************************************************************/
#include "WebView.hpp"

#include <Core/Input/Input.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Renderer/ResourceShared.hpp>
#include <Engine/World.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/RenderContext.hpp>
#include <Renderer/Resource/Texture2D.hpp>
#include <Renderer/Scene.hpp>
#include <Renderer/Viewport.hpp>
#include <Engine/Renderer/Pass/FullScreenPass.hpp>

#include <Web/WebBrowser.hpp>
#include <Web/WebPage.hpp>
#include <Web/WebFunction.hpp>

/******************************************************************************
**	Defines
******************************************************************************/
#define MOUSE_SCROLL_SPEED 1.0f

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Component
{	
	COMPONENT_REGISTER(Gorilla::Component::WebView)

	//!	@brief		Constructor
	//!	@date		2015-04-04
	WebView::WebView()
		: m_pInstance(nullptr)
		, m_pTexture(nullptr)
		, RenderTexture(nullptr)
		, Transparent(false)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	WebView::~WebView()
	{
		// Nothing to do
	}

	//bool WebView::Serialize		(FileWriter& /*_kStream*/) { return true; };
	//bool WebView::Deserialize	(FileReader& /*_kStream*/) { return true; };

	//!	@brief		Start
	//!	@date		2015-04-04
	void WebView::Start()
	{
		// Set default RenderTexture if neeeded
		if(!RenderTexture)
		{
			Renderer::Viewport* pViewport = nullptr;
			Renderer::Scene* pScene = GetGameObject()->GetWorld()->GetScene();
			if(pScene->GetCameraCount()) pViewport = pScene->GetCamera(0)->GetViewport();
			else pViewport = GetRenderer()->GetViewport(0);
			SIGNAL_CONNECT(pViewport, this, SizeChanged);
			RenderTexture = pViewport->GetRenderTarget()->GetTarget(0);
		}
		
		// Open page 
		uint32 uiWidth = RenderTexture->GetWidth(); 
		uint32 uiHeight = RenderTexture->GetHeight();
		m_pInstance = GetBrowser()->Open(nullptr, uiWidth, uiHeight, Transparent);
		OnSizeChanged(uiWidth, uiHeight);

		Renderable::Start();
	}

	//!	@brief		Stop
	//!	@date		2015-04-04
	void WebView::Update()
	{	
		if(Document.IsLoaded() && Document->GetUrl() != m_pInstance->GetUrl())
		{
			m_pInstance->Open(Document->GetUrl().GetBuffer());
		}

		//LOG_PRINT("from game %.2f / %.2f", GetInput()->GetMouse()->GetPosition().GetX(), GetInput()->GetMouse()->GetPosition().GetY());

		//m_pInstance->SetMousePosition()

		//if(m_pInstance->HasFlag(Web::WebPage::EFlag::Changed))
		//{
		//	//m_pRenderer->CopyRegion(m_pTexture, 0, 0, _iWidth, _iHeight, _pBuffer);
		//	//GetRenderer()->CopyRegion(RenderTexture, 0, 0, RenderTexture->GetWidth(), RenderTexture->GetHeight(), m_pInstance->Buffer);
		//	m_pInstance->RemoveFlag(Web::WebPage::EFlag::Changed);
		//}
		
		// Scroll up
		//if(GetInput()->IsMouseScroll())
		//{
		//	float32 fScroll = -GetInput()->GetMouseWheel() * MOUSE_SCROLL_SPEED;
		//	//m_pInstance->ScrollY(fScroll);
		//}

		m_pInstance->Update();
	}

	//!	@brief		Stop
	//!	@date		2015-04-04
	void WebView::Stop()
	{
		GetBrowser()->Close(m_pInstance);	
		Renderable::Stop();
	}

	//!	@brief		OnSizeChanged
	//!	@date		2015-04-04
	void WebView::OnSizeChanged(uint32 _uiWidth, uint32 _uiHeight)
	{
		Renderer::Renderer* pRenderer = GetRenderer();
		if(m_pTexture)
		{
			if(m_pTexture->GetWidth() == _uiWidth && m_pTexture->GetHeight() == _uiHeight) return;
			pRenderer->DestroyResource(m_pTexture);
		}
		m_pTexture = pRenderer->CreateTexture2D(_uiWidth, _uiHeight, 1, Renderer::EFormat::R8G8B8A8_UNORM, Renderer::EBind::ShaderResource | Renderer::EBind::RenderTarget);
		m_pInstance->Resize(_uiWidth, _uiHeight);
	}

	//!	@brief		PushCommand
	//!	@date		2015-04-04
	void WebView::PushCommand(Renderer::RenderBuffer* _pBuffer)
	{
		Engine::RenderBuffer::Operation::TextureCopy* pOperation = nullptr;
		if(m_pInstance->HasFlag(Web::WebPage::EFlag::Changed))
		{
			// Copy buffer to the ABGR internal texture
			pOperation = _pBuffer->Push<Engine::RenderBuffer::Operation::TextureCopy>();
			pOperation->Type = Engine::RenderBuffer::Operation::Buffer2Texture;
			pOperation->Source = const_cast<void*>(m_pInstance->GetBuffer());
			pOperation->Destination = m_pInstance->GetTexture();

			m_pInstance->RemoveFlag(Web::WebPage::EFlag::Changed);

			// Convert the texture to RGBA render texture (used for sampling in case a quad)
			pOperation = _pBuffer->Push<Engine::RenderBuffer::Operation::TextureCopy>();
			pOperation->Type = Engine::RenderBuffer::Operation::ABGR2RGBA;
			pOperation->Source = m_pInstance->GetTexture();
			pOperation->Destination = m_pTexture;
		}

		// Copy to RenderTexture
		Engine::FullScreenPass::Command* pCommand = _pBuffer->Push<Engine::FullScreenPass::Command>();
		pCommand->Source = m_pTexture;
	}

	//!	@brief		GetBrowser
	//!	@date		2016-05-01
	Web::WebBrowser* WebView::GetBrowser()
	{
		static Web::WebBrowser kBrowser(GetRenderer());
		return &kBrowser;
	}
}}	
