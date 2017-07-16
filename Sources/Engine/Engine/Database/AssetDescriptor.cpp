/******************************************************************************
**	Includes
******************************************************************************/
#include "AssetDescriptor.hpp"
#include <Core/Hash.hpp>
#include <Core/File/FileManager.hpp>
#include <Core/File/File.hpp>
#include <Engine/Asset/AssetProcess.hpp>
#include <Engine/Database/Database.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine 
{
	//!	@brief		Contructor
	//!	@date		2015-11-21
	AssetDescriptor::AssetDescriptor()
		: m_pProcess(NULL)
		, m_uiProcessTime((uint64)-1)
		, m_bIsExternal(false)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-11-21
	AssetDescriptor::~AssetDescriptor()
	{
		// Nothing to do
	}

	//!	@brief		Deserialize
	//!	@date		2015-11-21
	bool AssetDescriptor::Deserialize(Node _kNode)
	{
		// Compute ids for all input
		_kNode.GetValue("Inputs", m_vInput);
		_kNode.GetValue("IsExternal", m_bIsExternal);

		// Cooker
		Node kCookerNode = _kNode["Cooker"];
		if(kCookerNode.IsValid())
		{
			// Path & Parameter
			String sPath = kCookerNode["Path"].GetString(); 
			String sParameters = kCookerNode["Parameters"].GetString();
				
			// Create Cooker
			m_pProcess = new AssetProcess();
			m_pProcess->Initialize(sPath.GetBuffer(), sParameters.GetBuffer());

			// Read last write time of the Serializer
			File* pFile = FileManager::OpenAFile(sPath.GetBuffer());
			if(pFile)
			{
				m_uiProcessTime = pFile->GetLastWrite();
				FileManager::CloseAFile(pFile);
			}
		}

		return true;
	}

	//!	@brief		Deserialize
	//!	@date		2015-11-21
	bool AssetDescriptor::Serialize(Node /*_kNode*/)
	{
		return true;
	}
}}