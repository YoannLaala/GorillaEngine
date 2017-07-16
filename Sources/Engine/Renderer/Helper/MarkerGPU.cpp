/******************************************************************************
**	Includes
******************************************************************************/
#include "MarkerGPU.hpp"
#include <Renderer/Renderer.hpp>
#include <Renderer/RenderContext.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor
	//!	@date		2015-12-30
	MarkerGPU::MarkerGPU(RenderContext* _pContext, const char* _szName)
		: m_pContext(_pContext)
	{
		m_pHandle = m_pContext->GetRenderer()->PushMarker(_pContext, _szName);
	}

	//!	@brief		Destructor
	//!	@date		2015-12-30
	MarkerGPU::~MarkerGPU()
	{
		m_pContext->GetRenderer()->PopMarker(m_pContext, m_pHandle);
	}
}}