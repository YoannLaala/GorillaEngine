/******************************************************************************
**	Includes
******************************************************************************/
#include "World.hpp"
#include <Core/Container/Dictionary.hpp>
#include <Core/Stream/Dictionary/DictionaryStreamJson.hpp>
#include <Core/Time/Profiler.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Component/Component.hpp>
#include <Engine/Object/GameObject.hpp>

#include <Renderer/Renderer.hpp>
#include <Renderer/Scene.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	ASSET_REGISTER(Gorilla::Engine::World);

	//!	@brief		Contructor
	//!	@date		2015-11-10
	World::World()
		: m_pScene(nullptr)
		, m_eFlag(0)
	{
		m_pScene = GetRenderer()->CreateScene();
	}

	//!	@brief		Destructor
	//!	@date		2015-11-10
	World::~World()
	{
		GetRenderer()->DestroyScene(m_pScene);
		ASSERT(m_vGameObject.GetSize() == 0, "World not released");
	}

	//!	@brief		Initialize
	//!	@date		2015-11-07
	void World::Initialize(StreamReader* _pStream)
	{
		Dictionary dReader;
		if(!dReader.Read<DictionaryStream>(_pStream))
		{
			LOG_ERROR("Failed to initialize World");
			return;
		}
		Deserialize(dReader);
	}

	//!	@brief		Release
	//!	@date		2015-11-07
	void World::Release()
	{
		// Delete all GameObject
		const uint32 uiGameObjectCount = m_vGameObject.GetSize();
		for(uint32 uiGameObject = 0; uiGameObject < uiGameObjectCount; ++uiGameObject)
		{
			GameObject* pGameObject = GetGameObject(uiGameObject);
			SAFE_RELEASE_AND_DELETE(pGameObject)
		}
		m_vGameObject.Clear();
	}

	//!	@brief		Start
	//!	@date		2015-11-07
	void World::Start()
	{
		RemoveFlag(EFlag::Paused);
	}

	//!	@brief		Update
	//!	@date		2015-11-07
	void World::Update()
	{
		// Iterates all GameObject
		const uint32 uiGameObjectCount = m_vGameObject.GetSize();
		for(uint32 uiGameObject = 0; uiGameObject < uiGameObjectCount; ++uiGameObject)
		{
			// Get all component for the current pass on the current GameObject
			GameObject* pGameObject = m_vGameObject[uiGameObject];
			if(pGameObject->IsActivated())
			{
				// Update all component available for this pass
				pGameObject->Update();
			}
		}
	}

	//!	@brief		Stop
	//!	@date		2015-11-07
	void World::Stop()
	{
		RemoveFlag(EFlag::Paused);

		// Iterates all GameObject
		const uint32 uiGameObjectCount = m_vGameObject.GetSize();
		for(uint32 uiGameObject = 0; uiGameObject < uiGameObjectCount; ++uiGameObject)
		{
			// Get all component for the current pass on the current GameObject
			GameObject* pGameObject = m_vGameObject[uiGameObject];
			Component::Iterator it = pGameObject->GetFirstComponent();
			Component::Iterator itEnd = pGameObject->GetLastComponent();
			while(it != itEnd)
			{
				Component* pComponent = *it;

				// Stop the component if needed
				if(pComponent->GetState() != Component::EState::Inactive)
				{
					pComponent->SetState(Component::EState::Stop);
					pComponent->Execute();
				}

				++it;
			}
		}

		// Start all component
		for(uint32 uiGameObject = 0; uiGameObject < uiGameObjectCount; ++uiGameObject)
		{
			// Get all component for the current pass on the current GameObject
			GameObject* pGameObject = m_vGameObject[uiGameObject];
			Component::Iterator it = pGameObject->GetFirstComponent();
			Component::Iterator itEnd = pGameObject->GetLastComponent();
			while(it != itEnd)
			{
				Component* pComponent = *it;
				pComponent->SetState(Component::EState::Start);
				pComponent->Execute();
				++it;
			}
		}

		SetFlag(EFlag::Paused);
	}

	//!	@brief		AddGameObject
	//!	@date		2015-04-04
	GameObject*	World::AddGameObject(const char* _szName /*= nullptr*/, GameObject* _pParent /*= NULL*/)
	{
		GameObject* pGameObject = new GameObject();
		pGameObject->Initialize(this, _pParent, _szName ? _szName : "GameObject");
		m_vGameObject.Add(pGameObject);

		return pGameObject;
	}

	//!	@brief		RemoveGameObject
	//!	@date		2015-04-04
	void World::RemoveGameObject(GameObject* _pGameObject) 
	{
		m_vGameObject.Remove(_pGameObject); 
		SAFE_RELEASE_AND_DELETE(_pGameObject);
	}

	//!	@brief		RemoveGameObject
	//!	@date		2015-04-04
	void World::RemoveGameObject(uint32 _uiIndex) 
	{ 
		GameObject* pGameObject = m_vGameObject[_uiIndex];
		m_vGameObject.RemoveIndex(_uiIndex); 
		SAFE_RELEASE_AND_DELETE(pGameObject);
	}

	//!	@brief		Serialize
	//!	@date		2015-11-07
	bool World::Serialize(Node& _dWriter)
	{
		Node dGameObjects = _dWriter.Add("game_objects");
		const uint32 uiGameObjectCount = m_vGameObject.GetSize();
		for(uint32 uiGameObject = 0; uiGameObject < uiGameObjectCount; ++uiGameObject)
		{
			GameObject* pGameObject = m_vGameObject[uiGameObject];
			if(pGameObject->GetName()[0] == '@') continue; // Avoid intrinsic GameObject

			// Iterates all component
			Node dGameObject = dGameObjects.Add();
			if(!pGameObject->Serialize(dGameObject))
			{
				LOG_ERROR("Failed to serialize GameObject %s", pGameObject->GetName().GetBuffer());
			}
		}

		return true;
	}

	//!	@brief		Deserialize
	//!	@date		2015-11-07
	bool World::Deserialize(Node& _dReader)
	{
		Node dGameObject = _dReader["game_objects"].GetFirstChild();
		while(dGameObject.IsValid())
		{
			GameObject* pGameObject = AddGameObject();
			if(!pGameObject->Deserialize(dGameObject))
			{
				LOG_ERROR("Failed to deserialize GameObject %s", pGameObject->GetName().GetBuffer());
				SAFE_RELEASE_AND_DELETE(pGameObject);
				dGameObject.Next();
				continue;
			}
			dGameObject.Next();
		}

		return true;
	}
}}