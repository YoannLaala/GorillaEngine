/******************************************************************************
**	Includes
******************************************************************************/
#include "CanvasImage.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	CanvasImage::CanvasImage()
		: m_uiWidth(0)
		, m_uiHeight(0)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	CanvasImage::~CanvasImage()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2015-11-11
	void CanvasImage::Initialize(uint32 _uiWidth, uint32 _uiHeight)
	{
		m_uiWidth = _uiWidth;
		m_uiHeight = _uiHeight;
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void CanvasImage::Release()
	{
		//Renderer::DestroyResource(m_pHandle);
	}
}}