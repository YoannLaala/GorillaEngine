/******************************************************************************
**	Includes
******************************************************************************/
#include <Component/Environment/EnvironmentMap.hpp>
#include <Engine/Renderer/ResourceShared.hpp>
#include <Renderer/RenderContext.hpp>	
#include <Engine/Object/GameObject.hpp>	
#include <Component/Node.hpp>	

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Component
{
	COMPONENT_REGISTER(Gorilla::Component::EnvironmentMap)

	//!	@brief		Constructor
	//!	@date		2015-04-04
	EnvironmentMap::EnvironmentMap()
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	EnvironmentMap::~EnvironmentMap()
	{
		// Nothing to do
	}

	//!	@brief		PushCommand
	//!	@date		2016-06-19
	void EnvironmentMap::PushCommand(Renderer::RenderBuffer* _pBuffer)
	{
		if(Asset.IsLoaded())
		{
			Renderer::Texture2D* pTexture = Asset->GetData();
			
			Engine::RenderBuffer::Light::Environment* pEnvironment = _pBuffer->Push<Engine::RenderBuffer::Light::Environment>();
			pEnvironment->Texture = pTexture;
			pEnvironment->Position = GetOrCreate<Node>()->GetPosition();
		}
	}
}}