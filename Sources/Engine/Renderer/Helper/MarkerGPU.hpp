#ifndef _RENDERER_MARKER_GPU_HPP_
#define _RENDERER_MARKER_GPU_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class RenderContext;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class MarkerGPU
	{
	public:
		MarkerGPU(RenderContext* _pContext, const char* _szName);
		~MarkerGPU();

	private:
		RenderContext*	m_pContext;
		void*			m_pHandle;
	};
}}

#endif