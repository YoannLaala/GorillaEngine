#ifndef _COOKER_REFLECTION_COOKER_HPP_
#define _COOKER_REFLECTION_COOKER_HPP_

/******************************************************************************
**	Include
******************************************************************************/
#include <Helper/AssetCooker.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class ClangCooker : public AssetCooker
	{
	public:
		virtual bool	Serialize	(const char* _szInputPath, FileWriter& _kOutputStream) override;
	};
}
#endif