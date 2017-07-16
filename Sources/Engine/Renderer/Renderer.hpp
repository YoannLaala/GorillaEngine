#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "Common.hpp"
#include <Core/Thread/Thread.hpp>
#include <Core/Thread/Mutex.hpp>
#include <Core/Thread/ConditionVariable.hpp>
#include <Core/Signal.hpp>
#include <Renderer/Color.hpp>
#include <Renderer/RenderBuffer.hpp>
#include <Renderer/Primitive.hpp>
#include <Renderer/Resource/RenderState.hpp>
#include <Renderer/Resource/Shader.hpp>
#include <Renderer/Helper/Gizmo.hpp>
#include <Renderer/Helper/MarkerGPU.hpp>

/******************************************************************************
**	Defines
******************************************************************************/
#define MARKER_GPU(_context_, _name_) Gorilla::Renderer::MarkerGPU kMarkerGPU(_context_, _name_);

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla 
{
	class Window;
}

namespace Gorilla { namespace Renderer
{
	class RenderContext;
	class SwapChain;
	class Texture2D;
	class Texture3D;

	class Material;
	class Geometry;
	class Buffer;

	class Viewport;
	class RenderTarget;
	class SamplerState;
	class Effect;
	class Shader;

	class Scene;

	class IRenderable;

	class RenderCommandList;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	/******************************************************************************
	**	Renderer
	******************************************************************************/
	class Renderer
	{
		friend class CanvasRenderer;
		friend class Viewport;

		struct EThread
		{
			enum Type
			{
				Prepare = 0,
				Render,

				Count
			};
		};

	public:
		Renderer();
		~Renderer();

		inline static Renderer*						GetInstance						() { return m_pInstance; }

		bool										Initialize						();
		void										Release							();

		inline Gizmo*								GetGizmo						() { return m_pGizmo; }

		void										Display							();
		void										Display							(RenderCommandList* pCommandList);

		inline RenderBuffer*						GetFrameBuffer					() { return Thread::IsMainThread() ? &m_aFrameBuffer[GetFrameIndex(EThread::Prepare)] : &m_aFrameBuffer[GetFrameIndex(EThread::Render)]; }
		inline RenderBuffer*						GetGlobalBuffer					() { return &m_kGlobalBuffer; }

		// Marker
		void*										PushMarker						(RenderContext* _pContext, const char* _szName);
		void										PopMarker						(RenderContext* _pContext, void* _pMarker);

		// Creation / Destruction
		Scene*										CreateScene						();
		void										DestroyScene					(Scene* _pScene);

		inline Viewport*							GetViewport						(uint32 _uiIndex) { return m_vViewport[_uiIndex]; }
		inline uint32								GetViewportCount				() const { return m_vViewport.GetSize(); }
		Viewport*									CreateViewport					(Window* _pWindow, EFormat::Type _eFormat, uint8 _eFilter = 0);
		Viewport*									CreateViewport					(uint32 _uiWidth, uint32 _uiHeight, EFormat::Type _eFormat, uint8 _eFilter = 0);
		void										DestroyViewport					(Viewport* _pViewport);

		RenderTarget*								CreateRenderTarget				();

		Texture2D*									CreateTextureCube				(uint32 _uiWidth, uint32 _uiHeight, uint32 _uiMipCount, EFormat::Type _eFormat, uint32 _eBindFlag, EUsage::Type _eUsage = EUsage::Default, const void* _pData = NULL);
		Texture2D*									CreateTexture2D					(uint32 _uiWidth, uint32 _uiHeight, uint32 _uiMipCount, EFormat::Type _eFormat, uint32 _eBindFlag, EUsage::Type _eUsage = EUsage::Default, const void* _pData = NULL);
		Texture2D*									CreateTexture2D					(uint32 _uiWidth, uint32 _uiHeight, uint32 _uiMipCount, uint32 _uiCount, EFormat::Type _eFormat, uint32 _eBindFlag, EUsage::Type _eUsage, const void* _pData, uint32 _eFlag);
		Texture3D*									CreateTexture3D					(uint32 _uiWidth, uint32 _uiHeight, uint32 _uiDepth, uint32 _uiMipCount, EFormat::Type _eFormat, uint32 _eBindFlag, EUsage::Type _eUsage = EUsage::Default, const void* _pData = NULL);

		Material*									CreateMaterial					(Shader* _pPixelShader, float32* _pNumber = NULL, uint32 _uiNumberSize = 0);
		Geometry*									CreateGeometry					(const void* _pVertexBuffer, uint32 _uiVertexSize, uint32 _uiVertexCount, const uint32* _pIndexBuffer, uint32 _uiIndexCount, const uint32* _pSectionBuffer = NULL, uint32 _uiSectionCount = 0);

		Buffer*										CreateConstantBuffer			(uint32 _uiElementSize, uint32 _uiElementCount, const void* _pData = NULL);
		Buffer*										CreateShaderResourceBuffer		(uint32 _uiElementSize, uint32 _uiElementCount, uint32 _uiBindFlag, EUsage::Type _eUsage = EUsage::Default, const void* _pData = NULL);
		Buffer*										CreateIndirectBuffer			(uint32 _uiElementSize, uint32 _uiElementCount, const void* _pData = NULL);
		Buffer*										CreateBuffer					(uint32 _uiElementSize, uint32 _uiElementCount, uint32 _eBindFlag, EUsage::Type _eUsage, uint32 _uiCpuFag, uint32 _uiMiscFlag, const void* _pData);

		RenderState*								CreateRenderState				(RenderState::Key _kKey);
		SamplerState*								CreateSamplerState				(ESampler::Type _eSampler, EAddressingMode::Type _eU, EAddressingMode::Type _eV, EAddressingMode::Type _eW, EDepthComparaison::Type _eComparaison = EDepthComparaison::Never);
		Effect*										CreateEffect					(RenderState::Key _kKey, ETopology::Type _eTopology);
		Shader*										CreateVertexShader				(uint8* _pBuffer, uint32 _uiSize, Shader::Parameter* _pParameterArray, uint32 _uiParameterCount);
		Shader*										CreateGeometryShader			(uint8* _pBuffer, uint32 _uiSize);
		Shader*										CreatePixelShader				(uint8* _pBuffer, uint32 _uiSize);
		Shader*										CreateComputeShader				(uint8* _pBuffer, uint32 _uiSize);

		RenderCommandList*							CreateCommandList				(RenderContext* _pContext);

		// Update
		void										CopyTexture						(RenderContext* _pContext, Texture2D* _pSource, Texture2D* _pDestination, uint32 _uiOffsetX = 0, uint32 _uiOffsetY = 0);
		void										CopyRegion						(RenderContext* _pContext, Texture2D* _pTexture, uint32 _uiLeft, uint32 _uiTop, uint32 _uiWidth, uint32 _uiHeight, const void* _pBuffer);
		uint8*										Map								(RenderContext* _pContext, Buffer* _pBuffer, EMap::Type _eMap = EMap::WriteDiscard);
		void										Unmap							(RenderContext* _pContext, Buffer* _pBuffer);
		void										Synchronize						(RenderContext* _pContext, Material* _pMaterial);
		void										UpdateMipMap					(RenderContext* _pContext, Texture3D* _pTexture);

		// Context Preparation / Draw
		void										SetRenderTarget					(RenderContext* _pContext, RenderTarget* _pRenderTarget);
		void										SetViewport						(RenderContext* _pContext, uint32 _uiOffsetX, uint32 _uiOffsetY, uint32 _uiWidth, uint32 _uiHeight);
		void										SetBlendState					(RenderContext* _pContext, EBlend::Type _eSource, EBlendOperation::Type _eOperation, EBlend::Type _eDestination);
		void										SetRenderState					(RenderContext* _pContext, RenderState* _pRenderState);
		void										SetTopology						(RenderContext* _pContext, ETopology::Type _eTopology);
		void										SetVertexBuffer					(RenderContext* _pContext, Buffer* _pVertexBuffer);
		void										SetIndexBuffer					(RenderContext* _pContext, Buffer* _pIndexBuffer);

		void										SetVertexShader					(RenderContext* _pContext, Shader* _pShader);
		void										SetVertexShaderConstantBuffer	(RenderContext* _pContext, uint32 _uiIndex, Buffer* _pBuffer);
		void										SetVertexShaderResource			(RenderContext* _pContext, uint32 _uiIndex, Buffer* _pBuffer);
		void										SetVertexShaderResource			(RenderContext* _pContext, uint32 _uiIndex, Texture2D* _pTexture);
		void										SetVertexShaderSamplerState		(RenderContext* _pContext, uint32 _uiIndex, SamplerState* _pSamplerState);

		void										SetGeometryShader				(RenderContext* _pContext, Shader* _pShader);
		void										SetGeometryShaderConstantBuffer	(RenderContext* _pContext, uint32 _uiIndex, Buffer* _pBuffer);
		void										SetGeometryShaderResource		(RenderContext* _pContext, uint32 _uiIndex, Buffer* _pBuffer);
		void										SetGeometryShaderResource		(RenderContext* _pContext, uint32 _uiIndex, Texture2D* _pTexture);
		void										SetGeometryShaderSamplerState	(RenderContext* _pContext, uint32 _uiIndex, SamplerState* _pSamplerState);

		void										SetPixelShader					(RenderContext* _pContext, Shader* _pShader);
		void										SetPixelShaderConstantBuffer	(RenderContext* _pContext, uint32 _uiIndex, Buffer* _pBuffer);
		void										SetPixelShaderResource			(RenderContext* _pContext, uint32 _uiIndex, Buffer* _pBuffer);
		void										SetPixelShaderResource			(RenderContext* _pContext, uint32 _uiIndex, Texture2D* _pTexture);
		void										SetPixelShaderResource			(RenderContext* _pContext, uint32 _uiIndex, Texture3D* _pTexture);
		void										SetPixelShaderSamplerState		(RenderContext* _pContext, uint32 _uiIndex, SamplerState* _pSamplerState);

		void										SetComputeShader				(RenderContext* _pContext, Shader* _pShader);
		void										SetComputeShaderSamplerState	(RenderContext* _pContext, uint32 _uiIndex, SamplerState* _pSamplerState);
		void										SetComputeShaderConstantBuffer	(RenderContext* _pContext, uint32 _uiIndex, Buffer* _pBuffer);
		void										SetComputeShaderResource		(RenderContext* _pContext, uint32 _uiIndex, Buffer* _pBuffer);
		void										SetComputeShaderResource		(RenderContext* _pContext, uint32 _uiIndex, Texture2D* _pTexture);
		void										SetComputeShaderResource		(RenderContext* _pContext, uint32 _uiIndex, Texture3D* _pTexture);
		void										UnsetComputeShaderResource		(RenderContext* _pContext, uint32 _uiIndex, uint32 _uiCount);
		void										SetComputeShaderUnorderedAccess (RenderContext* _pContext, uint32 _uiIndex, Buffer* _pBuffer);
		void										SetComputeShaderUnorderedAccess (RenderContext* _pContext, uint32 _uiIndex, Texture3D* _pTexture);
		void										UnsetComputeShaderUnorderedAccess(RenderContext* _pContext, uint32 _uiIndex, uint32 _uiCount);

		void										Compute							(RenderContext* _pContext, uint32 _uiThreadGroupCountX, uint32 _uiThreadGroupCountY, uint32 _uiThreadGroupCountZ);
		void										Compute							(RenderContext* _pContext, Buffer* _pBuffer, uint32 _uiOffset = 0);

		void										Draw							(RenderContext* _pContext, uint32 _uiIndexOffset, uint32 _uiIndexCount, uint32 _uiInstanceCount = 1);
		void										Draw							(RenderContext* _pContext, RenderCommandList* pCommandList);
		void										Clear							(RenderContext* _pContext, RenderTarget* _pRenderTarget, uint32 _eBindFlag = (uint32)-1);

		void										DestroyResource					(Resource* _pResource);
		static void									DestroyHandle					(void* _pHandle);

		// Rendering
	private:
		void										Prepare							();
		static void									RenderThreadEntry				(void* _pData);
		void										Render							();

	private:
		Viewport*									CreateViewport					(SwapChain* _pSwapChain, uint8 _eFilter = 0);
		SwapChain*									CreateSwapChain					(Window* _pWindow, uint32 _uiWidth, uint32 _uiHeight, EFormat::Type _eFormat);

		void*										CreateShaderResourceView		(Texture2D* _pTexture, uint32 _uiMipCount, uint32 _uiCount, EFormat::Type _eFormat, uint32 _eFlag);
		void*										CreateShaderResourceView		(Texture3D* _pTexture, uint32 _uiMipCount, EFormat::Type _eFormat);
		void*										CreateShaderResourceView		(Buffer* _pBuffer, EFormat::Type _eFormat);
		void*										CreateRenderTargetView			(void* _pHandle);
		void*										CreateDepthStenciltView			(Texture2D* _pTexture);
		void*										CreateUnorderedAccessView		(Buffer* _pBuffer, EFormat::Type _eFormat);
		void*										CreateUnorderedAccessView		(Texture2D* _pTexture, EFormat::Type _eFormat);
		void*										CreateUnorderedAccessView		(Texture3D* _pTexture, EFormat::Type _eFormat);

		void										SetVertexShaderResource			(RenderContext* _pContext, uint32 _uiIndex, void* _pView);
		void										SetPixelShaderResource			(RenderContext* _pContext, uint32 _uiIndex, void* _pView);
		void										SetGeometryShaderResource		(RenderContext* _pContext, uint32 _uiIndex, void* _pView);
		
		// Frame related
		inline uint8								GetFrameDifference				() const { return m_aFrameId[EThread::Prepare] - m_aFrameId[EThread::Render]; }
		inline uint8								GetFrameIndex					(EThread::Type _eThread) const { return m_aFrameId[_eThread] % RENDERER_FRAME_COUNT; }

	private:
		inline void*			GetDevice			() { return m_pDevice; }

		struct DestroyRequest
		{
			uint32		Frame;
			Resource*	Resource;
		};		

	private:
		static Renderer*		m_pInstance;
		RenderBuffer			m_aFrameBuffer[RENDERER_FRAME_COUNT];
		RenderBuffer			m_kGlobalBuffer;
		Vector<Viewport*>		m_vViewport;
		Vector<Scene*>			m_vScene;
		Vector<DestroyRequest>	m_vDestroyRequest;
		Vector<void*>			m_vPointer;
		SamplerState*			m_aSampler[ESampler::Count];
		void*					m_pDevice;
		void*					m_pImmediateContext;
		Gizmo*					m_pGizmo;

				
		


		// Frame Index
		uint8					m_aFrameId[EThread::Count];
		Mutex					m_aMutex[EThread::Count];
		ConditionVariable		m_aCondition[EThread::Count];
		bool					m_bIsRunning;

	public:
		SIGNAL_SENDER(RenderContextCreated, RenderContext* /*_pContext*/, uint8 /*_eFilter */);
	};
}}

#endif