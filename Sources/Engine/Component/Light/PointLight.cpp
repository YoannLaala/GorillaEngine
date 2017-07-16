/******************************************************************************
**	Includes
******************************************************************************/
#include "PointLight.hpp"
#include <Engine/Renderer/ResourceShared.hpp>
#include <Engine/Renderer/Pass/LightPass.hpp>	
#include <Renderer/RenderContext.hpp>	
#include <Renderer/Light/PointLight.hpp>	
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
	{
		m_pLight = new Renderer::PointLight();
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	PointLight::~PointLight()
	{
		SAFE_DELETE(m_pLight);
	}

	//!	@brief		PushCommand
	//!	@date		2015-04-04
	void PointLight::PushCommand(Renderer::RenderBuffer* _pBuffer)
	{
		Engine::RenderBuffer::Light::Point* pLightBuffer = _pBuffer->Push<Engine::RenderBuffer::Light::Point>();

		Node* pCpnNode = GetOrCreate<Node>();
		pLightBuffer->Position = pCpnNode->GetPosition();
		pLightBuffer->Color = m_pLight->GetColor();
		pLightBuffer->Radius = m_pLight->GetRadius();
		pLightBuffer->Lumen = m_pLight->GetPower();
	}
}}