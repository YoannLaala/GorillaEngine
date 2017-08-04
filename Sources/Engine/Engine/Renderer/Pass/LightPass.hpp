#ifndef _ENGINE_LIGHT_PASS_HPP_
#define _ENGINE_LIGHT_PASS_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/RenderPass.hpp>
#include <Engine/Asset/AssetHandle.hpp>
#include <Engine/Asset/Texture/Texture.hpp>
#include <Engine/Asset/Effect/Effect.hpp>
#include <Engine/Asset/Mesh/Mesh.hpp>
#include <Engine/Common.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class RenderFilter;
}}

namespace Gorilla { namespace Renderer
{
	class Renderer;
	class RenderTarget;
	class RenderBuffer;
	class Geometry;
	class Buffer;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class LightPass : public Renderer::RenderPass
	{
	public:
		LightPass();
		~LightPass();
		DECLARE_RENDER_PASS(LightPass);

		virtual void Initialize		(Renderer::RenderContext* _pRenderContext) override;
		virtual void Release		(Renderer::RenderContext* _pRenderContext) override;

		virtual void Execute		(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pBuffer) override;

	public:

		struct DirectionalBuffer
		{
			DECLARE_RENDER_BUFFER(LightPass::DirectionalBuffer)

			Math::Matrix44	ViewProjection;
			Math::Vector3	Direction;
			Math::Vector3	Color;
			float32			Power;
		};
		
		struct SpotBuffer
		{
			DECLARE_RENDER_BUFFER(LightPass::SpotBuffer)

			Math::Matrix44	ViewProjection;
			Math::Vector3	Position;
			Math::Vector3	Direction;
			Math::Vector3	Color;
			float32			Size;
			float32			Power;
		};

		struct PointBuffer
		{
			DECLARE_RENDER_BUFFER(LightPass::PointBuffer)

			Math::Vector3	Position;
			Math::Vector3	Color;
			float32			Radius;
			float32			Power;
		};

	private:
		AssetHandle<Texture>	m_hNoiseTextute;
		Renderer::RenderTarget* m_pRenderTarget;
		RenderFilter*			m_pLightFilter;
		Renderer::RenderTarget* m_pGeometryRenderTarget;
	};
}}

#endif