#ifndef _ENGINE_COMMON_HPP_
#define _ENGINE_COMMON_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>
#include <Core/Class/ClassManager.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	namespace EMemoryDomain
	{
		enum Type : uint8
		{
			Core = 0,
			Debug,
		};
	};

	namespace EClass
	{
		enum Type : uint8
		{
			Asset = ClassManager::EClass::Count,
			Component,
		};
	};

	namespace EEnginePath
	{
		enum Type
		{
			Source = 0,
			VisualStudio,
			Project,
			Asset,
			Cooked,
			Temporary,
			Count,
		};
	}

	namespace ERenderPass
	{
		enum Type
		{
			Gui = 0,
			Geometry,
			Light,
			Shadow,
			PostProcess,

			Count,
		};
	}
}}	

#endif