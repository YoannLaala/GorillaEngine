/******************************************************************************
**	Includes
******************************************************************************/
#include "DirectionalLight.hpp"
#include <Engine/Renderer/ResourceShared.hpp>
#include <Engine/Renderer/Pass/LightPass.hpp>	
#include <Renderer/RenderContext.hpp>	
#include <Renderer/Light/DirectionalLight.hpp>	
#include <Renderer/Camera.hpp>
#include <Component/Node.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Component
{
	COMPONENT_REGISTER(Gorilla::Component::DirectionalLight)

	//!	@brief		Constructor
	//!	@date		2015-04-04
	DirectionalLight::DirectionalLight()
	{
		m_pLight = new Renderer::DirectionalLight();
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	DirectionalLight::~DirectionalLight()
	{
		SAFE_DELETE(m_pLight);
	}

	//!	@brief		PushCommand
	//!	@date		2015-04-04
	void DirectionalLight::PushCommand(Renderer::RenderBuffer* _pBuffer)
	{
		Engine::RenderBuffer::Light::Directional* pLightBuffer = _pBuffer->Push<Engine::RenderBuffer::Light::Directional>();

		Node* pCpnNode = GetOrCreate<Node>();
		Renderer::Camera::ComputeView(pLightBuffer->View, Math::Vector3::Zero, pCpnNode->GetRight(), pCpnNode->GetUp(), pCpnNode->GetForward());
		Renderer::Camera::ComputeOthographic(pLightBuffer->Projection, 2.0f, 2.0f, -5.0f, 5.0f);
		pLightBuffer->Color = m_pLight->GetColor();
		pLightBuffer->Lux = m_pLight->GetPower();
	}
}}