#ifndef _RENDERER_OCTREE_HPP_
#define _RENDERER_OCTREE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>
#include <Renderer/Tree/OctreeBlock.hpp>

/******************************************************************************
**	Defines
******************************************************************************/
#define OCTREE_TREE_SECTION			8

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Renderer;
	class Frustum;
	class Node;
}}

namespace Gorilla { namespace Renderer
{
	class Octree
	{
		friend class Renderer;
		friend class Scene;
	
	private:
		Octree();
		~Octree();

		void Add			(Node* _pNode);
		void Update			(Node* _pNode);
		void Remove			(Node* _pNode);

	public:
		void FindVisible(const Frustum& _kFrustum, Vector<Node*>& _vNodeOut) const;

	private:
		bool	AddNodeInBlock			(Node* _pNode, OctreeBlock* _pBlock);
		bool	AddNodeInBlockChild		(Node* _pNode, OctreeBlock* _pBlock);
		bool	RemoveNodeInBlock		(Node* _pNode, OctreeBlock* _pBlock);

	private:
		void	FindVisible				(const Frustum& _kFrustum, const OctreeBlock* _pBlock, Vector<Node*>& _vNodeOut) const;

	protected:
		OctreeBlock				m_kRoot;
		Vector<OctreeBlock*>	m_vChild;
		Vector<Vector<Node*>*>	m_mNode;
	};
}}

#endif