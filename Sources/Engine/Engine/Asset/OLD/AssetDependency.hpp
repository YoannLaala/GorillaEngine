//#ifndef _ENGINE_DEPENDENCY_HPP_
//#define _ENGINE_DEPENDENCY_HPP_
//
///******************************************************************************
//**	Includes
//******************************************************************************/
//#include <Common/Common.hpp>
//#include "AssetFile.hpp"
//
///******************************************************************************
//**	Forward Declarations
//******************************************************************************/
//namespace PantherRenderer
//{
//	class Asset;
//}
//
///******************************************************************************
//**	Class Declaration
//******************************************************************************/
//namespace PantherRenderer
//{
//	// Handle Dependencies
//	class AssetDependency : public AssetFile
//	{
//	public:
//		CLASS_ID(AssetDependency)
//		AssetDependency();
//		~AssetDependency();
//
//		void				Init				(uint32 _uiAssetClassId, Asset** _ppAsset);
//		void				Init				(const char* _szRelativeFilePath, const char* _szParams = NULL);
//
//		Asset*&				GetAsset			() { return *m_ppAsset; }
//		void				SetAsset			(const Asset* _pAsset);
//
//		uint32				GetAssetClassId		() const { return m_uiAssetClassId; }
//
//	private:
//		uint32				m_uiAssetClassId;
//		Asset**				m_ppAsset;
//	};
//}
//
//#endif