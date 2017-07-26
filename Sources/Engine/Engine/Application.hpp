#ifndef _ENGINE_APPLICATION_HPP_
#define _ENGINE_APPLICATION_HPP_

/******************************************************************************
**	Include
******************************************************************************/
#include <Core/Core.hpp>
#include <Engine/Asset/System/Module.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class Engine;
	class View;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class Application
	{
	public:
		Application();
		~Application();

		virtual void Run();

	protected:
		View*	CreateView(const char* _szName, uint32 _uiWidth, uint32 _uiHeight, uint8 _eFlag = 0);
	};
}}

#endif