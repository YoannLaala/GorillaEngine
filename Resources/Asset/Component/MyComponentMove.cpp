/******************************************************************************
**	Includes
******************************************************************************/
#include "MyComponentMove.hpp"

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace MyGame
{
	//!	@brief		Start
	//!	@date		2015-04-04
	void MyComponentMove::Start()
	{
		m_bMove = true;
	}

	//!	@brief		Stop
	//!	@date		2015-04-04
	void MyComponentMove::Update()
	{		
		if(m_bRotate)
		{
			m_pGameObject->Move(0.1f * Gorilla::Engine::GetTime()->GetDetaTime());
		}
		
	}

	//!	@brief		Stop
	//!	@date		2015-04-04
	void MyComponentMove::Stop()
	{
		// Nothing to do
	}
}