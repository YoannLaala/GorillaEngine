/******************************************************************************
**	Includes
******************************************************************************/
#include "../CanvasImageFactory.hpp"

#include <Core/File/FileManager.hpp>
#include <Core/Log/LogManager.hpp>
#include <Renderer/Canvas/Resource/Texture/CanvasSurface.hpp>
#include <Renderer/Canvas/Resource/Texture/CanvasImage.hpp>

#include <Wincodec.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor
	//!	@date		2015-11-01
	CanvasImageFactory::CanvasImageFactory()
		: m_pFactory(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-11-01
	CanvasImageFactory::~CanvasImageFactory()
	{
		// Nothing to do
	}

	//!	@brief		Initialize
	//!	@date		2015-11-01
	bool CanvasImageFactory::Initialize()
	{		
		HRESULT hResult = CoInitialize(NULL);
		if(FAILED(hResult))
		{
			LOG_INTERNAL_ERROR("[CanvasImageFactory] CoInitialize failed (HRESULT = 0x%0X)", hResult);
			return false;
		}

		// Create  WIC Image Factory
        hResult = CoCreateInstance(CLSID_WICImagingFactory2, NULL, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory), (void**)&m_pFactory);
		if(FAILED(hResult))
		{
			LOG_INTERNAL_ERROR("[CanvasImageFactory] WICImageFactory Creation failed (HRESULT = 0x%0X)", hResult);
			return false;
		}
		
		return true;
	}

	//!	@brief		CreateImage
	//!	@date		2015-11-01
	CanvasImage* CanvasImageFactory::CreateImage(CanvasSurface* _pSurface, const char* _szFilePath)
	{		
		IWICImagingFactory* pWICImageFactory = static_cast<IWICImagingFactory*>(m_pFactory);
		ID2D1RenderTarget* pD2D1RenderTarget = static_cast<ID2D1RenderTarget*>(_pSurface->GetHandle());

		// Open the Texture
		File* pFile = FileManager::OpenAFile(_szFilePath);
		if(!pFile)
		{
			return NULL;
		}

		// Create decoder from handle
		IWICBitmapDecoder* pWICBitmapDecoder = NULL;
		HRESULT hResult = pWICImageFactory->CreateDecoderFromFileHandle((ULONG_PTR)pFile, NULL, WICDecodeMetadataCacheOnLoad, &pWICBitmapDecoder);
        if(FAILED(hResult))		
		{
			LOG_INTERNAL_ERROR("[CanvasImageFactory] WICImageFactory CreateDecoderFromFileHandle failed (HRESULT = 0x%0X)", hResult);
			return false;
		}
		
		// Prepare each frame
		uint32 uiFrameCount = 0;
		pWICBitmapDecoder->GetFrameCount(&uiFrameCount);

		CanvasImage* pImage = NULL;
		if(uiFrameCount)
		{
			pImage = new CanvasImage();

			// Retrieve each frame available
			IWICFormatConverter* pWICFormatConverter = NULL;
			IWICBitmapFrameDecode* pWICBitmapFrameDecode = NULL;
			for(uint32 uiFrame = 0; uiFrame < uiFrameCount; ++uiFrame)
			{
				// Rerieve the frame
				hResult = pWICBitmapDecoder->GetFrame(uiFrame, &pWICBitmapFrameDecode);
				if(FAILED(hResult))		
				{
					LOG_INTERNAL_ERROR("[CanvasImageFactory] WICBitmapDecoder GetFrame failed (HRESULT = 0x%0X)", hResult);
					return false;
				}

				// Retrieve the size of the image
				uint32 uiWidth, uiHeight;
				hResult = pWICBitmapFrameDecode->GetSize(&uiWidth, &uiHeight);
				if(FAILED(hResult))		
				{
					LOG_INTERNAL_ERROR("[CanvasImageFactory] WICBitmapSource GetSize Failed (HRESULT = 0x%0X)", hResult);
					return false;
				}

				// Initialize once the CanvasImage
				if(uiFrame == 0)
				{
					pImage->Initialize(uiWidth, uiHeight);
				}

				// Create a format Converter
				hResult = pWICImageFactory->CreateFormatConverter(&pWICFormatConverter);
				if(FAILED(hResult))		
				{
					LOG_INTERNAL_ERROR("[CanvasImageFactory] WICImageFactory CreateDecoderFromFileHandle failed (HRESULT = 0x%0X)", hResult);
					return false;
				}

				// Initialize Converter for this frame
				hResult = pWICFormatConverter->Initialize(pWICBitmapFrameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeCustom);
				if(FAILED(hResult))		
				{
					LOG_INTERNAL_ERROR("[CanvasRenderer] WICFormatConverter Initialize failed (HRESULT = 0x%0X)", hResult);
					return false;
				}

				// Create Bitmap
				ID2D1Bitmap* pD2D1Bitmap = NULL;
				hResult = pD2D1RenderTarget->CreateBitmapFromWicBitmap(pWICFormatConverter, &pD2D1Bitmap);
				if(FAILED(hResult))		
				{
					LOG_INTERNAL_ERROR("[CanvasRenderer] WICFormatConverter Initialize failed (HRESULT = 0x%0X)", hResult);
					return false;
				}

				pImage->AddFrame(pD2D1Bitmap);
				pWICFormatConverter->Release();
				pWICBitmapFrameDecode->Release();
			}
		}
		FileManager::CloseAFile(pFile);
		pWICBitmapDecoder->Release();
		
		return pImage;
	}
}}