/******************************************************************************
**	Includes
******************************************************************************/
#include "Geometry.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor
	//!	@date		2015-10-18
	Geometry::Geometry()
		: m_pVertexBuffer(NULL)
		, m_pIndexBuffer(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Constructor
	//!	@date		2015-10-18
	Geometry::~Geometry()
	{
		// Nothing to do
	}

	//!	@brief		Initialize
	//!	@date		2015-10-18
	void Geometry::Initialize(Buffer* _pVertexBuffer, Buffer* _pIndexBuffer, const uint32* _pGroupBuffer, uint32 _uiGroupCount)
	{
		m_pVertexBuffer = _pVertexBuffer;
		m_pIndexBuffer = _pIndexBuffer;
		m_vGroup.Resize(_uiGroupCount);

		uint32 uiIndexOffset = 0;
		for(uint32 uiGroup = 0; uiGroup < _uiGroupCount; ++uiGroup)
		{
			Group& kGroup = m_vGroup[uiGroup];
			kGroup.IndexOffset = uiIndexOffset;
			kGroup.IndexCount = _pGroupBuffer[uiGroup];
			uiIndexOffset += kGroup.IndexCount;
		}
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void Geometry::Release()
	{
		
	}
}}