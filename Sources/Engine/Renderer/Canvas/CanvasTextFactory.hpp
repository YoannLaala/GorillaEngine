#ifndef _RENDERER_CANVAS_TEXT_FACTORY_HPP_
#define _RENDERER_CANVAS_TEXT_FACTORY_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "../../Common.hpp"

/******************************************************************************
**	Define
******************************************************************************/
#undef CreateFont 

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class CanvasFont;
	class CanvasText;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class CanvasTextFactory
	{
	public:
		CanvasTextFactory();
		~CanvasTextFactory();

		bool										Initialize						();
		void										Release							();

		bool										HasFont							(const wchar_t* _szFamilyName) const;
		CanvasFont*									CreateFont						(const wchar_t* _szFamilyName, float32 fSize, uint32 uiWeight, EFontStyle::Type _eFont);
		CanvasText*									CreateText						(const wchar_t* _szText, float32 _fWidthMax, float32 _fHeightMax, CanvasFont* _pFont);

	private:
		void*					m_pFactory;
		Vector<uint32>			m_vFamilyId;
	};
}}

#endif