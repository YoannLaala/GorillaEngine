/******************************************************************************
**	Includes
******************************************************************************/
#include "CanvasCommand.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2016-03-26
	CanvasCommand::CanvasCommand()
		: m_uiId(0)
		, m_pSurface(NULL)
		, m_uiOffset(0)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2016-03-26
	CanvasCommand::~CanvasCommand()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2016-03-26
	void CanvasCommand::Initialize(ECanvasCommand::Type _eType, CanvasSurface* _pSurface, uint8 _uiDepth, uint32 _uiOffset)
	{
		m_eType = _eType;
		m_pSurface = _pSurface;
		m_kKey.SurfaceId = *(uint16*)_pSurface;
		m_kKey.Depth = _uiDepth;
		m_kKey.Priority = _eType;
		m_uiOffset = _uiOffset;
	}

	//!	@brief		Execute 
	//!	@date		2016-03-26
	void CanvasCommand::Execute(CanvasRenderer* _pRenderer, uint8* _pParameter)
	{
		ECanvasCommand::Execute[m_eType](_pRenderer, m_pSurface, &_pParameter[m_uiOffset]);
	}
}}