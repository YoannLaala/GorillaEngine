/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/Canvas/CanvasTextFactory.hpp>

#include <Core/String/String16.hpp>
#include <Renderer/Canvas/Resource/Text/CanvasFont.hpp>
#include <Renderer/Canvas/Resource/Text/CanvasText.hpp>

/******************************************************************************
**	Define
******************************************************************************/
#define FAMILY_NAME_MAX_SIZE 256

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor
	//!	@date		2015-11-01
	CanvasTextFactory::CanvasTextFactory()
		: m_pFactory(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-11-01
	CanvasTextFactory::~CanvasTextFactory()
	{
		// Nothing to do
	}

	//!	@brief		Initialize
	//!	@date		2015-11-01
	bool CanvasTextFactory::Initialize()
	{
		// DirectWrite Factory
		//HRESULT hResult = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_pFactory);
		//if(FAILED(hResult))		
		//{
		//	LOG_INTERNAL_ERROR("[CanvasTextFactory] DirectWrite Factory creation failed (HRESULT = 0x%0X)", hResult);
		//	return false;
		//}

		//IDWriteFactory* pDWriteFactory = static_cast<IDWriteFactory*>(m_pFactory);
		//
		//// Retrieve the font collection
		//IDWriteFontCollection* pDWriteFontCollection = NULL;
		//hResult = pDWriteFactory->GetSystemFontCollection(&pDWriteFontCollection);
		//if(FAILED(hResult))		
		//{
		//	LOG_INTERNAL_ERROR("[CanvasTextFactory] GetSystemFontCollection failed (HRESULT = 0x%0X)", hResult);
		//	return false;
		//}

		//// Retrieve all font familly in the collection
		//String16 sFamilyName(FAMILY_NAME_MAX_SIZE);
		//uint32 uiFontFamilyCount = pDWriteFontCollection->GetFontFamilyCount();
		//m_vFamilyId.Reserve(uiFontFamilyCount);
		//for (uint32 uiFontFamily = 0; uiFontFamily != uiFontFamilyCount; ++uiFontFamily)
		//{
		//	IDWriteFontFamily* pDWriteFontFamily = NULL;
		//	hResult = pDWriteFontCollection->GetFontFamily(uiFontFamily, &pDWriteFontFamily);
		//	if(FAILED(hResult))		
		//	{
		//		LOG_INTERNAL_ERROR("[CanvasTextFactory] GetFontFamily failed (HRESULT = 0x%0X)", hResult);
		//		return false;
		//	}

		//	// Retrieve all name associated to the current font familly
		//	IDWriteLocalizedStrings* pDWriteFontFamilyName = NULL;
		//	hResult = pDWriteFontFamily->GetFamilyNames(&pDWriteFontFamilyName);
		//	if(FAILED(hResult))		
		//	{
		//		LOG_INTERNAL_ERROR("[CanvasTextFactory] GetFamilyNames failed (HRESULT = 0x%0X)", hResult);
		//		return false;
		//	}

		//	// Check if the family exist for the current locale setting
		//	uint32 uiIndex;
		//	BOOL bExist;
		//	hResult = pDWriteFontFamilyName->FindLocaleName(L"en-us", &uiIndex, &bExist);
		//	if(FAILED(hResult))		
		//	{
		//		LOG_INTERNAL_ERROR("[CanvasTextFactory] FindLocaleName failed (HRESULT = 0x%0X)", hResult);
		//		continue;
		//	}

		//	// Get the name of the family for the current locale setting
		//	hResult = pDWriteFontFamilyName->GetString(uiIndex, (wchar_t*)sFamilyName.GetBuffer(), FAMILY_NAME_MAX_SIZE);
		//	if(FAILED(hResult))		
		//	{
		//		LOG_INTERNAL_ERROR("[CanvasTextFactory] FontFamilyName::GetString failed (HRESULT = 0x%0X)", hResult);
		//		return false;
		//	}

		//	uint32 uiLength = (uint32)wcslen(sFamilyName.GetBuffer());
		//	uint32 uiFamilyId = Identifier::Compute(sFamilyName.GetBuffer(), uiLength * sizeof(wchar_t));
		//	m_vFamilyId.Add(uiFamilyId);
		//}

		return true;
	}

	//!	@brief		Release
	//!	@date		2015-11-01
	void CanvasTextFactory::Release()
	{		
		IDWriteFactory* pDWriteFactory = static_cast<IDWriteFactory*>(m_pFactory);
		pDWriteFactory->Release();
	}

	//!	@brief		HasFont 
	//!	@date		2015-12-06
	bool CanvasTextFactory::HasFont(const wchar_t* _szFamilyName) const
	{
		// Font family not supported
		uint32 uiFamilyId = Hash::Generate(_szFamilyName, (uint32)wcslen(_szFamilyName) * sizeof(wchar_t));
		return m_vFamilyId.Find(uiFamilyId) != (uint32)-1;
	}

	//!	@brief		CreateFont 
	//!	@date		2015-12-06
	CanvasFont* CanvasTextFactory::CreateFont(const wchar_t* _szFamilyName, float32 fSize, uint32 uiWeight, EFontStyle::Type _eFont)
	{
		IDWriteFactory* pDWriteFactory = static_cast<IDWriteFactory*>(m_pFactory);
	
		// Create a TextFormat
		IDWriteTextFormat* pWriteTextFormat = NULL;
		HRESULT hResult = pDWriteFactory->CreateTextFormat(_szFamilyName, NULL, (DWRITE_FONT_WEIGHT)uiWeight, (DWRITE_FONT_STYLE)_eFont, DWRITE_FONT_STRETCH_NORMAL, fSize, L"en-us", &pWriteTextFormat);
		if(FAILED(hResult))		
		{
			LOG_INTERNAL_ERROR("[CanvasTextFactory] CreateTextFormat failed (HRESULT = 0x%0X)", hResult);
			return NULL;
		}

		// Finalize Font
		CanvasFont* pFont = new CanvasFont();
		pFont->Initialize(_szFamilyName, fSize, uiWeight, _eFont, pWriteTextFormat);

		return pFont;
	}

	//!	@brief		CreateText 
	//!	@date		2015-12-06
	CanvasText*	CanvasTextFactory::CreateText(const wchar_t* _szText, float32 _fWidthMax, float32 _fHeightMax, CanvasFont* _pFont)
	{
		IDWriteFactory* pDWriteFactory = static_cast<IDWriteFactory*>(m_pFactory);
		IDWriteTextFormat* pWriteTextFormat = static_cast<IDWriteTextFormat*>(_pFont->GetHandle());

		IDWriteTextLayout* pWriteTextLayout = NULL;
		HRESULT hResult = pDWriteFactory->CreateTextLayout(_szText, (uint32)wcslen(_szText), pWriteTextFormat, _fWidthMax, _fHeightMax, &pWriteTextLayout);
		if(FAILED(hResult))		
		{
			LOG_INTERNAL_ERROR("[CanvasTextFactory] CreateTextLayout failed (HRESULT = 0x%0X)", hResult);
			return NULL;
		}		

		// Retrieve Metrics
		DWRITE_TEXT_METRICS kDWriteTextMetric;
		hResult = pWriteTextLayout->GetMetrics(&kDWriteTextMetric);
		if(FAILED(hResult))		
		{
			LOG_INTERNAL_ERROR("[CanvasTextFactory] CreateTextLayout failed (HRESULT = 0x%0X)", hResult);
			return NULL;
		}

		// Create Canvas Text
		CanvasText* pText = new CanvasText();
		pText->Initialize(kDWriteTextMetric.width, kDWriteTextMetric.height, pWriteTextLayout);

		return pText;	
	}
}}