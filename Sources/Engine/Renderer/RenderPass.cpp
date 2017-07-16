/******************************************************************************
**	Includes
******************************************************************************/
#include "RenderPass.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor
	//!	@date		2015-11-07
	RenderPass::RenderPass()
		: m_eFlag(EFlag::Activated)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-11-07
	RenderPass::~RenderPass()
	{
		// Nothing to do
	}
}}