#ifndef _RENDERER_SCENE_HPP_
#define _RENDERER_SCENE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>
#include <Renderer/Tree/Octree.hpp>
#include <Renderer/Camera.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Renderer;
	class IRenderable;
	class Octree;
	class Node;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Scene
	{
		friend class Renderer;

	private:
		Scene();
		~Scene();

	public:
		// Node
		inline void		AddNode			(Node* _pNode) { m_pTree->Add(_pNode); }
		inline void		UpdateNode		(Node* _pNode) { m_pTree->Update(_pNode); }
		inline void		RemoveNode		(Node* _pNode) { m_pTree->Remove(_pNode); } 

		// Camera
		inline uint32	GetCameraCount  () const { return m_vCamera.GetSize(); }
		inline Camera*	GetCamera		(uint32 _uiIndex) { return m_vCamera[_uiIndex]; }
		Camera*			CreateCamera	();
		void			DestroyCamera	(Camera* _pCamera);

		// Renderable
		inline void		AddRenderable	(IRenderable* _pObject) { m_vRenderable.Add(_pObject); }
		inline void		RemoveRenderable(IRenderable* _pObject) { m_vRenderable.Remove(_pObject); }

	private:
		inline Octree*	GetTree() { return m_pTree; }

		inline uint32	GetRenderableCount() const { return m_vRenderable.GetSize(); }
		inline IRenderable*	GetRenderable(uint32 _uiIndex) const { return m_vRenderable[_uiIndex]; }
			
	private:
		Vector<Camera*>			m_vCamera;
		Vector<IRenderable*>	m_vRenderable;	// Node independant
		Octree*					m_pTree;
	};
}}

#endif