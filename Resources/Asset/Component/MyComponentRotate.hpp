#ifndef _MY_COMPONENT_ROTATE_HPP_
#define _MY_COMPONENT_ROTATE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Common.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace MyGame
{
	class MyComponentRotate : public Gorilla::Engine::Component
	{
	protected:
		virtual void Start			() OVERRIDE;
		virtual void Update			() OVERRIDE;
		virtual void Stop			() OVERRIDE;
		
	public:
		bool m_bRotate; // Rotate ?
	};
}

#endif