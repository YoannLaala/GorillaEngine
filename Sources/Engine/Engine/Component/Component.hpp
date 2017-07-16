#pragma once

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>
#include <Engine/Common.hpp>
#include <Engine/Object/GameObject.hpp>
#include <Engine/System/Module.hpp>

/******************************************************************************
**	Define
******************************************************************************/
#define COMPONENT_DECLARE_EXTENDED(_type_, _flag_) CLASS_DECLARE_IMPL(Gorilla::Engine::EClass::Component, _type_, _flag_)
#define COMPONENT_DECLARE(_type_) COMPONENT_DECLARE_EXTENDED(_type_, 0)
#define COMPONENT_REGISTER(_type_, ...) CLASS_REGISTER(_type_);
#define COMPONENT_CLASS(_name_) Gorilla::ClassManager::GetInstance()->Get(Gorilla::Engine::EClass::Component, _name_)

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class Engine;
	class World;
	class GameObject;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class Component
	{
		friend class World;
		friend class GameObject;
	
	public:
		typedef HashMap<uint32, Component*>::Iterator Iterator; 

	protected:
		struct EPass
		{
			enum Type : uint8
			{
				Script = 0,		// Execute Script
				Node,			// Build Nodes
				Camera,			// Build Cameras

				Count,
			};
		};

	private:
		struct EState
		{
			enum Type : uint8
			{
				Inactive = 0,
				Start,
				Update,
				Stop,

				Count
			};
		};

	public:
		struct EFlag
		{
			enum Type : uint8
			{
				Intrinsic = 1 << 0,
			};
		};

	protected:
		Component();
		~Component();
	
	public:
		CLASS_DECLARE_VIRTUAL()

	protected:
		void Execute();
		virtual void Start();
		virtual void Update();
		virtual void Stop();

	protected:
		template <class T>
		T* GetOrCreate();

	protected:
		inline bool			HasFlag			(uint32 _eFlag) const { return (m_eFlag & _eFlag) != 0; }
		inline void			SetFlag			(uint32 _eFlag) { m_eFlag |= _eFlag; }
		inline void			RemoveFlag		(uint32 _eFlag) { m_eFlag &= ~_eFlag; }

	private:
		inline EState::Type	GetState		() const { return m_eState; }
		inline void			SetState		(EState::Type _eState) { m_eState = _eState; }

		inline void			SetGameObject	(GameObject* _pGameObject) { m_pGameObject = _pGameObject; }

	public:
		inline GameObject*	GetGameObject	() { return m_pGameObject; }

	private:
		GameObject*		m_pGameObject;
		uint32			m_eFlag;
		EState::Type	m_eState;
	};

	//!	@brief		CheckDependency
	//!	@date		2015-11-01
	template <class T>
	T* Component::GetOrCreate()
	{
		T* pComponent = GetGameObject()->GetComponent<T>();
		if(!pComponent)
		{
			pComponent = GetGameObject()->AddComponent<T>();
			pComponent->Execute();	
		}

		return pComponent;
	}
}}