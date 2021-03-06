/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/Resource/Texture3D.hpp>
#include <Renderer/Renderer.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	Texture3D::Texture3D()
		: m_uiWidth(0)
		, m_uiHeight(0)
		, m_pHandle(NULL)
	{
		memset(m_aView, 0, sizeof(void*) * EView::Count);
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	Texture3D::~Texture3D()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2015-11-11
	void Texture3D::Initialize(uint32 _uiWidth, uint32 _uiHeight, void* _pHandle)
	{
		m_uiWidth = _uiWidth;
		m_uiHeight = _uiHeight;
		m_pHandle = _pHandle;
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void Texture3D::Release()
	{
		Renderer::DestroyHandle(m_pHandle);
		//Renderer::DestroyHandle(m_pView);
	}
}}