#ifndef _ENGINE_GEOMETRY_PASS_HPP_
#define _ENGINE_GEOMETRY_PASS_HPP_

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
namespace Gorilla { namespace Engine { namespace RenderBuffer { namespace Geometry
{
	struct Batch;
}}}}

namespace Gorilla { namespace Renderer
{
	class Renderer;
	class RenderContext;
	class RenderTarget;
	class Texture2D;
	class Buffer;
	class Geometry;
	class Material;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class GeometryPass : public Renderer::RenderPass
	{
	public:
		/******************************************************************************
		**	Command Declaration
		******************************************************************************/
		/*struct Command
		{
			Renderer::Material* Material;
			Renderer::Geometry* Geometry;
			Math::Matrix44		Transform;
			uint8				Section;
		};*/

		/*class Command : public Renderer::RenderCommand
		{
			struct Key
			{
				uint32 MaterialId : 16;
				uint32 GeometryId : 16;
			};

		public:
			void Set(Renderer::Material* _pMaterial, Renderer::Geometry* _pGeometry, uint8 _uiSection, const Math::Matrix44& _mTransform)
			{
				m_pMaterial = _pMaterial;
				m_pGeometry = _pGeometry;
				m_mTransform = _mTransform;
				m_uiSection = _uiSection;

				// Generate the key associated
				Key& kKey = GetId<Key>();
				kKey.MaterialId = (uint16)_pMaterial;
				kKey.GeometryId = (uint16)_pGeometry;
			}

			inline Renderer::Material* GetMaterial() { return m_pMaterial; }
			inline Renderer::Geometry* GetGeometry() { return m_pGeometry; }

		private:
			Renderer::Material* m_pMaterial;
			Renderer::Geometry* m_pGeometry;
			Math::Matrix44 m_mTransform;
			uint8 m_uiSection;
		};*/

	public:
		GeometryPass();
		~GeometryPass();
		DECLARE_RENDER_PASS(GeometryPass);

		virtual void Initialize		(Renderer::RenderContext* _pRenderContext) override;
		virtual void Release		(Renderer::RenderContext* _pRenderContext) override;

		virtual void Execute(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pBuffer) override;

	public:
		static void DrawAllGeometry(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, RenderBuffer::Geometry::Batch* _pBatchArray, uint32 _uiBatchCount, Renderer::Buffer** _ppInstanceBufferOut, Renderer::Shader* _pPixelShader = NULL);
		static void DrawOneGeometry(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, RenderBuffer::Geometry::Batch* _pBatchArray, uint32 _uiInstanceCount, Renderer::Buffer** _ppInstanceBufferOut, Renderer::Shader* _pPixelShader);

	private:
		AssetHandle<Effect>			m_hEffect;
		Renderer::RenderTarget*		m_pRenderTarget;
		Renderer::Buffer*			m_pSceneBuffer;
		Renderer::Buffer*			m_pInstanceBuffer;
	};
}}

#endif