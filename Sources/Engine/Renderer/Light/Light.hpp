#ifndef _LIGHT_HPP_
#define _LIGHT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Light
	{
	public:
		enum Type : uint8
		{
			Directional = 0,
			Point,
			Spot,

			Count,
		};

	protected:
		Light();
		Light(float32 _fPower);
		~Light();
	
	public:
		// Diffuse
		inline const Math::Vector3&	GetColor		() const { return m_vColor; }
		inline void					SetColor		(float32 _fRed, float32 _fGreen, float32 _fBlue) { m_vColor.Set(_fRed, _fGreen, _fBlue); }

		inline float32				GetPower		() const { return m_fPower; }
		inline void					SetPower		(float32 _fPower) { m_fPower = _fPower; }									 

	protected:
		Math::Vector3	m_vColor;
		float32			m_fPower;
	};
}}

#endif