#ifndef _COMPONENT_DIRECTIONAL_LIGHT_HPP_
#define _COMPONENT_DIRECTIONAL_LIGHT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Component/Renderable.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Component
{
	class DirectionalLight : public Renderable // Rendering simple geometry
	{
	public:
		DirectionalLight();
		~DirectionalLight();
		COMPONENT_DECLARE(Gorilla::Component::DirectionalLight);

	protected:
		virtual void PushCommand(Renderer::RenderBuffer* _pBuffer) override;

	public:
		Math::Vector3	Color; 	
		float32			Power;
	};
}}

#endif