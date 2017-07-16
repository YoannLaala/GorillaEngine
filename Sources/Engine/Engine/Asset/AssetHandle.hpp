#ifndef _ENGINE_HANDLE_HPP_
#define _ENGINE_HANDLE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "Asset.hpp"

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	template <class T>
	class AssetHandle
	{
	public:
		AssetHandle();
		AssetHandle(const AssetHandle<T>& _hAsset);
		AssetHandle(T* _pAsset);
		~AssetHandle();

		void Release();

		inline bool  HasValue() const { return m_pValue != nullptr; }
		inline bool  IsLoading() const { return HasValue() && !m_pValue->IsLoaded(); }
		inline bool  IsLoaded() const { return HasValue() && m_pValue->IsLoaded(); }

		AssetHandle<T>&	operator=(const AssetHandle<T>& _hAsset);
		inline T*		operator->() const { while(IsLoading()) {} return m_pValue; } 

	private:
		T*	m_pValue;
	};

	//!	@brief		Constructor
	//!	@date		2015-04-04
	template <class T>
	AssetHandle<T>::AssetHandle()
		: m_pValue(nullptr)
	{
		// Nothing to do
	}

	//!	@brief		Constructor
	//!	@date		2015-04-04
	template <class T>
	AssetHandle<T>::AssetHandle(const AssetHandle<T>& _hAsset)
		: m_pValue(_hAsset.m_pValue)
	{
		m_pValue->AddReference();
	}

	//!	@brief		Constructor
	//!	@date		2015-04-04
	template <class T>
	AssetHandle<T>::AssetHandle(T* _pAsset)
		: m_pValue(_pAsset)
	{
		m_pValue->AddReference();
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	template <class T>
	AssetHandle<T>::~AssetHandle()
	{
		Release();
	}

	//!	@brief		Release
	//!	@date		2015-04-04
	template <class T>
	void AssetHandle<T>::Release()
	{
		if(m_pValue)
		{
			m_pValue->RemoveReference();
			if(!m_pValue->IsReferenced())
			{
				Asset* pAsset = static_cast<Asset*>(m_pValue);
				SAFE_RELEASE_AND_DELETE(pAsset);
			}
			m_pValue = nullptr;
		}
	}

	//!	@brief		Opereator
	//!	@date		2015-04-04
	template <class T>
	AssetHandle<T>& AssetHandle<T>::operator=(const AssetHandle<T>& _hAsset)
	{
		m_pValue = static_cast<T*>(_hAsset.m_pValue);
		m_pValue->AddReference();
		
		return *this;
	}
}}	// Gorilla::Asset
#endif