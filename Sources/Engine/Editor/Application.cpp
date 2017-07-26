/******************************************************************************
**	Includes
******************************************************************************/
#include "Application.hpp"

// Engine
#include <Engine/View.hpp>
#include <Engine/Engine.hpp>
#include <Engine/World.hpp>
#include <Engine/Object/GameObject.hpp>

// Component
#include <Component/Camera/Camera.hpp>

// Renderer
#include <Renderer/RenderContext.hpp>
#include <Renderer/Viewport.hpp>
#include <Renderer/Resource/RenderTarget.hpp>

// Editor
#include <Editor/Component/Controller/EditorController.hpp>

#include <Core/Time/Profiler.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Editor
{
	namespace EView
	{
		enum Type
		{
			Editor = 0,
			Material,

			Count,
		};

		static const char* Name[EView::Count] = 
		{
			"Editor",
			"Material"
		};
	};

	//!	@brief		Constructor
	//!	@date		2015-04-08
	Application::Application()
		: m_pWorld(nullptr)
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
		Engine::View* pView = CreateView("Gorilla Editor", 1280, 720, Renderer::EFlag::EmptyPass);


		// Create the world and add the Editor GameObject
		m_pWorld = GetEngine()->CreateWorld();

		// Editor specific (Gui)
		Engine::GameObject* pGoEditor = m_pWorld->AddGameObject("Editor");
		pGoEditor->AddComponent<Component::Camera>()->Viewport = pView->GetViewport();
		pGoEditor->AddComponent<EditorController>()->View = pView;

		Engine::Application::Run();
	}
}}