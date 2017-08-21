/******************************************************************************
**	Includes
******************************************************************************/
#include "SpotLight.hpp"
#include <Engine/Renderer/Pass/LightPass.hpp>	
#include <Component/Node.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Component
{
	COMPONENT_REGISTER(Gorilla::Component::SpotLight)

	//!	@brief		Constructor
	//!	@date		2015-04-04
	SpotLight::SpotLight()
		: Size(1.0f)
		, Power(1200.0f)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	SpotLight::~SpotLight()
	{
		// Nothing to do
	}

	//!	@brief		PushCommand
	//!	@date		2015-04-04
	void SpotLight::PushCommand(Renderer::RenderBuffer* _pBuffer)
	{
		Engine::LightPass::SpotBuffer* pCommand = _pBuffer->Push<Engine::LightPass::SpotBuffer>();

		Node* pCpnNode = GetOrCreate<Node>();
		pCommand->Position = pCpnNode->GetPosition();
		pCommand->Direction = pCpnNode->GetForward();
		pCommand->Size = Size;
		pCommand->Power = Power;
	}
}}