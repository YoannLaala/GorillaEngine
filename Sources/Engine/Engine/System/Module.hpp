#pragma once

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla 
{
	class Class;
}

namespace Gorilla { namespace Engine
{
	class Engine;
}}

/******************************************************************************
**	Mofule functions
******************************************************************************/
typedef Gorilla::HashMap<Gorilla::uint32, Gorilla::Class*>& (*fnGetAllComponentClass)();
typedef void (*fnInitializeEngineModule)(Gorilla::Engine::Engine*);
#ifdef GORILLA_GAME
	extern "C" __declspec(dllexport) const Gorilla::HashMap<Gorilla::uint32, Gorilla::Class*>& GetAllComponentClass();
	extern "C" __declspec(dllexport) void InitializeEngineModule(Gorilla::Engine::Engine* _pEngine);
#else
	void InitializeSymbol();
#endif