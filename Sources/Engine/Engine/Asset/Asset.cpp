/******************************************************************************
**	Includes
******************************************************************************/
#include "Asset.hpp"
#include <Engine/Engine.hpp>
#include <Engine/Asset/AssetManager.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	//!	@brief		Contructor
	//!	@date		2015-04-04
	Asset::Asset()
		: m_uiId((uint32)-1)
		, m_uiReferenceCount(0)
		, m_eState(EState::Unloaded)
		, m_eFlag(0)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	Asset::~Asset()
	{
		// Nothing to do
	}

	//!	@brief		AddLink
	//!	@date		2015-04-04
	void Asset::AddLink(const char* _szRelativeFilePath)
	{ 
		GetAssetManager()->AddLink(_szRelativeFilePath, this); 
	}

	//!	@brief		ReadAssetInternal
	//!	@date		2015-04-04
	void Asset::ReadAssetInternal(const Class* _pClass, const char* _szFilePath, const char* _szParameter, AssetHandle<Asset>& _hAsset)
	{
		_hAsset = GetAssetManager()->Get(_pClass, _szFilePath, _szParameter);
	}

	//void Asset::Initialize(const char* _szRelativeFilePath, const char* _szParams /*= NULL*/, const char* _szExtension /*= NULL*/, const char* _szInputPath /*= NULL*/, const char* _szOutputPath /*= NULL*/)
	//{
	//	// Build Source filepath
	//	String sSourceFilePath(_szInputPath);
	//	sSourceFilePath.Append(_szRelativeFilePath);

	//	// Initialize the FileDescriptor and the FileIdentifier
	//	FileDescriptor::Initialize(sSourceFilePath.GetBuffer(), _szParams);
	//	m_kFileIdentifier.Initialize(sSourceFilePath.GetBuffer(), _szParams);

	//	/*AssetIdentifier::Init(_szRelativeFilePath, _szParams);
	//	m_sRelativeFilePath = _szRelativeFilePath;*/
	//	
	//	// Full FilePath
	//	//m_sFilePath = _szRelativeFilePath;
	//	/*if(_szInputPath)
	//	{
	//		m_sFilePath -= _szInputPath;
	//	}*/

	//	// Params
	//	/*if(_szParams)
	//	{
	//		m_sParams = _szParams;
	//	}*/

	//	// Build unique Output filePath
	//	/*if(_szOutputPath)
	//	{
	//		Path kRelativePath(_szRelativeFilePath);
	//		String sFilePathOutput = _szOutputPath;
	//		sFilePathOutput += kRelativePath.GetDirectory() + kRelativePath.GetFile();

	//		// Check if we need to make a unique name (input != ouput)
	//		if(strcmp(_szInputPath, _szOutputPath) != 0)
	//		{
	//			sFilePathOutput += "_";
	//			sFilePathOutput += GetId();
	//		}

	//		if(_szExtension)
	//		{
	//			sFilePathOutput += ".";
	//			sFilePathOutput += _szExtension;
	//		}
	//		
	//		// Keep Path with final Output File Path
	//		m_kOutput = sFilePathOutput;
	//	}*/
	//}

	/*bool Asset::Save(void* _pWriter)
	{
		return true;
	}*/

	//bool Asset::Load(/*void* _pWriter*/)
	//{
	//	return true;
	//}

	//void Asset::Unload(/*Renderer* _pRenderer*/)
	//{
	//	/*m_bIsLoaded = false;
	//	m_vecDependency.Clear();
	//	_pRenderer->RemoveResource(this);*/
	//}
}}	// Gorilla::Asset
