/******************************************************************************
**	Includes
******************************************************************************/
#include "DecalPass.hpp"

#include <Engine/Engine.hpp>
#include <Engine/Asset/AssetManager.hpp>
#include <Engine/Renderer/ResourceShared.hpp>
#include <Engine/Renderer/GeometryHelper.hpp>

#include <Renderer/Renderer.hpp>
#include <Renderer/RenderContext.hpp>
#include <Renderer/Viewport.hpp>
#include <Renderer/Frustum.hpp>
#include <Renderer/Node.hpp>
#include <Renderer/IRenderable.hpp>
#include <Renderer/Resource/RenderTarget.hpp>
#include <Renderer/Resource/Geometry.hpp>
#include <Renderer/Effect.hpp>
#include <Renderer/Material.hpp>

#include <Renderer/Helper/Gizmo.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	DecalPass::DecalPass()
		: m_pRenderTarget (NULL)
		, m_pSceneBuffer (NULL)
		, m_pInstanceBuffer (NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	DecalPass::~DecalPass()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2015-11-11
	void DecalPass::Initialize(Renderer::RenderContext* _pContext)
	{
		Renderer::Renderer* pRenderer = _pContext->GetRenderer();

		Renderer::RenderTarget* pRenderTarget = _pContext->GetRenderTarget(RENDERER_TARGET_GEOMETRY);
		m_pRenderTarget = pRenderer->CreateRenderTarget();
		m_pRenderTarget->AddTarget(pRenderTarget->GetTarget(0));

		// Buffer
		m_pSceneBuffer = _pContext->GetBuffer(RENDERER_BUFFER_SCENE);
		m_pInstanceBuffer = pRenderer->CreateShaderResourceBuffer(sizeof(RenderBuffer::Decal::Instance), 1, Renderer::EBind::ShaderResource, Renderer::EUsage::Dynamic);
		m_hEffect = GetAssetManager()->Get<Effect>("@Effect/Decal.fx");
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void DecalPass::Release(Renderer::RenderContext* _pContext)
	{
		_pContext->DestroyResource(m_pRenderTarget->GetTarget(0));
		_pContext->DestroyResource(m_pRenderTarget);

		_pContext->DestroyResource(m_pSceneBuffer);
		_pContext->DestroyResource(m_pInstanceBuffer);
	}	

	//!	@brief		DrawOneGeometry 
	//!	@date		2016-06-25
	void DrawOneGeometry(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, RenderBuffer::Decal::Batch* _pBatchArray, uint32 _uiInstanceCount, Renderer::Buffer** _ppInstanceBufferOut, Renderer::Geometry* _pGeometry)
	{
		// Allocate enough memory in buffer
		if((*_ppInstanceBufferOut)->GetElementCount() < _uiInstanceCount)
		{
			_pRenderer->DestroyResource(*_ppInstanceBufferOut);
			*_ppInstanceBufferOut = _pRenderer->CreateShaderResourceBuffer(sizeof(RenderBuffer::Decal::Instance), _uiInstanceCount, Renderer::EBind::ShaderResource, Renderer::EUsage::Dynamic);
		}

		// Update Instance buffer
		RenderBuffer::Decal::Instance* pInstanceBuffer = reinterpret_cast<RenderBuffer::Decal::Instance*>(_pRenderer->Map(_pContext, *_ppInstanceBufferOut));
		for(uint32 uiInstance = 0; uiInstance < _uiInstanceCount; ++uiInstance)
		{
			RenderBuffer::Decal::Batch& kBatch = _pBatchArray[uiInstance];
			pInstanceBuffer[uiInstance].World = kBatch.Transform;
			pInstanceBuffer[uiInstance].WorldInverse = kBatch.Transform.Inverse();
		}
		_pRenderer->Unmap(_pContext, *_ppInstanceBufferOut);

		// Retrieve proper Pixel shader
		Renderer::Material* pMaterial = _pBatchArray->Material;
		if(pMaterial)
		{
			// Synchronize Material if needed
			if(pMaterial->HasChanged())
			{
				_pRenderer->Synchronize(_pContext, pMaterial);
			}
			
			// Float Shader resource
			Renderer::Buffer* pNumberBuffer = pMaterial->GetNumberBuffer();
			if(pNumberBuffer)
			{
				_pRenderer->SetPixelShaderResource(_pContext, 0, pNumberBuffer);
			}

			// Texture shader resource
			const Vector<Renderer::Texture2D*>& vTexture = pMaterial->GetVecTexture();
			const uint32 uiTextureCount = vTexture.GetSize();
			for(uint32 uiTexture = 0; uiTexture < uiTextureCount; ++uiTexture)
			{
				_pRenderer->SetPixelShaderResource(_pContext, uiTexture + 1, vTexture[uiTexture]);
			}
		}

		// Draw the section
		uint32 uiIndexOffset = _pGeometry->GetIndexOffset();
		uint32 uiIndexCount = _pGeometry->GetIndexCount();
		_pRenderer->Draw(_pContext, uiIndexOffset, uiIndexCount, _uiInstanceCount);
	}

	//!	@brief		DrawAllGeometry 
	//!	@date		2016-06-25
	void DrawAllGeometry(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, RenderBuffer::Decal::Batch* _pBatchArray, uint32 _uiBatchCount, Renderer::Buffer** _ppInstanceBufferOut, Renderer::Geometry* _pGeometry)
	{
		// HACK
		for(uint32 uiCommand = 0; uiCommand < _uiBatchCount; ++uiCommand)
		{
			DrawOneGeometry(_pRenderer, _pContext, &_pBatchArray[uiCommand], 1, _ppInstanceBufferOut, _pGeometry);
		}
	}

	//!	@brief		Execute 
	//!	@date		2015-11-11
	void DecalPass::Execute(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pBuffer)
	{	
		RenderBuffer::Decal::Batch* pBatchArray; uint32 uiBatchCount;
		if(_pBuffer->Get(&pBatchArray, uiBatchCount))
		{
			// Update ConstantBuffer
			RenderBuffer::Constant::Scene* pSceneBufferSource; uint32 uiSceneCount;
			if(_pBuffer->Get(&pSceneBufferSource, uiSceneCount))
			{
				RenderBuffer::Constant::Scene* pSceneBufferDestination = reinterpret_cast<RenderBuffer::Constant::Scene*>(_pRenderer->Map(_pContext, m_pSceneBuffer));
				memcpy_s(pSceneBufferDestination, sizeof(RenderBuffer::Constant::Scene), pSceneBufferSource, sizeof(RenderBuffer::Constant::Scene));
				_pRenderer->Unmap(_pContext, m_pSceneBuffer);
			}

			// Set RenderState
			Renderer::Effect* pEffect = m_hEffect->GetData();
			_pRenderer->SetRenderState(_pContext, pEffect->GetRenderState());
			_pRenderer->SetTopology(_pContext, pEffect->GetTopology());
			_pRenderer->SetRenderTarget(_pContext, m_pRenderTarget);
			_pRenderer->SetViewport(_pContext, 0, 0, m_pRenderTarget->GetTarget(0)->GetWidth(), m_pRenderTarget->GetTarget(0)->GetHeight());

			// Set Vertex Shader and resources
			Renderer::Shader* pVertexShader = pEffect->GetShader(Renderer::EShader::Vertex, Renderer::ETechnique::Instancing);
			_pRenderer->SetVertexShader(_pContext, pVertexShader);
			_pRenderer->SetVertexShaderConstantBuffer(_pContext, 0, m_pSceneBuffer);
			_pRenderer->SetVertexShaderResource(_pContext, 0, m_pInstanceBuffer);

			Renderer::Geometry* pGeometry = GeometryHelper::GetOrCreate(_pRenderer, Renderer::EPrimitive::Cube);
			Renderer::Buffer* pVertexBuffer = pGeometry->GetVertexBuffer();
			Renderer::Buffer* pIndexBuffer = pGeometry->GetIndexBuffer();
			_pRenderer->SetVertexBuffer(_pContext, pVertexBuffer);
			_pRenderer->SetIndexBuffer(_pContext, pIndexBuffer);

			Renderer::Shader* pPixelShader = m_hEffect->GetData()->GetShader(Renderer::EShader::Pixel, Renderer::ETechnique::Instancing);
			_pRenderer->SetPixelShader(_pContext, pPixelShader);
			Renderer::RenderTarget* pRenderTarget = _pContext->GetRenderTarget(RENDERER_TARGET_GEOMETRY);
			_pRenderer->SetPixelShaderResource(_pContext, 2, pRenderTarget->GetDepthStencil());
			_pRenderer->SetPixelShaderResource(_pContext, 3, m_pInstanceBuffer);

			DrawAllGeometry(_pRenderer, _pContext, pBatchArray, uiBatchCount, &m_pInstanceBuffer, pGeometry);
		}
	}
}}
