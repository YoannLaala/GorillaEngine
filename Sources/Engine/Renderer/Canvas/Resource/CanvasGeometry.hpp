#ifndef _RENDERER_CANVAS_GEOMETRY_HPP_
#define _RENDERER_CANVAS_GEOMETRY_HPP_

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
	class CanvasGeometry : public CanvasResource
	{
	public:
        CanvasGeometry();
        ~CanvasGeometry();

		static EResource::Type GetType() { return EResource::Geometry; }

        void Initialize(void* _pHandle);
        virtual void Release() override;
        
        inline void* GetHandle() { return m_pHandle; }

    private:
        void* m_pHandle;
	};
}}

#endif