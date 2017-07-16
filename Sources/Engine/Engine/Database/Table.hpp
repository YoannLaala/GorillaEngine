#ifndef _ENGINE_TABLE_HPP_
#define _ENGINE_TABLE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>
#include <Core/Thread/Mutex.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla
{
	class StreamReader;
	class StreamWriter;
}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class Table
	{
	public:	
		inline uint64	GetValue(uint32 _uiRowId) const { return m_mRow[_uiRowId]; }
		inline void		SetValue(uint32 _uiRowId, uint64 _uiValue) { m_kMutex.Lock(); m_mRow.Add(_uiRowId, _uiValue); m_kMutex.Unlock(); }

		bool	Deserialize(StreamReader* _pReader);
		bool	Serialize(StreamWriter* _pWriter);

		inline HashMap<uint32, uint64>::Iterator GetFirstRow() { return m_mRow.GetFirst(); }
		inline HashMap<uint32, uint64>::Iterator GetLastRow() { return m_mRow.GetLast(); }

	private:
		HashMap<uint32, uint64> m_mRow;
		Mutex m_kMutex;
	};
}}

#endif