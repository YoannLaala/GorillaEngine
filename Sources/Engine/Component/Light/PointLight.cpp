/******************************************************************************
**	Includes
******************************************************************************/
#include "PointLight.hpp"
#include <Engine/Renderer/Pass/LightPass.hpp>	
#include <Component/Node.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Component
{
	COMPONENT_REGISTER(Gorilla::Component::PointLight)

	//!	@brief		Constructor
	//!	@date		2015-04-04
	PointLight::PointLight()
		: Color(255, 255, 255)
		, Radius(0.15f)
		, Power(1200.0f)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	PointLight::~PointLight()
	{
		// Nothing to do
	}

	//!	@brief		PushCommand
	//!	@date		2015-04-04
	void PointLight::PushCommand(Renderer::RenderBuffer* _pBuffer)
	{
		Engine::LightPass::PointBuffer* pCommand = _pBuffer->Push<Engine::LightPass::PointBuffer>();

		Node* pCpnNode = GetOrCreate<Node>();
		pCommand->Position = pCpnNode->GetPosition();
		pCommand->Color = Color;
		pCommand->Radius = Radius;
		pCommand->Power = Power;
	}
}}