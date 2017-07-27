/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Renderer/Pass/ShadowPass.hpp>

#include <Engine/Engine.hpp>
#include <Engine/Asset/AssetManager.hpp>
#include <Engine/Renderer/ResourceShared.hpp>
#include <Engine/Renderer/Pass/GeometryPass.hpp>

#include <Renderer/Renderer.hpp>
#include <Renderer/RenderContext.hpp>
#include <Renderer/Camera.hpp>
#include <Renderer/Resource/RenderTarget.hpp>
#include <Renderer/Resource/Geometry.hpp>
#include <Renderer/Effect.hpp>

/******************************************************************************
**	Define
******************************************************************************/
#define LIGHT_DIRECTIONAL_RESOLUTION	1024
#define LIGHT_POINT_RESOLUTION			256

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	struct LightConstantBuffer
	{
		Math::Matrix44	View[6];
		Math::Matrix44	Projection;
	};

	Renderer::Buffer* SharedResource::Light::ConstantBuffer[SharedResource::Light::Count];
	Renderer::Buffer* SharedResource::Light::Buffer[SharedResource::Light::Count];
	Renderer::Texture2D* SharedResource::Light::ShadowMap[SharedResource::Light::Count];


	//!	@brief		Constructor 
	//!	@date		2015-11-11
	ShadowPass::ShadowPass()
		: m_pSceneBuffer (NULL)
		, m_pInstanceBuffer (NULL)
	{
		memset(m_aRenderTarget, 0, sizeof(void*) * SharedResource::Light::Count);
		
		// Move this...
		memset(SharedResource::Light::ConstantBuffer, 0, sizeof(void*) * SharedResource::Light::Count);
		memset(SharedResource::Light::Buffer, 0, sizeof(void*) * SharedResource::Light::Count);
		memset(SharedResource::Light::ShadowMap, 0, sizeof(void*) * SharedResource::Light::Count);
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	ShadowPass::~ShadowPass()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2015-11-11
	void ShadowPass::Initialize(Renderer::RenderContext* _pContext)
	{
		SharedResource::Light::ConstantBuffer[SharedResource::Light::Point] = _pContext->GetRenderer()->CreateConstantBuffer(sizeof(LightConstantBuffer), 1);

		// RenderTarget
		m_aRenderTarget[SharedResource::Light::Directional] = _pContext->CreateRenderTarget(RENDERER_TARGET_SHADOW_DIRECTIONAL);
		m_aRenderTarget[SharedResource::Light::Point] = _pContext->CreateRenderTarget(RENDERER_TARGET_SHADOW_POINT);
		
		// Buffer
		m_pSceneBuffer = _pContext->GetBuffer(RENDERER_BUFFER_SCENE);
		m_pInstanceBuffer = _pContext->GetBuffer(RENDERER_BUFFER_INSTANCE);
		m_aEffect[SharedResource::Light::Directional] = GetAssetManager()->Get<Effect>("@Effect/Shadow/DirectionalShadowMap.fx");
		m_aEffect[SharedResource::Light::Point] = GetAssetManager()->Get<Effect>("@Effect/Shadow/PointShadowMap.fx");
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void ShadowPass::Release(Renderer::RenderContext* _pContext)
	{
		for(uint32 eLight = 0; eLight < SharedResource::Light::Count; ++eLight)
		{
			Renderer::RenderTarget*& pRenderTarget = m_aRenderTarget[eLight];
			if(pRenderTarget)
			{
				_pContext->DestroyResource(pRenderTarget);
				pRenderTarget = NULL;
			}
		}

		_pContext->DestroyResource(m_pSceneBuffer);
		_pContext->DestroyResource(m_pInstanceBuffer);
	}

	//!	@brief		Execute 
	//!	@date		2015-11-11
	void ShadowPass::Execute(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pBuffer)
	{	
		ExecuteDirectionalLight(_pRenderer, _pContext, _pBuffer);
		//ExecutePointLight(_pRenderer, _pContext, _pBuffer);
	}

	template <class T>
	bool UpdateBuffer(Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pRenderBuffer, Renderer::Buffer** _ppBuffer)
	{
		bool bChanged = false;

		// Point light
		T* pBufferSource; uint32 uiCount;
		if(_pRenderBuffer->Get(&pBufferSource, uiCount))
		{
			Renderer::Renderer* pRenderer = _pContext->GetRenderer();

			// Generate Buffer if needed
			if(!*_ppBuffer || (*_ppBuffer)->GetElementCount() < uiCount)
			{
				if(*_ppBuffer)
				{
					pRenderer->DestroyResource(*_ppBuffer);
				}
				
				*_ppBuffer = pRenderer->CreateShaderResourceBuffer(sizeof(T), uiCount, Renderer::EBind::ShaderResource, Renderer::EUsage::Dynamic);
				bChanged = true;
			}

			// Update buffer
			uint32 uiBufferSize = (*_ppBuffer)->GetSize();
			void* pDestination = pRenderer->Map(_pContext, *_ppBuffer);
			memcpy_s(pDestination, uiBufferSize, pBufferSource, uiBufferSize);
			pRenderer->Unmap(_pContext, *_ppBuffer);
		}

		return bChanged;
	}

	//!	@brief		ExecuteDirectionalLight 
	//!	@date		2015-11-11
	void ShadowPass::ExecuteDirectionalLight(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pBuffer)
	{	
		MARKER_GPU(_pContext, "Directional");
		Renderer::RenderBuffer* pFrameBuffer = _pRenderer->GetFrameBuffer();
		pFrameBuffer;

		RenderBuffer::Geometry::Batch* pBatchArray; uint32 uiBatchCount;
		if(_pBuffer->Get(&pBatchArray, uiBatchCount))
		{
			UpdateBuffer<RenderBuffer::Light::Directional>(_pContext, _pBuffer, &SharedResource::Light::Buffer[SharedResource::Light::Directional]);

			Renderer::RenderTarget* pRenderTarget = m_aRenderTarget[SharedResource::Light::Directional];
			Renderer::Texture2D*& pShadowMap = SharedResource::Light::ShadowMap[SharedResource::Light::Directional];
			if(!pShadowMap)
			{
				pShadowMap = _pRenderer->CreateTexture2D(LIGHT_DIRECTIONAL_RESOLUTION, LIGHT_DIRECTIONAL_RESOLUTION, 1, Renderer::EFormat::R24G8_TYPELESS, Renderer::EBind::DepthStencil | Renderer::EBind::ShaderResource);
				pRenderTarget->SetDepthStencil(pShadowMap);
			}

			// Clear
			Renderer::Texture2D* pDepthStencilTexture = pRenderTarget->GetDepthStencil();
			_pRenderer->SetRenderTarget(_pContext, pRenderTarget);
			_pRenderer->SetViewport(_pContext, 0, 0, pDepthStencilTexture->GetWidth(), pDepthStencilTexture->GetHeight());
			_pRenderer->Clear(_pContext, pRenderTarget);

			// Set RenderState
			Renderer::Effect* pEffect = m_aEffect[SharedResource::Light::Directional]->GetData();
			_pRenderer->SetRenderState(_pContext, pEffect->GetRenderState());
			_pRenderer->SetTopology(_pContext, pEffect->GetTopology());

			// Set Vertex Shader and resources
			Renderer::Shader* pVertexShader = pEffect->GetShader(Renderer::EShader::Vertex, Renderer::ETechnique::Instancing);
			_pRenderer->SetVertexShader(_pContext, pVertexShader);
			_pRenderer->SetVertexShaderConstantBuffer(_pContext, 0, m_pSceneBuffer);
			_pRenderer->SetVertexShaderResource(_pContext, 0, m_pInstanceBuffer);

			// Draw shadow for all lights
			RenderBuffer::Light::Directional* pBufferSource; uint32 uiCount;
			_pBuffer->Get(&pBufferSource, uiCount);
			for(uint32 uiLight = 0; uiLight < uiCount; ++uiLight)
			{
				RenderBuffer::Light::Directional& kLight = pBufferSource[uiLight];

				// Update ConstantBuffer
				RenderBuffer::Constant::Scene* pSceneBuffer = reinterpret_cast<RenderBuffer::Constant::Scene*>(_pRenderer->Map(_pContext, m_pSceneBuffer));
				pSceneBuffer->View = kLight.View; 
				pSceneBuffer->Projection = kLight.Projection;
				_pRenderer->Unmap(_pContext, m_pSceneBuffer);

				GeometryPass::DrawAllGeometry(_pRenderer, _pContext, pBatchArray, uiBatchCount, &m_pInstanceBuffer);
			}
		}
	}

	//!	@brief		ExecutePointLight 
	//!	@date		2015-11-11
	void ShadowPass::ExecutePointLight(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pBuffer)
	{	
		MARKER_GPU(_pContext, "Point");

		RenderBuffer::Geometry::Batch* pBatchArray; uint32 uiBatchCount;
		if(_pBuffer->Get(&pBatchArray, uiBatchCount))
		{
			// Synchronize light buffer and texture array for shadows
			Renderer::RenderTarget* pRenderTarget = m_aRenderTarget[SharedResource::Light::Point];
			if(UpdateBuffer<RenderBuffer::Light::Point>(_pContext, _pBuffer, &SharedResource::Light::Buffer[SharedResource::Light::Point]))
			{
				Renderer::Texture2D*& pShadowMap = SharedResource::Light::ShadowMap[SharedResource::Light::Point];
				if(pShadowMap)
				{
					_pRenderer->DestroyResource(pShadowMap);
					pRenderTarget->Clear();
				}

				pShadowMap = _pRenderer->CreateTextureCube(LIGHT_POINT_RESOLUTION, LIGHT_POINT_RESOLUTION, 1, Renderer::EFormat::R32_FLOAT, Renderer::EBind::ShaderResource | Renderer::EBind::UnorderedAccess);
				pRenderTarget->AddOutput(pShadowMap);
			}

			if(!SharedResource::Light::Buffer[SharedResource::Light::Point]) return;

			// Set RenderTarget
			Renderer::Texture2D* pTextureArray = static_cast<Renderer::Texture2D*>(pRenderTarget->GetOutput(0));
			_pRenderer->SetRenderTarget(_pContext, pRenderTarget);
			_pRenderer->SetViewport(_pContext, 0, 0, pTextureArray->GetWidth(), pTextureArray->GetHeight());
			_pRenderer->Clear(_pContext, pRenderTarget);

			// Set RenderState
			Renderer::Effect* pEffect = m_aEffect[SharedResource::Light::Point]->GetData();
			_pRenderer->SetRenderState(_pContext, pEffect->GetRenderState());
			_pRenderer->SetTopology(_pContext, pEffect->GetTopology());

			// Set Vertex Shader and resources
			Renderer::Shader* pVertexShader = pEffect->GetShader(Renderer::EShader::Vertex, Renderer::ETechnique::Instancing);
			_pRenderer->SetVertexShader(_pContext, pVertexShader);
			_pRenderer->SetVertexShaderConstantBuffer(_pContext, 0, m_pSceneBuffer);
			_pRenderer->SetVertexShaderResource(_pContext, 0, m_pInstanceBuffer);

			Renderer::Shader* pGeometryShader = pEffect->GetShader(Renderer::EShader::Geometry);
			_pRenderer->SetGeometryShader(_pContext, pGeometryShader);
			_pRenderer->SetGeometryShaderConstantBuffer(_pContext, 0, SharedResource::Light::ConstantBuffer[SharedResource::Light::Point]);
			_pRenderer->SetGeometryShaderResource(_pContext, 1, SharedResource::Light::Buffer[SharedResource::Light::Point]);		// index 1 since 0 is used for Material Number buffer

			Renderer::Shader* pPixelShader = pEffect->GetShader(Renderer::EShader::Pixel);
			_pRenderer->SetPixelShader(_pContext, pPixelShader);
			_pRenderer->SetPixelShaderConstantBuffer(_pContext, 0, SharedResource::Light::ConstantBuffer[SharedResource::Light::Point]);
			_pRenderer->SetPixelShaderResource(_pContext, 1, SharedResource::Light::Buffer[SharedResource::Light::Point]);		// index 1 since 0 is used for Material Number buffer

			RenderBuffer::Constant::Scene* pSceneBufferSource; uint32 uiSceneCount;
			_pBuffer->Get(&pSceneBufferSource, uiSceneCount);

			RenderBuffer::Light::Point* pBufferSource; uint32 uiCount;
			_pBuffer->Get(&pBufferSource, uiCount);
			for(uint32 uiLight = 0; uiLight < uiCount; ++uiLight)
			{
				RenderBuffer::Light::Point& kLight = pBufferSource[uiLight];

				LightConstantBuffer* pDestination = reinterpret_cast<LightConstantBuffer*>(_pRenderer->Map(_pContext, SharedResource::Light::ConstantBuffer[SharedResource::Light::Point]));
				Renderer::Camera::ComputeView(pDestination->View[0], kLight.Position, Math::Vector3::UnitZ * -1, Math::Vector3::UnitY, Math::Vector3::UnitX);		// +X
				Renderer::Camera::ComputeView(pDestination->View[1], kLight.Position, Math::Vector3::UnitZ, Math::Vector3::UnitY, Math::Vector3::UnitX * -1);		// -X
				Renderer::Camera::ComputeView(pDestination->View[2], kLight.Position, Math::Vector3::UnitX, Math::Vector3::UnitZ * -1, Math::Vector3::UnitY);		// +Y
				Renderer::Camera::ComputeView(pDestination->View[3], kLight.Position, Math::Vector3::UnitX, Math::Vector3::UnitZ, Math::Vector3::UnitY * -1);		// -Y
				Renderer::Camera::ComputeView(pDestination->View[4], kLight.Position, Math::Vector3::UnitX, Math::Vector3::UnitY, Math::Vector3::UnitZ);			// +Z
				Renderer::Camera::ComputeView(pDestination->View[5], kLight.Position, Math::Vector3::UnitX * -1, Math::Vector3::UnitY, Math::Vector3::UnitZ * -1);	// -Z
				Renderer::Camera::ComputePerspective(pDestination->Projection, 90.0f, 1.0f, 0.1f, 10.0f);													// -Z
				_pRenderer->Unmap(_pContext, SharedResource::Light::ConstantBuffer[SharedResource::Light::Point]);

				GeometryPass::DrawAllGeometry(_pRenderer, _pContext, pBatchArray, uiBatchCount, &m_pInstanceBuffer);
			}

			_pRenderer->SetGeometryShader(_pContext, NULL);
		}
	}
}}
