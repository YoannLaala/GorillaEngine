#ifndef _SPOT_LIGHT_HPP_
#define _SPOT_LIGHT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "Light.hpp"

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class SpotLight : public Light
	{
	public:
		SpotLight();
		~SpotLight();								 

	private:
		float32			m_fAngle;
	};
}}

#endif