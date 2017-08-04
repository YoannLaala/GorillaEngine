#ifndef _COMPONENT_POINT_LIGHT_HPP_
#define _COMPONENT_POINT_LIGHT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Component/RenderableNode.hpp>
#include <Renderer/Color.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Component
{
	class PointLight : public RenderableNode // Rendering simple geometry
	{
	public:
		PointLight();
		~PointLight();
		COMPONENT_DECLARE(Gorilla::Component::PointLight);

	protected:
		virtual void PushCommand(Renderer::RenderBuffer* _pBuffer) override;

	public:
		Renderer::Color	Color;
		float32			Radius;
		float32			Power;
	};
}}

#endif