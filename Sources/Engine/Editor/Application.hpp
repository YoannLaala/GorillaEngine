#ifndef _EDITOR_APPLICATION_HPP_
#define _EDITOR_APPLICATION_HPP_

/******************************************************************************
**	Include
******************************************************************************/
#include <Engine/Application.hpp>

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
namespace Gorilla { namespace Editor
{
	class Application : public Engine::Application
	{		
	public:
		Application();
		~Application();

		virtual void Run() override;

	private:
		Engine::World* m_pWorld;
	};
}}

#endif