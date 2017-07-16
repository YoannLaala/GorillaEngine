/******************************************************************************
**	Includes
******************************************************************************/
#include "CanvasBrush.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor
	//!	@date		2015-11-07
	CanvasResource::CanvasResource()
		: m_uiId((uint32)-1)
		, m_uiReferenceCount(1)
	{
		// Nothing to do
	}
		
	//!	@brief		Destructor
	//!	@date		2015-11-07
	CanvasResource::~CanvasResource()
	{
		// Nothing to do
	}
		
	//!	@brief		Release
	//!	@date		2015-11-07
	void CanvasResource::Release()
	{
			
	}
}}