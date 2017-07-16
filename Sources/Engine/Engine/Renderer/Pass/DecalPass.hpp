#ifndef _ENGINE_DECAL_PASS_HPP_
#define _ENGINE_DECAL_PASS_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/RenderPass.hpp>
#include <Engine/Asset/AssetHandle.hpp>
#include <Engine/Asset/Effect/Effect.hpp>
#include <Engine/Common.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Renderer;
	class RenderContext;
	class RenderTarget;
	class RenderBuffer;
	class Buffer;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class DecalPass : public Renderer::RenderPass
	{
	public:
		DecalPass();
		~DecalPass();
		DECLARE_RENDER_PASS(DecalPass);

		virtual void Initialize		(Renderer::RenderContext* _pRenderContext) override;
		virtual void Release		(Renderer::RenderContext* _pRenderContext) override;

		virtual void Execute(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pBuffer) override;

	private:
		AssetHandle<Effect>			m_hEffect;
		Renderer::RenderTarget*		m_pRenderTarget;
		Renderer::Buffer*			m_pSceneBuffer;
		Renderer::Buffer*			m_pInstanceBuffer;
	};
}}

#endif