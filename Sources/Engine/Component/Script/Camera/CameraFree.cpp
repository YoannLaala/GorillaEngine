/******************************************************************************
**	Includes
******************************************************************************/
#include "CameraFree.hpp"
#include <Core/Time/Time.hpp>
#include <Core/Input/Input.hpp>
#include <Engine/Engine.hpp>
#include <Component/Node.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Component
{
	COMPONENT_REGISTER(CameraFree)

	//!	@brief		Constructor
	//!	@date		2015-04-04
	CameraFree::CameraFree()
		: m_fRotationSpeed(100.0f)
		, m_fMoveSpeed(1.0f)
	{
		// Nothing to do
	}
	
	//!	@brief		Destructor
	//!	@date		2015-04-04
	CameraFree::~CameraFree()
	{
		// Nothing to do
	}

	//!	@brief		Stop
	//!	@date		2015-04-04
	void CameraFree::Update()
	{		
		Rotate();
		Move();
	}

	//!	@brief		Rotate
	//!	@date		2015-04-04
	void CameraFree::Rotate()
	{
		Node* pCpnNode = GetOrCreate<Node>();
		const Mouse* pMouse = GetInput()->GetMouse();
		float32 fDeltaTime = GetTime()->GetDeltaTime();

		// Mouse Right Button to Rotate
		if(pMouse->IsPressed(Mouse::Right))
		{

			// Y Axis
			if(pMouse->GetRelative().GetX() != 0)
			{
				pCpnNode->RotateY(pMouse->GetRelative().GetX() * m_fRotationSpeed * fDeltaTime);	
			}

			// Right Axis
			/*if(pMouse->GetRelative().GetY() != 0)
			{
				pCpnNode->RotateRight(pMouse->GetRelative().GetY() * m_fRotationSpeed * fDeltaTime);
			}*/
		}

		//
		//if(GetInput()->IsKeyPressed(Gorilla::EInput::Keyboard::A))
		//{
		//	pCpnNode->RotateUp(-m_fRotationSpeed * fDeltaTime);
		//}

		//if(GetInput()->IsKeyPressed(Gorilla::EInput::Keyboard::Z))
		//{
		//	pCpnNode->RotateUp(m_fRotationSpeed * fDeltaTime);
		//}

		//if(GetInput()->IsKeyPressed(Gorilla::EInput::Keyboard::Left))
		//{
		//	pCpnNode->RotateRight(-m_fRotationSpeed * fDeltaTime);
		//}

		//if(GetInput()->IsKeyPressed(Gorilla::EInput::Keyboard::Right))
		//{
		//	pCpnNode->RotateRight(m_fRotationSpeed * fDeltaTime);
		//}

		//if(GetInput()->IsKeyPressed(Gorilla::EInput::Keyboard::Up))
		//{
		//	pCpnNode->RotateForward(m_fRotationSpeed * fDeltaTime);
		//}

		//if(GetInput()->IsKeyPressed(Gorilla::EInput::Keyboard::Down))
		//{
		//	pCpnNode->RotateForward(-m_fRotationSpeed * fDeltaTime);
		//}
	}
	
	//!	@brief		Move
	//!	@date		2015-04-04
	void CameraFree::Move()
	{
		Node* pCpnNode = GetOrCreate<Node>();
		//const Mouse* pMouse = GetInput()->GetMouse();
		float32 fDeltaTime = GetTime()->GetDeltaTime();

		// Add translation to position
		if(GetInput()->IsKeyPressed(Keyboard::Up))
		{
			pCpnNode->MoveForward(m_fMoveSpeed * fDeltaTime);
		}

		if(GetInput()->IsKeyPressed(Keyboard::Down))
		{
			pCpnNode->MoveForward(-m_fMoveSpeed * fDeltaTime);
		}

		if(GetInput()->IsKeyPressed(Keyboard::Left))
		{
			pCpnNode->MoveRight(-m_fMoveSpeed * fDeltaTime);
		}

		if(GetInput()->IsKeyPressed(Keyboard::Right))
		{
			pCpnNode->MoveRight(m_fMoveSpeed * fDeltaTime);
		}

		if(GetInput()->IsKeyPressed(Keyboard::A))
		{
			pCpnNode->MoveUp(m_fMoveSpeed * fDeltaTime);
		}

		if(GetInput()->IsKeyPressed(Keyboard::Z))
		{
			pCpnNode->MoveUp(-m_fMoveSpeed * fDeltaTime);
		}

		// Y Axis
		if(GetInput()->IsKeyPressed(Keyboard::E))
		{
			pCpnNode->RotateY(-m_fRotationSpeed * fDeltaTime);	
		}

		// Y Axis
		if(GetInput()->IsKeyPressed(Keyboard::R))
		{
			pCpnNode->RotateY(m_fRotationSpeed * fDeltaTime);
		}
	}
}}