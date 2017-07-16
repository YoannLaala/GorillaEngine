#ifndef _RENDERER_CANVAS_SURFACE_HPP_
#define _RENDERER_CANVAS_SURFACE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/Common.hpp>
#include <Renderer/Canvas/Resource/CanvasResource.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class CanvasSurface : public CanvasResource
	{
	public:
		CanvasSurface();
		~CanvasSurface();
		
		static EResource::Type GetType() { return EResource::Texture2D; }

		void Initialize	(uint32 _uiWidth, uint32 _uiHeight, void* _pHandle);
		void Release	();
		
		inline uint32			GetWidth	() const { return m_uiWidth; }
		inline uint32			GetHeight	() const { return m_uiHeight; }
		inline void*			GetHandle	() { return m_pHandle; }

	private:
		uint32		m_uiWidth;
		uint32		m_uiHeight;
		void*		m_pHandle;
	};
}}

#endif