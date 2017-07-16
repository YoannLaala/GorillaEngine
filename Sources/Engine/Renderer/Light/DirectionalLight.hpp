#ifndef _DIRECTINNAL_LIGHT_HPP_
#define _DIRECTIONAL_LIGHT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "Light.hpp"

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight();
		~DirectionalLight();
	};
}}

#endif