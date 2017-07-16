/******************************************************************************
**	Includes
******************************************************************************/
#include "MyComponentRotate.hpp"

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace MyGame
{
	//!	@brief		Start
	//!	@date		2015-04-04
	void MyComponentRotate::Start()
	{
		m_bRotate = true;
	}

	//!	@brief		Stop
	//!	@date		2015-04-04
	void MyComponentRotate::Update()
	{		
		if(m_bRotate)
		{
			m_pGameObject->RotateUp(0.1f * Gorilla::Engine::GetTime()->GetDetaTime());
		}
		
	}

	//!	@brief		Stop
	//!	@date		2015-04-04
	void MyComponentRotate::Stop()
	{
		// Nothing to do
	}
}