#ifndef _ENGINE_ENGINE_MANAGER_HPP_
#define _ENGINE_ENGINE_MANAGER_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>
#include <Core/File/FileWatcher.hpp>
#include <Core/Thread/Thread.hpp>
#include <Core/Thread/Mutex.hpp>
#include <Core/Thread/ConditionVariable.hpp>
#include <Engine/Asset/AssetHandle.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla 
{
	class Class;
	class AssetAttribute;
}

namespace Gorilla { namespace Engine
{
	class Engine;
	class Asset;
	class AssetDescriptor;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class AssetManager : public IFileWatcher
	{
		friend class Engine;
		friend class Asset;
		friend class AssetAttribute;

		struct EPath
		{
			enum Type : uint8
			{
				Asset = 0,
				Cooked,
				AssetIntrinsic,
				CookedIntrinsic,

				Count,
			};
		};

		struct EDatabase
		{
			enum Type : uint8
			{
				Asset = 0,
				Descriptor,
				File,
				Time,

				Count
			};
		};

		struct EThread
		{
			enum Type : uint8
			{
				LoadingThreadLowPriority = 0,
				LoadingThreadNormalPriority,
				LoadingThreadHighPriority,
				UnloadingThread,
				CookingThread,

				Count,
			};		
		};

	private:
		AssetManager();
		~AssetManager();

		void Initialize	();
		void Update		();
		void Release	();
	
	public:
		struct EEvent
		{
			enum Type : uint8
			{
				CookingStarted = 0,
				CookingFinished,
				LoadingStarted,
				LoadingFinished,
			};
		};

	#if defined(GORILLA_EDITOR)
		SIGNAL_SENDER(AssetChanged, uint8 /*_eEvent*/, Asset* /*_pAsset*/, bool /*_bSucceeded*/);

		void					SetPath(const char* _szPath);
		inline const String&	GetPath() const { return m_aPath[EPath::Asset]; }
	#endif

		template <class T>
		inline AssetHandle<T>		Get				(const char* _szRelativeFilePath, const char* _szParam = nullptr);
		inline AssetHandle<Asset>	Get				(const Class* _pClass, const char* _szRelativeFilePath, const char* _szParam = nullptr);


		const AssetDescriptor* GetDescriptor(const char* szClass) const { return GetDescriptor(ASSET_CLASS(szClass)); }
		const AssetDescriptor* GetDescriptor(const Class* _pClass) const;

		template <class T>
		AssetHandle<T>			Create();
		bool					FormatToRelative(String& _sPath);
		bool					FormatToAbsolute(String& _sPath);

	private:
		Asset*					GetInternal		(const Class* _pClass, const char* _szRelativeFilePath, const char* _szParam, bool _bSerialize);
		void					AddLink			(const char* _szRelativeFilePath, Asset* _pAsset);

		void					PushLoading		(Asset* pAsset, Thread::EPriority::Type ePriority);
		void					ClearLoading	(Thread::EPriority::Type ePriority);

		void					PushUnloading	(Asset* pAsset);
		void					ClearUnloading	();

		void					PushCooking		(AssetDescriptor* _pAssetDescriptor, Asset* _pAsset);
		void					ClearCooking	();

		template <typename TYPE>
		inline TYPE*			GetTable(EDatabase::Type _eDatabase, uint32 _uiTableId) { return static_cast<Database<TYPE>*>(m_aDatabase[_eDatabase])->GetTable(_uiTableId); }
		
		template <typename TYPE>
		inline TYPE*			GetOrCreateTable(EDatabase::Type _eDatabase, uint32 _uiTableId) { return static_cast<Database<TYPE>*>(m_aDatabase[_eDatabase])->GetOrCreateTable(_uiTableId); }

		// IFileWatcher
		virtual void			OnFileChanged(FileWatcher::Notification::Type _eType, const char* _szDirectoryPath, const char* _szRelativePath) override;

		inline void PushEvent(EEvent::Type _eType, Asset* _pAsset, bool _bSucceeded)
		{
		#if defined(GORILLA_EDITOR)
			Event kEvent(_eType, _pAsset, _bSucceeded);
			m_kMutexEvent.Lock();
			m_vEvent.Add(kEvent);
			m_kMutexEvent.Unlock();
		#else
			UNUSED(_eType, _pAsset, _bSucceeded);
		#endif
		}

		// Loading
	private:
		
		static void				LoadingThreadHighPriorityEntry	(void* _pData);
		static void				LoadingThreadNormalPriorityEntry(void* _pData);
		static void				LoadingThreadLowPriorityEntry	(void* _pData);
		static void				UnloadingThreadEntry			(void* _pData);

		void					LoadingLoop			(Thread::EPriority::Type _ePriotity);
		bool					Load				(Asset* _pAsset);
		void					UnloadingLoop		();

		// Cooking
	#if defined(GORILLA_EDITOR)
		static void				CookingThreadEntry				(void* _pData);
		void					CookingLoop			();
		bool					Cook				(AssetDescriptor* _pDescriptor, Asset* _pAsset);
	#endif

	private:
		String					m_aPath[EPath::Count];
		void*					m_aDatabase[EDatabase::Count];
		uint32					m_aThreadId[EThread::Count];

		// Loading
		Vector<Asset*>			m_aAssetToLoad[Thread::EPriority::Count];
		Mutex					m_aMutexAssetToLoad[Thread::EPriority::Count];
		ConditionVariable		m_aConditionAssetToLoad[Thread::EPriority::Count];
		
		// Unloading
		Vector<Asset*>			m_vAssetToUnload;
		Mutex					m_kMutexAssetToUnload;
		ConditionVariable		m_kConditionAssetToUnload;

	#if defined(GORILLA_EDITOR)
		// Cooking
		Vector<Pair<AssetDescriptor*, Asset*>> m_vAssetToCook;
		Mutex					m_kMutexAssetToCook;
		ConditionVariable		m_kConditionAssetToCook;
	
		// Event
		struct Event
		{
			Event() { }
			Event(EEvent::Type _eType, Asset* _pAsset, bool _bSucceeded) : Type(_eType), Asset(_pAsset), Succeeded(_bSucceeded) { }
			EEvent::Type		Type;
			AssetHandle<Asset>	Asset;
			bool				Succeeded;
		};			
		Vector<Event>	m_vEvent;
		Mutex			m_kMutexEvent;
	#endif

		bool m_bIsRunning;
	};

	//!	@brief		Get
	//!	@date		2015-11-21
	template <class T>
	AssetHandle<T> AssetManager::Get(const char* _szRelativeFilePath, const char* _szParam /*= nullptr*/)
	{
		T* pAsset = static_cast<T*>(GetInternal(T::Class::GetInstance(), _szRelativeFilePath, _szParam, false));
		return AssetHandle<T>(pAsset);
	}

	//!	@brief		Get
	//!	@date		2015-11-21
	AssetHandle<Asset> AssetManager::Get(const Class* _pClass, const char* _szRelativeFilePath, const char* _szParam /*= nullptr*/)
	{
		Asset* pAsset = GetInternal(_pClass, _szRelativeFilePath, _szParam, false);
		return AssetHandle<Asset>(pAsset);
	}


	//!	@brief		Create
	//!	@date		2015-11-21
	template <class T>
	AssetHandle<T> AssetManager::Create()
	{
		T* pAsset = static_cast<T*>(T::Class::GetInstance()->New());
		pAsset->SetState(Asset::EState::Loaded);
		return AssetHandle<T>(pAsset);
	}

}}	// Gorilla::Engine

#endif