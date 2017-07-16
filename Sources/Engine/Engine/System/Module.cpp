/******************************************************************************
**	Includes
******************************************************************************/
#include "Module.hpp"
#include <Engine/Engine.hpp>

/******************************************************************************
**	Mofule functions
******************************************************************************/
#ifdef GORILLA_GAME
	const Gorilla::HashMap<Gorilla::uint32, Gorilla::Class*>& GetAllComponentClass()
	{					
		return Gorilla::ClassManager::GetInstance()->GetAll(Gorilla::Engine::EClass::Component);
	}
	void InitializeEngineModule(Gorilla::Engine::Engine* _pEngine)
	{					
		Gorilla::Engine::Engine::SetInstance(_pEngine);
	}
#else
	void InitializeSymbol() { }
#endif