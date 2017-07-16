/******************************************************************************
**	Includes
******************************************************************************/
#include "Enum.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Helper
{
	//!	@brief		GetEnumValue
	//!	@date		2015-11-22
	uint8 GetEnumValue(const char* _szName, const uint8* _pArrayValue, const char** _ppArrayName, uint32 _uiCount)
	{
		for(uint32 uiElement = 0; uiElement < _uiCount; ++uiElement)
		{
			if(strcmp(_szName, _ppArrayName[uiElement]) == 0)
			{
				return _pArrayValue[uiElement];
			}
		}

		return (uint8)-1;
	}
}}
