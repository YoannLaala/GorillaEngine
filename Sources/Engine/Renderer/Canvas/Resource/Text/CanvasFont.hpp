#ifndef _RENDERER_CANVAS_FONT_HPP_
#define _RENDERER_CANVAS_FONT_HPP_

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
	class CanvasFont : public CanvasResource
	{
	public:
		CanvasFont();
		~CanvasFont();

		static EResource::Type GetType() { return EResource::Buffer; }

		void Initialize(const wchar_t* _szFamily, float32 _fSize, uint32 _uiWeight, EFontStyle::Type _eStyle, void* _pHandle);
		void Release();
		
		inline void*			GetHandle() { return m_pHandle; }
		inline const String&	GetFamily() { return m_sFamily; }
		inline float32			GetSize() { return m_fSize; }
		inline uint32			GetWeight() { return m_uiWeight; }
		inline EFontStyle::Type	GetStyle() { return m_eStyle; }

	private:
		void* m_pHandle;
		String m_sFamily;
		float32 m_fSize;
		uint32 m_uiWeight;
		EFontStyle::Type m_eStyle;
	};
}}

#endif