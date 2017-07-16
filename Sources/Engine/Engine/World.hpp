#ifndef _ENGINE_WORLD_HPP_
#define _ENGINE_WORLD_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Asset/Asset.hpp>
#include <Renderer/Tree/Octree.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla
{
	class Node;
}

namespace Gorilla { namespace Engine
{
	class Engine;
	class GameObject;
	class Component;
}}

namespace Gorilla { namespace Renderer
{
	class Scene;
}}

#define WORLD_GAME_OBJECT_DEFAULT_NAME "GameObject"

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class World : public Asset
	{
		friend class Engine;
		friend class GameObject;
		friend class Component;

		struct EFlag 
		{
			enum Type : uint8
			{
				Paused = 1 << 0,
			};
		};

	private:
		World();
		~World();

	public:
		ASSET_DECLARE(Gorilla::Engine::World);

	private:
		virtual void		Initialize	(StreamReader* _pStream) override;
		virtual void		Release	() override;

		void				Update				();

	public:
		void				Start				();
		inline void			Pause				() { SetFlag(EFlag::Paused); }
		void				Stop				();

		bool				Serialize			(Node& _dWriter);
		bool				Deserialize			(Node& _dReader);

	public:
		GameObject*			AddGameObject		(const char* _szName = nullptr, GameObject* _pParent = nullptr);
		GameObject*			AddGameObject		(GameObject* _pParent) { AddGameObject(nullptr, _pParent); }
		
		uint32				GetGameObjectCount	() { return m_vGameObject.GetSize(); }
		GameObject*			GetGameObject		(uint32 _uiIndex) { return m_vGameObject[_uiIndex]; }
		void				RemoveGameObject	(GameObject* _pGameObject);
		void				RemoveGameObject	(uint32 _uiIndex);

		inline Renderer::Scene*	GetScene		() { return m_pScene; }

	private:
		inline bool			HasFlag		(EFlag::Type _eFlag) const { return (m_eFlag & _eFlag) != 0; }
		inline void			SetFlag		(EFlag::Type _eFlag) { m_eFlag |= _eFlag; }
		inline void			RemoveFlag	(EFlag::Type _eFlag) { m_eFlag &= ~_eFlag; }

	private:
		Vector<GameObject*>	m_vGameObject;
		Renderer::Scene*	m_pScene;
		uint8				m_eFlag;
	};
}}

#endif