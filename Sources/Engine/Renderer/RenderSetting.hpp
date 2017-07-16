//#ifndef _SETTING_HPP_
//#define _SETTING_HPP_
//
///******************************************************************************
//**	Includes
//******************************************************************************/
//#include <Core/Core.hpp>
//#include <Common/JsonHelper.hpp>
//#include <d3d11.h>
//
///******************************************************************************
//**	Forward Declarations
//******************************************************************************/
//namespace PantherRenderer
//{
//}
//
///******************************************************************************
//**	Class Declaration
//******************************************************************************/
//namespace PantherRenderer
//{
//	/******************************************************************************
//	**	Class Declaration
//	******************************************************************************/
//	class Setting
//	{
//	public:
//		Setting		();
//		~Setting	();
//
//
//		void Initialize		(ID3D11Device* _pDevice, IDXGIFactory* _pInterfaceFactory);
//		bool Deserialize	(const rapidjson::Value& _kDocument);
//		void SelectMode		(uint32 _uiAdapter, uint32 _uiOutput, uint32 _uiMode);
//
//		const DXGI_MODE_DESC&	GetModeDesc		() const { return m_kModeDesc; }
//		bool					IsFullScreen	() const { return m_bFullScreen; }
//		const DXGI_SAMPLE_DESC& GetSampleDesc	() const { return m_vecSampleDesc[m_uiMSAA]; }
//
//	private:
//		// Output Container;
//		struct Output
//		{
//			DXGI_OUTPUT_DESC m_kDesc;
//			Vector<DXGI_MODE_DESC> m_vecModeDesc;
//		};
//
//		// Adapter Container
//		struct Adapter
//		{
//			DXGI_ADAPTER_DESC m_kDesc;
//			Vector<Output> m_vecOutput;
//		};
//
//	private:
//		// Current Settings
//		DXGI_MODE_DESC m_kModeDesc;			
//		bool m_bFullScreen;
//		uint32 m_uiMSAA;
//		uint32 m_uiFXAA;
//			
//		// Information available
//		Vector<Adapter>				m_vecAdapter;
//		Vector<DXGI_SAMPLE_DESC>	m_vecSampleDesc;
//	};
//}
//
//#endif