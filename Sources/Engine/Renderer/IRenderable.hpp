#ifndef _RENDERER_IRENDERABLE_HPP_
#define _RENDERER_IRENDERABLE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class RenderBuffer;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class IRenderable
	{
	public:
		virtual void PushCommand(RenderBuffer* /*_pBuffer*/) = 0;
	};
}}

#endif