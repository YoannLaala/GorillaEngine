#ifndef _RENDERER_OCTREE_BLOCK_HPP_
#define _RENDERER_OCTREE_BLOCK_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Renderer;
	class Node;
	class Octree;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class OctreeBlock
	{
		friend class Renderer;
		friend class Octree;

	private:
		OctreeBlock();
		~OctreeBlock();

		void Initialize	();
		void Initialize	(uint16 _uiIndex, uint16 _uiDepth);

	public:
		bool Contains	(const Node* _pNode) const;

		inline const Math::Vector3& GetPosition() const { return m_vPosition; }
		inline float32				GetSize() const { return (float32)(1024 >> (m_uiDepth * 2)); }

		inline uint16 GetIndex() const { return (m_uiDepth * 8) + m_uiIndex; /*m_uiIndex;*/ }
		inline uint16 GetDepth() const { return m_uiDepth; }

	private:
		bool Contains	(const Math::Vector3& _vPosition) const;

	private:
		Math::Vector3	m_vPosition;
		uint16			m_uiIndex;
		uint16			m_uiDepth;
	};
}}

#endif