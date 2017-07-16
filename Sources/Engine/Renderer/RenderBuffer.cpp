/******************************************************************************
**	Includes
******************************************************************************/
#include "RenderBuffer.hpp"
#include <Renderer/Renderer.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	RenderBuffer::RenderBuffer()
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	RenderBuffer::~RenderBuffer()
	{
		// Nothing to do
	}

	//!	@brief		Clear 
	//!	@date		2015-11-11
	void RenderBuffer::Clear()
	{		
		HashMap<uint32, Section*>::Iterator it = m_mSection.GetFirst();
		HashMap<uint32, Section*>::Iterator itEnd = m_mSection.GetLast();
		while(it != itEnd)
		{
			Section* pSection = *it;
			pSection->Buffer.Clear();
			pSection->Size = 0;
			
			++it;
		}
	}
}}