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
		Vector<uint32>				Sections;
		Renderer::BoundingBox		BBox;
	};
}

#endif
