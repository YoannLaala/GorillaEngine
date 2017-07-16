/******************************************************************************
**	Includes
******************************************************************************/
#include "RenderState.hpp"
#include <Renderer/Renderer.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	RenderState::RenderState()
		: m_pRasterizerState(NULL)
		, m_pDepthStencilState(NULL)
		, m_pBlendState(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	RenderState::~RenderState()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2015-11-11
	void RenderState::Initialize(void* _pRasterizerState, void* _pDepthStencilState, void* _pBlendState)
	{
		m_pRasterizerState = _pRasterizerState;
		m_pDepthStencilState = _pDepthStencilState;
		m_pBlendState = _pBlendState;
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void RenderState::Release()
	{
		Renderer::DestroyHandle(m_pRasterizerState);
		Renderer::DestroyHandle(m_pDepthStencilState);
		Renderer::DestroyHandle(m_pBlendState);
	}
}}