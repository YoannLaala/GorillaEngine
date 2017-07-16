/******************************************************************************
**	Includes
******************************************************************************/
#include "Color.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor
	//!	@date		2015-12-30
	Color::Color() 
		: m_uiValue(255) 
	{ 
		// Nothing to do
	}

	//!	@brief		Constructor 
	//!	@date		2015-12-30
	Color::Color(uint8 _uiRed, uint8 _uiGreen, uint8 _uiBlue, uint8 _uiAlpha /*= 255*/) 
	{ 
		m_kValue.Red = _uiRed;
		m_kValue.Green = _uiGreen;
		m_kValue.Blue = _uiBlue;
		m_kValue.Alpha = _uiAlpha;
	}

	//!	@brief		Constructor 
	//!	@date		2015-12-30
	Color::Color(uint32 _uiColor)
		: m_uiValue(_uiColor)
	{ 
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-12-30
	Color::~Color() 
	{ 
		// Nothing to do
	}

	//!	@brief		Basic Color
	//!	@date		2015-12-30
	Color Color::Black(0,0,0);
	Color Color::White(255,255,255);
	Color Color::Red(255,0,0);
	Color Color::Green(0,255,0);
	Color Color::Blue(0,0,255);
}}