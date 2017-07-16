#ifndef _RENDERER_CANVAS_BRUSH_HPP_
#define _RENDERER_CANVAS_BRUSH_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/Common.hpp>
#include <Renderer/Canvas/Resource/CanvasResource.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class CanvasBrush : public CanvasResource
	{
	public:
        CanvasBrush();
        ~CanvasBrush();

		static EResource::Type GetType() { return EResource::Buffer; }

        void Initialize(void* _pHandle);
        virtual void Release() override;
        
        inline void* GetHandle() { return m_pHandle; }

    private:
        void* m_pHandle;
	};
}}

#endif