#ifndef _COMPONENT_CAMERA_FREE_HPP_
#define _COMPONENT_CAMERA_FREE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Component/Component.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Component
{
	class CameraFree : public Engine::Component 
	{
	public:
		CameraFree();
		~CameraFree();
		COMPONENT_DECLARE(CameraFree);

	protected:
		virtual void Update			() override;

	private:
		void Rotate					();
		void Move					();

	private:
		float32	m_fRotationSpeed;
		float32	m_fMoveSpeed;
	};
}}

#endif