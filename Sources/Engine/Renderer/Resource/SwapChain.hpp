#ifndef _RENDERER_SWAP_CHAIN_HPP_
#define _RENDERER_SWAP_CHAIN_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/Resource/Resource.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla 
{
	class Window;
}

namespace Gorilla { namespace Renderer
{
	class Renderer;
	class Texture2D;
	class Viewport;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class SwapChain : public Resource
	{
		friend class Renderer;
		friend class Viewport;

	public:
		DECLARE_RESOURCE(EResource::Swapchain);

	private:
		SwapChain();

	public:

		~SwapChain();

	private:
		void Initialize(Texture2D* _pTexture);
		virtual void Release() override;

		inline Window*			GetWindow	() { return m_pWindow; }
		inline void				SetWindow	(Window* _pWindow) { m_pWindow = _pWindow; }
		inline void*			GetHandle	() { return m_pHandle; }
		inline void				SetHandle	(void* _pHandle) { m_pHandle = _pHandle; }
		inline Texture2D*		GetTexture	() { return m_pTexture; }

	private:
		Window*		m_pWindow;
		void*		m_pHandle;
		Texture2D*	m_pTexture;
	};
}}

#endif