/******************************************************************************
**	Includes
******************************************************************************/
#include "CanvasSurface.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	CanvasSurface::CanvasSurface()
		: m_uiWidth(0)
		, m_uiHeight(0)
		, m_pHandle(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	CanvasSurface::~CanvasSurface()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2015-11-11
	void CanvasSurface::Initialize(uint32 _uiWidth, uint32 _uiHeight, void* _pHandle)
	{
		m_uiWidth = _uiWidth;
		m_uiHeight = _uiHeight;
		m_pHandle = _pHandle;
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void CanvasSurface::Release()
	{
		//Renderer::DestroyResource(m_pHandle);
	}
}}