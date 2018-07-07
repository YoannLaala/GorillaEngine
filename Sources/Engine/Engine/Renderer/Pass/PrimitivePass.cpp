/******************************************************************************
**	Includes
******************************************************************************/
#include "PrimitivePass.hpp"

#include <Engine/Engine.hpp>
#include <Engine/Asset/AssetManager.hpp>
#include <Engine/Renderer/ResourceShared.hpp>
#include <Engine/Renderer/GeometryHelper.hpp>

#include <Renderer/Renderer.hpp>
#include <Renderer/RenderContext.hpp>
#include <Renderer/Viewport.hpp>
#include <Renderer/Effect.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	PrimitivePass::PrimitivePass()
		: m_pRenderState(NULL)
		, m_pDepthStencilTexture(NULL)
		, m_pSceneBuffer(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	PrimitivePass::~PrimitivePass()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2015-11-11
	void PrimitivePass::Initialize(Renderer::RenderContext* _pContext)
	{
		Renderer::Renderer* pRenderer = _pContext->GetRenderer();
		Renderer::RenderBuffer* pGlobalBuffer = pRenderer->GetGlobalBuffer();

		// Create RenderState
		Renderer::RenderState::Key kRenderStateKey;
		kRenderStateKey.WireFrame = true;
		kRenderStateKey.Cull = false;
		m_pRenderState = pRenderer->CreateRenderState(kRenderStateKey);

		m_pRenderTarget = pRenderer->CreateRenderTarget();
		m_pRenderTarget->AddTarget(_pContext->GetTexture2D(RENDERER_TEXTURE_LIGHT_DIRECT));

		m_pDepthStencilTexture = _pContext->GetTexture2D(RENDERER_TEXTURE_GEOMETRY_DEPTH_STENCIL);
		m_pSceneBuffer = _pContext->GetBuffer(RENDERER_BUFFER_SCENE);

		Renderer::Primitive* pPrimitive; uint32 uiCount;
		if(!pGlobalBuffer->Get(&pPrimitive, uiCount))
		{
			pPrimitive = pGlobalBuffer->Push<Renderer::Primitive>(Renderer::Primitive::Count);
			memset(pPrimitive, 0, Renderer::Primitive::Count * sizeof(Renderer::Primitive));

			// Using geometry shader for lines
			pPrimitive[Renderer::Primitive::Line].Geometry = GeometryHelper::GetOrCreate(pRenderer, Renderer::EPrimitive::Point);
			pPrimitive[Renderer::Primitive::Line].VertexShader = new AssetHandle<Shader>(GetAssetManager()->Get<Shader>("@Effect/Primitive/Line.vs", "--technique Instancing"));
			pPrimitive[Renderer::Primitive::Line].GeometryShader = new AssetHandle<Shader>(GetAssetManager()->Get<Shader>("@Effect/Primitive/Line.gs", "--technique Instancing"));

			pPrimitive[Renderer::Primitive::Quad].Geometry = GeometryHelper::GetOrCreate(pRenderer, Renderer::EPrimitive::Quad);
			pPrimitive[Renderer::Primitive::Quad].VertexShader = new AssetHandle<Shader>(GetAssetManager()->Get<Shader>("@Effect/Primitive/Quad.vs", "--technique Instancing"));

			pPrimitive[Renderer::Primitive::Cube].Geometry = GeometryHelper::GetOrCreate(pRenderer, Renderer::EPrimitive::Point);
			pPrimitive[Renderer::Primitive::Cube].VertexShader = new AssetHandle<Shader>(GetAssetManager()->Get<Shader>("@Effect/Primitive/Cube.vs", "--technique Instancing"));
			pPrimitive[Renderer::Primitive::Cube].GeometryShader = new AssetHandle<Shader>(GetAssetManager()->Get<Shader>("@Effect/Primitive/Cube.gs", "--technique Instancing"));

			// Using geometry shader for frustum
			pPrimitive[Renderer::Primitive::Frustum].Geometry = GeometryHelper::GetOrCreate(pRenderer, Renderer::EPrimitive::Point);
			pPrimitive[Renderer::Primitive::Frustum].VertexShader = new AssetHandle<Shader>(GetAssetManager()->Get<Shader>("@Effect/Primitive/Frustum.vs", "--technique Instancing"));
			pPrimitive[Renderer::Primitive::Frustum].GeometryShader = new AssetHandle<Shader>(GetAssetManager()->Get<Shader>("@Effect/Primitive/Frustum.gs", "--technique Instancing"));
		}

		m_hPixelShader =  GetAssetManager()->Get<Shader>("@Effect/Primitive/Primitive.ps", "--technique Instancing");
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void PrimitivePass::Release(Renderer::RenderContext* _pContext)
	{
		Renderer::Renderer* pRenderer = _pContext->GetRenderer();

		pRenderer->DestroyResource(m_pRenderState);
		m_pDepthStencilTexture = NULL;
		m_pSceneBuffer = NULL;
		m_hPixelShader.Release();
	}

	//!	@brief		UpdatePrimitiveBuffer 
	//!	@date		2016-06-25
	template <Renderer::Primitive::Type T>
	void ExecutePrimitive(Renderer::RenderContext* _pContext, Renderer::Primitive* pPrimitiveArray)
	{
		Renderer::Renderer* pRenderer = _pContext->GetRenderer();
		Renderer::RenderBuffer* pFrameBuffer = pRenderer->GetFrameBuffer();

		// Retrieve the buffer
		Renderer::Primitive::Instance<T>* pInstance; uint32 uiInstanceCount; 
		if(pFrameBuffer->Get(&pInstance, uiInstanceCount))
		{
			Renderer::Primitive& kPrimitive = pPrimitiveArray[T];
			Renderer::Buffer*& pBuffer = kPrimitive.Buffer;
			Renderer::Geometry* pGeometry = kPrimitive.Geometry;

			// Generate Buffer if needed
			if(!pBuffer || pBuffer->GetElementCount() < uiInstanceCount)
			{
				if(pBuffer)
				{
					pRenderer->DestroyResource(pBuffer);
				}
				
				pBuffer = pRenderer->CreateShaderResourceBuffer(sizeof(Renderer::Primitive::Instance<T>), uiInstanceCount, Renderer::EBind::ShaderResource, Renderer::EUsage::Dynamic);
			}

			// Update buffer
			uint32 uiBufferSize = pBuffer->GetSize();
			void* pDestination = pRenderer->Map(_pContext, pBuffer);
			memcpy_s(pDestination, uiBufferSize, pInstance, uiBufferSize);
			pRenderer->Unmap(_pContext, pBuffer);

			// Set proper vertex shader
			ASSERT(kPrimitive.VertexShader != NULL, "[PrimitivePass] VertexShader NULL");
			Renderer::Shader* pVertexShader = (*static_cast<AssetHandle<Shader>*>(kPrimitive.VertexShader))->GetData();
			pRenderer->SetVertexShader(_pContext, pVertexShader);
			pRenderer->SetVertexShaderResource(_pContext, 0, pBuffer);

			// Set Geometry shader if needed
			Renderer::ETopology::Type eTopology = Renderer::ETopology::TriangleList;
			if(kPrimitive.GeometryShader)
			{
				eTopology = Renderer::ETopology::PointList;
				Renderer::Shader* pGeometryShader = (*static_cast<AssetHandle<Shader>*>(kPrimitive.GeometryShader))->GetData();
				pRenderer->SetGeometryShader(_pContext, pGeometryShader);
				pRenderer->SetGeometryShaderResource(_pContext, 0, pBuffer);
			}
			
			// Set proper geometry and topology
			pRenderer->SetTopology(_pContext, eTopology);
			pRenderer->SetVertexBuffer(_pContext, pGeometry->GetVertexBuffer());
			pRenderer->SetIndexBuffer(_pContext, pGeometry->GetIndexBuffer());
			pRenderer->Draw(_pContext, 0, pGeometry->GetIndexCount(), uiInstanceCount);			
		}
		pRenderer->SetGeometryShader(_pContext, NULL);
	}

	//!	@brief		Execute 
	//!	@date		2016-06-25
	void PrimitivePass::Execute(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* /*_pBuffer*/)
	{
		Renderer::RenderBuffer* pGlobalBuffer = _pRenderer->GetGlobalBuffer();

		Renderer::Primitive* pPrimitiveArray; uint32 uiCount;
		if(pGlobalBuffer->Get(&pPrimitiveArray, uiCount))
		{
			// Prepare Rendering
			_pRenderer->SetRenderTarget(_pContext, m_pRenderTarget);
			_pRenderer->SetViewport(_pContext, 0, 0, m_pRenderTarget->GetTarget(0)->GetWidth(), m_pRenderTarget->GetTarget(0)->GetHeight());
			_pRenderer->SetRenderState(_pContext, m_pRenderState);
		
			// Prepare Pixel Shader
			Renderer::Shader* pPixelShader = m_hPixelShader->GetData();
			_pRenderer->SetVertexShaderConstantBuffer(_pContext, 0, m_pSceneBuffer);
			_pRenderer->SetGeometryShaderConstantBuffer(_pContext, 0, m_pSceneBuffer);
			_pRenderer->SetPixelShader(_pContext, pPixelShader);

			// Prepare all primitive buffer needed
			ExecutePrimitive<Renderer::Primitive::Line>(_pContext, pPrimitiveArray);
			ExecutePrimitive<Renderer::Primitive::Quad>(_pContext, pPrimitiveArray);
			ExecutePrimitive<Renderer::Primitive::Cube>(_pContext, pPrimitiveArray);
			ExecutePrimitive<Renderer::Primitive::Frustum>(_pContext, pPrimitiveArray);
		}
	}
}}
