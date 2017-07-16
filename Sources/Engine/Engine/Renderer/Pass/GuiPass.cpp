/******************************************************************************
**	Includes
******************************************************************************/
#include "GuiPass.hpp"

#include <Engine/Renderer/RenderFilter.hpp>
#include <Engine/Renderer/ResourceShared.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/RenderContext.hpp>
#include <Renderer/Resource/RenderTarget.hpp>
#include <Renderer/RenderBuffer.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	GuiPass::GuiPass()
		: m_pRenderTarget(nullptr)
	{
		memset(m_aFilter, 0, sizeof(m_aFilter));
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	GuiPass::~GuiPass()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2015-11-11
	void GuiPass::Initialize(Renderer::RenderContext* _pContext)
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
	void GuiPass::Release(Renderer::RenderContext* _pContext)
	{
		m_aFilter[EFilter::ABGR2RGBA]->Release(_pContext);
		SAFE_DELETE(m_aFilter[EFilter::ABGR2RGBA]);

		_pContext->GetRenderer()->DestroyResource(m_pRenderTarget);
	}

	//!	@brief		Execute 
	//!	@date		2016-06-25
	void GuiPass::Execute(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pFrameBuffer)
	{
		// Texture to Texture
		RenderBuffer::Operation::TextureCopy* pOperation; uint32 uiCount;
		if(_pFrameBuffer->Get(&pOperation, uiCount))
		{
			for(uint32 uiOperation = 0; uiOperation < uiCount; ++uiOperation)
			{
				RenderBuffer::Operation::TextureCopy& kOperation = pOperation[uiOperation];
				switch(kOperation.Type)
				{
					case RenderBuffer::Operation::Texture2Texture:
					{
						Renderer::Texture2D* pSource = static_cast<Renderer::Texture2D*>(kOperation.Source);
						Renderer::Texture2D* pDestination = static_cast<Renderer::Texture2D*>(kOperation.Destination);
						
						// Prepare Render Target
						m_pRenderTarget->Clear();
						m_pRenderTarget->AddTarget(pDestination);

						// Execute Filter
						RenderFilter* pFilter = m_aFilter[EFilter::Identity];
						pFilter->SetRenderTarget(m_pRenderTarget);
						pFilter->SetResource(0, pSource);	
						_pRenderer->SetViewport(_pContext, (uint32)kOperation.Box.GetX(), (uint32)kOperation.Box.GetY(), pSource->GetWidth(), pSource->GetHeight());
						_pRenderer->SetBlendState(_pContext, Renderer::EBlend::SourceAlpha, Renderer::EBlendOperation::Add, Renderer::EBlend::SourceAlphaInversed);
						pFilter->Execute3(_pContext);

						break;
					}

					case RenderBuffer::Operation::Buffer2Texture:
					{
						Renderer::Texture2D* pDestination = static_cast<Renderer::Texture2D*>(kOperation.Destination);
						_pRenderer->CopyRegion(_pContext, pDestination, (uint32)kOperation.Box.GetX(), (uint32)kOperation.Box.GetY(), pDestination->GetWidth(), pDestination->GetHeight(), kOperation.Source);
						break;
					}

					case RenderBuffer::Operation::ABGR2RGBA:
					{
						Renderer::Texture2D* pSource = static_cast<Renderer::Texture2D*>(kOperation.Source);
						Renderer::Texture2D* pDestination = static_cast<Renderer::Texture2D*>(kOperation.Destination);

						// Prepare Render Target
						m_pRenderTarget->Clear();
						m_pRenderTarget->AddTarget(pDestination);

						// Execute Filter
						RenderFilter* pFilter = m_aFilter[EFilter::ABGR2RGBA];
						pFilter->SetRenderTarget(m_pRenderTarget);
						pFilter->SetResource(0, pSource);	
						pFilter->Execute(_pContext);
						break;
					}
				}	
			}
		}		
	}
}}