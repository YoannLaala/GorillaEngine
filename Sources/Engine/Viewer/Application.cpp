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
#include <Component/Script/Camera/CameraFree.hpp>
#include <Component/Script/Camera/CameraOrbital.hpp>
#include <Component/Environment/EnvironmentMap.hpp>
#include <Component/Environment/Decal.hpp>

#include <Core/Time/Time.hpp>


#include <Core/Time/Profiler.hpp>

using namespace Gorilla;

namespace Gorilla
{
	class DynamicPointLight : public Gorilla::Engine::Component
	{
	public:
		COMPONENT_DECLARE(DynamicPointLight);

		virtual void Start() override
		{
			Gorilla::Component::Node* pCpnNode = GetOrCreate<Gorilla::Component::Node>();
			pCpnNode->SetPosition(Math::Cos(0.0f) * 0.7f, Math::Sin(0.0f) * 0.7f, Math::Sin(0.0f) * 0.7f);
		}

		virtual void Update() override
		{
			Gorilla::Component::Node* pCpnNode = GetOrCreate<Gorilla::Component::Node>();
			float32 fTime = GetTime()->GetToltalTime();
			pCpnNode->SetPosition(Math::Cos(fTime) * 0.7f, Math::Sin(fTime) * 0.7f, Math::Sin(fTime) * 0.7f);
			//pCpnNode->RotateY(GetTime()->GetDeltaTime());

			GetRenderer()->GetGizmo()->SetIdentity();
			GetRenderer()->GetGizmo()->SetPosition(pCpnNode->GetPosition());
			GetRenderer()->GetGizmo()->PushCube();
		}
	};
	COMPONENT_REGISTER(DynamicPointLight);

	//class GlobalIlluminationScene : public Gorilla::Engine::Component
	//{
	//public:
	//	COMPONENT_DECLARE(GlobalIlluminationScene);

	//	virtual void Update() override
	//	{
	//		//GetRenderer()->GetGizmo()->PushQuad();
	//	}
	//};
	//COMPONENT_REGISTER(GlobalIlluminationScene);
}

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
		GetAssetManager()->SetPath(nullptr);
		uint32 LAYER_CAMERA1 = GetEngine()->AddLayer("camera1");

		// Create world and objects
		Engine::World* pWorld = GetEngine()->CreateWorld();		
		Engine::GameObject* pScene = nullptr;
		Engine::GameObject* pGameObject = nullptr;
		Engine::GameObject* pGameObjectChild = nullptr; pGameObjectChild;
		Component::Node* pCpnNode = nullptr;
		Component::Mesh* pCpnMesh = nullptr;
		//pGameObject->AddComponent<Test>();
		//pGameObject->AddComponent<DebuggerCo>();

		// Default Camera
		pGameObject = pWorld->AddGameObject("Camera1");
		Component::Camera* pCpnCamera = pGameObject->AddComponent<Component::Camera>();
		pCpnCamera->Viewport = pView->GetViewport();
		pCpnCamera->SetLayer(LAYER_CAMERA1);
		//pGameObject->AddComponent<Component::CameraFree>();
		Component::CameraOrbital* pCpnOrbitalCamera = pGameObject->AddComponent<Component::CameraOrbital>();
		pGameObject->AddComponent<Component::Node>()->SetPosition(0.0f, 0.0f, -3.0f);
		//pGameObject->AddComponent<GlobalIlluminationScene>();

		/*pGameObject = pWorld->AddGameObject("EnvironmentMap");
		pGameObject->AddComponent<Component::Node>()->SetPosition(0.0f, 0.35f, -3.0f);
		pGameObject->AddComponent<Component::EnvironmentMap>()->Asset = GetAssetManager()->Get<Engine::CubeMap>("CubeMap/Lycksele/test.dds");*/

		// Top Camera
		/*pGameObject = pWorld->AddGameObject("Camera2");
		Component::Camera* pCameraTop = pGameObject->AddComponent<Component::Camera>();
		pCameraTop->Viewport = GetRenderer()->CreateViewport(300, 300, Renderer::EFormat::R8G8B8A8_UNORM);
		Component::Node* pNode = pGameObject->AddComponent<Component::Node>();
		pNode->MoveUp(-5.0f);
		pNode->LookAt(0, 0, 0);*/

		// Sprite only visible by camera1
		/*pGameObject = pWorld->AddGameObject("Sprite");
		pGameObject->AddComponent<Component::Node>()->SetLayer(LAYER_CAMERA1);
		pGameObject->AddComponent<Component::Sprite>()->Texture = pCameraTop->Viewport->GetRenderTarget()->GetTexture(0);*/

		// Dragon
		/*pGameObject = pWorld->AddGameObject("test2");
		pCpnNode = pGameObject->AddComponent<Component::Node>();
		pCpnNode->SetPosition(0.0f,0.0f, 1.0f);
		Component::Mesh* pCpnMesh = pGameObject->AddComponent<Component::Mesh>();
		pCpnMesh->Asset = GetAssetManager()->Get<Engine::Mesh>("Mesh/Dragon.fbx");*/
		//pGameObject->AddComponent<Test>();

		// Scene
		pScene = pWorld->AddGameObject("Scene");
	
		// Light
		pGameObject = pWorld->AddGameObject("Light", pScene);
		pGameObject->AddComponent<Component::DirectionalLight>();
		pCpnNode = pGameObject->AddComponent<Component::Node>();
		pCpnNode->SetPosition(0.00f, 1.0f, 0.0f);
		pCpnNode->LookAt(0.0f, 0.0, 0.0f);

		pGameObject = pWorld->AddGameObject("Light2", pScene);
		pGameObject->AddComponent<DynamicPointLight>();
		pGameObject->AddComponent<Component::PointLight>();
		pCpnNode = pGameObject->AddComponent<Component::Node>();
		pCpnNode->SetPosition(0.5f, -0.5f, 0.0f);

		Engine::AssetHandle<Engine::Material> hMaterialWhite = GetAssetManager()->Create<Engine::Material>();
		hMaterialWhite->SetShader(GetAssetManager()->Get<Engine::Shader>("@Effect/Generated/ColorMetallicRoughness.ps"));
		hMaterialWhite->SetNumber(0, Math::Vector3(1.0f, 1.0f, 1.0f));
		hMaterialWhite->SetNumber(3, 0.0f);	// Metallic
		hMaterialWhite->SetNumber(4, 1.0f);	// Roughness

		Engine::AssetHandle<Engine::Material> hMaterialRed = GetAssetManager()->Create<Engine::Material>();
		hMaterialRed->SetShader(GetAssetManager()->Get<Engine::Shader>("@Effect/Generated/ColorMetallicRoughness.ps"));
		hMaterialRed->SetNumber(0, Math::Vector3(1.0f, 0.0f, 0.0f));
		hMaterialRed->SetNumber(3, 0.0f);	// Metallic
		hMaterialRed->SetNumber(4, 1.0f);	// Roughness

		Engine::AssetHandle<Engine::Material> hMaterialGreen = GetAssetManager()->Create<Engine::Material>();
		hMaterialGreen->SetShader(GetAssetManager()->Get<Engine::Shader>("@Effect/Generated/ColorMetallicRoughness.ps"));
		hMaterialGreen->SetNumber(0, Math::Vector3(0.0f, 1.0f, 0.0f));
		hMaterialGreen->SetNumber(3, 0.0f);	// Metallic
		hMaterialGreen->SetNumber(4, 1.0f);	// Roughness

		// Walls
		pGameObject = pWorld->AddGameObject("Walls", pScene);
		pGameObjectChild = pWorld->AddGameObject("Far", pGameObject);
		pCpnMesh = pGameObjectChild->AddComponent<Component::Mesh>();
		pCpnMesh->Asset = GetAssetManager()->Get<Engine::Mesh>("@Mesh/Quad.fbx");
		pCpnMesh->Materials.Add(hMaterialWhite);
		pCpnNode = pGameObjectChild->AddComponent<Component::Node>();
		pCpnNode->SetScale(2.0f);
		pCpnNode->RotateRight(90.0f);
		pCpnNode->SetPosition(0.0f, 0.0f, 1.0f);
		pGameObjectChild = pWorld->AddGameObject("Left", pGameObject);
		pCpnMesh = pGameObjectChild->AddComponent<Component::Mesh>();
		pCpnMesh->Asset = GetAssetManager()->Get<Engine::Mesh>("@Mesh/Quad.fbx");
		pCpnMesh->Materials.Add(hMaterialRed);
		pCpnNode = pGameObjectChild->AddComponent<Component::Node>();
		pCpnNode->SetScale(2.0f);
		pCpnNode->RotateForward(90.0f);
		pCpnNode->SetPosition(-1.0f, 0.0f, 0.0f);
		pGameObjectChild = pWorld->AddGameObject("Right", pGameObject);
		pCpnMesh = pGameObjectChild->AddComponent<Component::Mesh>();
		pCpnMesh->Asset = GetAssetManager()->Get<Engine::Mesh>("@Mesh/Quad.fbx");
		pCpnMesh->Materials.Add(hMaterialGreen);
		pCpnNode = pGameObjectChild->AddComponent<Component::Node>();
		pCpnNode->SetScale(2.0f);
		pCpnNode->RotateForward(-90.0f);
		pCpnNode->SetPosition(1.0f, 0.0f, 0.0f);
		/*pGameObjectChild = pWorld->AddGameObject("Top", pGameObject);
		pCpnMesh = pGameObjectChild->AddComponent<Component::Mesh>();
		pCpnMesh->Asset = GetAssetManager()->Get<Engine::Mesh>("Mesh/Quad.fbx");
		pCpnNode = pGameObjectChild->AddComponent<Component::Node>();
		pCpnNode->SetScale(2.0f);
		pCpnNode->SetPosition(0.0f, 1.0f, 0.0f);
		pCpnNode->RotateRight(180.0f);*/
		pGameObjectChild = pWorld->AddGameObject("Bottom", pGameObject);
		pCpnMesh = pGameObjectChild->AddComponent<Component::Mesh>();
		pCpnMesh->Asset = GetAssetManager()->Get<Engine::Mesh>("@Mesh/Quad.fbx");
		//pCpnMesh->Materials.Add(hMaterialWhite);
		pCpnNode = pGameObjectChild->AddComponent<Component::Node>();
		pCpnNode->SetScale(2.0f);
		pCpnNode->SetPosition(0.0f, -1.0f, 0.0f);

		Engine::AssetHandle<Engine::Material> hMaterialDragon = GetAssetManager()->Create<Engine::Material>();
		hMaterialDragon->SetShader(GetAssetManager()->Get<Engine::Shader>("@Effect/Generated/ColorMetallicRoughness.ps"));
		hMaterialDragon->SetNumber(0, Math::Vector3(0.5f, 0.5f, 0.5f));
		hMaterialDragon->SetNumber(3, 0.0f);	// Metallic
		hMaterialDragon->SetNumber(4, 1.0f);	// Roughness

		/*Engine::AssetHandle<Engine::Texture> hTexture = GetAssetManager()->Get<Engine::Texture>("Texture/Sadface.png");
		pGameObject = pWorld->AddGameObject("Decal", pScene);	
		Component::Decal* pCpnDecal = pGameObject->AddComponent<Component::Decal>();
		pCpnDecal->Asset = GetAssetManager()->Create<Engine::Material>();
		pCpnDecal->Asset->SetShader(GetAssetManager()->Get<Engine::Shader>("Decal.ps"));
		pCpnDecal->Asset->SetTexture(0, hTexture);*/

		// Object 
		pGameObject = pWorld->AddGameObject("Object", pScene);	
		pCpnMesh = pGameObject->AddComponent<Component::Mesh>();
		pCpnMesh->Asset = GetAssetManager()->Get<Engine::Mesh>("@Mesh/Sphere.fbx");
		pCpnMesh->Materials.Add(hMaterialDragon);
		pCpnNode = pGameObject->AddComponent<Component::Node>();
		pCpnNode->SetScale(0.5f);
		pCpnNode->SetPosition(-0.0f, -0.5f, 0.0f);

		pCpnOrbitalCamera->Target = pGameObject;

		//Engine::Material* pMaterial = new Engine::Material();
		//pMaterial->SetShader(GetAssetManager()->Get<Engine::Shader>("Effect/FakeGenerated.ps"));
	
		//pCpnMesh->Materials.Add(pMaterial);
		pView->Show();
		Engine::Application::Run();
	}
}}