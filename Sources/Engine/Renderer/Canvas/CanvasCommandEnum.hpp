#ifndef _RENDER_CANVAS_COMMAND_ENUM_HPP_
#define _RENDER_CANVAS_COMMAND_ENUM_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/Canvas/CanvasContext.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class CanvasRenderer;
	class CanvasSurface;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	/******************************************************************************
	**	Enum
	******************************************************************************/
	namespace ECanvasCommand
	{
		enum Type
		{
			Geometry = 0,
			GeometryFilled,
			Line,
			Text,

			Count
		};

		typedef void (*fnCanvasCommandExecute)(CanvasRenderer* _pRenderer, CanvasSurface* _pSurface, void* _pParameter);
		static const fnCanvasCommandExecute Execute[ECanvasCommand::Count] = 
		{
			CanvasContext::ExecuteGeometry,
			CanvasContext::ExecuteGeometryFilled,
			CanvasContext::ExecuteLine,
			CanvasContext::ExecuteText,
		};
	}

	/******************************************************************************
	**	CanvasCommandParameter
	******************************************************************************/
	template <typename ECanvasCommand::Type>
	struct CanvasCommandParameter;

	template <>
	struct CanvasCommandParameter<ECanvasCommand::Geometry>
	{
		CanvasGeometry*	Geometry;
		CanvasBrush*	Brush;
		Math::Matrix33	Transform;
	};

	template <>
	struct CanvasCommandParameter<ECanvasCommand::GeometryFilled>
	{
		CanvasGeometry*	Geometry;
		CanvasBrush*	Brush;
		Math::Matrix33	Transform;
	};

	template <>
	struct CanvasCommandParameter<ECanvasCommand::Line>
	{
		CanvasBrush*	Brush;
		Math::Vector2	PointA;
		Math::Vector2	PointB;
		CanvasStyle*	Style;
		float32			Width;
	};

	template <>
	struct CanvasCommandParameter<ECanvasCommand::Text>
	{
		CanvasText*		Text;
		CanvasBrush*	Brush;
		Math::Matrix33	Transform;
	};
}}

#endif