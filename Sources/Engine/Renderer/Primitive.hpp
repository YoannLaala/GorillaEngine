#ifndef _RENDERER_PTIMITIVE_HPP_
#define _RENDERER_PTIMITIVE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/RenderBuffer.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Geometry;
	class Buffer;
	class Shader;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	struct EPrimitive
	{
		DECLARE_RENDER_BUFFER(EPrimitive)

		enum Type : uint8
		{
			Point = 0,
			Quad,
			Cube,
			Cone,
			Sphere,
			Cylinder,
			Torus,

			Count
		};

		Gorilla::Renderer::Geometry*	Geometry;
	};

	struct Primitive
	{
		DECLARE_RENDER_BUFFER(Primitive)

		enum Type : uint8
		{
			Line = 0,
			Quad,
			Cube,
			Cone,
			Sphere,
			Cylinder,
			Torus,
			Frustum,

			Count
		};
		
		Gorilla::Renderer::Geometry*	Geometry;
		Gorilla::Renderer::Buffer*		Buffer;
		void*							VertexShader;
		void*							GeometryShader;

		// Buffer
		template <Type T> 
		struct Instance;

		// Line
		template <>
		struct Instance<Line>
		{
			DECLARE_RENDER_BUFFER(Buffer<Line>)

			Math::Vector3	Start;
			Math::Vector3	End;
			Math::Vector3	Color;
		};

		// Quad
		template <>
		struct Instance<Quad>
		{
			DECLARE_RENDER_BUFFER(Buffer<Quad>)

			Math::Matrix44	World;
			Math::Vector3	Color;
		};

		// Cube
		template <>
		struct Instance<Cube>
		{
			DECLARE_RENDER_BUFFER(Buffer<Cube>)

			Math::Matrix44	World;
			Math::Vector3	Color;
		};

		// Frustum
		template <>
		struct Instance<Frustum>
		{
			DECLARE_RENDER_BUFFER(Buffer<Frustum>)

			Math::Matrix44	World;
			Math::Vector3	Color;
			float32			Fov;
			float32			Near;
			float32			Far;
			float32			Aspect;
		};
	};
}}

#endif