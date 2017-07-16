#ifndef _ENGINE_GEOMETRY_HELPER_HPP_
#define _ENGINE_GEOMETRY_HELPER_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Renderer/ResourceShared.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Renderer;
	class Geometry;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class GeometryHelper
	{
	public:
		static Renderer::Geometry* GetOrCreate(Renderer::Renderer* _pRenderer, Renderer::EPrimitive::Type _ePrimitive);
	};
}}

#endif