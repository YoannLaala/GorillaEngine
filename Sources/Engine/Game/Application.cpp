/******************************************************************************
**	Includes
******************************************************************************/
#include "Application.hpp"

// Core
#include <Core/File/FileManager.hpp>

// Engine
#include <Engine/View.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Asset/AssetManager.hpp>
#include <Engine/World.hpp>

// Renderer
#include <Renderer/Viewport.hpp>

// Component
#include <Component/Node.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Game
{
	//!	@brief		Constructor
	//!	@date		2015-04-08
	Application::Application()
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-04-08
	Application::~Application()
	{
		// Nothing to do
	}
	
	//!	@brief		Run
	//!	@date		2015-04-08
	void Application::Run()
	{
		Component::Node::Class::GetInstance();	// Fore Component Loading

		// Read Project file
		Dictionary dProject;
		if(!dProject.Read<DictionaryStream>("Core.bin"))
		{
			LOG_ERROR("Failed to read project file");
		}

		// Create the view
		const char* szProjectName = dProject["Name"].GetString();
		Engine::View* pView = CreateView(szProjectName, 1280, 720, Renderer::Viewport::EFilter::Gui);
		pView->Show();

		// Load first world
		const char* szWorldPath = dProject["World"].GetString();
		Engine::AssetHandle<Engine::World> hWorld = GetAssetManager()->Get<Engine::World>(szWorldPath);
		GetEngine()->AddWorld(hWorld);

		Engine::Application::Run();
	}
}}