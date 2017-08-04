/******************************************************************************
**	Includes
******************************************************************************/
#include "DirectionalLight.hpp"
#include <Engine/Renderer/Pass/LightPass.hpp>	
#include <Renderer/RenderContext.hpp>	
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
		: Color(1.0f, 1.0f, 1.0f)
		, Power(64000.0f)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	DirectionalLight::~DirectionalLight()
	{
		// Nothing to do
	}

	//!	@brief		PushCommand
	//!	@date		2015-04-04
	void DirectionalLight::PushCommand(Renderer::RenderBuffer* _pBuffer)
	{
		static Math::Matrix44 mView, mProjection;

		Engine::LightPass::DirectionalBuffer* pCommand = _pBuffer->Push<Engine::LightPass::DirectionalBuffer>();

		Node* pCpnNode = GetOrCreate<Node>();
		Renderer::Camera::ComputeView(mView, Math::Vector3::Zero, pCpnNode->GetRight(), pCpnNode->GetUp(), pCpnNode->GetForward());
		Renderer::Camera::ComputeOthographic(mProjection, 2.0f, 2.0f, -5.0f, 5.0f);

		pCommand->ViewProjection = mView * mProjection;
		pCommand->Direction.Set(mView[0][2], mView[1][2], mView[2][2]);
		pCommand->Color = Color;
		pCommand->Power = Power;
	}
}}