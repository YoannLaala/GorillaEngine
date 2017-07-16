#ifndef _RENDERER_CANVAS_IMAGE_HPP_
#define _RENDERER_CANVAS_IMAGE_HPP_

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
	class CanvasImage : public CanvasResource
	{
	public:
		CanvasImage();
		~CanvasImage();
		
		static EResource::Type GetType() { return EResource::Texture2D; }

		void Initialize	(uint32 _uiWidth, uint32 _uiHeight);
		virtual void Release() override;
		
		inline uint32			GetWidth() const { return m_uiWidth; }
		inline uint32			GetHeight() const { return m_uiHeight; }

		inline uint32			GetFrameCount() const { return m_vFrame.GetSize(); }
		inline void*			GetFrame(uint32 _uiIndex) { return m_vFrame[_uiIndex]; }
		inline void*			AddFrame(void* _pHandle) { return m_vFrame.Add(_pHandle); }

	private:
		Vector<void*>	m_vFrame;
		uint32			m_uiWidth;
		uint32			m_uiHeight;
	};
}}

#endif