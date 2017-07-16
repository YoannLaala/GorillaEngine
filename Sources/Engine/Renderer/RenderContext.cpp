/******************************************************************************
**	Includes
******************************************************************************/
#include "RenderContext.hpp"

#include <Core/Time/Profiler.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/RenderPass.hpp>
#include <Renderer/Resource/RenderTarget.hpp>
#include <Renderer/Resource/SamplerState.hpp>
#include <Renderer/IRenderable.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	RenderContext::RenderContext()
		: m_pRenderer(NULL)
		, m_pHandle(NULL)
		, m_pViewport(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	RenderContext::~RenderContext()
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	void RenderContext::Initialize(Renderer* _pRenderer, void* _pHandle, Viewport* _pViewport)
	{
		m_pRenderer = _pRenderer;
		m_pHandle = _pHandle;
		m_pViewport = _pViewport;

		// Create Command list for each frame
		for(uint32 uiFrame = 0; uiFrame < RENDERER_FRAME_COUNT; ++uiFrame)
		{
			m_aFrame[uiFrame].CommandList = _pRenderer->CreateCommandList(this);
		}
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void RenderContext::Release()
	{
		m_pRenderer = NULL;
	}

	//!	@brief		Prepare 
	//!	@details	Prepare Renreable object camera independent
	//!	@date		2015-11-11
	void RenderContext::Prepare(uint8 _uiFrameIndex, IRenderable* _pRenderable)
	{ 
		RenderBuffer* pBuffer = &m_aFrame[_uiFrameIndex].Buffer;
		_pRenderable->PushCommand(pBuffer);
	}

	//!	@brief		Prepare 
	//!	@details	Prepare Renreable object camera dependent
	//!	@date		2015-11-11
	void RenderContext::Prepare(uint8 _uiFrameIndex, const Camera* _pCamera, const Octree* _pTree)
	{ 
		RenderBuffer* pBuffer = &m_aFrame[_uiFrameIndex].Buffer;

		// Prepare all RenderPass available
		const uint32 uiRenderPassCount = m_vRenderPass.GetSize();
		for(uint32 uiRenderPass = 0; uiRenderPass < uiRenderPassCount; ++uiRenderPass)
		{
			// Execute the RenderPass
			RenderPass* pRenderPass = m_vRenderPass[uiRenderPass];
			if(pRenderPass->IsActivated())
			{
				PROFILE_SCOPE(pRenderPass->GetName());
				pRenderPass->Prepare(m_pRenderer, this, _pCamera, _pTree, pBuffer);
			}
		}
	}

	//!	@brief		Render 
	//!	@date		2015-11-11
	void RenderContext::Render(uint8 _uiFrameIndex) 
	{ 
		// Retrieve Frame Index
		RenderFrame& kFrame = m_aFrame[_uiFrameIndex];
		
		// Execute all RenderPass available
		const uint32 uiRenderPassCount = m_vRenderPass.GetSize();
		for(uint32 uiRenderPass = 0; uiRenderPass < uiRenderPassCount; ++uiRenderPass)
		{
			// Execute the RenderPass
			RenderPass* pRenderPass = m_vRenderPass[uiRenderPass];
			if(pRenderPass && pRenderPass->IsActivated())
			{
				MARKER_GPU(this, pRenderPass->GetName());
				//PROFILE_SCOPE(pRenderPass->GetName());
				pRenderPass->Execute(m_pRenderer, this, &kFrame.Buffer);
			}
		}
		kFrame.Buffer.Clear();

		// Draw all registered command
		RenderCommandList* pCommandList = kFrame.CommandList;
		m_pRenderer->Draw(this, pCommandList);
	}

	//!	@brief		Display 
	//!	@date		2015-11-11
	void RenderContext::Display(uint8 _uiFrameIndex)
	{
		RenderCommandList* pCommandList = m_aFrame[_uiFrameIndex].CommandList;
		m_pRenderer->Display(pCommandList);
	}

	//!	@brief		Resize 
	//!	@date		2015-11-11
	void RenderContext::Resize(Texture2D* /*_pRenderTexture*/)
	{
		// Execute all RenderPass available
		const uint32 uiRenderPassCount = m_vRenderPass.GetSize();
		for(uint32 uiRenderPass = 0; uiRenderPass < uiRenderPassCount; ++uiRenderPass)
		{
			// Execute the RenderPass
			RenderPass* pRenderPass = m_vRenderPass[uiRenderPass];
			if(pRenderPass)
			{
				pRenderPass->Release(this);
				pRenderPass->Initialize(this);
			}
		}
	}

	//!	@brief		CreateRenderTarget 
	//!	@date		2015-11-11
	RenderTarget* RenderContext::CreateRenderTarget(const char* _szName)
	{
		RenderTarget* pRenderTarget = m_pRenderer->CreateRenderTarget();
		if(pRenderTarget)
		{
			AddResource(_szName, pRenderTarget);
		}

		return pRenderTarget;
	}

	//!	@brief		CreateTexture2D 
	//!	@date		2015-11-11
	Texture2D* RenderContext::CreateTexture2D(const char* _szName, uint32 _uiWidth, uint32 _uiHeigth, uint32 _uiMipCount, EFormat::Type _eFormat, uint32 _eBindFlag, EUsage::Type _eUsage /*= EUsage::Default*/, void* _pData /*= NULL*/)
	{
		Texture2D* pTexture = m_pRenderer->CreateTexture2D(_uiWidth, _uiHeigth, _uiMipCount, _eFormat, _eBindFlag, _eUsage, _pData);
		if(pTexture)
		{
			AddResource(_szName, pTexture);
		}

		return pTexture;
	}

	//!	@brief		CreateTexture3D 
	//!	@date		2015-11-11
	Texture3D* RenderContext::CreateTexture3D(const char* _szName, uint32 _uiWidth, uint32 _uiHeigth, uint32 _uiDepth, uint32 _uiMipCount, EFormat::Type _eFormat, uint32 _eBindFlag, EUsage::Type _eUsage /*= EUsage::Default*/, void* _pData /*= NULL*/)
	{
		Texture3D* pTexture = m_pRenderer->CreateTexture3D(_uiWidth, _uiHeigth, _uiDepth, _uiMipCount, _eFormat, _eBindFlag, _eUsage, _pData);
		if(pTexture)
		{
			AddResource(_szName, pTexture);
		}

		return pTexture;
	}

	//!	@brief		CreateConstantBuffer 
	//!	@date		2015-11-21
	Buffer*	RenderContext::CreateConstantBuffer(const char* _szName, uint32 _uiElementSize, uint32 _uiElementCount, const void* _pData /*= NULL*/)
	{
		Buffer* pBuffer = m_pRenderer->CreateConstantBuffer(_uiElementSize, _uiElementCount, _pData);
		if(pBuffer)
		{
			AddResource(_szName, pBuffer);
		}

		return pBuffer;
	}

	//!	@brief		CreateShaderResourceBuffer 
	//!	@date		2015-11-21
	Buffer*	RenderContext::CreateShaderResourceBuffer(const char* _szName, uint32 _uiElementSize, uint32 _uiElementCount, uint32 _uiBindFlag, EUsage::Type _eUsage /*= EUsage::Default*/, const void* _pData /*= NULL*/)
	{
		Buffer* pBuffer = m_pRenderer->CreateShaderResourceBuffer(_uiElementSize, _uiElementCount, _uiBindFlag, _eUsage, _pData);
		if(pBuffer)
		{
			AddResource(_szName, pBuffer);
		}

		return pBuffer;
	}

	//!	@brief		CreateGeometry 
	//!	@date		2015-11-21
	Geometry* RenderContext::CreateGeometry(const char* _szName, const void* _pVertexBuffer, uint32 _uiVertexSize, uint32 _uiVertexCount, const uint32* _pIndexBuffer, uint32 _uiIndexCount, const uint32* _pSectionBuffer /*= NULL*/, uint32 _uiSectionCount /*= 0*/)
	{
		Geometry* pGeometry = m_pRenderer->CreateGeometry(_pVertexBuffer, _uiVertexSize, _uiVertexCount, _pIndexBuffer, _uiIndexCount, _pSectionBuffer, _uiSectionCount);
		if(pGeometry)
		{
			AddResource(_szName, pGeometry);
		}

		return pGeometry;
	}

	//!	@brief		DestroyResource 
	//!	@date		2015-11-11
	void RenderContext::DestroyResource(Resource* _pResource)
	{
		_pResource->RemoveReference();
			
		// Release the resource
		if(!_pResource->HasReference())
		{
			//HashMap<uint32, Resource*>& mResource = m_aResource[T::GetType()];
			//mResource.Remove(_pResource->GetId());
			SAFE_RELEASE_AND_DELETE(_pResource);
		}
	}	
}}