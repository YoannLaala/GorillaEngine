/******************************************************************************
**	Includes
******************************************************************************/
#include "Light.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor
	//!	@date		2015-11-07
	Light::Light()
		:	m_vColor(1.0f, 1.0f, 1.0f)
	{
		// Nothing to do
	}

	//!	@brief		Constructor
	//!	@date		2015-11-07
	Light::Light(float32 _fPower)
		:	m_vColor(1.0f, 1.0f, 1.0f)
		,	m_fPower(_fPower)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-11-20
	Light::~Light()
	{
		// Nothing to do
	}
}}