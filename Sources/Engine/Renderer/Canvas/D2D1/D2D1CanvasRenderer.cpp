/******************************************************************************
**	Includes
******************************************************************************/
#include "../CanvasRenderer.hpp"

#include <Core/Log/LogManager.hpp>

#include <Renderer/Renderer.hpp>
#include <Renderer/Resource/Texture2D.hpp>

#include <Renderer/Canvas/CanvasQueue.hpp>
#include <Renderer/Canvas/CanvasImageFactory.hpp>
#include <Renderer/Canvas/CanvasTextFactory.hpp>
#include <Renderer/Canvas/Resource/Text/CanvasText.hpp>
#include <Renderer/Canvas/Resource/Texture/CanvasSurface.hpp>
#include <Renderer/Canvas/Resource/Texture/CanvasImage.hpp>
#include <Renderer/Canvas/Resource/CanvasBrush.hpp>
#include <Renderer/Canvas/Resource/CanvasStyle.hpp>
#include <Renderer/Canvas/Resource/CanvasGeometry.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor
	//!	@date		2015-11-01
	CanvasRenderer::CanvasRenderer()
		: m_pDevice(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-11-01
	CanvasRenderer::~CanvasRenderer()
	{
		// Nothing to do
	}

	//!	@brief		Initialize
	//!	@date		2015-11-01
	bool CanvasRenderer::Initialize(Renderer* /*_pRenderer*/)
	{
		//D2D1_CREATION_PROPERTIES kCreationProperty;
		//ZeroMemory(&kCreationProperty, sizeof(D2D1_CREATION_PROPERTIES));
		//#if defined(_DEBUG)
		//	kCreationProperty.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
		//#endif
		//
		//// Retrieve IDXGIDevice
		//IDXGIDevice* pDXGIDevice = NULL;
		//ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(_pRenderer->GetDevice());
		//HRESULT hResult = pD3D11Device->QueryInterface(_uuidof(IDXGIDevice), reinterpret_cast<void**>(&pDXGIDevice));
		//if(FAILED(hResult))
		//{
		//	LOG_INTERNAL_ERROR("[CanvasRenderer] Failed to retrieve IDXGIDevice (HRESULT = 0x%0X)", hResult);
		//	return false;
		//}
		//
		//// Create D2D1 Device
		//hResult = D2D1CreateDevice(pDXGIDevice, &kCreationProperty, (ID2D1Device**)&m_pDevice);
		//if(FAILED(hResult))
		//{
		//	LOG_INTERNAL_ERROR("[CanvasRenderer] Direct2D Device Creation failed (HRESULT = 0x%0X)", hResult);
		//	return false;
		//}

		//// Create Immediate Context for D2D1 Device
		//ID2D1Device* pD2D1Device = static_cast<ID2D1Device*>(m_pDevice);
		//hResult = pD2D1Device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, (ID2D1DeviceContext**)&m_pDeviceContext);
		//if(FAILED(hResult))
		//{
		//	LOG_INTERNAL_ERROR("[CanvasRenderer] Direct2D Device Context Creation failed (HRESULT = 0x%0X)", hResult);
		//	return false;
		//}

		//// ImageFactory
		//m_pImageFactory = new CanvasImageFactory();
		//m_pImageFactory->Initialize();

		//// TextFactory
		//m_pTextFactory = new CanvasTextFactory();
		//m_pTextFactory->Initialize();
	
		return true;
	}

	//!	@brief		Release
	//!	@date		2015-11-01
	void CanvasRenderer::Release()
	{		
		SAFE_RELEASE_AND_DELETE(m_pTextFactory);

		// Clean Immediate Context
		/*if(m_pImmediateContext) 
		{
			ID3D11DeviceContext* pD3D11ImmediateContext = static_cast<ID3D11DeviceContext*>(m_pImmediateContext);
			pD3D11ImmediateContext->ClearState();
			pD3D11ImmediateContext->Flush();
		}

		// Release Device
		if(m_pDevice) 
		{
			ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(m_pDevice);
			pD3D11Device->Release();
			pD3D11Device = NULL;
		}*/
	}

	//!	@brief		Present
	//!	@date		2016-05-22
	void CanvasRenderer::Present()
	{		
		const uint32 uiContextCount = m_vContext.GetSize();
		for(uint32 uiContext = 0; uiContext < uiContextCount; ++uiContext)
		{
			CanvasQueue* pQueue = m_vContext[uiContext]->GetQueue();
			if(pQueue->HasCommand())
			{	
				pQueue->SwitchBuffer();
			}
		}
	}

	//!	@brief		CreateSurface 
	//!	@date		2015-12-19
	CanvasSurface* CanvasRenderer::CreateSurface(Texture2D* _pTexture)
	{
		ID2D1DeviceContext* pD2D1DeviceContext = static_cast<ID2D1DeviceContext*>(m_pDeviceContext);

		ID2D1Factory* pD2D1Factory = NULL;
		pD2D1DeviceContext->GetFactory(&pD2D1Factory);
		
		//pD2D1Factory->

		/*float32 dpiX;
		float32 dpiY;
		pD2D1SpriteFactory->GetDesktopDpi(&dpiX, &dpiY);*/
 
		// Create a DC render target.
		/*D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE),
			96.0f,
			96.0f
		);*/

		// Retrieve DXGISurface from D3D11Texture
		IDXGISurface* pDXGISurface = NULL; pDXGISurface;
		/*ID3D11Texture2D* pD3D11Texture = static_cast<ID3D11Texture2D*>(_pTexture->GetHandle());
		pD3D11Texture->QueryInterface<IDXGISurface>(&pDXGISurface);*/

		/*D2D1_BITMAP_PROPERTIES1 prop;
		ZeroMemory(&prop, sizeof(D2D1_BITMAP_PROPERTIES1));
		prop.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE);*/

		ID2D1RenderTarget* pD2D1RenderTarget = NULL; pD2D1RenderTarget;
		/*HRESULT hResult = pD2D1Factory->CreateDxgiSurfaceRenderTarget(pDXGISurface, &props, &pD2D1RenderTarget);
		if(FAILED(hResult))		
		{
			LOG_INTERNAL_ERROR("[CanvasRenderer] CreateDxgiSurfaceRenderTarget failed (HRESULT = 0x%0X)", hResult);
			return NULL;
		}*/
		
		CanvasSurface* pSurface = new CanvasSurface();
		pSurface->Initialize(_pTexture->GetWidth(), _pTexture->GetHeight(), pD2D1RenderTarget);

		return pSurface;
	}

	//!	@brief		CreateImage 
	//!	@date		2015-12-19
	CanvasImage* CanvasRenderer::CreateImage(CanvasSurface* _pSurface, const char* szFilePath)
	{
		return m_pImageFactory->CreateImage(_pSurface, szFilePath);
	}

	//!	@brief		CreateImage 
	//!	@date		2015-12-19
	CanvasImage* CanvasRenderer::CreateImage(Texture2D* _pTexture)
	{
		ID2D1DeviceContext* pD2D1DeviceContext = static_cast<ID2D1DeviceContext*>(m_pDeviceContext);

		// Retrieve DXGISurface from D3D11Texture
		IDXGISurface* pDXGISurface = NULL;
		/*ID3D11Texture2D* pD3D11Texture = static_cast<ID3D11Texture2D*>(_pTexture->GetHandle());
		pD3D11Texture->QueryInterface<IDXGISurface>(&pDXGISurface);*/

		D2D1_BITMAP_PROPERTIES1 prop;
		ZeroMemory(&prop, sizeof(D2D1_BITMAP_PROPERTIES1));
		prop.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE);

		ID2D1Bitmap1* pD2D1Bitmap = NULL;
		HRESULT hResult = pD2D1DeviceContext->CreateBitmapFromDxgiSurface(pDXGISurface, &prop, &pD2D1Bitmap);
		if(FAILED(hResult))		
		{
			LOG_INTERNAL_ERROR("[CanvasRenderer] CreateDxgiSurfaceRenderTarget failed (HRESULT = 0x%0X)", hResult);
			return NULL;
		}
		
		CanvasImage* pImage = new CanvasImage();
		pImage->Initialize(_pTexture->GetWidth(), _pTexture->GetHeight());
		pImage->AddFrame(pD2D1Bitmap);

		return pImage;
	}

	//!	@brief		CreateBrush
	//!	@date		2016-01-01
	CanvasBrush* CanvasRenderer::CreateBrush(CanvasSurface* _pSurface, Color _kColor)
	{
		ID2D1RenderTarget* pD2D1RenderTarget = static_cast<ID2D1RenderTarget*>(_pSurface->GetHandle());

		float32 fFactor = 1.0f / 255.0f;
		D2D1_COLOR_F kD2D1Color = D2D1::ColorF(_kColor.GetRed() * fFactor, _kColor.GetGreen() * fFactor, _kColor.GetBlue() * fFactor, _kColor.GetAlpha() * fFactor);

		ID2D1SolidColorBrush* pD2D1SolidBrushColor = NULL;
		HRESULT hResult = pD2D1RenderTarget->CreateSolidColorBrush(kD2D1Color, &pD2D1SolidBrushColor);
		if(FAILED(hResult))
		{
			LOG_INTERNAL_ERROR("[CanvasRenderer] CreateSolidColorBrush failed (HRESULT = 0x%0X)", hResult);
			return NULL;
		}

		CanvasBrush* pBrush = new CanvasBrush();
		pBrush->Initialize(pD2D1SolidBrushColor);

		return pBrush;
	}

	//!	@brief		CreateStyle
	//!	@date		2016-01-01
	CanvasStyle* CanvasRenderer::CreateStyle(CanvasSurface* _pSurface, ELineStyle::Type _eBorder)
	{
		ID2D1RenderTarget* pD2D1RenderTarget = static_cast<ID2D1RenderTarget*>(_pSurface->GetHandle());

		// Rerieve the factory
		ID2D1Factory* pD2D1Factory = NULL;
		pD2D1RenderTarget->GetFactory(&pD2D1Factory);

		// Create style
		ID2D1StrokeStyle* pD2D1Style = NULL;
		D2D1_STROKE_STYLE_PROPERTIES kD1D1StyleProperty = D2D1::StrokeStyleProperties(D2D1_CAP_STYLE_FLAT,D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_FLAT, D2D1_LINE_JOIN_MITER, 10.0f, (D2D1_DASH_STYLE)_eBorder);
		HRESULT hResult = pD2D1Factory->CreateStrokeStyle(kD1D1StyleProperty, NULL, 0, &pD2D1Style);
		if(FAILED(hResult))
		{
			LOG_INTERNAL_ERROR("[CanvasRenderer] CreateStyle failed (HRESULT = 0x%0X)", hResult);
			return NULL;
		}

		CanvasStyle* pStyle = new CanvasStyle();
		pStyle->Initialize(pD2D1Style);

		return pStyle;
	}

	//!	@brief		CreateLine
	//!	@date		2016-01-01
	//CanvasPath* CanvasRenderer::CreateDrawing(CanvasSurface* _pSurface, float32 _fFirstPointX, float32 _fFirstPointY, float32 _fSecondPointX, float32 _fSecondPointY, float32 _fRadiusX /*= 0.0f*/, float32 _fRadiusY /*= 0.0f*/)
	//{
	//	ID2D1RenderTarget* pD2D1RenderTarget = static_cast<ID2D1RenderTarget*>(_pSurface->GetHandle());

	//	// Retrieve the factory
	//	ID2D1Factory* pD2D1Factory = NULL;
	//	pD2D1RenderTarget->GetFactory(&pD2D1Factory);

	//	// Prepare Path
	//	ID2D1PathGeometry* pD2D1Path = NULL;
	//	HRESULT hResult = pD2D1Factory->CreatePathGeometry(&pD2D1Path);
	//	if(FAILED(hResult))
	//	{
	//		LOG_INTERNAL_ERROR("[CanvasRenderer] CreatePathGeometry failed (HRESULT = 0x%0X)", hResult);
	//		return NULL;
	//	}

	//	// Open Path
	//	ID2D1GeometrySink* pSink = NULL;
	//	hResult = pD2D1Path->Open(&pSink);
	//	if(FAILED(hResult))
	//	{
	//		LOG_INTERNAL_ERROR("[CanvasRenderer] ID2D1PathGeometry::Open failed (HRESULT = 0x%0X)", hResult);
	//		return NULL;
	//	}

	//	pSink->BeginFigure(D2D1::Point2F(0.0f, 0.0f), D2D1_FIGURE_BEGIN_FILLED);
	//	pSink->AddLine(D2D1::Point2F(0.0f, 0.0f));
	//	pSink->EndFigure(D2D1_FIGURE_END_OPEN);
	//	pSink->Close();

	//	// Prepare Geometry
	//	ID2D1RoundedRectangleGeometry* pD2D1Rectangle = NULL;
	//	D2D1_ROUNDED_RECT kD2D1Rectangle = D2D1::RoundedRect(D2D1::RectF(_fLeft, _fTop, _fRight, _fBottom), _fRadiusX, _fRadiusY);
	//	HRESULT hResult = pD2D1Factory->CreateRoundedRectangleGeometry(kD2D1Rectangle, &pD2D1Rectangle);
	//	if(FAILED(hResult))
	//	{
	//		LOG_INTERNAL_ERROR("[CanvasRenderer] CreateRoundedRectangleGeometry failed (HRESULT = 0x%0X)", hResult);
	//		return NULL;
	//	}

	//	CanvasGeometry* pGeometry = new CanvasGeometry();
	//	pGeometry->Initialize(pD2D1Elipse);
	//}

	//!	@brief		CreateRectangle
	//!	@date		2016-01-01
	CanvasGeometry* CanvasRenderer::CreateRectangle(CanvasSurface* _pSurface, float32 _fWidth, float32 _fHeight, float32 _fRadiusX /*= 0.0f*/, float32 _fRadiusY /*= 0.0f*/)
	{
		ID2D1RenderTarget* pD2D1RenderTarget = static_cast<ID2D1RenderTarget*>(_pSurface->GetHandle());

		// Retrieve the factory
		ID2D1Factory* pD2D1Factory = NULL;
		pD2D1RenderTarget->GetFactory(&pD2D1Factory);

		// Prepare Geometry
		ID2D1RoundedRectangleGeometry* pD2D1Rectangle = NULL;
		D2D1_ROUNDED_RECT kD2D1Rectangle = D2D1::RoundedRect(D2D1::RectF(0.0f, 0.0f, _fWidth, _fHeight), _fRadiusX, _fRadiusY);
		HRESULT hResult = pD2D1Factory->CreateRoundedRectangleGeometry(kD2D1Rectangle, &pD2D1Rectangle);
		if(FAILED(hResult))
		{
			LOG_INTERNAL_ERROR("[CanvasRenderer] CreateRoundedRectangleGeometry failed (HRESULT = 0x%0X)", hResult);
			return NULL;
		}

		CanvasGeometry* pGeometry = new CanvasGeometry();
		pGeometry->Initialize(pD2D1Rectangle);

		return pGeometry;
	}

	//!	@brief		CreateElipse
	//!	@date		2016-01-01
	CanvasGeometry* CanvasRenderer::CreateElipse(CanvasSurface* _pSurface, float32 _fRadiusX, float32 _fRadiusY)
	{
		ID2D1RenderTarget* pD2D1RenderTarget = static_cast<ID2D1RenderTarget*>(_pSurface->GetHandle());

		// Retrieve the factory
		ID2D1Factory* pD2D1Factory = NULL;
		pD2D1RenderTarget->GetFactory(&pD2D1Factory);

		// Prepare Geometry
		ID2D1EllipseGeometry* pD2D1Elipse = NULL;
		D2D1_ELLIPSE kD2D1Elipse = D2D1::Ellipse(D2D1::Point2F(0.0f, 0.0f), _fRadiusX, _fRadiusY);
		HRESULT hResult = pD2D1Factory->CreateEllipseGeometry(kD2D1Elipse, &pD2D1Elipse);
		if(FAILED(hResult))
		{
			LOG_INTERNAL_ERROR("[CanvasRenderer] CreateEllipseGeometry failed (HRESULT = 0x%0X)", hResult);
			return NULL;
		}

		CanvasGeometry* pGeometry = new CanvasGeometry();
		pGeometry->Initialize(pD2D1Elipse);

		return pGeometry;
	}

	//!	@brief		HasFont 
	//!	@date		2015-12-06
	bool CanvasRenderer::HasFont(const wchar_t* _szFamilyName) const
	{
		return m_pTextFactory->HasFont(_szFamilyName);
	}

	//!	@brief		CreateFont 
	//!	@date		2015-12-06
	CanvasFont* CanvasRenderer::CreateFont(const wchar_t* _szFamilyName, float32 fSize, uint32 uiWeight, EFontStyle::Type _eFont)
	{
		return m_pTextFactory->CreateFont(_szFamilyName, fSize, uiWeight, _eFont);
	}

	//!	@brief		CreateText
	//!	@date		2016-01-01
	CanvasText* CanvasRenderer::CreateText(const wchar_t* _szText, float32 _fWidthMax, float32 _fHeightMax, CanvasFont* _pFont)
	{
		return m_pTextFactory->CreateText(_szText, _fWidthMax, _fHeightMax, _pFont);
	}

	//!	@brief		DrawBegin
	//!	@date		2016-01-01
	void CanvasRenderer::DrawBegin(CanvasSurface* _pSurface)
	{
		ID2D1RenderTarget* pD2D1RenderTarget = static_cast<ID2D1RenderTarget*>(_pSurface->GetHandle());
		pD2D1RenderTarget->BeginDraw();
		pD2D1RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
	}

	//!	@brief		DrawEnd
	//!	@date		2016-01-01
	void CanvasRenderer::DrawEnd(CanvasSurface* _pSurface)
	{
		ID2D1RenderTarget* pD2D1RenderTarget = static_cast<ID2D1RenderTarget*>(_pSurface->GetHandle());
		pD2D1RenderTarget->EndDraw();
	}

	//!	@brief		DrawLine
	//!	@date		2016-01-01
	void CanvasRenderer::DrawLine(CanvasSurface* _pSurface, CanvasBrush* _pBrush, const Math::Vector2& _vPointA, const Math::Vector2& _vPointB, CanvasStyle* _pStyle /*= NULL*/, float32 _fWidth /*= 1.0f*/)
	{
		ID2D1RenderTarget* pD2D1RenderTarget = static_cast<ID2D1RenderTarget*>(_pSurface->GetHandle());
		ID2D1Brush* pD2D1Brush = static_cast<ID2D1Brush*>(_pBrush->GetHandle());
		ID2D1StrokeStyle* pD2D1Style = static_cast<ID2D1StrokeStyle*>(_pStyle->GetHandle());

		D2D1_POINT_2F vFirstPoint = D2D1::Point2F(_vPointA.GetX(), _vPointA.GetY());
		D2D1_POINT_2F vSecondPoint = D2D1::Point2F(_vPointB.GetX(), _vPointB.GetY());
		pD2D1RenderTarget->DrawLine(vFirstPoint, vSecondPoint, pD2D1Brush, _fWidth, pD2D1Style);
	}

	//!	@brief		DrawText
	//!	@date		2016-01-01
	void CanvasRenderer::DrawText(CanvasSurface* _pSurface, CanvasText* _pText, CanvasBrush* _pBrush, const Math::Matrix33& _mTransform)
	{
		ID2D1RenderTarget* pD2D1RenderTarget = static_cast<ID2D1RenderTarget*>(_pSurface->GetHandle());
		IDWriteTextLayout* pDWriteTextLayout = static_cast<IDWriteTextLayout*>(_pText->GetHandle());
		ID2D1Brush* pD2D1Brush = static_cast<ID2D1Brush*>(_pBrush->GetHandle());
		static D2D1_POINT_2F kD2D1Point = D2D1::Point2F(0.0f, 0.0f);

		// Positioning
		D2D1_MATRIX_3X2_F kMatrix = D2D1::Matrix3x2F(_mTransform[0][0], _mTransform[0][1], _mTransform[1][0], _mTransform[1][1], _mTransform[2][0], _mTransform[2][1]);
		pD2D1RenderTarget->SetTransform(kMatrix);

		// Draw
		pD2D1RenderTarget->DrawTextLayout(kD2D1Point, pDWriteTextLayout, pD2D1Brush);
	}

	//!	@brief		DrawImage
	//!	@date		2016-01-01
	void CanvasRenderer::DrawImage(CanvasSurface* _pSurface, CanvasImage* _pImage,  const Math::Matrix33& _mTransform)
	{
		ID2D1RenderTarget* pD2D1RenderTarget = static_cast<ID2D1RenderTarget*>(_pSurface->GetHandle());
		ID2D1Bitmap* pD2D1Bitmap = static_cast<ID2D1Bitmap*>(_pImage->GetFrame(0));
		
		// Positioning
		D2D1_MATRIX_3X2_F kMatrix = D2D1::Matrix3x2F(_mTransform[0][0], _mTransform[0][1], _mTransform[1][0], _mTransform[1][1], _mTransform[2][0], _mTransform[2][1]);
		pD2D1RenderTarget->SetTransform(kMatrix);

		// Draw
		D2D1_RECT_F kD2D1Rect = D2D1::RectF(0.0f, 0.0f, (float32)_pImage->GetWidth(), (float32)_pImage->GetHeight());  
		pD2D1RenderTarget->DrawBitmap(pD2D1Bitmap, kD2D1Rect);
	}

	//!	@brief		DrawGeometry
	//! @detail		Draw only wire
	//!	@date		2016-01-01
	void CanvasRenderer::DrawGeometry(CanvasSurface* _pSurface, CanvasGeometry* _pGeometry, CanvasBrush* _pBrush, const Math::Matrix33& _mTransform, float32 _fWidth, CanvasStyle* _pStyle /*= NULL*/)
	{
		ID2D1RenderTarget* pD2D1RenderTarget = static_cast<ID2D1RenderTarget*>(_pSurface->GetHandle());
		ID2D1Geometry* pD2D1Geometry = static_cast<ID2D1Geometry*>(_pGeometry->GetHandle());
		ID2D1Brush* pD2D1Brush = static_cast<ID2D1Brush*>(_pBrush->GetHandle());
		ID2D1StrokeStyle* pD2D1Style = _pStyle ? static_cast<ID2D1StrokeStyle*>(_pStyle->GetHandle()) : NULL;

		// Positioning
		D2D1_MATRIX_3X2_F kMatrix = D2D1::Matrix3x2F(_mTransform[0][0], _mTransform[0][1], _mTransform[1][0], _mTransform[1][1], _mTransform[2][0], _mTransform[2][1]);
		pD2D1RenderTarget->SetTransform(kMatrix);

		// Draw
		pD2D1RenderTarget->DrawGeometry(pD2D1Geometry, pD2D1Brush, _fWidth, pD2D1Style);
	}

	//!	@brief		DrawGeometry
	//! @detail		Draw a complete geometry (with in)
	//!	@date		2016-01-01
	void CanvasRenderer::DrawGeometry(CanvasSurface* _pSurface, CanvasGeometry* _pGeometry, CanvasBrush* _pBrush, const Math::Matrix33& _mTransform)
	{
		ID2D1RenderTarget* pD2D1RenderTarget = static_cast<ID2D1RenderTarget*>(_pSurface->GetHandle());
		ID2D1Geometry* pD2D1Geometry = static_cast<ID2D1Geometry*>(_pGeometry->GetHandle());
		ID2D1Brush* pD2D1Brush = static_cast<ID2D1Brush*>(_pBrush->GetHandle());

		// Positioning
		D2D1_MATRIX_3X2_F kMatrix = D2D1::Matrix3x2F(_mTransform[0][0], _mTransform[0][1], _mTransform[1][0], _mTransform[1][1], _mTransform[2][0], _mTransform[2][1]);
		pD2D1RenderTarget->SetTransform(kMatrix);

		// Draw
		pD2D1RenderTarget->FillGeometry(pD2D1Geometry, pD2D1Brush, NULL);
	}

	////!	@brief		CreateImage 
	////!	@date		2015-12-06
	//Texture2D* CanvasRenderer::CreateImage(Surface* _pSurface, const char* _szFilePath)
	//{
	//
	//}

	////!	@brief		CreateImage 
	////!	@date		2015-12-06
	//Texture2D* CanvasRenderer::CreateImage(Surface* _pSurface, float32 _fWidth, float32 _fHeight, EFormat::Type _eFormat, const void* _pData /*= NULL*/)
	//{
	//	ID2D1Factory2* pD2D1SpriteFactory = static_cast<ID2D1Factory2*>(m_pSpriteFactory);
	//	ID2D1RenderTarget* pD2D1RenderTarget = static_cast<ID2D1RenderTarget*>(_pSurface->GetHandle());

	//	D2D1_BITMAP_PROPERTIES kD2D1BiptmapProperty;
	//	pD2D1SpriteFactory->GetDesktopDpi(&kD2D1BiptmapProperty.dpiX, &kD2D1BiptmapProperty.dpiY);
	//	kD2D1BiptmapProperty.pixelFormat.format = (DXGI_FORMAT)_eFormat;
	//	kD2D1BiptmapProperty.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;

	//	ID2D1Bitmap* pD2D1Image = NULL;

	//	D2D1_SIZE_U kSize;
	//	kSize.width = _fWidth;
	//	kSize.height = _fHeight;
	//	HRESULT hResult = pD2D1RenderTarget->CreateBitmap(kSize, _pData, 1, kD2D1BiptmapProperty, &pD2D1Image);
	//	if(FAILED(hResult))		
	//	{
	//		LOG_INTERNAL_ERROR("[CanvasRenderer] CreateBitmap failed (HRESULT = 0x%0X)", hResult);
	//		return NULL;
	//	}

	//	Texture2D* pTexture = new Texture2D();
	//	pTexture->Initialize(_fWidth, _fHeight, pD2D1Image);
	//	return pTexture;
	//}

	////!	@brief		CreateText 
	////!	@date		2015-12-19
	//void CanvasRenderer::DrawText2(Surface* _pSurface, Font* _pFont, float32 _fPositionX, float32 _fPositionY, float32 _fWidth, float32 _fHeight, const wchar_t* _szText, uint32 _uiSize /*= (uint32)-1*/, Color kColor /*= Color::White*/)
	//{
	//	//IDWriteFactory* pDWriteTextFactory = static_cast<IDWriteFactory*>(m_pTextFactory);
	//	_uiSize = _uiSize == (uint32)-1 ? (uint32)wcslen(_szText) : _uiSize;

	//	// Create a TextLayout
	//	/*IDWriteTextLayout* pWriteTextLayout = NULL;
	//	HRESULT hResult = pDWriteTextFactory->CreateTextLayout(_szText, _uiSize, (IDWriteTextFormat*)_pFont->GetHandle(), 120.0f, 120.0f, &pWriteTextLayout);
	//	if(FAILED(hResult))		
	//	{
	//		LOG_INTERNAL_ERROR("[CanvasRenderer] CreateTextLayout failed (HRESULT = 0x%0X)", hResult);
	//		return;
	//	}*/
	//
	//	ID2D1RenderTarget* pD2D1RenderTarget = static_cast<ID2D1RenderTarget*>(_pSurface->GetHandle());

	//	ID2D1SolidColorBrush* pD2D1SolidColorBrush = NULL;
	//	pD2D1RenderTarget->CreateSolidColorBrush(D2D1::ColorF(kColor.GetRed() / 255.0f, kColor.GetGreen() / 255.0f, kColor.GetBlue() / 255.0f, kColor.GetAlpha() / 255.0f) , &pD2D1SolidColorBrush); 

	//	pD2D1RenderTarget->BeginDraw();
	//	//pRenderTarget->SetTransform(D2D1::IdentityMatrix());
	//	//pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	//	D2D1_RECT_F layoutRect = D2D1::RectF((float32)_fPositionX, (float32)_fPositionY, (float32)(_fPositionX + _fWidth), (float32)(_fPositionY + _fHeight));  
	//	IDWriteTextFormat* pWriteTextFormat = static_cast<IDWriteTextFormat*>(_pFont->GetHandle());
	//	pD2D1RenderTarget->DrawTextA(_szText, _uiSize, pWriteTextFormat, &layoutRect, pD2D1SolidColorBrush);
	//
	//	pD2D1RenderTarget->EndDraw();

	//	pD2D1SolidColorBrush->Release();
	//}

	////!	@brief		DrawRectangle 
	////!	@date		2015-11-21
	//void CanvasRenderer::DrawRectangle(Surface* _pSurface, float32 _fPositionX, float32 _fPositionY, float32 _fWidth, float32 _fHeight, Color kColor, uint32 _uiBorderSize /*= 0*/, Color kBorderColor /*= Color::Black*/)
	//{
	//	ID2D1RenderTarget* pD2D1RenderTarget = static_cast<ID2D1RenderTarget*>(_pSurface->GetHandle());
	//	pD2D1RenderTarget->BeginDraw();

	//	if(_uiBorderSize > 0)
	//	{
	//		ID2D1SolidColorBrush* pD2D1SolidColorBrush = NULL;
	//		pD2D1RenderTarget->CreateSolidColorBrush(D2D1::ColorF(kBorderColor.GetRed() / 255.0f, kBorderColor.GetGreen() / 255.0f, kBorderColor.GetBlue() / 255.0f, kBorderColor.GetAlpha() / 255.0f) , &pD2D1SolidColorBrush); 
	//		D2D1_RECT_F kD2D1Rect = D2D1::RectF((float32)_fPositionX, (float32)_fPositionY, (float32)(_fPositionX + _fWidth), (float32)(_fPositionY + _fHeight));  
	//		pD2D1RenderTarget->DrawRectangle(&kD2D1Rect, pD2D1SolidColorBrush);
	//		pD2D1SolidColorBrush->Release();

	//		_fWidth -= _uiBorderSize;
	//		_fHeight -= _uiBorderSize;
	//	}

	//	// Create Brush color
	//	ID2D1SolidColorBrush* pD2D1SolidColorBrush = NULL;
	//	pD2D1RenderTarget->CreateSolidColorBrush(D2D1::ColorF(kColor.GetRed() / 255.0f, kColor.GetGreen() / 255.0f, kColor.GetBlue() / 255.0f, kColor.GetAlpha() / 255.0f) , &pD2D1SolidColorBrush); 

	//	D2D1_RECT_F kD2D1Rect = D2D1::RectF((float32)_fPositionX, (float32)_fPositionY, (float32)(_fPositionX + _fWidth), (float32)(_fPositionY + _fHeight));  
	//	pD2D1RenderTarget->FillRectangle(&kD2D1Rect, pD2D1SolidColorBrush);

	//	pD2D1RenderTarget->EndDraw();

	//	pD2D1SolidColorBrush->Release();
	//}

	//void CanvasRenderer::DrawImage(Surface* _pSurface, Texture2D* _pTexture, float32 _fPositionX, float32 _fPositionY, float32 _fWidth, float32 _fHeight, uint32 /*_uiBorderSize*/ /*= 0*/, Color /*kBorderColor*/ /*= Color::Black*/)
	//{
	//	ID2D1RenderTarget* pD2D1RenderTarget = static_cast<ID2D1RenderTarget*>(_pSurface->GetHandle());
	//	ID2D1Bitmap* pD2D1Bitpmap = static_cast<ID2D1Bitmap*>(_pTexture->GetHandle());

	//	pD2D1RenderTarget->BeginDraw();

	//	D2D1_RECT_F kD2D1Rect = D2D1::RectF((float32)_fPositionX, (float32)_fPositionY, (float32)(_fPositionX + _fWidth), (float32)(_fPositionY + _fHeight));  
	//	pD2D1RenderTarget->DrawBitmap(pD2D1Bitpmap, kD2D1Rect);

	//	pD2D1RenderTarget->EndDraw();
	//}

	//!	@brief		DestroyResource 
	//!	@date		2015-11-21
	void CanvasRenderer::DestroyResource(void* _pHandle)
	{
		ID2D1Resource* pD2D1Resource = static_cast<ID2D1Resource*>(_pHandle);
		pD2D1Resource->Release();
	}
}}