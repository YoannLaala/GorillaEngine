/******************************************************************************
**	Includes
******************************************************************************/
#include "DirectionalLight.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor
	//!	@date		2015-11-07
	DirectionalLight::DirectionalLight()
		: Light(64000.0f)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-11-07
	DirectionalLight::~DirectionalLight()
	{
		// Nothing to do
	}
}}