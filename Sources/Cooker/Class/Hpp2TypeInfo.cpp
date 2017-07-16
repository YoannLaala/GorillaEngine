/******************************************************************************
**	Includes
******************************************************************************/
#include "Hpp2TypeInfo.hpp"
#include <regex>

namespace Gorilla
{
	Hpp2TypeInfo::Hpp2TypeInfo()
	{

	}

	bool Hpp2TypeInfo::Execute()
	{
		//C:\Users\KOJIMAH\Downloads\ctags58\ctags58>ctags.exe --c++-kinds=+ --format=2 --fields=+aiKz-s --sort=no -f "_a.txt" F:\projects\GorillaEngine\Sources\GameEngine\Component\Mesh\MeshComponent.hpp	
		std::regex kRegExVariable("^(?<Name>.*)\t.*\.hpp\t\/\^(?:\t+)(?<Type>.*) .*\1;(?<Comment>.*)\$.*kind:member.*access:(?<Access>.*)$"); 
		std::sregex_iterator itVariable = std::sregex_iterator(s.begin(), s.end(), kRegExVariable);
		std::sregex_iterator itEnd = std::sregex_iterator();
 
		while(itVariable != itEnd)
		{
			std::smatch kMatch = *itVariable;

			++itVariable;
		}

		return true;
	}

	Hpp2TypeInfo::~Hpp2TypeInfo()
	{

	}
}