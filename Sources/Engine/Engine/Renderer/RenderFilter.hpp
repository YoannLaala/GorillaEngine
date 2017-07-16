#ifndef _ENGINE_RENDER_FILTER_HPP_
#define _ENGINE_RENDER_FILTER_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/RenderPass.hpp>
#include <Engine/Asset/AssetHandle.hpp>
#include <Engine/Asset/Effect/Effect.hpp>
#include <Engine/Asset/Mesh/Mesh.hpp>
#include <Engine/Common.hpp>
#include <Renderer/Resource/Buffer.hpp>
#include <Renderer/Resource/Texture2D.hpp>
#include <Renderer/Resource/Texture3D.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Renderer;
	class RenderContext;
	class RenderTarget;
	class Resource;
	class Geometry;
	class Buffer;
	class Texture2D;
	class Texture3D;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class RenderFilter
	{
		struct EShaderResource
		{
			enum Type : uint8
			{
				ConstantBuffer = 0,
				Resource,

				Count
			};
		};

	public:
		RenderFilter();
		~RenderFilter();
		
		void Initialize(Renderer::RenderContext* _pContext, const char* _szFilter, const char* _szParam = NULL);
		void Release(Renderer::RenderContext* _pContext);

		inline Renderer::RenderTarget* GetRenderTarget() { return m_pRenderTarget; }
		inline void SetRenderTarget(Renderer::RenderTarget* _pRenderTarget) { m_pRenderTarget = _pRenderTarget; }

		inline Renderer::Buffer* GetConstantBuffer(uint32 _uiIndex) { return reinterpret_cast<Renderer::Buffer*>(m_aShaderResource[EShaderResource::ConstantBuffer][_uiIndex]); }
		inline void SetConstantBuffer(uint32 _uiIndex, Renderer::Buffer* _pBuffer) { SetShaderResource(EShaderResource::ConstantBuffer, _uiIndex, _pBuffer); }

		inline void* GetResource(uint32 _uiIndex) { return m_aShaderResource[EShaderResource::Resource][_uiIndex]; }
		inline void SetResource(uint32 _uiIndex, Renderer::Texture2D* _pTexture) { SetShaderResource(EShaderResource::Resource, _uiIndex, _pTexture); }
		inline void SetResource(uint32 _uiIndex, Renderer::Texture3D* _pTexture) { SetShaderResource(EShaderResource::Resource, _uiIndex, _pTexture); }
		inline void SetResource(uint32 _uiIndex, Renderer::Buffer* _pBuffer) { SetShaderResource(EShaderResource::Resource, _uiIndex, _pBuffer); }

		void Execute(Renderer::RenderContext* _pContext);
		void Execute2(Renderer::RenderContext* _pContext);
		void Execute3(Renderer::RenderContext* _pContext);

	private:
		void SetShaderResource(EShaderResource::Type _eResource, uint32 _uiIndex, Renderer::Resource* _pResouce);

	private:
		Vector<Renderer::Resource*>	m_aShaderResource[EShaderResource::Count];
		AssetHandle<Effect>			m_hEffect;
		AssetHandle<Shader>			m_hPixelShader;
		Renderer::RenderTarget*		m_pRenderTarget;
		Renderer::Geometry*			m_pQuad;
	};
}}

#endif