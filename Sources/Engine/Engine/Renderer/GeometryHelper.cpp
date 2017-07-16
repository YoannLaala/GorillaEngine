/******************************************************************************
**	Includes
******************************************************************************/
#include "GeometryHelper.hpp"
#include <Renderer/RenderContext.hpp>
#include <Renderer/Resource/Geometry.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	//!	@brief		CreatePoint
	//!	@date		2015-11-07
	void CreatePoint(Vector<uint8>& _vVertexOut, uint32& _uiVertexSize, Vector<uint32>& _vIndexOut)
	{
		// Vertices
		_uiVertexSize = sizeof(RenderBuffer::Vertex::PositionTexcoord);
		_vVertexOut.Resize(_uiVertexSize);
		RenderBuffer::Vertex::PositionTexcoord* pArray = reinterpret_cast<RenderBuffer::Vertex::PositionTexcoord*>(&_vVertexOut[0]);
		pArray[0].Position.Set(0.0f, 0.0f, 0.0f);
		pArray[0].Texcoord.Set(+0.0f, +0.0f);

		// Indices
		_vIndexOut.Add(0);
	}

	//!	@brief		CreateQuad
	//!	@date		2015-11-07
	void CreateQuad(Vector<uint8>& _vVertexOut, uint32& _uiVertexSize, Vector<uint32>& _vIndexOut)
	{
		// Vertices
		_uiVertexSize = sizeof(RenderBuffer::Vertex::PositionTexcoord);
		_vVertexOut.Resize(4 * _uiVertexSize);
		RenderBuffer::Vertex::PositionTexcoord* pArray = reinterpret_cast<RenderBuffer::Vertex::PositionTexcoord*>(&_vVertexOut[0]);
		pArray[0].Position.Set(-1.0f, -1.0f, 1.0f);
		pArray[1].Position.Set(-1.0f, +1.0f, 1.0f);
		pArray[2].Position.Set(+1.0f, +1.0f, 1.0f);
		pArray[3].Position.Set(+1.0f, -1.0f, 1.0f);

		pArray[0].Texcoord.Set(+0.0f, +1.0f);
		pArray[1].Texcoord.Set(+0.0f, +0.0f);
		pArray[2].Texcoord.Set(+1.0f, +0.0f);
		pArray[3].Texcoord.Set(+1.0f, +1.0f);

		// Indices
		_vIndexOut.Resize(6);
		_vIndexOut[0] = 0; 
		_vIndexOut[1] = 1;
		_vIndexOut[2] = 2;
		_vIndexOut[3] = 2;
		_vIndexOut[4] = 3;
		_vIndexOut[5] = 0;
	}

	//!	@brief		CreateQuad
	//!	@date		2015-11-07
	void CreateCube(Vector<uint8>& _vVertexOut, uint32& _uiVertexSize, Vector<uint32>& _vIndexOut)
	{
		// Vertices
		_uiVertexSize = sizeof(RenderBuffer::Vertex::PositionTexcoord);
		_vVertexOut.Resize(8 * _uiVertexSize);
		RenderBuffer::Vertex::PositionTexcoord* pArray = reinterpret_cast<RenderBuffer::Vertex::PositionTexcoord*>(&_vVertexOut[0]);
		pArray[0].Position.Set(-1.0f, -1.0f, -1.0f);
		pArray[1].Position.Set(-1.0f, +1.0f, -1.0f);
		pArray[2].Position.Set(+1.0f, +1.0f, -1.0f);
		pArray[3].Position.Set(+1.0f, -1.0f, -1.0f);
		pArray[4].Position.Set(-1.0f, -1.0f, +1.0f);
		pArray[5].Position.Set(-1.0f, +1.0f, +1.0f);
		pArray[6].Position.Set(+1.0f, +1.0f, +1.0f);
		pArray[7].Position.Set(+1.0f, -1.0f, +1.0f);

		pArray[0].Texcoord.Set(+0.0f, +1.0f);
		pArray[1].Texcoord.Set(+0.0f, +0.0f);
		pArray[2].Texcoord.Set(+1.0f, +0.0f);
		pArray[3].Texcoord.Set(+1.0f, +1.0f);
		pArray[4].Texcoord.Set(+0.0f, +1.0f);
		pArray[5].Texcoord.Set(+0.0f, +0.0f);
		pArray[6].Texcoord.Set(+1.0f, +0.0f);
		pArray[7].Texcoord.Set(+1.0f, +1.0f);

		// Indices
		_vIndexOut.Resize(36);
		uint32* pIndex = &_vIndexOut[0];

		// Front
		*(pIndex)	= 0; 
		*(++pIndex) = 1; 
		*(++pIndex) = 2;
		*(++pIndex) = 2;
		*(++pIndex) = 3;
		*(++pIndex) = 0;

		// Back
		*(++pIndex) = 7; 
		*(++pIndex) = 6; 
		*(++pIndex) = 5;
		*(++pIndex) = 5;
		*(++pIndex) = 4;
		*(++pIndex) = 7;

		// Left
		*(++pIndex) = 4; 
		*(++pIndex) = 5; 
		*(++pIndex) = 1;
		*(++pIndex) = 1;
		*(++pIndex) = 0;
		*(++pIndex) = 4;

		// Right
		*(++pIndex) = 3; 
		*(++pIndex) = 2; 
		*(++pIndex) = 6;
		*(++pIndex) = 6;
		*(++pIndex) = 7;
		*(++pIndex) = 3;

		// Top
		*(++pIndex) = 1; 
		*(++pIndex) = 5; 
		*(++pIndex) = 6;
		*(++pIndex) = 6;
		*(++pIndex) = 2;
		*(++pIndex) = 1;

		// Bottom
		*(++pIndex) = 3; 
		*(++pIndex) = 0; 
		*(++pIndex) = 4;
		*(++pIndex) = 4;
		*(++pIndex) = 7;
		*(++pIndex) = 3;
	}

	//!	@brief		GetOrCreate
	//!	@date		2015-11-07
	Renderer::Geometry* GeometryHelper::GetOrCreate(Renderer::Renderer* _pRenderer, Renderer::EPrimitive::Type _ePrimitive)
	{
		Renderer::RenderBuffer* pGlobalBuffer = _pRenderer->GetGlobalBuffer();

		// Create once the shared primitive
		Renderer::EPrimitive* pPrimitive; uint32 uiCount;
		if(!pGlobalBuffer->Get<Renderer::EPrimitive>(&pPrimitive, uiCount))
		{
			pPrimitive = pGlobalBuffer->Push<Renderer::EPrimitive>(Renderer::EPrimitive::Count);
			memset(pPrimitive, 0, Renderer::EPrimitive::Count * sizeof(void*));
		}

		Renderer::Geometry*& pGeometry = pPrimitive[_ePrimitive].Geometry;
		if(!pGeometry)
		{
			uint32 uiVertexSize = 0;
			Vector<uint8> vVertex;
			Vector<uint32> vIndex;
			switch(_ePrimitive)
			{
				case Renderer::EPrimitive::Point: CreatePoint(vVertex, uiVertexSize, vIndex);break;
				case Renderer::EPrimitive::Quad: CreateQuad(vVertex, uiVertexSize, vIndex);break;
				case Renderer::EPrimitive::Cube: CreateCube(vVertex, uiVertexSize, vIndex);break;
			}

			pGeometry = _pRenderer->CreateGeometry(&vVertex[0], uiVertexSize, vVertex.GetSize() / uiVertexSize, &vIndex[0], vIndex.GetSize());
		}

		return pGeometry;
	}
}}
