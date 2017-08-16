/******************************************************************************
**	Includes
******************************************************************************/
#include "Camera.hpp"

#include <Engine/Engine.hpp>
#include <Engine/Object/GameObject.hpp>
#include <Engine/World.hpp>

#include <Renderer/Renderer.hpp>
#include <Renderer/Scene.hpp>
#include <Renderer/Viewport.hpp>
#include <Renderer/Resource/RenderTarget.hpp>
#include <Renderer/Resource/Texture2D.hpp>

#include <Component/Node.hpp>



#include <Core/Input/Input.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Component
{
	COMPONENT_REGISTER(Gorilla::Component::Camera)

	//!	@brief		Constructor
	//!	@date		2015-04-04
	Camera::Camera()
		: m_pCamera(NULL)
		, m_fFOV(75.0f)
		, m_fZNear(0.1f)
		, m_fZFar(10.0f)
		, m_eProjection(EProjection::Perspective)
		, m_uiLayer(0)
		, Viewport(NULL)
	{
		SetFlag(EFlag::Changed);
	}
	
	//!	@brief		Destructor
	//!	@date		2015-04-04
	Camera::~Camera()
	{
		// Nothing to do
	}	

	//!	@brief		Start
	//!	@date		2015-04-04
	void Camera::Start()
	{
		GetOrCreate<Node>();
		Renderer::Scene* pScene = GetGameObject()->GetWorld()->GetScene();
		m_pCamera =  pScene->CreateCamera();
		if(Viewport) m_pCamera->SetViewport(Viewport);
		SetFlag(EFlag::Changed);
	}

	//!	@brief		Update
	//!	@date		2015-04-04
	bool bPressedX = false;
	bool bPressedY = false;
	void Camera::Update()
	{	

		Node* pCpnNode = GetOrCreate<Node>();

		static float32 x = 0.0f, y = 0.0f, depth = 2.0f;
		
		depth = -pCpnNode->GetPosition().GetZ();

		const Mouse* pMouse = GetInput()->GetMouse();
		if(pMouse->IsPressed(Mouse::Left) && !bPressedX)
		{
			bPressedX = true;
			x+=0.1f;
		}

		// Mouse Right Button to Rotate
		if(pMouse->IsPressed(Mouse::Right) && !bPressedY)
		{
			bPressedY = true;
			y+=0.1f;		
		}

		if(!pMouse->IsPressed(Mouse::Left)) bPressedX = false;
		if(!pMouse->IsPressed(Mouse::Right)) bPressedY = false;
		
		float32 z_view = m_pCamera->GetProjection()[3][2] / (depth - m_pCamera->GetProjection()[2][2]);
		Math::Vector3 campos = pCpnNode->GetPosition();
		Math::Vector3 pos((2.0f * x - 1.0f) / m_pCamera->GetProjection()[0][0], -(2.0f*y - 1.0f) / m_pCamera->GetProjection()[1][1], z_view);
		//Math::Vector3 dir = pos-campos; dir.Normalize();
		//GetRenderer()->GetGizmo()->PushLine(Math::Vector3(0.0f,0.0f, -2.0f), pos);

		
		m_pCamera->SetView(pCpnNode->GetPosition(), pCpnNode->GetRight(), pCpnNode->GetUp(), pCpnNode->GetForward());

		// Specific Viewport
		if(Viewport) m_pCamera->SetViewport(Viewport);
		
		GetRenderer()->GetGizmo()->SetPosition(pCpnNode->GetPosition());
		//GetRenderer()->GetGizmo()->SetOrientation(pCpnNode->GetOrientation());
		//GetRenderer()->GetGizmo()->PushFrustum(m_fFOV, m_fZNear, m_fZFar, 1280.0f / 720.0f);

		if(Viewport && HasFlag(EFlag::Changed))
		{
			Renderer::Texture2D* pRenderTexture = Viewport->GetRenderTarget()->GetTarget(0);
			uint32 uiWidth = pRenderTexture->GetWidth();
			uint32 uiHeight = pRenderTexture->GetHeight();

			m_pCamera->SetLayer(m_uiLayer);
			switch(m_eProjection)
			{
				case EProjection::Perspective:
				{
					m_pCamera->SetPerspective(m_fFOV, uiWidth / (float32)uiHeight, m_fZNear, m_fZFar);
					break;			
				}

				case EProjection::Orthocgraphic:
				{
					m_pCamera->SetOrthographic((float32)uiWidth, (float32)uiHeight, m_fZNear, m_fZFar);
					break;			
				}
			}

			RemoveFlag(EFlag::Changed);
		}
	}

	//!	@brief		Stop
	//!	@date		2015-04-04
	void Camera::Stop()
	{
		GetGameObject()->GetWorld()->GetScene()->DestroyCamera(m_pCamera);
	}
}}