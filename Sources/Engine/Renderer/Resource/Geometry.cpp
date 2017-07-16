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
	void Geometry::Initialize(Buffer* _pVertexBuffer, Buffer* _pIndexBuffer, const uint32* _pSectionBuffer, uint32 _uiSectionCount)
	{
		m_pVertexBuffer = _pVertexBuffer;
		m_pIndexBuffer = _pIndexBuffer;
		m_vSection.Resize(_uiSectionCount);
		memcpy_s(&m_vSection[0], _uiSectionCount * sizeof(uint32), _pSectionBuffer, _uiSectionCount * sizeof(uint32));
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void Geometry::Release()
	{
		
	}
}}