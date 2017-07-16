/******************************************************************************
**	Includes
******************************************************************************/
#include "EditorPass.hpp"

#include <Engine/Renderer/RenderFilter.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/RenderContext.hpp>
#include <Renderer/Resource/RenderTarget.hpp>
#include <Renderer/Viewport.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Editor
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	EditorPass::EditorPass()
		: m_pRenderTarget(NULL)
		, m_pFilter(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	EditorPass::~EditorPass()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2015-11-11
	void EditorPass::Initialize(Renderer::RenderContext* _pContext)
	{
		m_pFilter = new Engine::RenderFilter();
		m_pFilter->Initialize(_pContext, "@Effect/Filter/Operation/Identity.ps");
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void EditorPass::Release(Renderer::RenderContext* _pContext)
	{
		m_pFilter->Release(_pContext);
		SAFE_DELETE(m_pFilter);
	}

	//!	@brief		Execute 
	//!	@date		2016-06-25
	void EditorPass::Execute(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* /*_pBuffer*/)
	{
		m_pFilter->SetRenderTarget(m_pRenderTarget);

		// Copy all Viewport properly
		const uint32 uiViewportCount = m_vViewport.GetSize();
		for(uint32 uiViewport = 0; uiViewport < uiViewportCount; ++uiViewport)
		{
			Renderer::Viewport* pViewport = m_vViewport[uiViewport];

			_pRenderer->SetViewport(_pContext, pViewport->GetLeft(), pViewport->GetTop(), pViewport->GetWidth(), pViewport->GetHeight());
			m_pFilter->SetResource(0, pViewport->GetRenderTarget()->GetTarget(0));
			m_pFilter->Execute3(_pContext);
		}

		/*_pRenderer->SetBlendState(_pContext, Renderer::EBlend::SourceAlpha, Renderer::EBlendOperation::Add, Renderer::EBlend::SourceAlphaInversed);
		m_pFilter->SetResource(0, m_pTexture);
		m_pFilter->Execute2(_pContext);*/
	}
}}