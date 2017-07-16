#ifndef _RENDERER_VIEWPORT_HPP_
#define _RENDERER_VIEWPORT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Renderer;
	class RenderContext;
	class RenderTarget;
	class SwapChain;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Viewport
	{
		friend class Renderer;
	public:
		struct EFilter
		{
			enum Type
			{
				None = 0,
				Gui
			};
		};
		SIGNAL_SENDER(SizeChanged, uint32 /*_uiWidth*/, uint32 /*_uiHeight*/);

	private:
		Viewport	();
		~Viewport	();

		void Initialize		(RenderTarget* _pRenderTarget, SwapChain* _pSwapChain);
		void Release		();

		inline void				SetRenderContext(RenderContext* _pContext) { m_pContext = _pContext; }

	public:
		void					Resize				(uint32 _uiWidth, uint32 _uiHeight);

		inline uint32			GetWidth			() const { return m_uiWidth; }
		inline uint32			GetHeight			() const { return m_uiHeight; }
		inline uint32			GetLeft				() const { return m_uiLeft; }
		inline uint32			GetTop				() const { return m_uiTop; }
		inline void				SetPosition			(uint32 _uiLeft, uint32 _uiTop) { m_uiLeft = _uiLeft; m_uiTop = _uiTop;}

		inline SwapChain*		GetSwapChain		() { return m_pSwapChain; }
		inline RenderTarget*	GetRenderTarget		() { return m_pRenderTarget; }
		inline RenderContext*	GetRenderContext	() { return m_pContext; }

	private:
		RenderTarget*	m_pRenderTarget;
		SwapChain*		m_pSwapChain;
		RenderContext*	m_pContext;
		uint32			m_uiLeft;
		uint32			m_uiTop;
		uint32			m_uiWidth;
		uint32			m_uiHeight;
	};
}}

#endif