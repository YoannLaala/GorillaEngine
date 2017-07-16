///******************************************************************************
//**	Includes
//******************************************************************************/
//#include "Setting.hpp"
//
///******************************************************************************
//**	Class Definition
//******************************************************************************/
//namespace PantherRenderer
//{
//	Setting::Setting()
//	{
//		m_kModeDesc.Width = (uint32)-1;
//		m_kModeDesc.Height = (uint32)-1;
//		m_kModeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//		m_kModeDesc.RefreshRate.Numerator = 60;
//		m_kModeDesc.RefreshRate.Denominator = 1;
//		m_bFullScreen = true;
//		m_uiMSAA = 0;
//		m_uiFXAA;
//	}
//
//	bool Setting::Deserialize(const rapidjson::Value& _kDocument)
//	{
//		// Resolution
//		if(Common::JsonHelper::IsReadable(_kDocument, "Resolution"))
//		{
//			const rapidjson::Value& kResolutionJson = _kDocument["Resolution"];
//			Common::JsonHelper::Deserialize(kResolutionJson, "Width", m_kModeDesc.Width);
//			Common::JsonHelper::Deserialize(kResolutionJson, "Height", m_kModeDesc.Height);
//		}
//
//		// FullScreen
//		if(Common::JsonHelper::IsReadable(_kDocument, "FullScreen"))
//		{
//			Common::JsonHelper::Deserialize(_kDocument, "FullScreen", m_bFullScreen);
//		}
//
//		// MultiSampling
//		if(Common::JsonHelper::IsReadable(_kDocument, "FXAA"))
//		{
//			Common::JsonHelper::Deserialize(_kDocument, "FXAA", m_uiFXAA);
//		}
//
//		return true;
//	}
//
//	// Get all information needed with the device and the factory pointer
//	void Setting::Initialize(ID3D11Device* _pDevice, IDXGIFactory* _pInterfaceFactory)
//	{
//		// Get All Adapter available
//		HRESULT hr = S_OK;
//		IDXGIAdapter* pInterfaceAdapter = NULL;
//		while(SUCCEEDED(hr))
//		{
//			hr = _pInterfaceFactory->EnumAdapters(m_vecAdapter.GetSize(), &pInterfaceAdapter);
//			if(SUCCEEDED(hr))
//			{
//				// Get desc on the adapter (Information on the graphics card)
//				Adapter& kAdapter = m_vecAdapter.Add();
//				pInterfaceAdapter->GetDesc(&kAdapter.m_kDesc);
//
//				// Get All Resolution available
//				IDXGIOutput* pInterfaceOutput = NULL;
//				HRESULT subhr = S_OK;
//				while(SUCCEEDED(subhr))
//				{
//					subhr = pInterfaceAdapter->EnumOutputs(kAdapter.m_vecOutput.GetSize(), &pInterfaceOutput);
//					if(SUCCEEDED(subhr))
//					{
//						// Get desc for the output
//						Setting::Output& kOutput = kAdapter.m_vecOutput.Add();
//						pInterfaceOutput->GetDesc(&kOutput.m_kDesc);
//
//						// Get Mode Count
//						uint32 uiModeCount = 0;
//						pInterfaceOutput->GetDisplayModeList(m_kModeDesc.Format, 0, &uiModeCount, NULL);
//
//						// Get All Mode
//						kOutput.m_vecModeDesc.Resize(uiModeCount);
//						pInterfaceOutput->GetDisplayModeList(m_kModeDesc.Format, 0, &uiModeCount, &kOutput.m_vecModeDesc[0]);
//
//						// Search proper mode if the user didn't set the value
//						if(m_kModeDesc.Width != (uint32)-1 && m_kModeDesc.Height != (uint32)-1)
//						{
//							for(uint32 uiMode = 0; uiMode < kOutput.m_vecModeDesc.GetSize(); ++uiMode)
//							{
//								DXGI_MODE_DESC& kModeDesc = kOutput.m_vecModeDesc[uiMode];
//								if(kModeDesc.Width == m_kModeDesc.Width && kModeDesc.Height == m_kModeDesc.Height)
//								{
//									m_kModeDesc = kModeDesc;
//									break;
//								}
//							}
//						}
//
//					}
//				}
//			}
//		}
//
//		// Set Default if no mode was found
//		if(m_kModeDesc.Width == (uint32)-1 || m_kModeDesc.Height == (uint32)-1)
//		{
//			SelectMode(0, 0, m_vecAdapter[0].m_vecOutput[0].m_vecModeDesc.GetSize()-1);
//		}
//
//		// Get all MultiSample available
//		uint32 uiQuality = 0;
//		for(uint32 uiSample = 1; uiSample < D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; ++uiSample)
//		{
//			hr = _pDevice->CheckMultisampleQualityLevels(m_kModeDesc.Format, uiSample, &uiQuality);
//			if(SUCCEEDED(hr) && uiQuality > 0)
//			{
//				DXGI_SAMPLE_DESC& kSampleDesc = m_vecSampleDesc.Add();
//				kSampleDesc.Count = uiSample;
//				kSampleDesc.Quality = --uiQuality;
//			}
//		}
//	}
//
//	void Setting::SelectMode(uint32 _uiAdapter, uint32 _uiOutput, uint32 _uiMode)
//	{
//		m_kModeDesc = m_vecAdapter[_uiAdapter].m_vecOutput[_uiOutput].m_vecModeDesc[_uiMode];
//	}
//
//	Setting::~Setting()
//	{
//
//	}
//}