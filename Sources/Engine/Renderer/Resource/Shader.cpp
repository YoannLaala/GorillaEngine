/******************************************************************************
**	Includes
******************************************************************************/
#include "Shader.hpp"
#include <Renderer/Renderer.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	Shader::Shader()
		: m_pInputLayout(NULL)
		, m_pHandle(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	Shader::~Shader()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2015-11-11
	void Shader::Initialize(void* _pHandle, void* _pInputLayout /*= NULL*/)
	{
		m_pHandle = _pHandle;
		m_pInputLayout = _pInputLayout;
	}

	//!	@brief		Initialize 
	//!	@date		2015-11-11
	void Shader::Release()
	{
		if(m_pInputLayout)
		{
			Renderer::DestroyHandle(m_pInputLayout);
			m_pInputLayout = NULL;
		}

		if(m_pHandle)
		{
			Renderer::DestroyHandle(m_pHandle);
			m_pHandle = NULL;
		}
	}
}}