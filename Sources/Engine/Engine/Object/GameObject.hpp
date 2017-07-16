#ifndef _ENGINE_GAME_OBJECT_HPP_
#define _ENGINE_GAME_OBJECT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Container/HashMap.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla 
{
	class Class;
	class Node;
}

namespace Gorilla { namespace Engine
{
	class Component;
	class World;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class GameObject
	{
		friend class World;

	private:
		struct EFlag
		{
			enum Type : uint8
			{
				Activated = 1 << 0,
			};
		};

	private:
		GameObject();
		~GameObject();

		void					Initialize		(World* _pWorld, GameObject* _pParent, const char* _szName);
		void					Release			();

		void					Update			();

	private:
		inline bool				HasFlag		(EFlag::Type _eFlag) const { return (m_eFlag & _eFlag) != 0; }
		inline void				SetFlag		(EFlag::Type _eFlag) { m_eFlag |= _eFlag; }
		inline void				RemoveFlag	(EFlag::Type _eFlag) { m_eFlag &= ~_eFlag; }

	public:
		inline const String&	GetName			() const { return m_sName; } 
		inline void				SetName			(const char* _szName) { m_sName = _szName; }
		inline GameObject*		GetParent		() { return m_pParent; }

		inline World*			GetWorld		() const { return m_pWorld; } 

		inline uint32			GetComponentCount() const { return m_mComponent.GetSize(); }
		HashMap<uint32, Component*>::Iterator GetFirstComponent() { return m_mComponent.GetFirst(); }
		HashMap<uint32, Component*>::Iterator GetLastComponent() { return m_mComponent.GetLast(); }

		template <class T>
		T*						GetComponent	();
		Component*				GetComponent	(const Class* _pClass);

		template <class T>
		T*						AddComponent	();
		Component*				AddComponent	(const Class* _pClass);

		template <class T>
		void					RemoveComponent	();

		bool					Serialize		(Node& _dWriter);
		bool					Deserialize		(Node& _dReader);

	public:
		inline void				Activate	() { SetFlag(EFlag::Activated); }
		inline void				Deactivate	() { RemoveFlag(EFlag::Activated); }
		inline bool				IsActivated	() const { return HasFlag(EFlag::Activated); }

	private:
		String						m_sName;
		HashMap<uint32, Component*>	m_mComponent;
		World*						m_pWorld;
		GameObject*					m_pParent;
		uint8						m_eFlag;
	};

	//!	@brief		GetComponent
	//!	@date		2015-04-04
	template <class T>
	T* GameObject::GetComponent()
	{
		uint32 uiComponentId = T::Class::GetId();
		return static_cast<T*>(m_mComponent[uiComponentId]);
	}

	//!	@brief		AddComponent
	//!	@date		2015-04-04
	template <class T>
	T* GameObject::AddComponent()
	{
		const Class* pClass = T::Class::GetInstance();
		return static_cast<T*>(AddComponent(pClass));
	}

	//!	@brief		RemoveComponent
	//!	@date		2015-04-04
	template <class T>
	void GameObject::RemoveComponent()
	{
		uint32 uiComponentId = T::Class::GetId();
		Vector<uint32>& vComponentOrder = m_aComponentOrder[uiPassId];

		// Delete it if found one
		Component* pComponent = m_mComponent[uiComponentId];
		if(pComponent)
		{
			m_mComponent.Remove(uiComponentId);
			SAFE_DELETE(pComponent);
		}
	}
}}

#endif