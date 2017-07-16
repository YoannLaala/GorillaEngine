///******************************************************************************
//**	Includes
//******************************************************************************/
//#include "EditorView.hpp"
//
//#include <Engine/Engine.hpp>
//#include <Engine/World.hpp>
//#include <Engine/Object/GameObject.hpp>
//
//#include <Renderer/Renderer.hpp>
//#include <Renderer/RenderContext.hpp>
//#include <Renderer/Viewport.hpp>
//
//#include <Component/Node.hpp>
//#include <Component/Camera/Camera.hpp>
//#include <Component/Gui/WebView.hpp>
//
//#include <Editor/Renderer/Pass/EditorPass.hpp>
//
///******************************************************************************
//**	Class Definition
//******************************************************************************/
//namespace Gorilla { namespace Editor
//{
//	//!	@brief		Constructor 
//	//!	@date		2015-04-04
//	EditorView::EditorView()
//		: m_pEditorPass(NULL)
//	{
//		// Nothing to do
//	}
//
//	//!	@brief		Destructor
//	//!	@date		2015-04-04
//	EditorView::~EditorView()
//	{
//		// Nothing to do
//	}
//
//	//!	@brief		Initialize 
//	//!	@date		2015-04-04
//	void EditorView::Initialize()
//	{
//		Engine::Engine* pEngine = GetEngine();
//		Renderer::Renderer* pRenderer = pEngine->GetRenderer();
//
//		// Create Window Viewport
//		Renderer::Viewport* pViewport = pRenderer->CreateViewport(m_hWindow, Renderer::EFormat::R8G8B8A8_UNORM, false);
//		m_vViewport.Add(pViewport);
//	
//		// Create texture for the gui rendering 
//		Renderer::Texture2D* pRenderTexture = pRenderer->CreateTexture2D(GetWidth(), GetHeight(), 1, Renderer::EFormat::B8G8R8A8_UNORM, Renderer::EBind::ShaderResource);
//
//		// Create the world and add the Editor GameObject
//		m_pWorld = pEngine->CreateWorld();
//
//		// Editor specific (Gui)
//		Engine::GameObject* pGoEditor = m_pWorld->AddGameObject("Editor");
//		Component::WebView* pCWebView = pGoEditor->AddComponent<Component::WebView>();
//		pCWebView->RenderTexture = pRenderTexture;
//
//		// Editor specific handle (Editor, Material, etc)
//		PostInitialize();
//	}
//
//	//!	@brief		PostInitialize 
//	//!	@date		2015-04-04
//	void EditorView::PostInitialize()
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
//
//	//!	@brief		Release 
//	//!	@date		2015-04-04
//	void EditorView::Release()
//	{
//		GetEngine()->DestroyWorld(m_pWorld);
//	}
//
//	//!	@brief		CreateViewport
//	//!	@date		2015-04-08
//	void EditorView::CreateViewport(uint32 _uiLeft, uint32 _uiTop, uint32 _uiWidth, uint32 _uiHeight)
//	{
//		Engine::View::CreateViewport(_uiLeft, _uiTop, _uiWidth, _uiHeight);
//		
//		const uint32 uiViewportCount = m_vViewport.GetSize();
//		Renderer::Viewport* pViewport = m_vViewport[uiViewportCount-1];
//		RefreshEditorPass();
//		m_pEditorPass->AddDisplay(pViewport->GetRenderTarget()->GetTarget(0), _uiLeft, _uiTop);
//
//		// Add a camera for this viewport
//		Engine::GameObject* pGoEditor = m_pWorld->GetGameObject(0);
//		Component::Camera* pCpnCamera = pGoEditor->AddComponent<Component::Camera>();
//		pCpnCamera->Viewport = pViewport;
//	}
//
//	//!	@brief		RefreshEditorPass
//	//!	@date		2015-04-08
//	void EditorView::RefreshEditorPass()
//	{
//		// Create the EditorPass
//		if(!m_pEditorPass)
//		{
//			m_pEditorPass = m_vViewport[EViewport::First]->GetRenderContext()->CreatePass<EditorPass>();
//
//			// Clear and set proper render target and texture to display
//			m_pEditorPass->SetRenderTarget(m_vViewport[EViewport::Window]->GetRenderTarget());
//
//			// Set the first texture to display (HTML Gui)
//			Engine::GameObject* pGoEditor = m_pWorld->GetGameObject(0);
//			Component::WebView* pCWebView = pGoEditor->GetComponent<Component::WebView>();
//			m_pEditorPass->AddDisplay(pCWebView->RenderTexture);
//		}
//	}
//}}