#ifndef _MY_COMPONENT_MOVE__HPP_
#define _MY_COMPONENT_MOVE__HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Common.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace MyGame
{
	class MyComponentMove : public Gorilla::Engine::Component
	{
	protected:
		virtual void Start			() OVERRIDE;
		virtual void Update			() OVERRIDE;
		virtual void Stop			() OVERRIDE;
		
	public:
		bool m_bMove; // Move ?
	};
}

#endif