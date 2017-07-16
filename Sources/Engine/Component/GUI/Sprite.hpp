#ifndef _COMPONENT_SPRITE_HPP_
#define _COMPONENT_SPRITE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Component/RenderableNode.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Texture2D;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Component
{
	class Sprite : public RenderableNode 
	{
	public:
		Sprite();
		~Sprite();
		COMPONENT_DECLARE(Gorilla::Component::Sprite);

	protected:
		virtual void PushCommand(Renderer::RenderBuffer* _pFrameBuffer) override;

	public:
		Renderer::Texture2D* Texture;
	};
}}

#endif