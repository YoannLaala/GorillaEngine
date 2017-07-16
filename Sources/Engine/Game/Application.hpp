#ifndef _GAME_APPLICATION_HPP_
#define _GAME_APPLICATION_HPP_

/******************************************************************************
**	Include
******************************************************************************/
#include <Engine/Application.hpp>
#include <Engine/Asset/AssetHandle.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class World;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Game
{
	class Application : public Engine::Application
	{		
	public:
		Application();
		~Application();

		virtual void Run() override;

	private:
		Engine::AssetHandle<Engine::World> m_hWorld;
	};
}}

#endif