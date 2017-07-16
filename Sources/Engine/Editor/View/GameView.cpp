///******************************************************************************
//**	Includes
//******************************************************************************/
//#include "GameView.hpp"
//
//#include <Engine/Engine.hpp>
//#include <Engine/Asset/AssetManager.hpp>
//
//#include <Component/Light/DirectionalLight.hpp>
//#include <Component/Mesh/Mesh.hpp>
//#include <Component/Gui/WebView.hpp>
//
//#include <Editor/Game/Component/GameViewHandler.hpp>
//
///******************************************************************************
//**	Class Definition
//******************************************************************************/
//namespace Gorilla { namespace Editor
//{
//	//!	@brief		PostInitialize 
//	//!	@date		2015-04-04
//	void GameView::PostInitialize()
//	{
//		Engine::GameObject* pGoEditor = m_pWorld->GetGameObject(0);
//
//		// Handller for the GameView
//		GameViewHandler* pCpnGameViewHandler = pGoEditor->AddComponent<GameViewHandler>();
//		pCpnGameViewHandler->View = this;
//
//		// Default Camera
//		Engine::GameObject* pGoGameObject = m_pWorld->AddGameObject("Camera");
//		pGoGameObject->AddComponent<Component::Camera>();
//
//		// Default Directional Light
//		pGoGameObject = m_pWorld->AddGameObject("DirectionalLight");
//		pGoGameObject->AddComponent<Component::DirectionalLight>();
//
//		// Dragon
//		pGoGameObject = m_pWorld->AddGameObject("Dragon");
//			
//			// Node
//		Component::Node* pCpnNode = pGoGameObject->AddComponent<Component::Node>();
//		pCpnNode->SetPosition(0,0, 20);
//
//			// Mesh
//		Component::Mesh* pCpnMesh = pGoGameObject->AddComponent<Component::Mesh>();
//		pCpnMesh->Asset = GetAssetManager()->Get<Gorilla::Engine::Mesh>("Mesh/Cube.fbx");
//	}
//}}