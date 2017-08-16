#ifndef _RENDERER_NODE_HPP_
#define _RENDERER_NODE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Octree;
	class IRenderable;
	class Gizmo;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Node
	{
		friend class Octree;
		friend class Gizmo;

	public:
		Node();
		~Node();

	public:
		inline uint32						GetLayer		() const { return m_uiLayer; };
		inline void							SetLayer		(uint32 _uiLayer) { m_uiLayer = _uiLayer; }
		
		Math::Matrix44& 					GetTransform	() { return m_mGlobal; }
		const Math::Matrix44& 				GetTransform	() const { return m_mGlobal; }

		// Component
		inline void							AddRenderable		(IRenderable* _pObject) { m_vRenderable.Add(_pObject); }
		inline void							RemoveRenderable	(IRenderable* _pObject) { m_vRenderable.Remove(_pObject); }
		inline const Vector<IRenderable*>&	GetVecRenderable	() const { return m_vRenderable; }

	private:
		Vector<IRenderable*>		m_vRenderable;
		Math::Matrix44				m_mGlobal;
		uint32						m_uiLayer;
	};
}}

#endif