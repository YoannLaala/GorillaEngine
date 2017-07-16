/******************************************************************************
**	Includes
******************************************************************************/
#include "CanvasText.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor
	//!	@date		2015-11-07
	CanvasText::CanvasText()
		: m_pHandle(NULL)
		, m_fWidth(0.0f)
		, m_fHeight(0.0f)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-11-07
	CanvasText::~CanvasText()
	{
		// Nothing to do
	}

	//!	@brief		SetPosition
	//!	@date		2015-04-04
	void CanvasText::Initialize(float32 _fWidth, float32 _fHeight, void* _pHandle)
	{
		m_fWidth = _fWidth;
		m_fHeight = _fHeight;
		m_pHandle = _pHandle;
	}

	//!	@brief		Translate
	//!	@date		2015-04-04
	void CanvasText::Release()
	{
	
	}
}}