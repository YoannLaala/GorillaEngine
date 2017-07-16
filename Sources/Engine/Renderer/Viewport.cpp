/******************************************************************************
**	Includes
******************************************************************************/
#include "Viewport.hpp"
#include <Renderer/Renderer.hpp>
#include <Renderer/Resource/Texture2D.hpp>
#include <Renderer/Resource/SwapChain.hpp>
#include <Renderer/Resource/RenderTarget.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	Viewport::Viewport()
		: m_pRenderTarget(nullptr)
		, m_pSwapChain(nullptr)
		, m_pContext(nullptr)
		, m_uiLeft(0)
		, m_uiTop(0)
		, m_uiWidth(0)
		, m_uiHeight(0)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	Viewport::~Viewport()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2015-11-11
	void Viewport::Initialize(RenderTarget* _pRenderTarget, SwapChain* _pSwapChain)
	{
		m_pRenderTarget = _pRenderTarget;
		m_pSwapChain = _pSwapChain;

		Texture2D* pRenderTexture = m_pRenderTarget->GetTarget(0);
		m_uiWidth = pRenderTexture->GetWidth();
		m_uiHeight = pRenderTexture->GetHeight();
	}

	//!	@brief		Release 
	//!	@date		2015-11-12
	void Viewport::Release()
	{
		SAFE_DELETE(m_pSwapChain);
	}

	//!	@brief		Resize 
	//!	@date		2015-11-11
	void Viewport::Resize(uint32 _uiWidth, uint32 _uiHeight)
	{
		// Check if we really need to resize
		if(m_uiWidth == _uiWidth && m_uiHeight == _uiHeight)
		{
			return;
		}
		m_uiWidth = _uiWidth;
		m_uiHeight = _uiHeight;

		// Create a new SwapChain
		Texture2D* pRenderTexture = m_pRenderTarget->GetTarget(0);
		if(m_pSwapChain)
		{
			Window* pWindow = m_pSwapChain->GetWindow();
			Renderer::GetInstance()->DestroyResource(m_pSwapChain);
			m_pSwapChain = Renderer::GetInstance()->CreateSwapChain(pWindow, _uiWidth, _uiHeight, EFormat::R8G8B8A8_UNORM);
			pRenderTexture = m_pSwapChain->GetTexture();
		}
		else
		{
			Renderer::GetInstance()->DestroyResource(pRenderTexture);
			pRenderTexture = Renderer::GetInstance()->CreateTexture2D(_uiWidth, _uiHeight, 1, EFormat::R8G8B8A8_UNORM, EBind::RenderTarget);
		}
		m_pRenderTarget->Clear();
		m_pRenderTarget->AddTarget(pRenderTexture);
		
		// Resize the render context and the output texture
		//if(m_pContext) m_pContext->Resize(pRenderTexture);
		SIGNAL_SEND(SizeChanged, _uiWidth, _uiHeight);
	}
}}