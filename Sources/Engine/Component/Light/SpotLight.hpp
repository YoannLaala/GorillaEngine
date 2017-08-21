#ifndef _COMPONENT_SPOT_LIGHT_HPP_
#define _COMPONENT_SPOT_LIGHT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Component/RenderableNode.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Component
{
	class SpotLight : public RenderableNode
	{
	public:
		SpotLight();
		~SpotLight();
		COMPONENT_DECLARE(Gorilla::Component::SpotLight);

	protected:
		virtual void PushCommand(Renderer::RenderBuffer* _pBuffer) override;

	private:
		float32			Size;
		float32			Power;
	};
}}

#endif