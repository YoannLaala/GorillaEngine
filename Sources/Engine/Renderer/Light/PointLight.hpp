#ifndef _POINT_LIGHT_HPP_
#define _POINT_LIGHT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "Light.hpp"

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class PointLight : public Light
	{
	public:
		PointLight();
		~PointLight();
									   
		// Radius									   
		inline float32		GetRadius			() const { return m_fRadius; }
		inline void			SetRadius			(float32 _fRadius) { m_fRadius = _fRadius; }

	private:
		float32			m_fRadius;
	};
}}

#endif