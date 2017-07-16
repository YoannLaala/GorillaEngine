#ifndef _RENDERER_RENDER_CONTEXT_HPP_
#define _RENDERER_RENDER_CONTEXT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Hash.hpp>
#include <Renderer/Common.hpp>
#include <Renderer/RenderBuffer.hpp>
#include <Renderer/Resource/Buffer.hpp>
#include <Renderer/Resource/Texture2D.hpp>
#include <Renderer/Resource/Texture3D.hpp>
#include <Renderer/Resource/RenderTarget.hpp>
#include <Renderer/Resource/Geometry.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Renderer;
	class RenderPass;
	class Viewport;
	class Camera;
	class Octree;
	class RenderCommandList;
	class IRenderable;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class RenderContext
	{
		friend class Renderer;
		friend class Viewport;

		struct RenderFrame
		{
			RenderBuffer		Buffer;
			RenderCommandList*	CommandList;
		};

	private:
		RenderContext	();
		~RenderContext	();

		void				Initialize			(Renderer* _pRenderer, void* _pHandle, Viewport* pViewport);
		void				Release				();

		void				Prepare				(uint8 _uiFrameIndex, IRenderable* _pRenderable);
		void				Prepare				(uint8 _uiFrameIndex, const Camera* _pCamera, const Octree* _pTree);
		void				Render				(uint8 _uiFrameIndex);
		void				Display				(uint8 _uiFrameIndex);
		void				Resize				(Texture2D* _pRenderTexture);

		inline void*		GetHandle() { return m_pHandle; }

	public:
		inline Renderer*		GetRenderer() { return m_pRenderer; }
		inline Viewport*		GetViewport() { return m_pViewport; }

		template <class T>
		inline T* CreatePass() { return CreatePassInternal<T>((uint32)-1, 0); }

		template <class T>
		inline T* CreatePassBefore(uint32 _uiPassId) { return CreatePassInternal<T>(_uiPassId, (uint32)-1); }

		template <class T>
		inline T* CreatePassAfter(uint32 _uiPassId) { return CreatePassInternal<T>(_uiPassId, (uint32)+1); }

		template <class T>
		inline void ActivatePass();

		template <class T>
		inline void DeactivatePass();

		// Creation / Destruction
		RenderTarget*		CreateRenderTarget		(const char* _szName);
		RenderTarget*		GetRenderTarget			(const char* _szName) { return GetResource<RenderTarget>(_szName); }

		Texture2D*			CreateTexture2D			(const char* _szName, uint32 _uiWidth, uint32 _uiHeigth, uint32 _uiMipCount, EFormat::Type _eFormat, uint32 _eBindFlag, EUsage::Type _eUsage = EUsage::Default, void * _pData = NULL);
		Texture2D*			GetTexture2D			(const char* _szName) { return GetResource<Texture2D>(_szName); }

		Texture3D*			CreateTexture3D			(const char* _szName, uint32 _uiWidth, uint32 _uiHeigth, uint32 _uiDepth, uint32 _uiMipCount, EFormat::Type _eFormat, uint32 _eBindFlag, EUsage::Type _eUsage = EUsage::Default, void* _pData = NULL);
		Texture3D*			GetTexture3D			(const char* _szName) { return GetResource<Texture3D>(_szName); }

		Buffer*				CreateConstantBuffer	(const char* _szName, uint32 _uiElementSize, uint32 _uiElementCount, const void* _pData = NULL);
		Buffer*				CreateShaderResourceBuffer(const char* _szName, uint32 _uiElementSize, uint32 _uiElementCount, uint32 _uiBindFlag, EUsage::Type _eUsage = EUsage::Default, const void* _pData = NULL);
		Buffer*				GetBuffer				(const char* _szName) { return GetResource<Buffer>(_szName); }

		Geometry*			CreateGeometry			(const char* _szName, const void* _pVertexBuffer, uint32 _uiVertexSize, uint32 _uiVertexCount, const uint32* _pIndexBuffer, uint32 _uiIndexCount, const uint32* _pSectionBuffer = NULL, uint32 _uiSectionCount = 0);
		Geometry*			GetGeometry				(const char* _szName) { return GetResource<Geometry>(_szName); }

		void				DestroyResource		(Resource* _pResource);

	private:
		template <class T>
		void			AddResource			(const char* _szName, T* _pResource) { return AddResource<T>(Hash::Generate(_szName), _pResource); }

		template <class T>
		void			AddResource			(uint32 _uiResourceId, T* _pResource);

		template <class T>
		inline T*		GetResource			(const char* _szName) { return GetResource<T>(Hash::Generate(_szName)); }

		template <class T>
		T*				GetResource			(uint32 _uiResourceId);

		template <class T>
		T*				CreatePassInternal(uint32 _uiPassId, uint32 uiOffset);

	private:
		HashMap<uint32, Resource*>	m_aResource[EResource::Count];
		Vector<RenderPass*>			m_vRenderPass;
		RenderFrame					m_aFrame[RENDERER_FRAME_COUNT];
		Renderer*					m_pRenderer;
		void*						m_pHandle;
		Viewport*					m_pViewport;
	};

	//!	@brief		CreatePass
	//!	@date		2015-11-07
	template <class T>
	T* RenderContext::CreatePassInternal(uint32 _uiPassId, uint32 _uiOffset) 
	{ 
		T* pRenderPass = new T();
		pRenderPass->Initialize(this);

		uint32 uiIndex = m_vRenderPass.GetSize();
		if(_uiPassId != (uint32)-1)
		{
			const uint32 uiRenderPassCount = m_vRenderPass.GetSize();
			for(uint32 uiRenderPass = 0; uiRenderPass < uiRenderPassCount; ++uiRenderPass)
			{
				RenderPass* pRenderPass  = m_vRenderPass[uiRenderPass];
				if(pRenderPass->GetType() == _uiPassId)
				{
					uiIndex = static_cast<uint32>(Math::Clamp<int32>(uiRenderPass + _uiOffset, 0, uiIndex));
					break;
				}
			}	
		}
		m_vRenderPass.Insert(uiIndex, pRenderPass);

		return pRenderPass;
	}

	//!	@brief		ActivatePass
	//!	@date		2015-11-07
	template <class T>
	void RenderContext::ActivatePass() 
	{ 
		const uint32 uiRenderPassCount = m_vRenderPass.GetSize();
		for(uint32 uiRenderPass = 0; uiRenderPass < uiRenderPassCount; ++uiRenderPass)
		{
			RenderPass* pRenderPass  = m_vRenderPass[uiRenderPass];
			if(pRenderPass->GetType() == T::Class::GetType())
			{
				pRenderPass->Activate();
			}
		}
	}

	//!	@brief		DeactivatePass
	//!	@date		2015-11-07
	template <class T>
	void RenderContext::DeactivatePass() 
	{ 
		const uint32 uiRenderPassCount = m_vRenderPass.GetSize();
		for(uint32 uiRenderPass = 0; uiRenderPass < uiRenderPassCount; ++uiRenderPass)
		{
			RenderPass* pRenderPass  = m_vRenderPass[uiRenderPass];
			if(pRenderPass->GetType() == T::Class::GetType())
			{
				pRenderPass->Deactivate();
			}
		}
	}

	//!	@brief		AddResource 
	//!	@date		2015-11-11
	template <class T>
	void RenderContext::AddResource(uint32 _uiResourceId, T* _pResource)
	{
		_pResource->SetId(_uiResourceId);
		HashMap<uint32, Resource*>& mResource = m_aResource[T::Class::GetType()];
		mResource.Add(_uiResourceId, _pResource);
	}

	//!	@brief		GetResource 
	//!	@date		2015-11-11
	template <class T>
	T* RenderContext::GetResource(uint32 _uiResourceId)
	{
		HashMap<uint32, Resource*>& mResource = m_aResource[T::Class::GetType()];
		T* pResource = static_cast<T*>(mResource[_uiResourceId]);
		if(pResource)
		{
			pResource->AddReference();
		}

		return pResource;
	}
}}

#endif