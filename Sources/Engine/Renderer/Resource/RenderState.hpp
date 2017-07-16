#ifndef _RENDERER_RASTERIZER_STATE_HPP_
#define _RENDERER_RASTERIZER_STATE_HPP_

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
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	struct BlendState
	{
		BlendState(EBlend::Type _eSource, EBlendOperation::Type _eOperation, EBlend::Type _eDestination)
			: BlendSource(_eSource)
			, BlendOperation(_eOperation)
			, BlendDestination(_eDestination)
		{
			// Nothing to do
		}

		inline operator uint16() const { return *(uint16*)this; }

		uint16 BlendSource:5;
		uint16 BlendOperation:3;
		uint16 BlendDestination:5;
		uint16 Extra:3;
	};

	class RenderState : public Resource
	{
		friend class Renderer;
		friend class RenderContext;
	
	public:
		struct Key
		{
			Key()
				: WireFrame(0)
				, Cull(1)
				, Clockwise(1)
				, Scissor(0)
				, MultiSample(0)
				, Depth(1)
				, DepthWrite(1)
				, DepthComparaison(EDepthComparaison::LessEqual)
				, Stencil(0)
				, Blend(1)
				, BlendSource(EBlend::One)
				, BlendOperation(EBlendOperation::Add)
				, BlendDestination(EBlend::Zero)
			{
				// Nothing to do
			}

			// Rasterizer
			uint32 WireFrame:1;
			uint32 Cull:1;
			uint32 Clockwise:1;
			uint32 Scissor:1;
			uint32 MultiSample:1;

			// DepthStencil
			uint32 Depth:1;
			uint32 DepthWrite:1;
			uint32 DepthComparaison:4;
			uint32 Stencil:1;

			// Blend
			uint32 Blend:1;
			uint32 BlendSource:5;
			uint32 BlendOperation:9;//3
			uint32 BlendDestination:5;
		};

	private:
		RenderState();
		~RenderState();
		DECLARE_RESOURCE(EResource::RenderState);

		void Initialize	(void* _pRasterizerState, void* _pDepthStencilState, void* _pBlendState);
		void Release	();
		
		inline void*	GetRasterizerState		() const { return m_pRasterizerState; }
		inline void*	GetDepthStencilState	() const { return m_pDepthStencilState; }
		inline void*	GetBlendState			() const { return m_pBlendState; }

	private:
		void*		m_pRasterizerState;
		void*		m_pDepthStencilState;
		void*		m_pBlendState;
	};
}}

#endif