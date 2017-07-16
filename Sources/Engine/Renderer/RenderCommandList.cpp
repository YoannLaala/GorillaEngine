/******************************************************************************
**	Includes
******************************************************************************/
#include "RenderCommandList.hpp"
#include <Renderer/Renderer.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	RenderCommandList::RenderCommandList()
		: m_pHandle(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	RenderCommandList::~RenderCommandList()
	{
		// Nothing to do
	}

	//!	@brief		Initiliaze 
	//!	@date		2015-11-11
	void RenderCommandList::Initiliaze(void* _pHandle)
	{
		m_pHandle = _pHandle;
	}
	
	//!	@brief		Release 
	//!	@date		2015-11-11
	void RenderCommandList::Release()
	{
		if(m_pHandle)
		{
			Renderer::DestroyHandle(m_pHandle);
		}
	}
}}