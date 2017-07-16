/******************************************************************************
**	Includes
******************************************************************************/
#include "Application.hpp"

#include <Engine/Engine.hpp>
#include <Engine/Asset/AssetManager.hpp>
#include <Engine/World.hpp>
#include <Engine/Object/GameObject.hpp>
#include <Engine/Renderer/Pass/PrimitivePass.hpp>
#include <Engine/View.hpp>

#include <Renderer/Renderer.hpp>
#include <Renderer/RenderContext.hpp>
#include <Renderer/Viewport.hpp>
#include <Renderer/Resource/RenderTarget.hpp>

#include <Component/Node.hpp>
#include <Component/Mesh/Mesh.hpp>
#include <Component/Camera/Camera.hpp>
#include <Component/Light/DirectionalLight.hpp>
#include <Component/Light/PointLight.hpp>
#include <Component/Gui/Sprite.hpp>
#include <Component/Script/CameraFree.hpp>
#include <Component/Environment/EnvironmentMap.hpp>
#include <Component/Environment/Decal.hpp>

#include <Core/Time/Time.hpp>


#include <Core/Time/Profiler.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Viewer
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
		Engine::View* pView = CreateView("Viewer", 1280, 720);
		
		Gorilla::uint32 LAYER_CAMERA1 = Gorilla::GetEngine()->AddLayer("camera1");

		// Create world and objects
		Gorilla::Engine::World* pWorld = Gorilla::GetEngine()->CreateWorld();		
		Gorilla::Engine::GameObject* pScene = NULL;
		Gorilla::Engine::GameObject* pGameObject = NULL;
		Gorilla::Engine::GameObject* pGameObjectChild = NULL;
		Gorilla::Component::Node* pCpnNode = NULL;
		Gorilla::Component::Mesh* pCpnMesh = NULL;
		//pGameObject->AddComponent<Gorilla::Test>();
		//pGameObject->AddComponent<Gorilla::DebuggerCo>();

		// Default Camera
		pGameObject = pWorld->AddGameObject("Camera1");
		pGameObject->AddComponent<Gorilla::Component::Node>()->SetPosition(0.0f, 0.0f, -3.0f);
		pGameObject->AddComponent<Gorilla::Component::Camera>()->SetLayer(LAYER_CAMERA1);
		pGameObject->AddComponent<Gorilla::Component::CameraFree>();
		//pGameObject->AddComponent<Gorilla::GlobalIlluminationScene>();

		/*pGameObject = pWorld->AddGameObject("EnvironmentMap");
		pGameObject->AddComponent<Gorilla::Component::Node>()->SetPosition(0.0f, 0.35f, -3.0f);
		pGameObject->AddComponent<Gorilla::Component::EnvironmentMap>()->Asset = Gorilla::GetAssetManager()->Get<Gorilla::Engine::CubeMap>("CubeMap/Lycksele/test.dds");*/

		// Top Camera
		/*pGameObject = pWorld->AddGameObject("Camera2");
		Gorilla::Component::Camera* pCameraTop = pGameObject->AddComponent<Gorilla::Component::Camera>();
		pCameraTop->Viewport = Gorilla::GetRenderer()->CreateViewport(300, 300, Gorilla::Renderer::EFormat::R8G8B8A8_UNORM);
		Gorilla::Component::Node* pNode = pGameObject->AddComponent<Gorilla::Component::Node>();
		pNode->MoveUp(-5.0f);
		pNode->LookAt(0, 0, 0);*/

		// Sprite only visible by camera1
		/*pGameObject = pWorld->AddGameObject("Sprite");
		pGameObject->AddComponent<Gorilla::Component::Node>()->SetLayer(LAYER_CAMERA1);
		pGameObject->AddComponent<Gorilla::Component::Sprite>()->Texture = pCameraTop->Viewport->GetRenderTarget()->GetTexture(0);*/

		// Dragon
		/*pGameObject = pWorld->AddGameObject("test2");
		pCpnNode = pGameObject->AddComponent<Gorilla::Component::Node>();
		pCpnNode->SetPosition(0.0f,0.0f, 1.0f);
		Gorilla::Component::Mesh* pCpnMesh = pGameObject->AddComponent<Gorilla::Component::Mesh>();
		pCpnMesh->Asset = Gorilla::GetAssetManager()->Get<Gorilla::Engine::Mesh>("Mesh/Dragon.fbx");*/
		//pGameObject->AddComponent<Gorilla::Test>();

		// Scene
		pScene = pWorld->AddGameObject("Scene");
	
		// Light
		pGameObject = pWorld->AddGameObject("Light", pScene);
		pGameObject->AddComponent<Gorilla::Component::DirectionalLight>();
		pCpnNode = pGameObject->AddComponent<Gorilla::Component::Node>();
		pCpnNode->SetPosition(0.00f, 1.0f, 0.0f);
		pCpnNode->LookAt(0.0f, 0.0, 0.0f);

		pGameObject = pWorld->AddGameObject("Light2", pScene);
		//pGameObject->AddComponent<Gorilla::DynamicPointLight>();
		pGameObject->AddComponent<Gorilla::Component::PointLight>()->SetColor(Gorilla::Renderer::Color::Blue);
		pCpnNode = pGameObject->AddComponent<Gorilla::Component::Node>();
		pCpnNode->SetPosition(0.5f, -0.5f, 0.0f);

		Gorilla::Engine::AssetHandle<Gorilla::Engine::Material> hMaterialWhite = Gorilla::GetAssetManager()->Create<Gorilla::Engine::Material>();
		hMaterialWhite->SetShader(Gorilla::GetAssetManager()->Get<Gorilla::Engine::Shader>("@Effect/Generated/ColorMetallicRoughness.ps"));
		hMaterialWhite->SetNumber(0, Gorilla::Math::Vector3(1.0f, 1.0f, 1.0f));
		hMaterialWhite->SetNumber(3, 0.0f);	// Metallic
		hMaterialWhite->SetNumber(4, 1.0f);	// Roughness

		Gorilla::Engine::AssetHandle<Gorilla::Engine::Material> hMaterialRed = Gorilla::GetAssetManager()->Create<Gorilla::Engine::Material>();
		hMaterialRed->SetShader(Gorilla::GetAssetManager()->Get<Gorilla::Engine::Shader>("@Effect/Generated/ColorMetallicRoughness.ps"));
		hMaterialRed->SetNumber(0, Gorilla::Math::Vector3(1.0f, 0.0f, 0.0f));
		hMaterialRed->SetNumber(3, 0.0f);	// Metallic
		hMaterialRed->SetNumber(4, 1.0f);	// Roughness

		Gorilla::Engine::AssetHandle<Gorilla::Engine::Material> hMaterialGreen = Gorilla::GetAssetManager()->Create<Gorilla::Engine::Material>();
		hMaterialGreen->SetShader(Gorilla::GetAssetManager()->Get<Gorilla::Engine::Shader>("@Effect/Generated/ColorMetallicRoughness.ps"));
		hMaterialGreen->SetNumber(0, Gorilla::Math::Vector3(0.0f, 1.0f, 0.0f));
		hMaterialGreen->SetNumber(3, 0.0f);	// Metallic
		hMaterialGreen->SetNumber(4, 1.0f);	// Roughness

		// Walls
		pGameObject = pWorld->AddGameObject("Walls", pScene);
		pGameObjectChild = pWorld->AddGameObject("Far", pGameObject);
		pCpnMesh = pGameObjectChild->AddComponent<Gorilla::Component::Mesh>();
		pCpnMesh->Asset = Gorilla::GetAssetManager()->Get<Gorilla::Engine::Mesh>("@Mesh/Quad.fbx");
		pCpnMesh->Materials.Add(hMaterialWhite);
		pCpnNode = pGameObjectChild->AddComponent<Gorilla::Component::Node>();
		pCpnNode->SetScale(2.0f);
		pCpnNode->SetPosition(0.0f, 0.0f, 1.0f);
		pCpnNode->RotateRight(90.0f);
		pGameObjectChild = pWorld->AddGameObject("Left", pGameObject);
		pCpnMesh = pGameObjectChild->AddComponent<Gorilla::Component::Mesh>();
		pCpnMesh->Asset = Gorilla::GetAssetManager()->Get<Gorilla::Engine::Mesh>("@Mesh/Quad.fbx");
		pCpnMesh->Materials.Add(hMaterialRed);
		pCpnNode = pGameObjectChild->AddComponent<Gorilla::Component::Node>();
		pCpnNode->SetScale(2.0f);
		pCpnNode->SetPosition(-1.0f, 0.0f, 0.0f);
		pCpnNode->RotateForward(90.0f);
		pGameObjectChild = pWorld->AddGameObject("Right", pGameObject);
		pCpnMesh = pGameObjectChild->AddComponent<Gorilla::Component::Mesh>();
		pCpnMesh->Asset = Gorilla::GetAssetManager()->Get<Gorilla::Engine::Mesh>("@Mesh/Quad.fbx");
		pCpnMesh->Materials.Add(hMaterialGreen);
		pCpnNode = pGameObjectChild->AddComponent<Gorilla::Component::Node>();
		pCpnNode->SetScale(2.0f);
		pCpnNode->SetPosition(1.0f, 0.0f, 0.0f);
		pCpnNode->RotateForward(-90.0f);
		/*pGameObjectChild = pWorld->AddGameObject("Top", pGameObject);
		pCpnMesh = pGameObjectChild->AddComponent<Gorilla::Component::Mesh>();
		pCpnMesh->Asset = Gorilla::GetAssetManager()->Get<Gorilla::Engine::Mesh>("Mesh/Quad.fbx");
		pCpnNode = pGameObjectChild->AddComponent<Gorilla::Component::Node>();
		pCpnNode->SetScale(2.0f);
		pCpnNode->SetPosition(0.0f, 1.0f, 0.0f);
		pCpnNode->RotateRight(180.0f);*/
		pGameObjectChild = pWorld->AddGameObject("Bottom", pGameObject);
		pCpnMesh = pGameObjectChild->AddComponent<Gorilla::Component::Mesh>();
		pCpnMesh->Asset = Gorilla::GetAssetManager()->Get<Gorilla::Engine::Mesh>("@Mesh/Quad.fbx");
		pCpnMesh->Materials.Add(hMaterialWhite);
		pCpnNode = pGameObjectChild->AddComponent<Gorilla::Component::Node>();
		pCpnNode->SetScale(2.0f);
		pCpnNode->SetPosition(0.0f, -1.0f, 0.0f);

		Gorilla::Engine::AssetHandle<Gorilla::Engine::Material> hMaterialDragon = Gorilla::GetAssetManager()->Create<Gorilla::Engine::Material>();
		hMaterialDragon->SetShader(Gorilla::GetAssetManager()->Get<Gorilla::Engine::Shader>("@Effect/Generated/ColorMetallicRoughness.ps"));
		hMaterialDragon->SetNumber(0, Gorilla::Math::Vector3(0.5f, 0.5f, 0.5f));
		hMaterialDragon->SetNumber(3, 0.0f);	// Metallic
		hMaterialDragon->SetNumber(4, 1.0f);	// Roughness

		/*Gorilla::Engine::AssetHandle<Gorilla::Engine::Texture> hTexture = Gorilla::GetAssetManager()->Get<Gorilla::Engine::Texture>("Texture/Sadface.png");
		pGameObject = pWorld->AddGameObject("Decal", pScene);	
		Gorilla::Component::Decal* pCpnDecal = pGameObject->AddComponent<Gorilla::Component::Decal>();
		pCpnDecal->Asset = Gorilla::GetAssetManager()->Create<Gorilla::Engine::Material>();
		pCpnDecal->Asset->SetShader(Gorilla::GetAssetManager()->Get<Gorilla::Engine::Shader>("Decal.ps"));
		pCpnDecal->Asset->SetTexture(0, hTexture);*/

		// Object 
		pGameObject = pWorld->AddGameObject("Object", pScene);	
		pCpnMesh = pGameObject->AddComponent<Gorilla::Component::Mesh>();
		pCpnMesh->Asset = Gorilla::GetAssetManager()->Get<Gorilla::Engine::Mesh>("@Mesh/Cube.fbx");
		pCpnMesh->Materials.Add(hMaterialDragon);
		pCpnNode = pGameObject->AddComponent<Gorilla::Component::Node>();
		//pCpnNode->SetScale(0.01f);
		//pCpnNode->SetPosition(-0.25f, -0.5f, 0.0f);

		//Gorilla::Engine::Material* pMaterial = new Gorilla::Engine::Material();
		//pMaterial->SetShader(Gorilla::GetAssetManager()->Get<Gorilla::Engine::Shader>("Effect/FakeGenerated.ps"));
	
		//pCpnMesh->Materials.Add(pMaterial);
		pView->Show();
		Engine::Application::Run();
	}
}}