#ifndef _COMPONENT_LIGHT_HPP_
#define _COMPONENT_LIGHT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Component/Component.hpp>
#include <Renderer/Light/Light.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace PantherRenderer
{
	namespace ELight
	{
		enum Type
		{
			Directional		= 0,
			Point			= 1,
			Spot			= 2,
			Count			= 3,
		};
	}
}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Component
{
	template <class T>
	class Light : public Engine::Component
	{
	public:

		Light();
		~Light();
		
		void		Release		();

		inline T*	GetLight	() { return m_pLight; }

	protected:
		T*								m_pLight;
		PantherRenderer::ELight::Type	m_eLight;
	};

	template <class T>
	Light::Light()
	{
		m_pLight = new T();
	}

	template <class T>
	Light::~LightComponent()
	{
		// Nothing to do
	}

	template <class T>
	void Light::Release()
	{
		SAFE_DELETE(m_pLight)
	}	
}}

#endif