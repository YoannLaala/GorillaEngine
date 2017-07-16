/******************************************************************************
**	Includes
******************************************************************************/
#include "PointLight.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor
	//!	@date		2015-11-07
	PointLight::PointLight()
		: m_fRadius(1.0f)
		, Light(1200.0f)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-11-07
	PointLight::~PointLight()
	{
		// Nothing to do
	}
}}