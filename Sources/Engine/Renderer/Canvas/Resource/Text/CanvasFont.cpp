/******************************************************************************
**	Includes
******************************************************************************/
#include "CanvasFont.hpp"
#include <Core/String/StringHelper.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor
	//!	@date		2015-11-07
	CanvasFont::CanvasFont()
		: m_pHandle(NULL)
		, m_fSize(0.0f)
		, m_uiWeight(0)
		, m_eStyle(EFontStyle::Normal)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-11-07
	CanvasFont::~CanvasFont()
	{
		// Nothing to do
	}

	//!	@brief		SetPosition
	//!	@date		2015-04-04
	void CanvasFont::Initialize(const wchar_t* _szFamily, float32 _fSize, uint32 _uiWeight, EFontStyle::Type _eStyle, void* _pHandle)
	{
		m_pHandle = _pHandle;
		StringHelper::ToString(_szFamily, m_sFamily);
		m_fSize = _fSize;
		m_uiWeight = _uiWeight;
		m_eStyle = _eStyle;
	}

	//!	@brief		Translate
	//!	@date		2015-04-04
	void CanvasFont::Release()
	{
		//m_vPosition += _vTranslation;
	}
}}