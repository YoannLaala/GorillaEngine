/******************************************************************************
**	Includes
******************************************************************************/
#include "RenderTarget.hpp"
#include <Renderer/Renderer.hpp>
#include <Renderer/Resource/Buffer.hpp>
#include <Renderer/Resource/Texture2D.hpp>
#include <Renderer/Resource/Texture3D.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	RenderTarget::RenderTarget()
		: m_pRenderer(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	RenderTarget::~RenderTarget()
	{
		// Nothing to do
	}

	//!	@brief		Initiliaze 
	//!	@date		2015-11-11
	void RenderTarget::Initiliaze(Renderer* _pRenderer)
	{
		m_pRenderer = _pRenderer;
		memset(m_aResource, 0, sizeof(void*) * EEntry::Count);
		memset(m_aView, 0, sizeof(void*) * EEntry::Count);
	}
	
	//!	@brief		Initiliaze 
	//!	@date		2015-11-11
	void RenderTarget::Release()
	{
		// Nothing to do
	}

	//!	@brief		AddTarget 
	//!	@date		2015-11-11
	void RenderTarget::AddTarget(Texture2D* _pTexture)
	{
		void* pView = _pTexture->GetView(Texture2D::EView::RenderTarget);
		m_aResource[EEntry::Target].Add(_pTexture);
		m_aView[EEntry::Target].Add(pView);
	}

	//!	@brief		SetDepthStencil 
	//!	@date		2015-11-11
	void RenderTarget::SetDepthStencil(Texture2D* _pTexture)
	{
		void* pView = _pTexture->GetView(Texture2D::EView::DepthStencil);
		m_aResource[EEntry::DepthStencil].Add(_pTexture);
		m_aView[EEntry::DepthStencil].Add(pView);
	}

	//!	@brief		AddBuffer 
	//!	@date		2015-11-11
	void RenderTarget::AddOutput(Buffer* _pBuffer)
	{
		void* pView = _pBuffer->GetView(Buffer::EView::UnorderedAccess);
		m_aResource[EEntry::Output].Add(_pBuffer);
		m_aView[EEntry::Output].Add(pView);
	}

	//!	@brief		AddBuffer 
	//!	@date		2015-11-11
	void RenderTarget::AddOutput(Texture2D* _pTexture)
	{
		void* pView = _pTexture->GetView(Texture2D::EView::UnorderedAccess);
		m_aResource[EEntry::Output].Add(_pTexture);
		m_aView[EEntry::Output].Add(pView);
	}

	//!	@brief		AddBuffer 
	//!	@date		2015-11-11
	void RenderTarget::AddOutput(Texture3D* _pTexture)
	{
		void* pView = _pTexture->GetView(Texture3D::EView::UnorderedAccess);
		m_aResource[EEntry::Output].Add(_pTexture);
		m_aView[EEntry::Output].Add(pView);
	}

	//!	@brief		Clear 
	//!	@date		2015-11-11
	void RenderTarget::Clear()
	{
		for(uint8 eResource = 0; eResource < EEntry::Count; ++eResource)
		{
			m_aResource[eResource].Clear();
			m_aView[eResource].Clear();
		}
	}
}}