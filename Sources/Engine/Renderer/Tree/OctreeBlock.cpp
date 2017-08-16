/******************************************************************************
**	Includes
******************************************************************************/
#include "OctreeBlock.hpp"
#include <Renderer/Node.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor
	//!	@date		2015-04-04
	OctreeBlock::OctreeBlock()
		: m_uiIndex(0)
		, m_uiDepth(0)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	OctreeBlock::~OctreeBlock()
	{
		// Nothing to do
	}

	//!	@brief		Initialize
	//!	@date		2015-04-04
	void OctreeBlock::Initialize()
	{		
		// Nothing to do
	}

	//!	@brief		Initialize
	//!	@date		2015-04-04
	void OctreeBlock::Initialize(uint16 _uiIndex, uint16 _uiDepth)
	{
		static float32 fFactor[8][3] = 
		{
			{-1.0f, +1.0f, +1.0f},
			{-1.0f, +1.0f, -1.0f},
			{+1.0f, +1.0f, +1.0f},
			{+1.0f, +1.0f, -1.0f},
			{-1.0f, -1.0f, +1.0f},
			{-1.0f, -1.0f, -1.0f},
			{+1.0f, -1.0f, +1.0f},
			{+1.0f, -1.0f, -1.0f},
		};

		m_uiIndex = _uiIndex;
		m_uiDepth = _uiDepth;
		
		// Initialize position
		float32 fOffset = GetSize();
		m_vPosition.Set(fFactor[_uiIndex][0] * fOffset, fFactor[_uiIndex][1] * fOffset, fFactor[_uiIndex][2] * fOffset);
	}

	//!	@brief		IsInside
	//!	details		Check if the position is located inside the block
	//!	@date		2015-11-07
	bool OctreeBlock::Contains(const Node* _pNode) const
	{
		return Contains(_pNode->GetTransform().GetTranslation());
	}

	//!	@brief		IsInside
	//!	details		Check if the position is located inside the block
	//!	@date		2015-11-07
	bool OctreeBlock::Contains(const Math::Vector3& _vPosition) const
	{
		float32 fBlockSize = GetSize();	

		// Check each coordinate
		const float32* pBlockPosition = reinterpret_cast<const float32*>(&m_vPosition);
		const float32* pNodePosition = reinterpret_cast<const float32*>(&_vPosition);
		for(uint32 uiCoordinate = 0; uiCoordinate < 3; ++uiCoordinate)
		{
			if(pNodePosition[uiCoordinate] < pBlockPosition[uiCoordinate] - fBlockSize || pNodePosition[uiCoordinate] > pBlockPosition[uiCoordinate] + fBlockSize)
			{
				return false;
			}
		}

		return true;
	}

	//!	@brief		IsEmbbeded
	//!	details		Check if the Node fit entirely in the block
	//!	@date		2015-11-07
	/*template <uint8 Dimension>
	bool OctreeBlock<Dimension>::Embbeded(const Node* _pNode) const
	{
		const Math::Vector3& vPosition = _pNode->GetPosition();
		return Embbeded(&vPosition[0])
	}

	//!	@brief		IsEmbbeded
	//!	details		Check if the Node fit entirely in the block
	//!	@date		2015-11-07
	template <uint8 Dimension>
	bool OctreeBlock<Dimension>::Embbeded(const float32* _pPosition, const float32* _pSize) const
	{
		float32 fBlockSize = (FLT_MAX >> m_uiIndex);
		float32 fNodeRadius = 1.0f * 0.5f;

		// Check each coordinate
		const Math::Vector3& vNodePosition = _pNode->GetPosition();
		for(uint32 uiCoordinate = 0; uiCoordinate < Dimension; ++uiCoordinate)
		{
			if(vNodePosition[uiCoordinate] - fNodeRadius < m_vPosition[uiCoordinate] - fBlockSize || vNodePosition[uiCoordinate] + fNodeRadius > m_vPosition[uiCoordinate] + fBlockSize)
			{
				return false;
			}
		}

		return true;
	}*/
}}