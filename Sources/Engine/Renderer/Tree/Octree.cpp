/******************************************************************************
**	Includes
******************************************************************************/
#include "Octree.hpp"
#include <Renderer/Node.hpp>
#include <Renderer/Frustum.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2015-04-04
	Octree::Octree()
	{
		m_kRoot.Initialize();
		m_mNode.Add(NULL);	// Root block
	}

	//!	@brief		Destructor 
	//!	@date		2015-04-04
	Octree::~Octree()
	{
		// Nothing to do
	}

	//!	@brief		Add
	//!	@date		2015-04-04
	void Octree::Add(Node* _pNode)
	{
		AddNodeInBlock(_pNode, &m_kRoot);
	}

	//!	@brief		Update 
	//!	@date		2015-04-04
	void Octree::Update(Node* /*_pNode*/)
	{
		//AddNodeInBlock(_pNode, &m_kRoot);
	}

	//!	@brief		Remove 
	//!	@date		2015-11-07
	void Octree::Remove(Node* _pNode)
	{
		RemoveNodeInBlock(_pNode, &m_kRoot);
	}

	//!	@brief		FindVisible 
	//!	@date		2015-11-11
	void Octree::FindVisible(const Frustum& _kFrustum, Vector<Node*>& _vNodeOut) const
	{
		FindVisible(_kFrustum, &m_kRoot, _vNodeOut);
	}

	//!	@brief		FindVisible 
	//!	@date		2015-11-11
	void Octree::FindVisible(const Frustum& _kFrustum, const OctreeBlock* _pBlock, Vector<Node*>& _vNodeOut) const
	{
		if(!_kFrustum.IsCubeVisible(_pBlock->GetPosition(), _pBlock->GetSize()))
		{
			return;
		}
	
		// Add all node in the current block
		uint32 uiIndex = _pBlock->GetIndex();
		if(uiIndex < m_mNode.GetSize())
		{
			// Check the Node vector, if empty NULL there is no need to try to iterate on children
			const Vector<Node*>* pVecNode = m_mNode[uiIndex];
			if(!pVecNode)
			{
				return;
			}
			_vNodeOut.Add(*pVecNode);
		}
		
		// Iterate on each block child
		if(uiIndex < m_vChild.GetSize())
		{
			const OctreeBlock* pBlockArray = m_vChild[uiIndex];
			if(pBlockArray)
			{
				for(uint32 uiChild = 0; uiChild < OCTREE_TREE_SECTION; ++uiChild)
				{
					FindVisible(_kFrustum, &pBlockArray[uiChild], _vNodeOut);
				}
			}
		}		
	}

	//!	@brief		AddNodeInBlock 
	//!	@date		2015-04-04
	bool Octree::AddNodeInBlock(Node* _pNode, OctreeBlock* _pBlock)
	{
		// Check if the node is inside this block
		if(!_pBlock->Contains(_pNode))
		{
			return false;
		}

		// If there isn't Node in this Block - we have to return it
		uint16 uiIndex = _pBlock->GetIndex(); m_mNode.ExpandTo(uiIndex, NULL);
		Vector<Node*>* pVecNodeSource = m_mNode[uiIndex];
		if(!pVecNodeSource)
		{
			pVecNodeSource = new Vector<Node*>();
			pVecNodeSource->Add(_pNode);
			m_mNode[uiIndex] = pVecNodeSource;
			return true;
		}

		// Limit the depth && Check the last one is in the same position
		uint16 uiDepth = _pBlock->GetDepth();
		if( uiDepth >= 0)
		{
			pVecNodeSource->Add(_pNode);
			return true;
		}
		
		// We need to split this block
		const uint16 uiBlockArrayCount = static_cast<uint16>(m_vChild.GetSize());
		if(uiIndex >= uiBlockArrayCount)
		{
			// Allocate enough memory to add this array to the vector of child
			m_vChild.ExpandTo(uiIndex, NULL);
			for(uint16 uiBlockArray = uiBlockArrayCount; uiBlockArray <= uiIndex; ++uiBlockArray)
			{
				uint16 uiChildDepth = (uiBlockArray / OCTREE_TREE_SECTION) + 1;

				// Create new Array of Block representing child of the current block
				OctreeBlock* pBlockArray = new OctreeBlock[OCTREE_TREE_SECTION];
				OctreeBlock* pBlockChild = pBlockArray;
				for(uint16 uiChild = 0; uiChild < OCTREE_TREE_SECTION; ++uiChild)
				{
					pBlockChild->Initialize(uiChild, uiChildDepth);
					++pBlockChild;
				}
				m_vChild[uiBlockArray] = pBlockArray;
			}

			// Move node inside this block to proper block child
			const uint32 uiNodeCount = pVecNodeSource->GetSize();
			for(uint16 uiNode = 0; uiNode < uiNodeCount; ++uiNode)
			{
				Node*& pNodeSource = pVecNodeSource->Get(uiNode);
				if(AddNodeInBlockChild(pNodeSource, _pBlock))
				{
					pVecNodeSource->Remove(pNodeSource);
				}
			}	
		}
	
		// At this point the current block has been split 
		// We need to find the proper block among his childs
		return AddNodeInBlockChild(_pNode, _pBlock);
	}

	//!	@brief		RemoveNodeInBlock 
	//!	@date		2015-04-04
	bool Octree::RemoveNodeInBlock(Node* _pNode, OctreeBlock* _pBlock)
	{
		// Check if the node is inside this block
		if(!_pBlock->Contains(_pNode)) return false;

		// If there isn't Node in this Block - we have to return it
		uint16 uiBlockIndex = _pBlock->GetIndex();
		if(m_mNode.GetSize() <= uiBlockIndex ) return false;

		// Check if there is Node attached to this block
		Vector<Node*>* pVecNodeSource = m_mNode[uiBlockIndex];
		if(pVecNodeSource)
		{
			// Search node in this block
			uint32 uiNodeIndex = pVecNodeSource->Find(_pNode);
			if(uiNodeIndex != (uint32)-1)
			{
				pVecNodeSource->RemoveIndex(uiNodeIndex);
				return true;
			}

			// Search Node in childs
			if(m_vChild.GetSize() >= uiBlockIndex ) return false;

			OctreeBlock* pBlockArray = m_vChild[uiBlockIndex];
			for(uint16 uiChild = 0; uiChild < OCTREE_TREE_SECTION; ++uiChild)
			{
				if(RemoveNodeInBlock(_pNode, &pBlockArray[uiChild]))
				{
					return true;
				}
			}
		}
		
		return false;
	}

	//!	@brief		FindBlockIndexInChild 
	//!	@date		2015-04-04
	bool Octree::AddNodeInBlockChild(Node* _pNode, OctreeBlock* _pBlock)
	{	
		uint16 uiIndex = _pBlock->GetIndex();
		OctreeBlock* pBlockArray = m_vChild[uiIndex];
		for(uint16 uiChild = 0; uiChild < OCTREE_TREE_SECTION; ++uiChild)
		{
			if(AddNodeInBlock(_pNode, &pBlockArray[uiChild]))
			{
				return true;
			}
		}

		return false;
	}
}}