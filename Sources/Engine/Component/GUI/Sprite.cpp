/******************************************************************************
**	Includes
******************************************************************************/
#include "Sprite.hpp"

#include <Engine/Engine.hpp>
#include <Engine/Renderer/ResourceShared.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/Viewport.hpp>
#include <Renderer/Resource/Texture2D.hpp>
#include <Renderer/Resource/RenderTarget.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Component
{	
	COMPONENT_REGISTER(Gorilla::Component::Sprite)

	//!	@brief		Constructor
	//!	@date		2015-04-04
	Sprite::Sprite()
		: Texture(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	Sprite::~Sprite()
	{
		// Nothing to do
	}

	//!	@brief		PushCommand
	//!	@date		2015-04-04
	void Sprite::PushCommand(Renderer::RenderBuffer* _pFrameBuffer)
	{
		if(Texture)
		{
			Engine::RenderBuffer::Operation::TextureCopy* pOperation = _pFrameBuffer->Push<Engine::RenderBuffer::Operation::TextureCopy>();
			pOperation->Type = Engine::RenderBuffer::Operation::Texture2Texture;
			pOperation->Box.Set(0.0f, 0.0f, static_cast<float32>(Texture->GetWidth()), static_cast<float32>(Texture->GetHeight()));
			pOperation->Source = Texture;
			pOperation->Destination = GetRenderer()->GetViewport(0)->GetRenderTarget()->GetTarget(0);
		}
	}
}}	
