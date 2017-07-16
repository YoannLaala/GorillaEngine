#ifndef _COMPONENT_RENDERABLE_HPP_
#define _COMPONENT_RENDERABLE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Component/Component.hpp>
#include <Renderer/IRenderable.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Component
{
	class Renderable : public Engine::Component, public Renderer::IRenderable
	{
	public:
		Renderable();
		~Renderable();

	protected:
		virtual void Start			() override;
		virtual void Stop			() override;
	};
}}

#endif