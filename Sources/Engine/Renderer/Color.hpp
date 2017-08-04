#ifndef _RENDERER_COLOR_HPP_
#define _RENDERER_COLOR_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>

/******************************************************************************
**	Defines
******************************************************************************/
#define  COLOR_FACTOR (1.0f / 255.0f)

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Color
	{
	public:

		Color();
		Color(uint8 _uiRed, uint8 _uiGreen, uint8 _uiBlue, uint8 _uiAlpha = 255);
		Color(uint32 _uiColor);
		~Color();

		inline uint8 GetRed		() const { return m_kValue.Red; }
		inline uint8 GetGreen	() const { return m_kValue.Green; }
		inline uint8 GetBlue	() const { return m_kValue.Blue; }
		inline uint8 GetAlpha	() const { return m_kValue.Alpha; }

		inline bool operator==(Color _kColor) { return m_uiValue == _kColor.m_uiValue; }
		inline bool operator!=(Color _kColor) { return m_uiValue != _kColor.m_uiValue; }
		inline operator uint32() const { return *(uint32*)this; }
		inline operator Math::Vector3() const { return Math::Vector3(GetRed() * COLOR_FACTOR, GetGreen() * COLOR_FACTOR, GetBlue() * COLOR_FACTOR); }
		inline operator Math::Vector4() const { return Math::Vector4(GetRed() * COLOR_FACTOR, GetGreen() * COLOR_FACTOR, GetBlue() * COLOR_FACTOR, GetAlpha() * COLOR_FACTOR); }

		static Color Black;
		static Color White;
		static Color Red;
		static Color Green;
		static Color Blue;

	private:
		struct Value
		{
			uint32 Alpha	:8;
			uint32 Blue		:8;
			uint32 Green	:8;
			uint32 Red		:8;
		};

		union
		{
			uint32 m_uiValue;
			Value m_kValue;
		};
	};
}}

#endif