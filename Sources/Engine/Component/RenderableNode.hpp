#ifndef _COMPONENT_RENDERABLE_NODE_HPP_
#define _COMPONENT_RENDERABLE_NODE_HPP_

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
	class RenderableNode : public Engine::Component, public Renderer::IRenderable
	{
	public:
		RenderableNode();
		~RenderableNode();

	protected:
		virtual void Start			() override;
		virtual void Stop			() override;
	};
}}

#endif