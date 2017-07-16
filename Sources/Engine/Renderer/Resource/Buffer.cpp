/******************************************************************************
**	Includes
******************************************************************************/
#include "Buffer.hpp"
#include <Renderer/Renderer.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	Buffer::Buffer()
		: m_uiElementSize(0)
		, m_uiElementCount(0)
		, m_pHandle(NULL)
	{
		memset(m_aView, 0, sizeof(void*) * EView::Count);
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	Buffer::~Buffer()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2015-11-11
	void Buffer::Initialize(uint32 _uiElementSize, uint32 _uiElementCount, void* _pHandle)
	{
		m_uiElementSize = _uiElementSize;
		m_uiElementCount = _uiElementCount;
		m_pHandle = _pHandle;
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void Buffer::Release()
	{
		Renderer::DestroyHandle(m_pHandle);
	}
}}