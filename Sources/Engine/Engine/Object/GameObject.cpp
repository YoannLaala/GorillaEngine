/******************************************************************************
**	Includes
******************************************************************************/
#include "GameObject.hpp"
#include <Core/Log/LogManager.hpp>
#include <Core/Container/Dictionary.hpp>
#include <Engine/Common.hpp>
#include <Engine/Engine.hpp>
#include <Engine/World.hpp>
#include <Engine/Component/Component.hpp>
#include <Engine/Asset/AssetManager.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	//!	@brief		Constructor
	//!	@date		2015-04-04
	GameObject::GameObject()
		: m_pWorld(nullptr)
		, m_pParent(nullptr)
		, m_eFlag(EFlag::Activated)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	GameObject::~GameObject()
	{
		// Nothing to do
	}

	//!	@brief		Release
	//!	@date		2015-04-04
	void GameObject::Initialize(World* _pWorld, GameObject* _pParent, const char* _szName)
	{
		m_pWorld = _pWorld;
		m_pParent = _pParent;
		m_sName = _szName;
	}

	//!	@brief		Release
	//!	@date		2015-04-04
	void GameObject::Release()
	{
		bool bHasPaused = m_pWorld->HasFlag(World::EFlag::Paused);
		m_pWorld->RemoveFlag(World::EFlag::Paused);
		HashMap<uint32, Component*>::Iterator it = m_mComponent.GetFirst();
		HashMap<uint32, Component*>::Iterator itEnd = m_mComponent.GetLast();
		while(it != itEnd)
		{
			Component* pComponent = (*it);
			pComponent->SetState(Component::EState::Stop);
			pComponent->Execute();
			SAFE_DELETE(pComponent);

			++it;
		}
		if(bHasPaused) m_pWorld->SetFlag(World::EFlag::Paused);
	}

	//!	@brief		Update
	//!	@date		2015-04-04
	void GameObject::Update()
	{
		HashMap<uint32, Component*>::Iterator it = m_mComponent.GetFirst();
		HashMap<uint32, Component*>::Iterator itEnd = m_mComponent.GetLast();
		while(it != itEnd)
		{
			it->Execute();
			++it;
		}
	}

	//!	@brief		GetComponent
	//!	@date		2015-04-04
	Component* GameObject::GetComponent(const Class* _pClass)
	{
		uint32 uiComponentId = _pClass->GetId();
		return m_mComponent[uiComponentId];
	}

	//!	@brief		AddComponent
	//!	@date		2015-04-04
	Component* GameObject::AddComponent(const Class* _pClass)
	{
		uint32 uiComponentId = _pClass->GetId();
		Component* pComponent = m_mComponent[uiComponentId];
		if(!pComponent)
		{
			pComponent = static_cast<Component*>(_pClass->New());
			pComponent->SetGameObject(this);
			m_mComponent.Add(uiComponentId, pComponent);

		#if defined GORILLA_EDITOR
			if(!_pClass->HasFlag(Component::EFlag::Intrinsic))
			{
				GetEngine()->RegisterScript(uiComponentId, this);
			}
		#endif
		}

		return pComponent;
	}

	//!	@brief		RemoveComponent
	//!	@date		2015-04-04
	void GameObject::RemoveComponent(const Class* _pClass)
	{
		// Delete it if found one
		uint32 uiComponentId = _pClass->GetId();
		Component* pComponent = m_mComponent[uiComponentId];
		if(pComponent)
		{
		#if defined GORILLA_EDITOR
			if(!pComponent->HasFlag(Component::EFlag::Intrinsic))
			{
				GetEngine()->UnregisterScript(uiComponentId, this);
			}
		#endif

			m_mComponent.Remove(uiComponentId);
			SAFE_DELETE(pComponent);
		}
	}

	//!	@brief		Serialize
	//!	@date		2015-11-07
	bool GameObject::Serialize(Node& _dWriter)
	{
		_dWriter["name"] = m_sName;
		
		Node dComponents = _dWriter.Add("components");
		Component::Iterator it = GetFirstComponent();
		Component::Iterator itEnd = GetLastComponent();
		while(it != itEnd)
		{
			Component* pComponent = *it;
			++it;
			
			// Component Type
			const String& sType = pComponent->GetClass()->GetName();
			Node dComponent = dComponents.Add(sType.GetBuffer());

			// Class Decriptor 
			Node dComponentDescriptor = GetEngine()->GetComponentDescriptor(sType.GetBuffer());
			if(!dComponentDescriptor.IsValid())
			{
				LOG_ERROR("Failed to find component descriptor for %s", sType.GetBuffer());
				continue;
			}

			// Iterates all fields
			Node dFieldDescriptor = dComponentDescriptor["fields"].GetFirstChild();
			while(dFieldDescriptor.IsValid())
			{
				if(dFieldDescriptor["visible"].GetBool())
				{
					const char* szFieldName = dFieldDescriptor.GetName();
					Node dField = dComponent.Add(szFieldName);
					uint32 uiOffset = dFieldDescriptor["offset"];
					String sFieldType = dFieldDescriptor["type"];
					if(sFieldType == "Gorilla::Math::Vector3")
					{
						float32* pValue = reinterpret_cast<float32*>(reinterpret_cast<uint8*>(pComponent) + uiOffset);
						dField.SetBufferFloat32(pValue, 3);
					}

					if(sFieldType == "Gorilla::Engine::AssetHandle")
					{
						AssetHandle<Asset>& hAsset = *reinterpret_cast<AssetHandle<Asset>*>(reinterpret_cast<uint8*>(pComponent) + uiOffset);
						if(hAsset.IsLoaded())
						{
							dField["filepath"] = hAsset->GetName();
							dField["parameters"] = hAsset->GetParam();
						}
					}
				}
				dFieldDescriptor.Next();	
			}
		}
		return true;
	}

	//!	@brief		Deserialize
	//!	@date		2015-11-07
	bool GameObject::Deserialize(Node& _dReader)
	{
		m_sName = _dReader["name"];

		Node dComponent = _dReader["components"].GetFirstChild();
		while(dComponent.IsValid())
		{
			const char* szComponentClass = dComponent.GetName();
			Node dComponentDescriptor = GetEngine()->GetComponentDescriptor(szComponentClass);
			Class* pComponentClass = COMPONENT_CLASS(szComponentClass);
			if(!pComponentClass || !dComponentDescriptor.IsValid())
			{
				LOG_ERROR("Failed to create component %s", szComponentClass);
				dComponent.Next();
				continue;
			}
			Component* pComponent = AddComponent(pComponentClass);

			Node dFieldDescriptors = dComponentDescriptor["fields"];
			Node dField = dComponent.GetFirstChild();
			while(dField.IsValid())
			{
				const char* szFieldName = dField.GetName();
				Node dFieldDescriptor = dFieldDescriptors.Get(szFieldName);
				if(dFieldDescriptor.IsValid())
				{
					uint32 uiOffset = dFieldDescriptor["offset"];
					String sFieldType = dFieldDescriptor["type"];
					if(sFieldType == "Gorilla::Math::Vector3")
					{
						uint32 uiCount;
						const float32* pValue = reinterpret_cast<const float32*>(reinterpret_cast<uint8*>(pComponent) + uiOffset);
						dField.GetBufferFloat32(&pValue, uiCount);
					}

					if(sFieldType == "Gorilla::Engine::AssetHandle")
					{
						const char* szFilePath = dField["filepath"];
						const char* szParameters = dField["parameters"];
						const char* szAssetClass = dFieldDescriptor["element_type"];
						Class* pAssetClass = ASSET_CLASS(szAssetClass);
						if(pAssetClass)
						{
							AssetHandle<Asset>& hAsset = *reinterpret_cast<AssetHandle<Asset>*>(reinterpret_cast<uint8*>(pComponent) + uiOffset);
							hAsset = GetAssetManager()->Get(pAssetClass, szFilePath, szParameters);
						}
					}
				}
				dField.Next();
			}
			dComponent.Next();
		}

		return true;
	}
}}