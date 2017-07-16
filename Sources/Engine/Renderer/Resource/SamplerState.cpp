/******************************************************************************
**	Includes
******************************************************************************/
#include "SamplerState.hpp"
#include <Renderer/Renderer.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	SamplerState::SamplerState()
		: m_pHandle(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	SamplerState::~SamplerState()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2015-11-11
	void SamplerState::Initialize(void* _pHandle)
	{
		m_pHandle = _pHandle;
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void SamplerState::Release()
	{

	}
}}