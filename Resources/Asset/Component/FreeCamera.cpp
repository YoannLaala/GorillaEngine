/******************************************************************************
**	Includes
******************************************************************************/
#include "FreeCamera.hpp"

/******************************************************************************
**	Forward Declarations
******************************************************************************/

/******************************************************************************
**	Class Declaration
******************************************************************************/
using namespace PantherEngine;

namespace MyGame
{
	void FreeCamera::Start()
	{
		m_fRotationSpeed = 100.0f;
		m_fMoveSpeed = 10.0f;
	}
	
	void FreeCamera::Update()
	{
		Move();
	}
	
	void FreeCamera::Release()
	{
	
	}

	// Mouvement
	void FreeCamera::Rotate()
	{
		// Mouse Right Button to Rotate
		/*if(GetInput()->IsMousePressed(EMouseInput::RightButton))
		{
			// X Axis
			if(GameInput::GetInstance()->GetMousePositionRelative().x != 0)
			{
				Rotate(m_vYawAxis, GameInput::GetInstance()->GetMousePositionRelative().x);		
			}

			// Y Axis
			if(GameInput::GetInstance()->GetMousePositionRelative().y != 0)
			{
				Rotate(m_vPitchAxis, GameInput::GetInstance()->GetMousePositionRelative().y);
			}
		}

		if(GetInput()->IsKeyPressed(DIK_3))
		{
			m_pGameObject->RotateUp(-0.01f);
		}

		if(GetInput()->IsKeyPressed(DIK_4))
		{
			m_pGameObject->RotateUp(0.01f);
		}

		if(GetInput()->IsKeyPressed(DIK_5))
		{
			m_pGameObject->RotateRight(-0.01f);
		}

		if(GetInput()->IsKeyPressed(DIK_6))
		{
			m_pGameObject->RotateRight(0.01f);
		}

		if(GetInput()->IsKeyPressed(DIK_7))
		{
			m_pGameObject->RotateForward(0.01f);
		}

		if(GetInput()->IsKeyPressed(DIK_8))
		{
			m_pGameObject->RotateForward(-0.01f);
		}*/
	}
	
	void FreeCamera::Move()
	{
		// Add translation to position
		if(GetInput()->IsKeyPressed(DIK_UP))
		{
			m_pGameObject->MoveForward(m_fMoveSpeed * GetTime()->GetDeltaTime());
		}

		if(GetInput()->IsKeyPressed(DIK_DOWN))
		{
			m_pGameObject->MoveForward(-m_fMoveSpeed * GetTime()->GetDeltaTime());
		}

		if(GetInput()->IsKeyPressed(DIK_LEFT))
		{
			m_pGameObject->MoveRight(-m_fMoveSpeed * GetTime()->GetDeltaTime());
		}

		if(GetInput()->IsKeyPressed(DIK_RIGHT))
		{
			m_pGameObject->MoveRight(m_fMoveSpeed * GetTime()->GetDeltaTime());
		}

		if(GetInput()->IsKeyPressed(DIK_1))
		{
			m_pGameObject->MoveUp(m_fMoveSpeed * GetTime()->GetDeltaTime());
		}

		if(GetInput()->IsKeyPressed(DIK_2))
		{
			m_pGameObject->MoveUp(-m_fMoveSpeed * GetTime()->GetDeltaTime());
		}
	}

	REGISTER_CLASS_SERIALIZABLE(MyGame::FreeCamera)
}