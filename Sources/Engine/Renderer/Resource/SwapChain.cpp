/******************************************************************************
**	Includes
******************************************************************************/
#include "SwapChain.hpp"
#include <Renderer/Resource/Texture2D.hpp>
#include <Renderer/Renderer.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	SwapChain::SwapChain()
		: m_pWindow(NULL)
		, m_pHandle(NULL)
		, m_pTexture(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	SwapChain::~SwapChain()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2015-11-11
	void SwapChain::Initialize(Texture2D* _pTexture)
	{
		m_pTexture = _pTexture;
	}

	//!	@brief		Release
	//!	@date		2015-11-11
	void SwapChain::Release()
	{
		if(m_pHandle) Renderer::DestroyHandle(m_pHandle);
		m_pHandle = nullptr;
		
		if(m_pTexture) Renderer::GetInstance()->DestroyResource(m_pTexture);
		m_pTexture = nullptr;
			
		m_pWindow = nullptr;
	}
}}