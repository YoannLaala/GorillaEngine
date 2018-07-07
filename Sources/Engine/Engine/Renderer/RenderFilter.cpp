/******************************************************************************
**	Includes
******************************************************************************/
#include "RenderFilter.hpp"

#include <Engine/Engine.hpp>
#include <Engine/Asset/AssetManager.hpp>
#include <Engine/Renderer/ResourceShared.hpp>
#include <Engine/Renderer/GeometryHelper.hpp>

#include <Renderer/Renderer.hpp>
#include <Renderer/RenderContext.hpp>
#include <Renderer/Resource/RenderTarget.hpp>
#include <Renderer/Resource/Geometry.hpp>
#include <Renderer/Effect.hpp>
#include <Renderer/Camera.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	RenderFilter::RenderFilter()
		: m_pRenderTarget(NULL)
		, m_pQuad(NULL)
	{
		// Nothing to do
	}
	
	//!	@brief		Destructor 
	//!	@date		2015-11-11
	RenderFilter::~RenderFilter()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2015-11-11
	void RenderFilter::Initialize(Renderer::RenderContext* _pContext, const char* _szFilter, const char* _szParam /*= NULL*/)
	{
		m_pQuad = GeometryHelper::GetOrCreate(_pContext->GetRenderer(), Renderer::EPrimitive::Quad);
		m_hEffect = GetAssetManager()->Get<Effect>("@Effect/Filter/Filter.fx");
		m_hPixelShader = GetAssetManager()->Get<Shader>(_szFilter, _szParam);
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void RenderFilter::Release(Renderer::RenderContext* _pContext)
	{
		_pContext->DestroyResource(m_pQuad);
	}

	//!	@brief		SetShaderResource 
	//!	@date		2015-11-11
	void RenderFilter::SetShaderResource(EShaderResource::Type _eResource, uint32 _uiIndex, Renderer::Resource* _pResouce)
	{ 
		Vector<Renderer::Resource*>& vResource = m_aShaderResource[_eResource];
		vResource.ExpandTo(_uiIndex, NULL);
		vResource[_uiIndex] = _pResouce;
	}

	//!	@brief		Execute 
	//!	@date		2015-11-11
	void RenderFilter::Execute3(Renderer::RenderContext* _pContext)
	{
		Renderer::Renderer* pRenderer = _pContext->GetRenderer();
		//Renderer::Texture2D* pRenderTexture = m_pRenderTarget->GetTarget(0);

		// Set RenderState
		Renderer::Effect* pEffect = m_hEffect->GetData();
		pRenderer->SetRenderTarget(_pContext, m_pRenderTarget);
		pRenderer->SetTopology(_pContext, pEffect->GetTopology());

		// Set Vertex & Pixel shaders
		Renderer::Shader* pVertexShader = pEffect->GetShader(Renderer::EShader::Vertex);
		pRenderer->SetVertexShader(_pContext, pVertexShader);
		pRenderer->SetPixelShader(_pContext, m_hPixelShader->GetData());

		// ConstantBuffer
		Vector<Renderer::Resource*>* pVecBuffer = &m_aShaderResource[EShaderResource::ConstantBuffer];
		uint32 uiResourceCount = pVecBuffer->GetSize();
		for(uint32 uiResource = 0; uiResource < uiResourceCount; ++uiResource)
		{
			Renderer::Buffer* pResource = static_cast<Renderer::Buffer*>(pVecBuffer->Get(uiResource));
			pRenderer->SetPixelShaderConstantBuffer(_pContext, uiResource, pResource);
		}

		// Resource
		pVecBuffer = &m_aShaderResource[EShaderResource::Resource];
		uiResourceCount = pVecBuffer->GetSize();
		for(uint32 uiResource = 0; uiResource < uiResourceCount; ++uiResource)
		{
			Renderer::Resource* pResource = pVecBuffer->Get(uiResource);
			if(pResource)
			{
				switch (pResource->GetType())
				{
					case Renderer::EResource::Buffer:
					{
						Renderer::Buffer* pBuffer = static_cast<Renderer::Buffer*>(pResource);
						pRenderer->SetPixelShaderResource(_pContext, uiResource, pBuffer);
						break;
					}

					case Renderer::EResource::Texture2D:
					{
						Renderer::Texture2D* pTexture = static_cast<Renderer::Texture2D*>(pResource);
						pRenderer->SetPixelShaderResource(_pContext, uiResource, pTexture);
						break;
					}

					case Renderer::EResource::Texture3D:
					{
						Renderer::Texture3D* pTexture = static_cast<Renderer::Texture3D*>(pResource);
						pRenderer->SetPixelShaderResource(_pContext, uiResource, pTexture);
						break;
					}
				}			
			}
		}

		// Draw
		pRenderer->SetVertexBuffer(_pContext, m_pQuad->GetVertexBuffer());
		pRenderer->SetIndexBuffer(_pContext, m_pQuad->GetIndexBuffer());
		pRenderer->Draw(_pContext, 0, m_pQuad->GetIndexCount());
	}

	//!	@brief		Execute 
	//!	@date		2015-11-11
	void RenderFilter::Execute2(Renderer::RenderContext* _pContext)
	{
		Renderer::Renderer* pRenderer = _pContext->GetRenderer();
		Renderer::Texture2D* pRenderTexture = m_pRenderTarget->GetTarget(0);

		// Set RenderState
		Renderer::Effect* pEffect = m_hEffect->GetData();
		pRenderer->SetRenderTarget(_pContext, m_pRenderTarget);
		pRenderer->SetViewport(_pContext, 0, 0, pRenderTexture->GetWidth(), pRenderTexture->GetHeight());

		// Set Vertex & Pixel shaders
		Renderer::Shader* pVertexShader = pEffect->GetShader(Renderer::EShader::Vertex);
		pRenderer->SetVertexShader(_pContext, pVertexShader);
		pRenderer->SetPixelShader(_pContext, m_hPixelShader->GetData());

		// ConstantBuffer
		Vector<Renderer::Resource*>* pVecBuffer = &m_aShaderResource[EShaderResource::ConstantBuffer];
		uint32 uiResourceCount = pVecBuffer->GetSize();
		for(uint32 uiResource = 0; uiResource < uiResourceCount; ++uiResource)
		{
			Renderer::Buffer* pResource = static_cast<Renderer::Buffer*>(pVecBuffer->Get(uiResource));
			pRenderer->SetPixelShaderConstantBuffer(_pContext, uiResource, pResource);
		}

		// Resource
		pVecBuffer = &m_aShaderResource[EShaderResource::Resource];
		uiResourceCount = pVecBuffer->GetSize();
		for(uint32 uiResource = 0; uiResource < uiResourceCount; ++uiResource)
		{
			Renderer::Resource* pResource = pVecBuffer->Get(uiResource);
			if(pResource)
			{
				switch (pResource->GetType())
				{
					case Renderer::EResource::Buffer:
					{
						Renderer::Buffer* pBuffer = static_cast<Renderer::Buffer*>(pResource);
						pRenderer->SetPixelShaderResource(_pContext, uiResource, pBuffer);
						break;
					}

					case Renderer::EResource::Texture2D:
					{
						Renderer::Texture2D* pTexture = static_cast<Renderer::Texture2D*>(pResource);
						pRenderer->SetPixelShaderResource(_pContext, uiResource, pTexture);
						break;
					}

					case Renderer::EResource::Texture3D:
					{
						Renderer::Texture3D* pTexture = static_cast<Renderer::Texture3D*>(pResource);
						pRenderer->SetPixelShaderResource(_pContext, uiResource, pTexture);
						break;
					}
				}			
			}
		}

		// Draw
		pRenderer->SetVertexBuffer(_pContext, m_pQuad->GetVertexBuffer());
		pRenderer->SetIndexBuffer(_pContext, m_pQuad->GetIndexBuffer());
		pRenderer->Draw(_pContext, 0, m_pQuad->GetIndexCount());
	}

	//!	@brief		Execute 
	//!	@date		2015-11-11
	void RenderFilter::Execute(Renderer::RenderContext* _pContext)
	{
		Renderer::Renderer* pRenderer = _pContext->GetRenderer();
		Renderer::Texture2D* pRenderTexture = m_pRenderTarget->GetTarget(0);

		// Set RenderState
		Renderer::Effect* pEffect = m_hEffect->GetData();
		pRenderer->SetRenderTarget(_pContext, m_pRenderTarget);
		pRenderer->SetViewport(_pContext, 0, 0, pRenderTexture->GetWidth(), pRenderTexture->GetHeight());
		pRenderer->SetTopology(_pContext, pEffect->GetTopology());
		pRenderer->SetRenderState(_pContext, pEffect->GetRenderState());

		// Set Vertex & Pixel shaders
		Renderer::Shader* pVertexShader = pEffect->GetShader(Renderer::EShader::Vertex);
		pRenderer->SetVertexShader(_pContext, pVertexShader);

		while(m_hPixelShader->GetData() == nullptr) Thread::Sleep(1);
		pRenderer->SetPixelShader(_pContext, m_hPixelShader->GetData());

		// ConstantBuffer
		Vector<Renderer::Resource*>* pVecBuffer = &m_aShaderResource[EShaderResource::ConstantBuffer];
		uint32 uiResourceCount = pVecBuffer->GetSize();
		for(uint32 uiResource = 0; uiResource < uiResourceCount; ++uiResource)
		{
			Renderer::Buffer* pResource = static_cast<Renderer::Buffer*>(pVecBuffer->Get(uiResource));
			pRenderer->SetPixelShaderConstantBuffer(_pContext, uiResource, pResource);
		}

		// Resource
		pVecBuffer = &m_aShaderResource[EShaderResource::Resource];
		uiResourceCount = pVecBuffer->GetSize();
		for(uint32 uiResource = 0; uiResource < uiResourceCount; ++uiResource)
		{
			Renderer::Resource* pResource = pVecBuffer->Get(uiResource);
			if(pResource)
			{
				switch (pResource->GetType())
				{
					case Renderer::EResource::Buffer:
					{
						Renderer::Buffer* pBuffer = static_cast<Renderer::Buffer*>(pResource);
						pRenderer->SetPixelShaderResource(_pContext, uiResource, pBuffer);
						break;
					}

					case Renderer::EResource::Texture2D:
					{
						Renderer::Texture2D* pTexture = static_cast<Renderer::Texture2D*>(pResource);
						pRenderer->SetPixelShaderResource(_pContext, uiResource, pTexture);
						break;
					}

					case Renderer::EResource::Texture3D:
					{
						Renderer::Texture3D* pTexture = static_cast<Renderer::Texture3D*>(pResource);
						pRenderer->SetPixelShaderResource(_pContext, uiResource, pTexture);
						break;
					}
				}			
			}
		}

		// Draw
		pRenderer->SetVertexBuffer(_pContext, m_pQuad->GetVertexBuffer());
		pRenderer->SetIndexBuffer(_pContext, m_pQuad->GetIndexBuffer());
		pRenderer->Draw(_pContext, 0, m_pQuad->GetIndexCount());
	}
}}