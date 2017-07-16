#ifndef _COMPONENT_DECAL_HPP_
#define _COMPONENT_DECAL_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Component/RenderableNode.hpp>
#include <Engine/Asset/AssetHandle.hpp>
#include <Engine/Asset/Material/Material.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class RenderContext;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Component
{
	class Decal : public RenderableNode
	{
	public:
		Decal();
		~Decal();
		COMPONENT_DECLARE(Gorilla::Component::Decal);

	protected:
		virtual void PushCommand(Renderer::RenderBuffer* _pBuffer) override;

	public:
		Engine::AssetHandle<Engine::Material> Asset;
	};
}}

#endif