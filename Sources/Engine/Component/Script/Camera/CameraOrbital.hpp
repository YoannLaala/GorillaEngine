#ifndef _COMPONENT_CAMERA_ORBITAL_HPP_
#define _COMPONENT_CAMERA_ORBITAL_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Component/Component.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class GameObject;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Component
{
	class CameraOrbital : public Engine::Component 
	{
	public:
		CameraOrbital();
		~CameraOrbital();
		COMPONENT_DECLARE(CameraOrbital);

	protected:
		virtual void Update			() override;

	public:
		Engine::GameObject* Target;
		float32				MinimumDistance;
		float32				MaximumDistance;
		float32				Speed;
	};
}}

#endif