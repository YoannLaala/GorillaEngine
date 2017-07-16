#ifndef _ENGINE_ENGINE_HPP_
#define _ENGINE_ENGINE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>
#include <Core/Stream/StreamReader.hpp>
#include <Core/Class/ClassManager.hpp>
#include <Engine/Common.hpp>

/******************************************************************************
**	Define
******************************************************************************/
#define ASSET_DECLARE(_type_) CLASS_DECLARE_IMPL(Gorilla::Engine::EClass::Asset, _type_, 0)
#define ASSET_REGISTER(_type_) CLASS_REGISTER(_type_)
#define ASSET_CLASS(_name_) Gorilla::ClassManager::GetInstance()->Get(Gorilla::Engine::EClass::Asset, _name_)
#define ASSET_INTRINSIC_CHARACTER '@'

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class AssetAttribute;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class Asset
	{
	public:
		CLASS_DECLARE_VIRTUAL()

	private:
		friend class AssetManager;

		template <class T>
		friend class AssetHandle;

		friend class AssetAttribute;

		struct EState
		{
			enum Type : uint8
			{
				Loading = 0,
				Loaded,
				Unloading,
				Unloaded,
				Cooking,
				Reloading,
			};
		};

		struct EFlag
		{
			enum Type : uint8
			{
				Intrinsic	= 1 << 0,
				External	= 1 << 1,
			};
		};

	protected:
		Asset();
		~Asset();

		//virtual const Gorilla::Engine::Class* GetClass() const = 0;

	protected:
		virtual void 			Initialize		(StreamReader* _pStream) = 0;
		virtual void			Release			() = 0;

	public:
		inline uint32			GetId() const { return m_uiId; }
		inline const String&	GetName() const { return m_sName; }

	protected:
		inline const String&	GetFilePath() const { return m_sFilePath; }

		template <class T>
		void ReadAsset(StreamReader* _pStream, AssetHandle<T>& _hAsset);
		void AddLink(const char* _szRelativeFilePath);

	private:
		inline void				SetId				(uint32 _uiId) { m_uiId = _uiId; }
		inline void				SetFilePath			(const char* _szFilePath) { m_sFilePath = _szFilePath; }
		inline void				SetName				(const char* _szName) { m_sName = _szName; }
		
		inline EState::Type		GetState			() const { return m_eState; }
		inline void				SetState			(EState::Type _eState) { m_eState = _eState; }

		inline bool				HasFlag				(EFlag::Type _eFlag) const { return (m_eFlag & _eFlag) != 0; }
		inline void				SetFlag				(EFlag::Type _eFlag) { m_eFlag |= _eFlag; }
		inline void				RemoveFlag			(EFlag::Type _eFlag) { m_eFlag &= ~_eFlag; }

		inline void				AddReference		() { ++m_uiReferenceCount; }
		inline void				RemoveReference		() { --m_uiReferenceCount; }
		inline bool				IsReferenced		() const { return m_uiReferenceCount != 0; }

		inline bool				IsLoaded			() const { return m_eState == EState::Loaded; }

		void					ReadAssetInternal	(const Class* _pClass, const char* _szFilePath, const char* _szParameter, AssetHandle<Asset>& _hAsset);

	private:
		String			m_sFilePath;
		String			m_sName;
		uint32			m_uiId;
		uint32			m_uiReferenceCount;
		EState::Type	m_eState;
		uint8			m_eFlag;

	public:
		inline const String&	GetSourcePath() const { return m_sSourcePath; }
		inline const String&	GetParam() const { return m_sParam; }

	private:
		inline void				SetSourcePath(const char* _szSourcePath, const char* _szParam) { m_sSourcePath = _szSourcePath; m_sParam = _szParam; }

	private:
		String	m_sSourcePath;
		String	m_sParam;		
	};

	//!	@brief		ReadAsset
	//!	@date		2015-04-04
	template <class T>
	void Asset::ReadAsset(StreamReader* _pStream, AssetHandle<T>& _hAsset)
	{
		String sFilePath, sParam;
		_pStream->Read(sFilePath);
		_pStream->Read(sParam);

		if(HasFlag(EFlag::Intrinsic)) sFilePath.Prepend(ASSET_INTRINSIC_CHARACTER);
		ReadAssetInternal(T::Class::GetInstance(), sFilePath.GetBuffer(), sParam.GetBuffer(), (AssetHandle<Asset>&)_hAsset);
	}


}}	// Gorilla::Data
#endif