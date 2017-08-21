/******************************************************************************
**	Includes
******************************************************************************/
#include "LightPass.hpp"

#include <Renderer/Renderer.hpp>
#include <Renderer/RenderContext.hpp>
#include <Renderer/Viewport.hpp>
#include <Renderer/Resource/RenderTarget.hpp>

#include <Engine/Renderer/RenderFilter.hpp>
#include <Engine/Renderer/Pass/ShadowPass.hpp>

/******************************************************************************
**	Define
******************************************************************************/
#define LIGHT_PASS_REGISTER_INDEX_DIRECTIONAL_BUFFER	4
#define LIGHT_PASS_REGISTER_INDEX_DIRECTIONAL_SHADOW	LIGHT_PASS_REGISTER_INDEX_DIRECTIONAL_BUFFER + 1
#define LIGHT_PASS_REGISTER_INDEX_POINT_BUFFER			LIGHT_PASS_REGISTER_INDEX_DIRECTIONAL_SHADOW + 1
#define LIGHT_PASS_REGISTER_INDEX_POINT_SHADOW			LIGHT_PASS_REGISTER_INDEX_POINT_BUFFER + 1
#define LIGHT_PASS_REGISTER_INDEX_ENVIRONMENT			LIGHT_PASS_REGISTER_INDEX_POINT_SHADOW + 1
#define LIGHT_PASS_REGISTER_INDEX_NOISE					LIGHT_PASS_REGISTER_INDEX_ENVIRONMENT + 1

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	LightPass::LightPass()
		: m_pRenderTarget(NULL)
		, m_pLightFilter(NULL)
		, m_pGeometryRenderTarget(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	LightPass::~LightPass()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2015-11-11	
	void LightPass::Initialize(Renderer::RenderContext* _pContext)
	{
		Renderer::Texture2D* pRenderTexture = _pContext->GetViewport()->GetRenderTarget()->GetTarget(0);
		uint32 uiWidth = pRenderTexture->GetWidth();
		uint32 uiHeight = pRenderTexture->GetHeight();

		// Input Resources
		m_pGeometryRenderTarget = _pContext->GetRenderTarget(RENDERER_TARGET_GEOMETRY);
		Renderer::Buffer* pSceneBuffer = _pContext->GetBuffer(RENDERER_BUFFER_SCENE);
		Renderer::Buffer* pVoxelGrid = _pContext->GetBuffer(RENDERER_BUFFER_VOXEL_GRID);
		//Renderer::Buffer* pVoxelFragment = _pContext->GetBuffer(RENDERER_BUFFER_VOXEL_FRAMENT);
		Renderer::Texture3D* pVoxelVolume = _pContext->GetTexture3D(RENDERER_TEXTURE_VOXEL_VOLUME_COLOR);

		m_hNoiseTextute = GetAssetManager()->Get<Texture>("@texture/noise/blue_noise_470.png");

		// Output Texture
		m_pRenderTarget = _pContext->GetRenderer()->CreateRenderTarget();
		Renderer::Texture2D* pDirectLight = _pContext->CreateTexture2D(RENDERER_TEXTURE_LIGHT_DIRECT, uiWidth, uiHeight, 1, Renderer::EFormat::R11G11B10_FLOAT, Renderer::EBind::RenderTarget | Renderer::EBind::ShaderResource);
		Renderer::Texture2D* pIndirectLight = _pContext->CreateTexture2D(RENDERER_TEXTURE_LIGHT_INDIRECT, uiWidth, uiHeight, 1, Renderer::EFormat::R8G8B8A8_UNORM, Renderer::EBind::RenderTarget | Renderer::EBind::ShaderResource);
		Renderer::Texture2D* pAmbientOcclusion = _pContext->CreateTexture2D(RENDERER_TEXTURE_LIGHT_AMBIENT_OCCLUSION, uiWidth, uiHeight, 1, Renderer::EFormat::R8G8B8A8_UNORM, Renderer::EBind::RenderTarget | Renderer::EBind::ShaderResource);
		m_pRenderTarget->AddTarget(pDirectLight);
		m_pRenderTarget->AddTarget(pIndirectLight);
		m_pRenderTarget->AddTarget(pAmbientOcclusion);

		// Initialize Light filter
		m_pLightFilter = new RenderFilter();
		m_pLightFilter->Initialize(_pContext, "@Effect/Filter/Display/Light.ps");
		m_pLightFilter->SetRenderTarget(m_pRenderTarget);
		m_pLightFilter->SetConstantBuffer(0, pSceneBuffer);
		m_pLightFilter->SetConstantBuffer(1, pVoxelGrid);	
		m_pLightFilter->SetResource(0, m_pGeometryRenderTarget->GetTarget(0));
		m_pLightFilter->SetResource(1, m_pGeometryRenderTarget->GetTarget(1));
		m_pLightFilter->SetResource(2, m_pGeometryRenderTarget->GetDepthStencil());
		m_pLightFilter->SetResource(3, pVoxelVolume);
		m_pLightFilter->SetResource(LIGHT_PASS_REGISTER_INDEX_NOISE, m_hNoiseTextute->GetData());
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void LightPass::Release(Renderer::RenderContext* _pContext)
	{
		SAFE_DELETE(m_pLightFilter)
		_pContext->DestroyResource(m_pRenderTarget);
		_pContext->DestroyResource(m_pGeometryRenderTarget);
		m_hNoiseTextute.Release();
	}

	template <class T>
	void UpdateBuffer(Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pRenderBuffer, RenderFilter* _pFilter, Renderer::Buffer** _ppBuffer, uint32 _uiRegisterIndex)
	{
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
			}

			// Update buffer
			uint32 uiBufferSize = (*_ppBuffer)->GetSize();
			void* pDestination = pRenderer->Map(_pContext, *_ppBuffer);
			memcpy_s(pDestination, uiBufferSize, pBufferSource, uiBufferSize);
			pRenderer->Unmap(_pContext, *_ppBuffer);

			_pFilter->SetResource(_uiRegisterIndex, *_ppBuffer);
		}
	}

	//!	@brief		Execute 
	//!	@date		2016-06-25
	void LightPass::Execute(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pBuffer)
	{
		Renderer::RenderBuffer* pFrameBuffer = _pRenderer->GetFrameBuffer();
		pFrameBuffer;
		
		Renderer::Buffer* pBuffer = SharedResource::Light::Buffer[SharedResource::Light::Directional];
		if(pBuffer)
		{
			Renderer::Texture2D* pShadowMap = SharedResource::Light::ShadowMap[SharedResource::Light::Directional];
			m_pLightFilter->SetResource(LIGHT_PASS_REGISTER_INDEX_DIRECTIONAL_BUFFER, pBuffer);
			m_pLightFilter->SetResource(LIGHT_PASS_REGISTER_INDEX_DIRECTIONAL_SHADOW, pShadowMap);
		}
		
		pBuffer = SharedResource::Light::Buffer[SharedResource::Light::Point];
		if(pBuffer)
		{
			Renderer::Texture2D* pShadowMap = SharedResource::Light::ShadowMap[SharedResource::Light::Point];
			m_pLightFilter->SetResource(LIGHT_PASS_REGISTER_INDEX_POINT_BUFFER, pBuffer);
			m_pLightFilter->SetResource(LIGHT_PASS_REGISTER_INDEX_POINT_SHADOW, pShadowMap);
			m_pLightFilter->SetConstantBuffer(1, SharedResource::Light::ConstantBuffer[SharedResource::Light::Point]);
		}

		// Apply environment map
		RenderBuffer::Light::Environment* pCommand; uint32 uiCount;
		if(_pBuffer->Get(&pCommand, uiCount))
		{
			m_pLightFilter->SetResource(LIGHT_PASS_REGISTER_INDEX_ENVIRONMENT, pCommand->Texture);
		}

		m_pLightFilter->Execute(_pContext);
	}
}}