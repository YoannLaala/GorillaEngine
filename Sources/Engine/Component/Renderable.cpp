/******************************************************************************
**	Includes
******************************************************************************/
#include "Renderable.hpp"

#include <Engine/Object/GameObject.hpp>
#include <Engine/Component/Component.hpp>
#include <Engine/World.hpp>
#include <Renderer/Scene.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Component
{		
	//!	@brief		Constructor
	//!	@date		2015-12-29
	Renderable::Renderable()
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-12-29
	Renderable::~Renderable()
	{
		// Nothing to do
	}

	//!	@brief		Start
	//!	@date		2016-03-26
	void Renderable::Start()
	{
		GetGameObject()->GetWorld()->GetScene()->AddRenderable(this);
	}

	//!	@brief		Stop
	//!	@date		2016-03-26
	void Renderable::Stop()
	{
		GetGameObject()->GetWorld()->GetScene()->RemoveRenderable(this);
	}
}}	
