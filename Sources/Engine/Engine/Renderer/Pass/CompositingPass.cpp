/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Renderer/Pass/CompositingPass.hpp>

#include <Renderer/Renderer.hpp>
#include <Renderer/RenderContext.hpp>
#include <Renderer/Viewport.hpp>
#include <Renderer/Resource/RenderTarget.hpp>
#include <Engine/Renderer/RenderFilter.hpp>
#include <Engine/Renderer/ResourceShared.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	CompositingPass::CompositingPass()
		: m_pFilter(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	CompositingPass::~CompositingPass()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2015-11-11
	void CompositingPass::Initialize(Renderer::RenderContext* _pContext)
	{
		Renderer::RenderTarget* pRenderTarget = _pContext->GetViewport()->GetRenderTarget();
		Renderer::Texture2D* pDirectLighting = _pContext->GetTexture2D(RENDERER_TEXTURE_LIGHT_DIRECT);
		Renderer::Texture2D* pIndirectLighting = _pContext->GetTexture2D(RENDERER_TEXTURE_LIGHT_INDIRECT);
		Renderer::Texture2D* pAmbientOcclusion = _pContext->GetTexture2D(RENDERER_TEXTURE_LIGHT_AMBIENT_OCCLUSION);

		// Initialize filter
		m_pFilter = new RenderFilter();
		m_pFilter->Initialize(_pContext, "@Effect/Filter/Display/Compositing.ps");
		m_pFilter->SetRenderTarget(pRenderTarget);
		m_pFilter->SetResource(0, pDirectLighting);
		m_pFilter->SetResource(1, pIndirectLighting);
		m_pFilter->SetResource(2, pAmbientOcclusion);
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void CompositingPass::Release(Renderer::RenderContext* /*_pContext*/)
	{
		SAFE_DELETE(m_pFilter);
	}

	//!	@brief		Execute 
	//!	@date		2016-06-25
	void CompositingPass::Execute(Renderer::Renderer* /*_pRenderer*/, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* /*_pBuffer*/)
	{
		m_pFilter->Execute(_pContext);
	}
}}