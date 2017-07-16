#ifndef _RENDERER_CANVAS_TEXT_HPP_
#define _RENDERER_CANVAS_TEXT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/Common.hpp>
#include <Renderer/Canvas/Resource/CanvasResource.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class CanvasFont;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class CanvasText : public CanvasResource
	{
	public:
		CanvasText();
		~CanvasText();

		static EResource::Type GetType() { return EResource::Buffer; }

		void Initialize(float32 _fWidth, float32 _fHeight, void* _pHandle);
		void Release();
		
		inline float32	GetWidth	() const { return m_fWidth; }
		inline float32	GetHeight	() const { return m_fHeight; }
		inline void*	GetHandle	() const { return m_pHandle; }

	private:
		void*		m_pHandle;
		float32		m_fWidth;
		float32		m_fHeight;
	};
}}

#endif