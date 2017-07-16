#ifndef _TEXTURE_HELPER_HPP_
#define _TEXTURE_HELPER_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "../Common.hpp"

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class TextureHelper
	{
	public:
		static void			GetSurfaceInfo	(uint32 _uiWidth, uint32 _uiHeight, EFormat::Type _eFormat, uint32& _uiRowByteOut, uint32& _uiSurfaceByteOut);
		static uint32		GetBitPerPixel	(EFormat::Type _eFormat);
	};
}}

#endif