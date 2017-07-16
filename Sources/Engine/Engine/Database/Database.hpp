#ifndef _ENGINE_DATABASE_HPP_
#define _ENGINE_DATABASE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>
#include <Core/Thread/Mutex.hpp>
#include <Core/Stream/FileReader.hpp>
#include <Core/Stream/FileWriter.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	template <typename TYPE>
	class Database
	{
	public:
		Database();
		~Database();

		bool Deserialize(const char* _szFilePath);
		bool Serialize(const char* _szFilePath);

		TYPE* GetTable(uint32 _uiTableId);
		TYPE* GetOrCreateTable(uint32 _uiTableId);
		
	private:
		HashMap<uint32, TYPE*> m_mTable;
		Mutex m_kMutex;
	};

	//!	@brief		Contructor
	//!	@date		2015-11-21
	template <typename TYPE>
	Database<TYPE>::Database()
	{
		//Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-11-21
	template <typename TYPE>
	Database<TYPE>::~Database()
	{
		//Nothing to do
	}
	
	//!	@brief		GetTable
	//!	@date		2015-11-21
	template <typename TYPE>
	TYPE* Database<TYPE>::GetTable(uint32 _uiTableId)
	{
		return m_mTable[_uiTableId];
	}

	//!	@brief		GetOrCreateTable
	//!	@date		2015-11-21
	template <typename TYPE>
	TYPE* Database<TYPE>::GetOrCreateTable(uint32 _uiTableId)
	{
		TYPE* pTable = GetTable(_uiTableId);
		if (!pTable)
		{
			pTable = new TYPE();

			m_kMutex.Lock();
			m_mTable.Add(_uiTableId, pTable);
			m_kMutex.Unlock();
		}

		return pTable;
	}

	//!	@brief		Deserialize
	//!	@date		2015-11-21
	template <typename TYPE>
	bool Database<TYPE>::Deserialize(const char* _szFilePath)
	{
		// Write default database
		FileReader kStream(_szFilePath);
		if(kStream.CanRead())
		{
			uint32 uiTableCount;
			kStream.Read(uiTableCount);

			// Read all table
			for(uint32 uiTable = 0; uiTable < uiTableCount; ++uiTable)
			{
				uint32 uiTableId;
				kStream.Read(uiTableId);

				TYPE* pTable = GetOrCreateTable(uiTableId);
				if(!pTable->Deserialize(&kStream))
				{
					return false;
				}
			}

			return true;
		}

		return false;		
	}

	//!	@brief		Serialize
	//!	@date		2015-11-21
	template <typename TYPE>
	bool Database<TYPE>::Serialize(const char* _szFilePath)
	{
		// Write default database
		FileWriter kStream(_szFilePath);
		if(kStream.CanWrite())
		{
			kStream.Write(m_mTable.GetSize());
			HashMap<uint32, TYPE*>::Iterator itTable = m_mTable.GetFirst();
			HashMap<uint32, TYPE*>::Iterator itTableLast = m_mTable.GetLast();
			while(itTable != itTableLast)
			{
				kStream.Write(itTable.GetKey());
				if(!itTable->Serialize(&kStream))
				{
					return false;
				}
				++itTable;
			}

			return true;
		}

		return false;
	}
}}

#endif