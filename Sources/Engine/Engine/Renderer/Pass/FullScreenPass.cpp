/******************************************************************************
**	Includes
******************************************************************************/
#include "FullScreenPass.hpp"

#include <Engine/Renderer/RenderFilter.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/RenderContext.hpp>
#include <Renderer/Resource/RenderTarget.hpp>
#include <Renderer/Viewport.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	FullScreenPass::FullScreenPass()
		: m_pRenderTarget(nullptr)
	{
		memset(m_aFilter, 0, sizeof(m_aFilter));
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	FullScreenPass::~FullScreenPass()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2015-11-11
	void FullScreenPass::Initialize(Renderer::RenderContext* _pContext)
	{
		m_pRenderTarget = _pContext->GetRenderer()->CreateRenderTarget();

		static const char* aFilterPath[EFilter::Count] = 
		{
			"@Effect/Filter/Operation/Identity.ps",
			"@Effect/Filter/Operation/ABGR_2_RGBA.ps"
		};

		for(uint32 eFilter = 0; eFilter < EFilter::Count; ++eFilter)
		{
			m_aFilter[eFilter] = new RenderFilter();
			m_aFilter[eFilter]->Initialize(_pContext, aFilterPath[eFilter]);
		}
		
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void FullScreenPass::Release(Renderer::RenderContext* _pContext)
	{
		m_aFilter[EFilter::ABGR2RGBA]->Release(_pContext);
		SAFE_DELETE(m_aFilter[EFilter::ABGR2RGBA]);

		_pContext->GetRenderer()->DestroyResource(m_pRenderTarget);
	}

	//!	@brief		Execute 
	//!	@date		2016-06-25
	void FullScreenPass::Execute(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pFrameBuffer)
	{
		// Texture to Texture
		Command* pCommand; uint32 uiCount;
		if(_pFrameBuffer->Get(&pCommand, uiCount))
		{
			for(uint32 uiCommand = 0; uiCommand < uiCount; ++uiCommand)
			{
				ExecuteCommand(_pRenderer, _pContext, &pCommand[uiCommand]);
			}
		}
	}

	//!	@brief		ExecuteCommand 
	//!	@date		2016-06-25
	void FullScreenPass::ExecuteCommand(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Command* pCommand)
	{
		Renderer::Texture2D* pSource = static_cast<Renderer::Texture2D*>(pCommand->Source);
		
		// Execute Filter
		RenderFilter* pFilter = m_aFilter[EFilter::Identity];
		pFilter->SetRenderTarget(_pContext->GetViewport()->GetRenderTarget());
		pFilter->SetResource(0, pSource);	
		_pRenderer->SetViewport(_pContext, 0, 0, pSource->GetWidth(), pSource->GetHeight());
		_pRenderer->SetBlendState(_pContext, Renderer::EBlend::SourceAlpha, Renderer::EBlendOperation::Add, Renderer::EBlend::SourceAlphaInversed);
		pFilter->Execute3(_pContext);
	}
}}