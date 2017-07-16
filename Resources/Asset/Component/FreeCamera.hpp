#ifndef _FREE_CAM_HPP_
#define _FREE_CAM_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Common.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace MyGame
{
	class FreeCamera : public PantherEngine::Component
	{
	public:
		CLASS_SERIALIZABLE(MyGame::FreeCamera)
	
		void Start		();
		void Update		();
		void Release	();

	private:
		void Rotate		();
		void Move		();

	private:
		float32						m_fRotationSpeed;
		float32						m_fMoveSpeed;
	};
}

#endif