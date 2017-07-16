#ifndef _ENGINE_SHADOW_PASS_HPP_
#define _ENGINE_SHADOW_PASS_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/RenderPass.hpp>
#include <Engine/Asset/AssetHandle.hpp>
#include <Engine/Asset/Effect/Effect.hpp>
#include <Engine/Common.hpp>

#include <Engine/Renderer/ResourceShared.hpp>

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
	class ShadowPass : public Renderer::RenderPass
	{
	public:
		ShadowPass();
		~ShadowPass();
		DECLARE_RENDER_PASS(ShadowPass);

		virtual void Initialize		(Renderer::RenderContext* _pRenderContext) override;
		virtual void Release		(Renderer::RenderContext* _pRenderContext) override;

		virtual void Execute(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pBuffer) override;

	private:
		void ExecuteDirectionalLight(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pBuffer);
		void ExecutePointLight(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pBuffer);

	private:
		AssetHandle<Effect>			m_aEffect[SharedResource::Light::Count];
		Renderer::RenderTarget*		m_aRenderTarget[SharedResource::Light::Count];
		Renderer::Buffer*			m_pSceneBuffer;
		Renderer::Buffer*			m_pInstanceBuffer;
	};
}}

#endif