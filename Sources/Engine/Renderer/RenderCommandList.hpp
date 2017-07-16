#ifndef _RENDERER_RENDER_COMMAND_LIST_HPP_
#define _RENDERER_RENDER_COMMAND_LIST_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Renderer;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class RenderCommandList
	{
		friend class Renderer;

	private:
		RenderCommandList();
		~RenderCommandList();

		void Initiliaze(void* _pHandle);
		void Release();

	public:
		inline void* GetHandle() { return m_pHandle; }

	private:
		void* m_pHandle;
	};
}}

#endif