#ifndef _RENDERER_BOUDING_BOX_HPP_
#define _RENDERER_BOUDING_BOX_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Viewport;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	struct BoundingBox
	{
		Math::Vector3 Min;
		Math::Vector3 Max;
	};
}}

#endif