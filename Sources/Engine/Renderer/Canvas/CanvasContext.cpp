/******************************************************************************
**	Includes
******************************************************************************/
#include "CanvasContext.hpp"

#include <Core/Thread/Thread.hpp>
#include <Renderer/Canvas/CanvasRenderer.hpp>
#include <Renderer/Canvas/CanvasQueue.hpp>
#include <Renderer/Canvas/Resource/CanvasBrush.hpp>
#include <Renderer/Canvas/Resource/CanvasStyle.hpp>
#include <Renderer/Canvas/Resource/Texture/CanvasSurface.hpp>
#include <Renderer/Canvas/Resource/Texture/CanvasImage.hpp>
#include <Renderer/Canvas/Resource/CanvasGeometry.hpp>
#include <Renderer/Canvas/Resource/Text/CanvasFont.hpp>
#include <Renderer/Canvas/Resource/Text/CanvasText.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	CanvasContext::CanvasContext()
		: m_pRenderer(NULL)
		, m_pQueue(NULL)
		, m_bIsRunning(false)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	CanvasContext::~CanvasContext()
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	void CanvasContext::Initialize(CanvasRenderer* _pRenderer)
	{
		m_pRenderer = _pRenderer;
		m_pQueue = new CanvasQueue();
		Thread::Create("RenderThread", &RenderThreadEntry, this);
		m_bIsRunning = true;
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	void CanvasContext::Release()
	{
		m_bIsRunning = false;
		m_pRenderer = NULL;
	}

	//!	@brief		RenderEntry
	//!	@date		2016-03-26
	void CanvasContext::RenderThreadEntry(void* _pData)
	{
		CanvasContext* pContext = static_cast<CanvasContext*>(_pData);
		while(pContext->m_bIsRunning)
		{
			pContext->RenderLoop();
		}
	}

	//!	@brief		LoadingLoop
	//!	@date		2016-03-26
	void CanvasContext::RenderLoop()
	{
		m_pQueue->Execute(m_pRenderer);
	}

	//!	@brief		Resize 
	//!	@date		2015-11-11
	/*void CanvasContext::Resize(Texture2D* _pRenderTexture)
	{
		m_pRenderTexture = _pRenderTexture;

		const uint32 uiRenderPassCount = m_vRenderPass.GetSize();
		for(uint32 uiRenderPass = 0; uiRenderPass < uiRenderPassCount; ++uiRenderPass)
		{
			RenderPass* pRenderPass = m_vRenderPass[uiRenderPass];
			pRenderPass->Release(this);
			pRenderPass->Initialize(this);
		}
	}*/

	//!	@brief		CreateSurface 
	//!	@date		2015-11-11
	CanvasSurface* CanvasContext::CreateSurface(Texture2D* _pTexture)
	{
		return m_pRenderer->CreateSurface(_pTexture);
	}

	//!	@brief		CreateSurface 
	//!	@date		2015-11-11
	CanvasSurface* CanvasContext::CreateSurface(const char* _szName, Texture2D* _pTexture)
	{
		CanvasSurface* pSurface = m_pRenderer->CreateSurface(_pTexture);
		if(pSurface)
		{
			AddResource( _szName, pSurface);
		}

		return pSurface;
	}

	//!	@brief		CreateImage 
	//!	@date		2015-11-11
	CanvasImage* CanvasContext::CreateImage(const char* _szName, CanvasSurface* _pSurface, const char* szFilePath)
	{
		CanvasImage* pImage = m_pRenderer->CreateImage(_pSurface, szFilePath);
		if(pImage)
		{
			AddResource(_szName, pImage);
		}

		return pImage;
	}

	//!	@brief		CreateImage 
	//!	@date		2015-11-11
	CanvasImage* CanvasContext::CreateImage(const char* _szName, Texture2D* _pTexture)
	{
		CanvasImage* pImage = m_pRenderer->CreateImage(_pTexture);
		if(pImage)
		{
			AddResource(_szName, pImage);
		}

		return pImage;
	}

	//!	@brief		CreateBrush 
	//!	@date		2015-11-11
	CanvasBrush* CanvasContext::CreateBrush(CanvasSurface* _pSurface, Color _kColor)
	{
		CanvasBrush* pBrush = m_pRenderer->CreateBrush(_pSurface, _kColor);
		if(pBrush)
		{
			AddResource(_kColor, pBrush);
		}

		return pBrush;
	}

	//!	@brief		CreateStyle 
	//!	@date		2015-11-11
	CanvasStyle* CanvasContext::CreateStyle(CanvasSurface* _pSurface, ELineStyle::Type _eLine)
	{
		CanvasStyle* pStyle = m_pRenderer->CreateStyle(_pSurface, _eLine);
		if(pStyle)
		{
			AddResource(_eLine, pStyle);
		}

		return pStyle;
	}

	//!	@brief		CreateRecangle 
	//!	@date		2015-11-11
	CanvasGeometry* CanvasContext::CreateRectangle(const char* _szName, CanvasSurface* _pSurface, float32 _fWidth, float32 _fHeight, float32 _fRadiusX /*= 0.0f*/, float32 _fRadiusY/* = 0.0f*/)
	{
		CanvasGeometry* pRectangle = m_pRenderer->CreateRectangle(_pSurface, _fWidth, _fHeight, _fRadiusX, _fRadiusY);
		if(pRectangle)
		{
			AddResource(_szName, pRectangle);
		}

		return pRectangle;
	}

	//!	@brief		CreateElipse 
	//!	@date		2015-11-11
	CanvasGeometry* CanvasContext::CreateElipse(const char* _szName, CanvasSurface* _pSurface, float32 _fRadiusX, float32 _fRadiusY)
	{
		CanvasGeometry* pElipse = m_pRenderer->CreateElipse(_pSurface, _fRadiusX, _fRadiusY);
		if(pElipse)
		{
			AddResource(_szName, pElipse);
		}

		return pElipse;
	}

	//!	@brief		HasFont 
	//!	@date		2015-12-06
	bool CanvasContext::HasFont(const wchar_t* _szFamilyName) const
	{
		return m_pRenderer->HasFont(_szFamilyName);
	}

	//!	@brief		CreateFont 
	//!	@date		2015-11-11
	CanvasFont* CanvasContext::CreateFont(const char* _szName, const wchar_t* _szFamilyName, float32 _fSize, uint32 _uiWeight, EFontStyle::Type _eFont)
	{
		CanvasFont* pFont = m_pRenderer->CreateFont(_szFamilyName, _fSize, _uiWeight, _eFont);
		if(pFont)
		{
			AddResource(_szName, pFont);
		}
		
		return pFont;
	}

	//!	@brief		CreateText 
	//!	@date		2015-11-11
	CanvasText* CanvasContext::CreateText(const char* _szName, const wchar_t* _szText, float32 _fWidthMax, float32 _fHeightMax, CanvasFont* _pFont)
	{
		CanvasText* pText = m_pRenderer->CreateText(_szText, _fWidthMax, _fHeightMax, _pFont);
		if(pText)
		{
			AddResource(_szName, pText);
		}
		
		return pText;
	}

	//!	@brief		PushGeometry 
	//!	@date		2016-03-26
	void CanvasContext::PushGeometry(CanvasSurface* _pSurface, CanvasGeometry* _pGeometry, CanvasBrush* _pBrush, const Math::Matrix33& _mTransform)
	{
		CanvasCommandParameter<ECanvasCommand::Geometry>* pParameter = m_pQueue->PushCommand<ECanvasCommand::Geometry>(_pSurface, 0);
		pParameter->Geometry = _pGeometry;
		pParameter->Brush = _pBrush;
		pParameter->Transform = _mTransform;
	}

	//!	@brief		PushGeometryFilled 
	//!	@date		2016-03-26
	void CanvasContext::PushGeometryFilled(CanvasSurface* _pSurface, CanvasGeometry* _pGeometry, CanvasBrush* _pBrush, const Math::Matrix33& _mTransform)
	{
		CanvasCommandParameter<ECanvasCommand::GeometryFilled>* pParameter = m_pQueue->PushCommand<ECanvasCommand::GeometryFilled>(_pSurface, 0);
		pParameter->Geometry = _pGeometry;
		pParameter->Brush = _pBrush;
		pParameter->Transform = _mTransform;
	}

	//!	@brief		PushLine 
	//!	@date		2016-03-26
	void CanvasContext::PushLine(CanvasSurface* _pSurface, CanvasBrush* _pBrush, const Math::Vector2& _vPointA, const Math::Vector2& _vPointB, CanvasStyle* _pStyle /*= NULL*/, float32 _fWidth /*= 1.0f*/)
	{
		CanvasCommandParameter<ECanvasCommand::Line>* pParameter = m_pQueue->PushCommand<ECanvasCommand::Line>(_pSurface, 0);
		pParameter->Brush = _pBrush;
		pParameter->PointA = _vPointA;
		pParameter->PointB = _vPointB;
		pParameter->Style = _pStyle;
		pParameter->Width = _fWidth;
	}
	
	//!	@brief		ExecuteLine 
	//!	@date		2016-03-26
	void CanvasContext::ExecuteLine(CanvasRenderer* _pRenderer, CanvasSurface* _pSurface, void* _pParameter)
	{
		CanvasCommandParameter<ECanvasCommand::Line>* pParameter = static_cast<CanvasCommandParameter<ECanvasCommand::Line>*>(_pParameter);
		_pRenderer->DrawLine(_pSurface, pParameter->Brush, pParameter->PointA, pParameter->PointB, pParameter->Style, pParameter->Width);
	}

	//!	@brief		PushText 
	//!	@date		2016-03-26
	void CanvasContext::PushText(CanvasSurface* _pSurface, CanvasText* _pText, CanvasBrush* _pBrush, const Math::Matrix33& _mTransform)
	{
		CanvasCommandParameter<ECanvasCommand::Text>* pParameter = m_pQueue->PushCommand<ECanvasCommand::Text>(_pSurface, 0);
		pParameter->Text = _pText;
		pParameter->Brush = _pBrush;
		pParameter->Transform = _mTransform;
	}

	//!	@brief		ExecuteGeometry 
	//!	@date		2016-03-26
	void CanvasContext::ExecuteGeometry(CanvasRenderer* _pRenderer, CanvasSurface* _pSurface, void* _pParameter)
	{
		CanvasCommandParameter<ECanvasCommand::Geometry>* pParameter = static_cast<CanvasCommandParameter<ECanvasCommand::Geometry>*>(_pParameter);
		_pRenderer->DrawGeometry(_pSurface, pParameter->Geometry, pParameter->Brush, pParameter->Transform, 1.0f);
	}

	//!	@brief		ExecuteGeometryFilled
	//!	@date		2016-03-26
	void CanvasContext::ExecuteGeometryFilled(CanvasRenderer* _pRenderer, CanvasSurface* _pSurface, void* _pParameter)
	{
		CanvasCommandParameter<ECanvasCommand::Geometry>* pParameter = static_cast<CanvasCommandParameter<ECanvasCommand::Geometry>*>(_pParameter);
		_pRenderer->DrawGeometry(_pSurface, pParameter->Geometry, pParameter->Brush, pParameter->Transform);
	}

	//!	@brief		ExecuteText 
	//!	@date		2016-03-26
	void CanvasContext::ExecuteText(CanvasRenderer* _pRenderer, CanvasSurface* _pSurface, void* _pParameter)
	{
		CanvasCommandParameter<ECanvasCommand::Text>* pParameter = static_cast<CanvasCommandParameter<ECanvasCommand::Text>*>(_pParameter);
		_pRenderer->DrawText(_pSurface, pParameter->Text, pParameter->Brush, pParameter->Transform);
	}

	//!	@brief		AddResource 
	//!	@date		2015-11-11
	template <class T>
	void CanvasContext::AddResource(uint32 _uiResourceId, T* _pResource)
	{
		_pResource->SetId(_uiResourceId);
		HashMap<uint32, CanvasResource*>& mResource = m_aResource[T::GetType()];
		mResource.Add(_uiResourceId, _pResource);
	}

	//!	@brief		GetResource 
	//!	@date		2015-11-11
	template <class T>
	T* CanvasContext::GetResource(uint32 _uiResourceId)
	{
		HashMap<uint32, CanvasResource*>& mResource = m_aResource[T::GetType()];
		T* pResource = static_cast<T*>(mResource[_uiResourceId]);
		if(pResource)
		{
			pResource->AddReference();
		}

		return pResource;
	}

	//!	@brief		DestroyResource
	//!	@date		2015-11-11
	template <class T>
	void CanvasContext::DestroyResource(uint32 _uiResourceId)
	{
		HashMap<uint32, CanvasResource*>& mResource = m_aResource[T::GetType()];
		T* pResource = static_cast<T*>(mResource[_uiResourceId]);
		if(pResource)
		{
			DestroyResource(pResource);
		}
	}

	//!	@brief		DestroyResource
	//!	@date		2015-11-11
	template <class T>
	void CanvasContext::DestroyResource(T*& _pResource)
	{
		if(_pResource)
		{
			_pResource->RemoveReference();
			if(!_pResource->IsReferenced())
			{
				HashMap<uint32, CanvasResource*>& mResource = m_aResource[T::GetType()];
				mResource.Remove(_pResource->GetId());
				SAFE_RELEASE_AND_DELETE(_pResource);
			}
		}
	}

	// Get
	CanvasSurface*	CanvasContext::GetSurface			(const char* _szName) { return GetResource<CanvasSurface>(_szName); }
	void			CanvasContext::DestroySurface		(const char* _szName) { DestroyResource<CanvasSurface>(_szName); }

	CanvasImage*	CanvasContext::GetImage				(const char* _szName) { return GetResource<CanvasImage>(_szName); }
	void			CanvasContext::DestroyImage			(const char* _szName) { DestroyResource<CanvasImage>(_szName); }

	CanvasBrush*	CanvasContext::GetBrush				(Color _kColor) { return GetResource<CanvasBrush>(_kColor); }
	void			CanvasContext::DestroyBrush			(const char* _szName) { DestroyResource<CanvasBrush>(_szName); }

	CanvasStyle*	CanvasContext::GetStyle				(ELineStyle::Type _eLine) { return GetResource<CanvasStyle>(_eLine); }
	void			CanvasContext::DestroyStyle			(const char* _szName) { DestroyResource<CanvasStyle>(_szName); }

	CanvasGeometry*	CanvasContext::GetRectangle			(const char* _szName) { return GetResource<CanvasGeometry>(_szName); }
	void			CanvasContext::DestroyRectangle		(const char* _szName) { DestroyResource<CanvasGeometry>(_szName); }

	CanvasGeometry*	CanvasContext::GetElipse			(const char* _szName) { return GetResource<CanvasGeometry>(_szName); }
	void			CanvasContext::DestroyElipse		(const char* _szName) { DestroyResource<CanvasGeometry>(_szName); }

	CanvasFont*		CanvasContext::GetFont				(const char* _szName) { return GetResource<CanvasFont>(_szName); }
	void			CanvasContext::DestroyFont			(const char* _szName) { DestroyResource<CanvasFont>(_szName); }

	CanvasText*		CanvasContext::GetText				(const char* _szName) { return GetResource<CanvasText>(_szName); }
	void			CanvasContext::DestroyText			(const char* _szName) { DestroyResource<CanvasText>(_szName); }
}}