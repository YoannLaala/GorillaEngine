#ifndef _ENGINE_PRIMITIVE_PASS_HPP_
#define _ENGINE_PRIMITIVE_PASS_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/RenderPass.hpp>
#include <Engine/Asset/AssetHandle.hpp>
#include <Engine/Asset/Effect/Shader.hpp>
#include <Engine/Common.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Renderer;
	class RenderContext;
	class RenderBuffer;
	class RenderState;
	class RenderTarget;
	class Geometry;
	class Texture2D;
	class Buffer;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class PrimitivePass : public Renderer::RenderPass
	{
	public:
		PrimitivePass();
		~PrimitivePass();
		DECLARE_RENDER_PASS(PrimitivePass)

		virtual void Initialize		(Renderer::RenderContext* _pRenderContext) override;
		virtual void Release		(Renderer::RenderContext* _pRenderContext) override;

		virtual void Execute		(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pBuffer) override;

	private:
		AssetHandle<Shader>			m_hPixelShader;
		Renderer::RenderTarget*		m_pRenderTarget;
		Renderer::RenderState*		m_pRenderState;
		Renderer::Texture2D*		m_pDepthStencilTexture;
		Renderer::Buffer*			m_pSceneBuffer;
	};
}}

#endif