#ifndef _RENDERER_TEXTURE_2D_HPP_
#define _RENDERER_TEXTURE_2D_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/Resource/Resource.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Renderer;
	class RenderContext;
	class SwapChain;
	class RenderTarget;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Texture2D : public Resource
	{
		friend class Renderer;
		friend class RenderContext;
		friend class SwapChain;
		friend class RenderTarget;

		struct EView
		{
			enum Type : uint8
			{
				ShaderResource = 0,
				UnorderedAccess,
				RenderTarget,
				DepthStencil,

				Count,
			};
		};

	public:
		DECLARE_RESOURCE(EResource::Texture2D);

	private:
		Texture2D();
		~Texture2D();

		void Initialize	(uint32 _uiWidth, uint32 _uiHeight, void* _pHandle);
		virtual void Release () override;

		inline void*			GetHandle	() { return m_pHandle; }

		inline void*			GetView	(EView::Type _eView) { return m_aView[_eView]; }
		inline void				SetView	(EView::Type _eView, void* _pView) { m_aView[_eView] = _pView; }
	
	public:
		inline uint32			GetWidth	() const { return m_uiWidth; }
		inline uint32			GetHeight	() const { return m_uiHeight; }
		
	private:
		uint32		m_uiWidth;
		uint32		m_uiHeight;
		void*		m_pHandle;
		void*		m_aView[EView::Count];
	};
}}

#endif