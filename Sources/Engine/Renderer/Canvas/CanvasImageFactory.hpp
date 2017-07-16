#ifndef _RENDERER_CANVAS_IMAGE_FACTORY_HPP_
#define _RENDERER_CANVAS_IMAGE_FACTORY_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/Common.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class CanvasSurface;
	class CanvasImage;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class CanvasImageFactory
	{
	public:
		CanvasImageFactory();
		~CanvasImageFactory();
		bool										Initialize						();
		void										Release							();

		CanvasImage*								CreateImage						(CanvasSurface* _pSurface, const char* _szFilePath);

	private:
		void*					m_pFactory;
	};
}}

#endif