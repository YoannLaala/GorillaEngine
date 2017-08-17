#ifndef _COOKER_GEOMETRY_HPP_
#define _COOKER_GEOMETRY_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>
#include <Engine/Renderer/ResourceShared.hpp>
#include <Renderer/Geometry/BoundingBox.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	struct Geometry
	{
		Vector<Engine::RenderBuffer::Vertex::PositionNormalTexcoord> Vertices;
		Vector<uint32>				Indices;
		Vector<uint32>				Groups;
		Renderer::BoundingBox		BBox;

		void ComputeBBox()
		{
			BBox.Min = Vertices[0].Position;
			BBox.Max = Vertices[0].Position;

			const uint32 uiVertexCount = Vertices.GetSize();
			for(uint32 uiVertex = 0; uiVertex < uiVertexCount; ++uiVertex)
			{
				const Math::Vector3& vPosition = Vertices[uiVertex].Position;

				BBox.Min.SetX(Math::Min(BBox.Min.GetX(), vPosition.GetX()));
				BBox.Min.SetY(Math::Min(BBox.Min.GetY(), vPosition.GetY()));
				BBox.Min.SetZ(Math::Min(BBox.Min.GetZ(), vPosition.GetZ()));
				BBox.Max.SetX(Math::Max(BBox.Max.GetX(), vPosition.GetX()));
				BBox.Max.SetY(Math::Max(BBox.Max.GetY(), vPosition.GetY()));
				BBox.Max.SetZ(Math::Max(BBox.Max.GetZ(), vPosition.GetZ()));
			}
		}
	};
}

#endif
