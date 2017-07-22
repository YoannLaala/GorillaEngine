/******************************************************************************
**	Includes
******************************************************************************/
#include "AssetManager.hpp"
#include <Core/File/FileManager.hpp>
#include <Core/File/File.hpp>
#include <Core/Container/Dictionary.hpp>
#include <Core/Stream/FileReader.hpp>
#include <Core/Stream/Dictionary/DictionaryStreamJson.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Asset/AssetProcess.hpp>
#include <Engine/Database/Database.hpp>
#include <Engine/Database/Table.hpp>
#include <Engine/Database/AssetDescriptor.hpp>

#include <Core/Time/Profiler.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine 
{
	//!	@brief		Contructor
	//!	@date		2015-11-21
	AssetManager::AssetManager()
		: m_bIsRunning(false)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-11-21
	AssetManager::~AssetManager()
	{	
		// Nothing to do
	}

	//!	@brief		Initialize
	//!	@date		2015-11-21
	void AssetManager::Initialize()
	{
	#if defined(GORILLA_EDITOR)
		// Create all DataBase
		String sDirectory, sFilePath;
		FileManager::GetDirectory(FileManager::Directory::Executable, sDirectory);
		m_aPath[EPath::AssetIntrinsic].Set(sDirectory).Append("..\\..\\Resources\\Asset\\"); Path::Format(m_aPath[EPath::AssetIntrinsic]);
		m_aPath[EPath::CookedIntrinsic].Set(sDirectory).Append("Resources\\Asset\\"); 
		m_aDatabase[EDatabase::File] = new Database<Table>();
		m_aDatabase[EDatabase::Time] = new Database<Table>();

		// Asset Descriptor
		Dictionary dAssetDescriptor;
		sFilePath.Append(sDirectory).Append("Resources\\Database\\AssetDescriptor.json"); 
		if(dAssetDescriptor.Read<DictionaryStreamJson>(sFilePath.GetBuffer()))
		{		
			Database<AssetDescriptor>* pDbDescriptor = new Database<AssetDescriptor>();
			Node kNode = dAssetDescriptor.GetFirstChild();
			while(kNode.IsValid())
			{
				AssetDescriptor* pDescriptor = pDbDescriptor->GetOrCreateTable(kNode.GetId());
				pDescriptor->Deserialize(kNode);
				++kNode;
			}
			m_aDatabase[EDatabase::Descriptor] = pDbDescriptor;
		}
		
		// Asset time
		//sFilePath.Set(sDirectory).Append("Resources\\Database\\AssetTime.db");
	#else
		String sDirectory;
		FileManager::GetDirectory(FileManager::Directory::Executable, sDirectory);
		for(uint32 ePath = 0; ePath < EPath::Count; ++ePath)
			m_aPath[ePath].Set(sDirectory).Append("Resources\\Cooked\\");
	#endif

		Database<Table>* pDbAsset = new Database<Table>();
		m_aDatabase[EDatabase::Asset] = pDbAsset;

		// Thread handling loading & cooking
		m_bIsRunning = true;
		m_aThreadId[EThread::LoadingThreadLowPriority] = Thread::Create("LoadingThreadLowPriority", &LoadingThreadLowPriorityEntry, this, Thread::EPriority::Low);
		m_aThreadId[EThread::LoadingThreadNormalPriority] = Thread::Create("LoadingThreadNormalPriority", &LoadingThreadNormalPriorityEntry, this, Thread::EPriority::Normal);
		m_aThreadId[EThread::LoadingThreadHighPriority] = Thread::Create("LoadingThreadHighPriority", &LoadingThreadHighPriorityEntry, this, Thread::EPriority::High);
		m_aThreadId[EThread::UnloadingThread] = Thread::Create("UnloadingThread", &UnloadingThreadEntry, this, Thread::EPriority::Low);

	#if defined(GORILLA_EDITOR)
		m_aThreadId[EThread::CookingThread] = Thread::Create("CookingThread", &CookingThreadEntry, this, Thread::EPriority::Low);
	#endif
	}
	
	//!	@brief		Release
	//!	@date		2015-11-21
	void AssetManager::Release()
	{	
		m_bIsRunning = false;

		// Remove Watchers
		if(m_aPath[EPath::Asset].IsEmpty())
		{
			GetFileManager()->RemoveFileWatcher(this, m_aPath[EPath::Asset].GetBuffer());
		
	#if !defined(MASTER)
			GetFileManager()->RemoveFileWatcher(this, m_aPath[EPath::AssetIntrinsic].GetBuffer());
	#endif
		}

		// Serialize all DataBase
		/*String sDirectory, sFilePath;
		FileManager::GetDirectory(FileManager::Directory::Executable, sDirectory);
		sFilePath.Append(sDirectory).Append("Resources\\Database\\AssetTime.db");
		static_cast<Database<Table>*>(m_aDatabase[EDatabase::Asset])->Serialize(sFilePath.GetBuffer());*/
	}

#if defined(GORILLA_EDITOR)
	//!	@brief		SetPath
	//!	@date		2015-11-21
	void AssetManager::SetPath(const char* _szPath)
	{
		static const char* aPath[AssetManager::EPath::AssetIntrinsic] = 
		{
			"Asset",
			"Cooked"
		};

		// Remove Watchers
		if(m_aPath[EPath::Asset].IsEmpty())
		{
			GetFileManager()->RemoveFileWatcher(this, m_aPath[EPath::Asset].GetBuffer());
		
	#if !defined(MASTER)
			GetFileManager()->RemoveFileWatcher(this, m_aPath[EPath::AssetIntrinsic].GetBuffer());
	#endif
		}

		// Build mandatory path
		for(uint32 ePath = 0; ePath < EPath::AssetIntrinsic; ++ePath)
		{
			m_aPath[ePath].Set(_szPath).Append(aPath[ePath]).Append(REPERTORY_SEPARATOR);
		}
		FileManager::CreateADirectory(m_aPath[EPath::Asset].GetBuffer());

		// Watch the asset directory to reload them on the fly
		GetFileManager()->AddFileWatcher(this, m_aPath[EPath::Asset].GetBuffer(), true);

	#if !defined(MASTER)
		GetFileManager()->AddFileWatcher(this, m_aPath[EPath::AssetIntrinsic].GetBuffer(), true);
	#endif
	}
#endif

#if defined(GORILLA_EDITOR)
	//!	@brief		Update
	//!	@date		2015-11-21
	void AssetManager::Update()
	{
		static Vector<Event> vEvent;

		// Synchronize events
		if(m_vEvent.GetSize())
		{
			m_kMutexEvent.Lock();
			vEvent = m_vEvent;
			m_vEvent.Clear();
			m_kMutexEvent.Unlock();

			// Propagate events
			const uint32 uiEventCount = vEvent.GetSize();
			for(uint32 uiEvent = 0; uiEvent < uiEventCount; ++uiEvent)
			{
				Event& kEvent = vEvent[uiEvent];
				SIGNAL_SEND(AssetChanged, kEvent.Type, kEvent.Asset.m_pValue, kEvent.Succeeded);
				kEvent.Asset.Release();
			}
			vEvent.Clear();
		}
	}
#endif

	//!	@brief		GetDescriptor
	//!	@date		2015-11-21
	const AssetDescriptor* AssetManager::GetDescriptor(const Class* _pClass) const 
	{ 
		return const_cast<AssetManager*>(this)->GetTable<AssetDescriptor>(EDatabase::Descriptor, _pClass->GetId()); 
	}

	//!	@brief		Get
	//!	@date		2015-11-21
	Asset* AssetManager::GetInternal(const Class* _pClass, const char* _szRelativeFilePath, const char* _szParam, bool /*_bSerialize*/)
	{
		// Format the path so it will be always the same
		String sOutputPath(_szRelativeFilePath);
		Path::Format(sOutputPath);

	#if defined(GORILLA_EDITOR)
		// Compute FileId
		uint32 uiFileId = Hash::Generate(sOutputPath);
	#endif

		// Compute AssetId
		if(_szParam && *_szParam != '\0')
		{
			sOutputPath.Append(' ').Append(_szParam);
		}
		uint32 uiAssetId = Hash::Generate(sOutputPath);

		// Retrieve the current asset if already exist
		Table* pTableAsset = GetOrCreateTable<Table>(EDatabase::Asset, _pClass->GetId());
		Asset* pAsset = reinterpret_cast<Asset*>(pTableAsset->GetValue(uiAssetId));
		if(!pAsset)
		{		
		#if defined(GORILLA_EDITOR)
			// Search for the proper descriptor
			AssetDescriptor* pDescriptor = GetTable<AssetDescriptor>(EDatabase::Descriptor, _pClass->GetId());;
			if(!pDescriptor)
			{
				LOG_INTERNAL_PRINT("Asset of type '%s' is not supported", _pClass->GetName().GetBuffer());
				return nullptr;
			}
		#endif

			bool bIntrinsic = _szRelativeFilePath[0] == ASSET_INTRINSIC_CHARACTER;
			if(bIntrinsic) _szRelativeFilePath = &_szRelativeFilePath[1];

			// Create the asset
			pAsset = (Asset*)_pClass->New();
			pAsset->SetState(Asset::EState::Unloaded);
			pAsset->SetId(uiAssetId);
			if(bIntrinsic) pAsset->SetFlag(Asset::EFlag::Intrinsic);
		
		#if defined(GORILLA_EDITOR)
			if(pDescriptor->IsExternal()) pAsset->SetFlag(Asset::EFlag::External);
		#endif

			// Build the relative path
			Path kRelativeInputPath = _szRelativeFilePath;

		#if defined(GORILLA_EDITOR)
			// Create Asset Source Path
			String sAssetSourcePath;
			sAssetSourcePath.Set(m_aPath[bIntrinsic ? EPath::AssetIntrinsic : EPath::Asset]).Append(kRelativeInputPath.GetFull());
			pAsset->SetSourcePath(sAssetSourcePath.GetBuffer(), _szParam ? _szParam : "");
		#endif

			// Build path for the asset
			String sAssetPath;
			sAssetPath.Set(m_aPath[bIntrinsic ? EPath::CookedIntrinsic : EPath::Cooked]).Append(kRelativeInputPath.GetDirectory()).Append(kRelativeInputPath.GetFileName()).Append('_').Append(uiAssetId).Append(".asset");
			pAsset->SetFilePath(sAssetPath.GetBuffer());
			pAsset->SetName(_szRelativeFilePath);

			// Check if the asset need to be serialized
			bool bCook = false;
			if(!bIntrinsic)
			{
				bCook = !FileManager::IsFileExist(sAssetPath.GetBuffer());

			#if defined(GORILLA_EDITOR)
				if (!bCook)
				{
					// Read last write time for output file
					File* pFile = FileManager::OpenAFile(sAssetPath.GetBuffer());
					uint64 uiOutputLastWriteTime =  pFile->GetLastWrite();
					FileManager::CloseAFile(pFile);

					// Read last write time for input file
					pFile = FileManager::OpenAFile(sAssetSourcePath.GetBuffer());
					uint64 uiInputLastWriteTime = pFile->GetLastWrite();
					FileManager::CloseAFile(pFile);

					// Register last write time if needed
					AssetProcess* pProcess = pDescriptor->GetProcess();
					if(uiInputLastWriteTime > uiOutputLastWriteTime) bCook = true;
					else if(pProcess && pDescriptor->GetProcessTime() > uiOutputLastWriteTime) bCook = true;
				}
			#endif
			}
			
			// Cook or load the asset
			if(bCook) PushCooking(pAsset);
			else PushLoading(pAsset, Thread::EPriority::Normal);

			// Register the file allowing to reload all asset related to a file modification
		#if defined(GORILLA_EDITOR)
			Table* pTableFile = GetOrCreateTable<Table>(EDatabase::File, uiFileId);
			pTableFile->SetValue(uiAssetId, reinterpret_cast<uint64>(pAsset));
		#endif

			// Add the asset to the database
			pTableAsset->SetValue(uiAssetId, reinterpret_cast<uint64>(pAsset));
		}

		return pAsset;
	}

	//!	@brief		PushLoading
	//!	@date		2015-11-29
	void AssetManager::PushLoading(Asset* _pAsset, Thread::EPriority::Type _ePriority)
	{
		// Load dependency directly
		if(Thread::GetId() == m_aThreadId[EThread::LoadingThreadLowPriority] 
			|| Thread::GetId() == m_aThreadId[EThread::LoadingThreadNormalPriority]
			|| Thread::GetId() == m_aThreadId[EThread::LoadingThreadHighPriority] )
		{
			Load(_pAsset);
			_pAsset->SetState(Asset::EState::Loaded);
			return;
		}

		Mutex& kMutex = m_aMutexAssetToLoad[_ePriority];
		kMutex.Lock();
		m_aAssetToLoad[_ePriority].Add(_pAsset);
		kMutex.Unlock();
		m_aConditionAssetToLoad[_ePriority].Signal();
	}

	//!	@brief		ClearLoading
	//!	@date		2015-11-29
	void AssetManager::ClearLoading(Thread::EPriority::Type _ePriority)
	{
		Mutex& kMutex = m_aMutexAssetToLoad[_ePriority];
		kMutex.Lock();
		m_aAssetToLoad[_ePriority].Clear();
		kMutex.Unlock();
	}

	//!	@brief		PushUnloading
	//!	@date		2015-11-29
	void AssetManager::PushUnloading(Asset* _pAsset)
	{
		_pAsset->SetState(Asset::EState::Unloading);

		m_kMutexAssetToUnload.Lock();
		m_vAssetToUnload.Add(_pAsset);
		m_kMutexAssetToUnload.Unlock();
		m_kConditionAssetToUnload.Signal();
	}

	//!	@brief		ClearUnloading
	//!	@date		2015-11-29
	void AssetManager::ClearUnloading()
	{
		m_kMutexAssetToUnload.Lock();
		m_vAssetToUnload.Clear();
		m_kMutexAssetToUnload.Unlock();
	}

	//!	@brief		PushCooking
	//!	@date		2015-11-29
	void AssetManager::PushCooking(Asset* _pAsset)
	{
	#if defined(GORILLA_EDITOR)
		// Wait the loading canceled before restarting
		if(_pAsset->HasFlag(Asset::EFlag::Changed))
		{
			while(_pAsset->GetState() != Asset::EState::Loaded)
			{
				Thread::Sleep(100);
			}
			_pAsset->RemoveFlag(Asset::EFlag::Changed);
		}
		_pAsset->SetState(Asset::EState::Cooking);

		// Load dependency directly
		if(Thread::GetId() == m_aThreadId[EThread::LoadingThreadLowPriority] 
			|| Thread::GetId() == m_aThreadId[EThread::LoadingThreadNormalPriority]
			|| Thread::GetId() == m_aThreadId[EThread::LoadingThreadHighPriority] )
		{
			bool bResult = Cook(_pAsset);
			if(bResult) PushLoading(_pAsset, Thread::EPriority::Normal);
			else _pAsset->SetState(Asset::EState::Loaded);
			return;
		}

		m_kMutexAssetToCook.Lock();
		m_vAssetToCook.Add(_pAsset);
		m_kMutexAssetToCook.Unlock();
		m_kConditionAssetToCook.Signal();
	#else
		LOG_INTERNAL_ERROR("Failed to find Asset '%s'", _pAsset->GetName().GetBuffer());
		_pAsset->SetState(Asset::EState::Loaded);
	#endif
	}

	//!	@brief		ClearCooking
	//!	@date		2015-11-29
	void AssetManager::ClearCooking()
	{
	#if defined(GORILLA_EDITOR)
		m_kMutexAssetToCook.Lock();
		m_vAssetToCook.Clear();
		m_kMutexAssetToCook.Unlock();
	#endif
	}

	//!	@brief		LoadingThreadHighPriorityEntry
	//!	@date		2015-11-21
	void AssetManager::LoadingThreadHighPriorityEntry(void* _pData)
	{
		AssetManager* pAssetManager = static_cast<AssetManager*>(_pData);
		while(pAssetManager->m_bIsRunning)
		{
			pAssetManager->LoadingLoop(Thread::EPriority::High);
		}
	}

	//!	@brief		LoadingThreadNormalPriorityEntry
	//!	@date		2015-11-21
	void AssetManager::LoadingThreadNormalPriorityEntry(void* _pData)
	{
		AssetManager* pAssetManager = static_cast<AssetManager*>(_pData);
		while(pAssetManager->m_bIsRunning)
		{
			pAssetManager->LoadingLoop(Thread::EPriority::Normal);
		}
	}

	//!	@brief		LoadingThreadLowPriorityEntry
	//!	@date		2015-11-21
	void AssetManager::LoadingThreadLowPriorityEntry(void* _pData)
	{
		AssetManager* pAssetManager = static_cast<AssetManager*>(_pData);
		while(pAssetManager->m_bIsRunning)
		{
			pAssetManager->LoadingLoop(Thread::EPriority::Low);
		}
	}

	//!	@brief		LoadingLoop
	//! @details	Load all asset pushed on a specific thread
	//!	@date		2015-11-21
	void AssetManager::LoadingLoop(Thread::EPriority::Type _ePriotity)
	{
		static Vector<Asset*> aAssetToLoadCopy[Thread::EPriority::Count];

		// Check if we need to wait
		Vector<Asset*>& vAssetToLoad = m_aAssetToLoad[_ePriotity];
		if(vAssetToLoad.IsEmpty())
		{
			m_aConditionAssetToLoad[_ePriotity].Wait();
		}		
		Vector<Asset*>& vAssetToLoadCopy = aAssetToLoadCopy[_ePriotity];
		vAssetToLoadCopy.Clear();

		// Copy in Loading thread
		Mutex& kMutex = m_aMutexAssetToLoad[_ePriotity];
		kMutex.Lock();
		vAssetToLoadCopy.Add(vAssetToLoad);
		vAssetToLoad.Clear();
		kMutex.Unlock();

		// Deserialize asset
		const uint32 uiAssetCount = vAssetToLoadCopy.GetSize();
		for (uint32 uiAsset = 0; uiAsset < uiAssetCount; ++uiAsset)
		{
			Asset* pAsset = vAssetToLoadCopy[uiAsset];
			
			PushEvent(EEvent::LoadingStarted, pAsset, true);
			bool bResult = Load(pAsset);
			pAsset->SetState(Asset::EState::Loaded);
			PushEvent(EEvent::LoadingFinished, pAsset, bResult);
		}
		vAssetToLoadCopy.Clear();
	}

	//!	@brief		Load
	//!	@date		2015-11-21
	bool AssetManager::Load(Asset* _pAsset)
	{
		if(_pAsset->HasFlag(Asset::EFlag::Changed)) return true;

		// Deserialize asset
		LOG_INTERNAL_PRINT("[AssetManager] Loading '%s'", _pAsset->GetFilePath().GetBuffer());
		if(!FileManager::IsFileExist(_pAsset->GetFilePath().GetBuffer()))
		{
			LOG_ERROR("File not found '%s'", _pAsset->GetFilePath().GetBuffer());
			return false;
		}

		if(_pAsset->HasFlag(Asset::EFlag::External)) _pAsset->Initialize(nullptr);
		else
		{
			FileReader kStream;
			kStream.Open(_pAsset->GetFilePath().GetBuffer());
			if(!kStream.CanRead())
			{
				LOG_ERROR("Failed to Open '%s'", _pAsset->GetFilePath().GetBuffer());
				return false;
			}
		
			// Deserialize the asset
			_pAsset->Initialize(&kStream);
		}

		LOG_INTERNAL_PRINT("[AssetManager] Loaded '%s'", _pAsset->GetFilePath().GetBuffer());
		return true;
	}

	//!	@brief		UnloadingThreadEntry
	//!	@date		2015-11-21
	void AssetManager::UnloadingThreadEntry(void* _pData)
	{
		AssetManager* pAssetManager = static_cast<AssetManager*>(_pData);
		while(pAssetManager->m_bIsRunning)
		{
			pAssetManager->UnloadingLoop();
		}
	}

	//!	@brief		Unload
	//! @details	Unload all asset pushed on a specific thread
	//!	@date		2015-11-21
	void AssetManager::UnloadingLoop()
	{
		static Vector<Asset*> vAssetToUnloadCopy;

		// Check if we need to wait
		if(m_vAssetToUnload.IsEmpty())
		{
			m_kConditionAssetToUnload.Wait();
		}
		
		// Copy in Unloading thread
		m_kMutexAssetToUnload.Lock();
		vAssetToUnloadCopy.Add(m_vAssetToUnload);
		m_vAssetToUnload.Clear();
		m_kMutexAssetToUnload.Unlock();

		// Release asset
		const uint32 uiAssetCount = vAssetToUnloadCopy.GetSize();
		for (uint32 uiAsset = 0; uiAsset < uiAssetCount; ++uiAsset)
		{
			Asset* pAsset = vAssetToUnloadCopy[uiAsset];

			LOG_INTERNAL_PRINT("[AssetManager] Unloading '%s'", pAsset->GetFilePath().GetBuffer());
			pAsset->Release();
			LOG_INTERNAL_PRINT("[AssetManager] Unloaded '%s'", pAsset->GetFilePath().GetBuffer());			

			if(pAsset->HasFlag(Asset::EFlag::Changed))
			{
				pAsset->SetState(Asset::EState::Loaded);	
				PushCooking(pAsset);
			}
			else pAsset->SetState(Asset::EState::Unloaded);	
		}
		vAssetToUnloadCopy.Clear();
	}

#if defined(GORILLA_EDITOR)
	//!	@brief		LoadingThreadHighPriorityEntry
	//!	@date		2015-11-21
	void AssetManager::CookingThreadEntry(void* _pData)
	{
		AssetManager* pAssetManager = static_cast<AssetManager*>(_pData);
		while(pAssetManager->m_bIsRunning)
		{
			pAssetManager->CookingLoop();
		}
	}

	//!	@brief		CookingLoop
	//! @details	Cook an asset at runtime depdending on type and AssetDescriptor associated
	//!	@date		2015-11-21
	void AssetManager::CookingLoop()
	{
		static Vector<Asset*> vAssetToCookCopy;

		// Check if we need to wait
		if(m_vAssetToCook.IsEmpty())
		{
			m_kConditionAssetToCook.Wait();
		}

		// Copy in Cooking thread
		m_kMutexAssetToCook.Lock();
		vAssetToCookCopy.Add(m_vAssetToCook);
		m_vAssetToCook.Clear();
		m_kMutexAssetToCook.Unlock();

		// Cook asset
		const uint32 uiAssetCount = vAssetToCookCopy.GetSize();
		for (uint32 uiAsset = 0; uiAsset < uiAssetCount; ++uiAsset)
		{
			Asset* pAsset = vAssetToCookCopy[uiAsset];

			PushEvent(EEvent::CookingStarted, pAsset, true);
			bool bResult = Cook(pAsset);
			if(bResult)
			{
				// Release the asset only if the new cooking has succeeded
				pAsset->Release();
				if(pAsset->HasFlag(Asset::EFlag::Changed)) pAsset->SetState(Asset::EState::Loaded);
				else PushLoading(pAsset, Thread::EPriority::Normal);				
			}
			else pAsset->SetState(Asset::EState::Loaded);
			PushEvent(EEvent::CookingFinished, pAsset, bResult);
		}
		vAssetToCookCopy.Clear();
	}

	//!	@brief		Cook
	//!	@date		2015-11-21
	bool AssetManager::Cook(Asset* _pAsset)
	{
		LOG_INTERNAL_PRINT("[AssetManager] Cooking '%s %s'", _pAsset->GetSourcePath().GetBuffer(), _pAsset->GetParam().GetBuffer());

		// Early fail because the file is not present
		if(!FileManager::IsFileExist(_pAsset->GetSourcePath().GetBuffer()))
		{
			LOG_ERROR("Cooking failed to find %s", _pAsset->GetSourcePath().GetBuffer());
			return false;
		}

		// Create Path if needed
		FileManager::CreateADirectory(_pAsset->GetFilePath().GetBuffer());

		// if there isn't converter, just copy to the output
		AssetDescriptor* pDescriptor = GetTable<AssetDescriptor>(EDatabase::Descriptor, _pAsset->GetClass()->GetId());
		AssetProcess* pProcess = pDescriptor->GetProcess();
		if(!pProcess) 
		{
			if (!FileManager::CopyAFile(_pAsset->GetSourcePath().GetBuffer(), _pAsset->GetFilePath().GetBuffer()))
			{
				LOG_ERROR("Cooking failed to copy '%s' to '%s'", _pAsset->GetSourcePath().GetBuffer(), _pAsset->GetFilePath().GetBuffer());
				return false;
			}
		}
		// Convert properly the asset
		else
		{
			// Generic Arguments
			String sError;
			Vector<Argument> vArgument(4);
			vArgument[0].Initialize("$[INPUT]", _pAsset->GetSourcePath().GetBuffer());
			vArgument[1].Initialize("$[PARAMETER]", _pAsset->GetParam().GetBuffer());
			vArgument[2].Initialize("$[OUTPUT]", _pAsset->GetFilePath().GetBuffer());
			vArgument[3].Initialize("$[PROJECT_PATH]", m_aPath[EPath::Asset].GetBuffer()); 

			// Execute the process
			if(!pProcess->Execute(vArgument, sError))
			{
				// Generate command line for debug purpose
				LOG_ERROR("Cooking failed for %s\n%s", _pAsset->GetSourcePath().GetBuffer(), sError.GetBuffer());
				return false;
			}
		}

		// Continue only if the asset is valid
		LOG_INTERNAL_PRINT("[AssetManager] Cooked '%s' '%s'", _pAsset->GetSourcePath().GetBuffer(), _pAsset->GetParam().GetBuffer());
		return true;
	}
#endif

	//!	@brief		OnFileChanged
	//!	@date		2015-11-21
	void AssetManager::AddLink(const char* _szRelativeFilePath, Asset* _pAsset)
	{
		uint32 uiFileId = Hash::Generate(_szRelativeFilePath);
		Table* pTable = GetOrCreateTable<Table>(EDatabase::File, uiFileId);
		pTable->SetValue(_pAsset->GetId(), reinterpret_cast<uint64>(_pAsset));
	}

	//!	@brief		FormatToRelative
	//!	@date		2015-04-04
	bool AssetManager::FormatToRelative(String& _sPath)
	{
		if(Path::IsAbsolute(_sPath.GetBuffer()))
		{	
			// Remove path to make relative path
			// if the path is still absolute this mean
			// the path doesn't belong to the project
			_sPath.Remove(m_aPath[EPath::Asset].GetBuffer());
			if(Path::IsAbsolute(_sPath.GetBuffer()))
			{
				_sPath.Set("");
				return false;
			}
		}

		return true;
	}

	//!	@brief		FormatToAbsolute
	//!	@date		2015-04-04
	bool AssetManager::FormatToAbsolute(String& _sPath)
	{
		if(Path::IsAbsolute(_sPath.GetBuffer())) return _sPath.Contains(m_aPath[EPath::Asset].GetBuffer());
		_sPath.Prepend(m_aPath[EPath::Asset]);

		return true;
	}

	//!	@brief		Load
	//!	@date		2015-11-21
	//bool AssetManager::Load(AssetDescriptor* /*_pAssetDescriptor*/, Asset* /*_pAsset*/, bool /*_bCook*/)
	//{
	//	//// Check if we need to build the asset
	//	//if(!_bCook)
	//	//{
	//	//	// Force to build if the file didn't exist
	//	//	_bCook = !FileManager::Exist(_pAsset->GetOutput().GetFull().GetBuffer());
	//	//	if(!_bCook)
	//	//	{
	//	//		// Check if the asset has ever been created or we need to rebuild it
	//	//		_bCook = FileManager::GetLastWriteTime(_pAsset->GetInput().GetFull().GetBuffer()) > FileManager::GetLastWriteTime(_pAsset->GetOutput().GetFull().GetBuffer());
	//	//	}
	//	//}

	//	//// Cook asset
	//	//if(_bCook && !Cook(_pAssetDescriptor, _pAsset))
	//	//{
	//	//	return false;
	//	//}

	//	//if(!_pAsset->Deserialize())
	//	//{
	//	//	LOG_INTERNAL_ERROR_STATIC("Failed to desezialize asset '%s'", _pAsset->GetInput().GetFull().GetBuffer());
	//	//	return false;
	//	//}

	//	//// Deserialize the asset if it is a 
	//	///*if(_pAssetDescriptor->IsContainer())
	//	//{
	//	//	_pAsset->SetInput(_pAsset->GetOutput().GetFull().GetBuffer());
	//	//	if(!_pAsset->Deserialize())
	//	//	{
	//	//		LOG_INTERNAL_ERROR_STATIC("Failed to desezialize asset '%s'", _pAsset->GetInput().GetFull().GetBuffer());
	//	//		return false;
	//	//	}
	//	//}*/

	//	//// Work on all Dependencies
	//	//const Vector<AssetDependency>& vDependecy = _pAsset->GetVecDependency();
	//	//const uint32 uiDependencyCount = vDependecy.GetSize();
	//	//for(uint32 uiDependency = 0; uiDependency < uiDependencyCount; ++uiDependency)
	//	//{
	//	//	AssetDependency& kAssetDependency = vDependecy[uiDependency];
	//	//	const Asset* pAssetDependency = Get(kAssetDependency.GetClassIdentifier(), kAssetDependency.GetFilePath().GetBuffer(), kAssetDependency.GetParam().GetBuffer(), _bCook);
	//	//	kAssetDependency.SetAsset(pAssetDependency);
	//	//}

	//	return true;
	//}

	// Unload an asset and his dependencies
	//void AssetManager::Unload(Asset* /*_pAsset*/)
	//{
	//	//_pAsset->Unload(GetRenderrer());

	//	// Work on all Dependencies
	//	/*const Vector<PantherRenderer::AssetDependency>& vecDependecy = _pAsset->GetVecDependency();
	//	for(uint32 uiSubAsset = 0; uiSubAsset < vecDependecy.GetSize(); uiSubAsset++)
	//	{
	//		PantherRenderer::AssetDependency& kDependency = vecDependecy[uiSubAsset];
	//		kDependency.GetAsset()->Unload(Engine::GetInstance()->GetRenderer());
	//	}	*/
	//}

	//!	@brief		OnFileChanged
	//!	@date		2015-11-21
	void AssetManager::OnFileChanged(FileWatcher::Notification::Type _eType, const char* _szDirectoryPath, const char* _szRelativePath)
	{
	#if defined(GORILLA_EDITOR)
		switch(_eType)
		{
			case FileWatcher::Notification::Modified:
			{
				static String sRelativePath;
				sRelativePath.Set(_szRelativePath);
				if(m_aPath[EPath::AssetIntrinsic] == _szDirectoryPath) sRelativePath.Prepend('@');

				uint32 uiFileId = Hash::Generate(sRelativePath.GetBuffer());
				Table* pTable = GetTable<Table>(EDatabase::File, uiFileId);
				if(pTable)
				{
					// Reload all asset related
					HashMap<uint32, uint64>::Iterator it = pTable->GetFirstRow();
					HashMap<uint32, uint64>::Iterator itEnd = pTable->GetLastRow();
					while(it != itEnd)
					{
						Asset* pAsset = reinterpret_cast<Asset*>(*it);						
						pAsset->SetFlag(Asset::EFlag::Changed); 
						PushCooking(pAsset);
						++it;
					}
				}

				break;
			}
		}
		//AssetDescriptor* pAssetDescriptor = NULL;
		//const Vector<Database::Row*>& vRow = GetDescriptorTable()->GetVecRow();
		//const uint32 uiRowCount = vRow.GetSize();
		//for(uint32 uiRow = 0; uiRow < uiRowCount; ++uiRow)
		//{
		//	AssetDescriptor* pAssetDescriptorTmp = (AssetDescriptor*)vRow[uiRow];
		//	const Vector<String>& vInputExtension = pAssetDescriptorTmp->GetVecInputExtension();
		//	if(vInputExtension.FindIndex(kRelativeFilPath.GetExtension()) != (uint32)-1)
		//	{
		//		pAssetDescriptor = pAssetDescriptorTmp;
		//		break;
		//	}
		//}

		//if(pAssetDescriptor)
		//{
		//	//Load(pAssetDescriptor, NULL, true);
		//}

		//GetDescriptor(


		//Identifier kAssetIdentifier(_szFilePath);

		//GetDatabase(EDatabase::Asset)->GetTable(
		// Find all asset related to this file
		//Vector<PantherRenderer::Asset*> vecAsset;
		//uint32 uiFileId = Guid::Compute(_szFilePath);
		//uint32 uiExtensionId = Guid::Compute(_kRelativeFilePath.GetExtension());
		//m_pAssetDataBase->GetVecAssetFromFile(uiFileId, uiExtensionId, vecAsset);

		//// Rebuild Asset
		//for(uint32 uiAsset = 0; uiAsset < vecAsset.GetSize(); uiAsset++)
		//{
		//	PantherRenderer::Asset*& pAsset = vecAsset[uiAsset];

		//	// Reload the Asset
		//	const AssetTableEntryDataBase* pAssetTable = m_pAssetDataBase->GetAssetTable(pAsset->GetClassId());
		//	UnloadAsset(pAsset);
		//	LoadAsset(pAssetTable, pAsset, true);
		//}
	#else
		UNUSED(_eType, _szDirectoryPath, _szRelativePath);
	#endif
	}
}}