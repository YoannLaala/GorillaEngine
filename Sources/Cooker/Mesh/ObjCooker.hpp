#ifndef _COOKER_OBJ_COOKER_HPP_
#define _COOKER_OBJ_COOKER_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Helper/AssetCooker.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla
{
	struct Geometry;
}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class ObjCooker : public AssetCooker
	{
	private:
		virtual bool	Serialize			(const char* _szInputPath, FileWriter& _kOutputStream) override;
	};
}

#endif
