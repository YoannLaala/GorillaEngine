/******************************************************************************
**	Includes
******************************************************************************/
#include "CameraOrbital.hpp"
#include <Core/Time/Time.hpp>
#include <Core/Input/Input.hpp>
#include <Engine/Engine.hpp>
#include <Component/Node.hpp>
#include <Component/Mesh/Mesh.hpp>

/******************************************************************************
**	Define
******************************************************************************/

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Component
{
	COMPONENT_REGISTER(CameraOrbital)

	//!	@brief		Constructor
	//!	@date		2015-04-04
	CameraOrbital::CameraOrbital()
		: Target(nullptr)
		, MinimumDistance(0.1f)
		, MaximumDistance(5.0f)
		, Speed(5.0f)
	{
		// Nothing to do
	}
	
	//!	@brief		Destructor
	//!	@date		2015-04-04
	CameraOrbital::~CameraOrbital()
	{
		// Nothing to do
	}

	//!	@brief		Stop
	//!	@date		2015-04-04	
	void CameraOrbital::Update()
	{	
		// Retrieve target position
		Math::Vector3 vTarget;
		if(Target)
		{
			Node* pCpnNodeTarget = Target->GetComponent<Node>();
			if(pCpnNodeTarget) vTarget = pCpnNodeTarget->GetPosition();

			Mesh* pCpnMeshTarget = Target->GetComponent<Mesh>();
			if(pCpnMeshTarget && pCpnMeshTarget->Asset.IsLoaded())
			{
				const Renderer::BoundingBox& kBoudingBox = pCpnMeshTarget->Asset->GetBoundingBox();
				vTarget += (kBoudingBox.Min + kBoudingBox.Max) * 0.5f;
			}
		}

		// Target To Camera Vector
		Node* pCpnNode = GetOrCreate<Node>();
		Math::Vector3 vTargetToCamera = pCpnNode->GetPosition() - (vTarget + m_vOffset); 
		float32 fDistance = vTargetToCamera.Length();
		vTargetToCamera.Normalize();

		// Apply input
		const Input* pInput = GetInput();
		if(pInput->IsKeyPressed(Keyboard::Alternative))
		{
			const Time* pTime = GetTime();
			const Mouse* pMouse = pInput->GetMouse();
			if(pMouse->IsPressed(Mouse::Left))
			{
				float32 fSpeedFactor = Speed * pTime->GetDeltaTime();
				
				float32 fPitch = -pMouse->GetRelative().GetY() * fSpeedFactor;
				Math::Quaternion qRotationPitch;
				qRotationPitch.Rotate(pCpnNode->GetRight(), fPitch);

				float32 fYaw = -pMouse->GetRelative().GetX() * fSpeedFactor;
				Math::Quaternion qRotationYaw;
				qRotationYaw.Rotate(pCpnNode->GetUp(), fYaw);
				
				Math::Quaternion qRotation = qRotationPitch * qRotationYaw;
				vTargetToCamera = qRotation * vTargetToCamera;
			}
			else if(pMouse->IsPressed(Mouse::Right))
			{
				float32 fSpeedFactor = Speed * pTime->GetDeltaTime();
				fDistance += pMouse->GetRelative().GetY() * fSpeedFactor; 
			}
			/*else if(pMouse->IsPressed(Mouse::Middle))
			{
				float32 fSpeedFactor = Speed * pTime->GetDeltaTime();
				m_vOffset += pCpnNode->GetRight() * pMouse->GetRelative().GetX() * fSpeedFactor;
				m_vOffset += pCpnNode->GetUp() * pMouse->GetRelative().GetY() * fSpeedFactor;
			}*/
		}		

		// Look at target
		fDistance = Math::Clamp(fDistance, MinimumDistance, MaximumDistance);
		pCpnNode->SetPosition(vTarget + vTargetToCamera * fDistance);
		pCpnNode->LookAt(vTarget);
	}
}}