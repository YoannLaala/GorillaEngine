///******************************************************************************
//**	Includes
//******************************************************************************/
//#include "MaterialView.hpp"
//
//#include <Engine/Engine.hpp>
//#include <Engine/Asset/AssetManager.hpp>
//#include <Engine/Object/GameObject.hpp>
//#include <Engine/World.hpp>
//#include <Editor/Material/Component/MaterialViewHandler.hpp>
//
//#include <Component/Node.hpp>
//#include <Component/Material/Material.hpp>
//#include <Component/Light/DirectionalLight.hpp>
//#include <Component/Mesh/Mesh.hpp>
//
//#include <Core/Stream/MemoryWriter.hpp>
//#include <Core/Stream/MemoryReader.hpp>
//
///******************************************************************************
//**	Class Definition
//******************************************************************************/
//namespace Gorilla { namespace Editor
//{
//	//!	@brief		PostInitialize 
//	//!	@date		2015-04-04
//	void MaterialView::PostInitialize()
//	{
//		Engine::GameObject* pGoEditor = m_pWorld->GetGameObject(0);
//
//		// Handller for the MaterialView
//		MaterialViewHandler* pCpnMaterialViewHandler = pGoEditor->AddComponent<MaterialViewHandler>();
//		pCpnMaterialViewHandler->View = this;
//
//		pGoEditor->AddComponent<Component::Material>();
//
//		// Default Directional Light
//		Engine::GameObject* pGoGameObject = m_pWorld->AddGameObject("DirectionalLight", pGoEditor);
//		pGoGameObject->AddComponent<Component::DirectionalLight>();
//
//		// Basic Mesh
//		pGoGameObject = m_pWorld->AddGameObject("Object");
//
//			// Node
//		Component::Node* pCpnNode = pGoGameObject->AddComponent<Component::Node>();
//		pCpnNode->SetPosition(0,-0.5f, 2.0f);
//
//			// Mesh
//		Component::Mesh* pCpnMesh = pGoGameObject->AddComponent<Component::Mesh>();
//		pCpnMesh->Asset = Gorilla::GetAssetManager()->Get<Gorilla::Engine::Mesh>("Mesh/Cube.fbx");
//		//pCpnMesh->Materials.Add(Gorilla::GetAssetManager()->Get<Gorilla::Engine::Material>("Mesh/Sphere.fbx"))
//
//		/*MemoryWriter kStream;
//		pGoEditor->Serialize(kStream);
//
//		MemoryReader kReader(&kStream[0], kStream.GetPosition());
//		Engine::GameObject* ptest = m_pWorld->AddGameObject();
//		ptest->Deserialize(kReader);*/
//	}
//}}