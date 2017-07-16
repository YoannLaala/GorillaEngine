///******************************************************************************
//**	Includes
//******************************************************************************/
//#include "AssetDependency.hpp"
//#include "Asset.hpp"
//
///******************************************************************************
//**	Class Definition
//******************************************************************************/
//namespace PantherRenderer
//{
//	REGISTER_CLASS_ID(AssetDependency)
//	AssetDependency::AssetDependency()
//	{
//		m_ppAsset = NULL;
//	}
//
//	void AssetDependency::Init(uint32 _uiAssetClassId, Asset** _ppAsset)
//	{
//		m_uiAssetClassId = _uiAssetClassId;
//		m_ppAsset = _ppAsset;
//	}
//
//	void AssetDependency::Init(const char* _szRelativeFilePath, const char* _szParams /*= NULL*/)
//	{
//		m_sRelativeFilePath = _szRelativeFilePath;
//		m_sParams = _szParams;
//	}
//
//	void AssetDependency::SetAsset(const Asset* _pAsset)
//	{
//		*m_ppAsset = const_cast<Asset*>(_pAsset);
//	}
//
//	AssetDependency::~AssetDependency()
//	{
//		if(m_ppAsset)
//		{
//			*m_ppAsset = NULL;
//			m_ppAsset = NULL;
//		}
//	}
//}