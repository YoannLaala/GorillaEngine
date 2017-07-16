/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Database/Table.hpp>
#include <Core/Stream/StreamReader.hpp>
#include <Core/Stream/StreamWriter.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine 
{
	//!	@brief		Deserialize
	//!	@date		2015-11-21
	bool Table::Deserialize(StreamReader* _pReader)
	{
		uint32 uiRowCount;
		_pReader->Read(uiRowCount);
					
		// Read all row
		for(uint32 uiRow = 0; uiRow < uiRowCount; ++uiRow)
		{
			uint32 uiRowId;
			_pReader->Read(uiRowId);

			uint64 uiRowValue;
			_pReader->Read(uiRowValue);

			SetValue(uiRowId, uiRowValue);
		}

		return true;
	}

	//!	@brief		Serialize
	//!	@date		2015-11-21
	bool Table::Serialize(StreamWriter* _pWriter)
	{		
		_pWriter->Write(m_mRow.GetSize());
		HashMap<uint32, uint64>::Iterator itRow = GetFirstRow();
		HashMap<uint32, uint64>::Iterator itRowLast = GetLastRow();
		while(itRow != itRowLast)
		{
			_pWriter->Write(itRow.GetKey());
			_pWriter->Write(*itRow);
			++itRow;
		}
		return true;
	}
}}