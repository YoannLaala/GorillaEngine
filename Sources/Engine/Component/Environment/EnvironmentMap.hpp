#ifndef _COMPONENT_ENVIRONMENT_MAP_HPP_
#define _COMPONENT_ENVIRONMENT_MAP_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Component/RenderableNode.hpp>
#include <Engine/Asset/AssetHandle.hpp>
#include <Engine/Asset/Texture/CubeMap.hpp>

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
	class EnvironmentMap : public RenderableNode
	{
	public:
		EnvironmentMap();
		~EnvironmentMap();
		COMPONENT_DECLARE(Gorilla::Component::EnvironmentMap);

	protected:
		virtual void PushCommand(Renderer::RenderBuffer* _pBuffer) override;

	public:
		Engine::AssetHandle<Engine::CubeMap> Asset;
	};
}}

#endif