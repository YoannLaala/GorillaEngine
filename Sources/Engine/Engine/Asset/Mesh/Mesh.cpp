/******************************************************************************
**	Includes
******************************************************************************/
#include "Mesh.hpp"

#include <Engine/Engine.hpp>
#include <Engine/Renderer/ResourceShared.hpp>
#include <Renderer/Geometry/BoundingBox.hpp>
#include <Renderer/Resource/Geometry.hpp>
#include <Renderer/Renderer.hpp>

#include <Core/Helper/Compression.hpp>
#include <Core/Stream/MemoryReader.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	ASSET_REGISTER(Gorilla::Engine::Mesh);

	//!	@brief		Contructor
	//!	@date		2015-10-18
	Mesh::Mesh()
		: m_pGeometry(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-10-18
	Mesh::~Mesh()
	{
		// Nothing to do
	}

	//!	@brief		Initialize
	//!	@date		2015-10-18
	void Mesh::Initialize(StreamReader* _pStream)
	{
		// Read compressed data
		uint32 uiSizeCompressed;
		_pStream->Read(uiSizeCompressed);
		uint8* pBufferCompressed = new uint8[uiSizeCompressed];
		_pStream->Read(pBufferCompressed, uiSizeCompressed);

		// Descompress the data
		uint32 uiSizeDecompressed;
		uint8* pBufferDecompressed;
		Helper::Decompress(pBufferCompressed, uiSizeCompressed, pBufferDecompressed, uiSizeDecompressed);
		uint8* pBuffer = pBufferDecompressed;

		// Read vertices		
		uint32 uiVertexCount = *reinterpret_cast<uint32*>(pBuffer);
		pBuffer += sizeof(uint32);

		RenderBuffer::Vertex::PositionNormalTexcoord* pVertices = reinterpret_cast<RenderBuffer::Vertex::PositionNormalTexcoord*>(pBuffer);
		pBuffer += uiVertexCount * sizeof(RenderBuffer::Vertex::PositionNormalTexcoord);

		// Read indices
		uint32 uiIndexCount= *reinterpret_cast<uint32*>(pBuffer);
		pBuffer += sizeof(uint32);

		uint32* pIndices = reinterpret_cast<uint32*>(pBuffer);
		pBuffer += uiIndexCount * sizeof(uint32);

		// Read sections
		uint32* pSections = NULL;
		uint32 uiSectionCount = *reinterpret_cast<uint32*>(pBuffer);
		pBuffer += sizeof(uint32);	
		if(uiSectionCount)
		{
			pSections = reinterpret_cast<uint32*>(pBuffer);
			pBuffer += uiSectionCount * sizeof(uint32);
		}

		// BoundingBox
		m_kBoundingBox = *reinterpret_cast<Renderer::BoundingBox*>(pBuffer);

		// Create Geometry
		m_pGeometry = GetRenderer()->CreateGeometry(pVertices, sizeof(RenderBuffer::Vertex::PositionNormalTexcoord), uiVertexCount, pIndices, uiIndexCount, pSections, uiSectionCount);
		SAFE_DELETE(pBufferCompressed);
		SAFE_DELETE(pBufferDecompressed);
	}

	//!	@brief		Release
	//!	@date		2015-10-18
	void Mesh::Release()
	{
		if(m_pGeometry)
		{
			GetRenderer()->DestroyResource(m_pGeometry);
			m_pGeometry = NULL;
		}
	}
}}