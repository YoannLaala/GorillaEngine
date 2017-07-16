#ifndef _VIEWER_APPLICATION_HPP_
#define _VIEWER_APPLICATION_HPP_

/******************************************************************************
**	Include
******************************************************************************/
#include <Engine/Application.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Viewer
{
	class Application : public Engine::Application
	{		
	public:
		Application();
		~Application();

		virtual void Run() override;
	};
}}

#endif