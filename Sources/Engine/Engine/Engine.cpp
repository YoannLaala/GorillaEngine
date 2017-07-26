/******************************************************************************
**	Includes
******************************************************************************/
#include "Engine.hpp"

// Core
#include <Core/Thread/Thread.hpp>
#include <Core/File/FileManager.hpp>
#include <Core/Time/Time.hpp>
#include <Core/Input/Input.hpp>
#include <Core/Time/Profiler.hpp>
#include <Core/Container/Dictionary.hpp>
#include <Core/Stream/Dictionary/DictionaryStreamJson.hpp>

// Engine
#include "Asset/AssetManager.hpp"
#include "World.hpp"

// Renderer
#include <Renderer/Renderer.hpp>
#include <Renderer/RenderContext.hpp>
#include <Renderer/Viewport.hpp>
#include <Engine/Renderer/Pass/GuiPass.hpp>
#include <Engine/Renderer/Pass/VisibilityPass.hpp>
#include <Engine/Renderer/Pass/ShadowPass.hpp>
#include <Engine/Renderer/Pass/GeometryPass.hpp>
#include <Engine/Renderer/Pass/DecalPass.hpp>
#include <Engine/Renderer/Pass/LightPass.hpp>
#include <Engine/Renderer/Pass/PrimitivePass.hpp>
#include <Engine/Renderer/Pass/CompositingPass.hpp>
#include <Engine/Renderer/Pass/FullScreenPass.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	//!	@brief		Constructor 
	//!	@date		2015-04-04
	Engine::Engine()
		: m_pLogManager		(nullptr)
		, m_pFileManager	(nullptr)
		, m_pRenderer		(nullptr)
		, m_pTime			(nullptr)
		, m_pInput			(nullptr)
		, m_pAssetManager	(nullptr)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-04-04
	Engine::~Engine()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2015-04-04
	bool Engine::Initialize()
	{
		Thread::IsMainThread();

		//m_pRendererSetting

		// Base Directory
		/*rapidjson::Document kDocument;
		String sProjectPath(_szProjectPath);
		String sAssetPath(_szProjectPath) 
		sAssetPath.Append("Assets/";)
		String sCookedPath = sProjectPath + "Cooked/";
		m_pFileSystem->AddPath(EEnginePath::Project, sProjectPath.GetBuffer());
		m_pFileSystem->AddPath(EEnginePath::Asset, sAssetPath.GetBuffer());
		m_pFileSystem->AddPath(EEnginePath::Cooked, sCookedPath.GetBuffer());

		// Try to read Renderer settings
		String sRendererSettingPath = sProjectPath + RENDERER_CONFIG_FILE;
		Common::JsonHelper::ReadJson(sRendererSettingPath.GetBuffer(), kDocument, false);*/
		
		// TaskManager
		//m_pTaskManager = new Common::TaskManager();

		// Read Renderer config file
		/*String sRendererConfig(_szProjectPath);
		sRendererConfig.Append("Renderer.cfg");
		JsonReader kRendererReader(sRendererConfig.GetBuffer());

		String sRendererAPI;
		kRendererReader.GetMember("API").GetValue(sRendererAPI);*/

		// Basic support
		m_pLogManager = new LogManager();
		m_pFileManager = new FileManager();

		// Renderer
		m_pRenderer = new Renderer::Renderer();
		if(!m_pRenderer->Initialize())
		{
			ASSERT(false, "Renderer Initialization failed");
			return false;
		}
		SIGNAL_CONNECT(m_pRenderer, this, RenderContextCreated);

		// Time & Input
		m_pTime = new Time();

		if(!Input::IsCreated()) Input::Create();
		m_pInput = Input::GetInstance();

		// AssetManager
		m_pAssetManager = new AssetManager();
		m_pAssetManager->Initialize();

		if(!Profiler::IsCreated()) Profiler::Create();

		return true;
	}

	//!	@brief		Release 
	//!	@date		2015-04-04
	void Engine::Release()
	{
		// Destroy all world
		const uint32 uiWorldCount = m_vWorld.GetSize();
		for(uint32 uiWorld = 0; uiWorld < uiWorldCount; ++uiWorld)
		{
			World* pWorld = m_vWorld[uiWorld];
			DestroyWorld(pWorld);			
		}
		m_vWorld.Clear();

		SAFE_RELEASE_AND_DELETE(m_pAssetManager)
		Input::Destroy();
		SAFE_DELETE(m_pTime)

		SIGNAL_DISCONNECT(m_pRenderer, this, RenderContextCreated);
		SAFE_RELEASE_AND_DELETE(m_pRenderer)
	}

	void Engine::Update()
	{
		Profiler::GetInstance()->StartFrame();

		//PROFILE_FUNCTION();
	#if defined(GORILLA_EDITOR)
		m_pFileManager->Update();
		m_pAssetManager->Update();
	#endif

		m_pTime->Update();

		// Update all world
		{
			PROFILE_SCOPE("World");
			const uint32 uiWorldCount = m_vWorld.GetSize();
			for(uint32 uiWorld = 0; uiWorld < uiWorldCount; ++uiWorld)
			{
				World* pWorld = m_vWorld[uiWorld];
				pWorld->Update();
			}
		}

		m_pInput->Update();

		// Display
		{
			PROFILE_SCOPE("Display");
			m_pRenderer->Display();
		}

		Profiler::GetInstance()->EndFrame();
	}

	//!	@brief		OnResize 
	//!	@date		2016-06-25
	void Engine::OnResize()
	{

	}

	//!	@brief		CreateWorld 
	//!	@date		2016-06-25
	World* Engine::CreateWorld()
	{
		World* pWorld = new World();
		m_vWorld.Add(pWorld);

		return pWorld;
	}
	
	//!	@brief		DestroyWorld 
	//!	@date		2016-06-25
	void Engine::DestroyWorld(World* _pWorld)
	{
		m_vWorld.Remove(_pWorld);
		SAFE_RELEASE_AND_DELETE(_pWorld)
	}

	//!	@brief		AddWorld 
	//!	@date		2016-06-25
	void Engine::AddWorld(const AssetHandle<World>& hWorld)
	{
		World* pWorld = hWorld.operator->();
		m_vWorld.Add(pWorld);
	}
		
	//!	@brief		RemoveWorld 
	//!	@date		2016-06-25
	void Engine::RemoveWorld(const AssetHandle<World>& hWorld)
	{
		World* pWorld = hWorld.operator->();
		DestroyWorld(pWorld);
	}

	//!	@brief		GetLayer 
	//!	@date		2015-11-11
	uint32 Engine::GetLayer(const char* _szName) const
	{
		return m_mLayer.Get(_szName, 0);
	}

	//!	@brief		AddLayer
	//!	@date		2015-11-11
	uint32 Engine::AddLayer(const char* _szName)
	{
		uint32 uiLayerId = 1 << m_mLayer.GetSize();
		m_mLayer.Add(_szName, uiLayerId);
		
		return uiLayerId;
	}

	//!	@brief		AddModule
	//!	@date		2015-11-11
	void Engine::AddModule(const char* _szRelativePath)
	{
		AssetHandle<Module> hAsset = GetAssetManager()->Get<Module>(_szRelativePath);
		m_vModule.Add(hAsset);
	}

	//!	@brief		RemoveModule
	//!	@date		2015-11-11
	void Engine::RemoveModule(const char* _szRelativePath)
	{
		const uint32 uiModuleCount = m_vModule.GetSize();
		for(uint32 uiModule = 0; uiModule < uiModuleCount; ++uiModule)
		{
			AssetHandle<Module>& hAsset = m_vModule[uiModule];
			if(hAsset->GetName() == _szRelativePath)
			{
				hAsset.Release();
				m_vModule.RemoveIndex(uiModule);
				break;
			}
		}
	}

	//!	@brief		LoadDescriptor
	//!	@date		2015-11-11
	void Engine::LoadDescriptor()
	{
	#if defined(GORILLA_EDITOR)
		// Intrinsic Descriptor
		String sDescriptor;
		FileManager::GetDirectory(FileManager::Directory::Executable, sDescriptor);
		sDescriptor.Append("Resources\\Database\\ComponentDescriptor.json");
		if(!m_dComponentDescriptor.Read<DictionaryStreamJson>(sDescriptor.GetBuffer()))
		{
			LOG_ERROR("Failed to read component descriptor %s", sDescriptor.GetBuffer());
		}

		// Update User Component Descriptor
		sDescriptor.Set(GetAssetManager()->GetPath()).Append("..\\Cooked\\Temp\\ComponentDescriptor.json");

		String sArgument;
		sArgument.Set("-input \"").Append(GetAssetManager()->GetPath()).Append("\" -output \"").Append(sDescriptor).Append("\"");
		sArgument.Replace("\\", "/");
		Process kProcess("Cooker/Reflection/Reflection.exe");
		kProcess.SetArgument(sArgument.GetBuffer());
		if(!kProcess.Execute())
		{
			LOG_ERROR("Failed to cook user component descriptor %s", sDescriptor.GetBuffer());
		}

		// User Descriptor
		m_dComponentDescriptor.Read<DictionaryStreamJson>(sDescriptor.GetBuffer());

		// Recreate Script if needed
		HashMap<uint32, Vector<GameObject*>*>::Iterator it = m_mScript.GetFirst();
		HashMap<uint32, Vector<GameObject*>*>::Iterator itEnd = m_mScript.GetLast();
		while(it != itEnd)
		{
			Class* pClass = ClassManager::GetInstance()->Get(EClass::Component, it.GetKey());

			Vector<GameObject*>* pVecGameObject = (*it);
			const uint32 uiElementCount = pVecGameObject->GetSize();
			for(uint32 uiElement = 0; uiElement < uiElementCount; ++uiElement)
			{
				GameObject* pGameObject = pVecGameObject->Get(uiElement);
				pGameObject->RemoveComponent(pClass);
				pGameObject->AddComponent(pClass);
			}

			++it;
		}

	#else
		// Intrinsic Descriptor
		if(!m_dComponentDescriptor.Read<DictionaryStream>("Component.bin"))
		{
			LOG_ERROR("Failed to read component descriptor");
		}
	#endif
	}

	//!	@brief		OnRenderContextCreated
	//!	@date		2015-11-11
	void Engine::OnRenderContextCreated(Renderer::RenderContext* _pContext, uint8 _eFlag)
	{
		if((_eFlag & Renderer::EFlag::EmptyPass) == 0)
		{
			_pContext->CreatePass<VisibilityPass>();
			_pContext->CreatePass<ShadowPass>();
			_pContext->CreatePass<GeometryPass>();
			_pContext->CreatePass<DecalPass>();
			_pContext->CreatePass<LightPass>();
			_pContext->CreatePass<PrimitivePass>();
			_pContext->CreatePass<GuiPass>();
			_pContext->CreatePass<CompositingPass>();
			_pContext->CreatePass<FullScreenPass>();
		}
	}

#if defined(GORILLA_EDITOR)
	//!	@brief		RegisterScript
	//!	@date		2015-11-11
	void Engine::RegisterScript(uint32 _uiComponentId, GameObject* _pGameObject)
	{
		Vector<GameObject*>* pVecGameObject = m_mScript.Get(_uiComponentId, nullptr);
		if(!pVecGameObject)
		{
			pVecGameObject = new Vector<GameObject*>();
			m_mScript.Add(_uiComponentId, pVecGameObject);
		}
		pVecGameObject->Add(_pGameObject);
	}

	//!	@brief		UnregisterScript
	//!	@date		2015-11-11
	void Engine::UnregisterScript(uint32 _uiComponentId, GameObject* _pGameObject)
	{
		Vector<GameObject*>* pVecGameObject = m_mScript.Get(_uiComponentId, nullptr);
		if(pVecGameObject)
		{
			pVecGameObject->Remove(_pGameObject);
		}
	}
#endif
}}