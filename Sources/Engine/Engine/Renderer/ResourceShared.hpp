#ifndef _ENGINE_RESOURCE_SHARED_HPP_
#define _ENGINE_RESOURCE_SHARED_HPP_

#include <Core/Core.hpp>
#include <Renderer/Renderer.hpp>

// RenderTarget
#define RENDERER_TARGET_GEOMETRY					"_GEOMETRY_"
#define RENDERER_TARGET_SHADOW_DIRECTIONAL			"_SHADOW_DIRECTINAL_"
#define RENDERER_TARGET_SHADOW_POINT				"_SHADOW_POINT_"

// Texture
#define RENDERER_TEXTURE_GEOMETRY_ALBEDO			"_GEOMETRY_ALBEDO_"
#define RENDERER_TEXTURE_GEOMETRY_NORMAL			"_GEOMETRY_NORMAL_"	
#define RENDERER_TEXTURE_GEOMETRY_DEPTH_STENCIL		"_GEOMETRY_DEPTH_STENCIL_"
#define RENDERER_TEXTURE_LIGHT_DIRECT				"_LIGHT_DIRECT_"
#define RENDERER_TEXTURE_LIGHT_INDIRECT				"_LIGHT_INDIRECT_"
#define RENDERER_TEXTURE_LIGHT_AMBIENT_OCCLUSION	"_LIGHT_AMBIENT_OCCLUSION_"
#define RENDERER_TEXTURE_VOXEL_VOLUME_COLOR			"_VOXEL_VOLUME_COLOR_"
#define RENDERER_TEXTURE_VOXEL_VOLUME_NORMAL		"_VOXEL_VOLUME_NORMAL_"

// Buffer
#define RENDERER_BUFFER_SCENE						"_SCENE_"
#define RENDERER_BUFFER_INSTANCE					"_INSTANCE_"
#define RENDERER_BUFFER_VOXEL_GRID					"_VOXEL_GRID_"
#define RENDERER_BUFFER_VOXEL_FRAMENT				"_VOXEL_FRAGMENT_"

namespace Gorilla { namespace Engine { namespace SharedResource
{
	struct Light
	{
		enum Type
		{
			Directional = 0,
			Spot,
			Point,

			Count,
		};

		static Renderer::Buffer*		ConstantBuffer[Count];
		static Renderer::Buffer*		Buffer[Count];
		static Renderer::Texture2D*		ShadowMap[Count];
	};
}}}

namespace Gorilla { namespace Engine { namespace RenderBuffer
{
	namespace Constant
	{
		struct Scene
		{
			DECLARE_RENDER_BUFFER(Scene)

			Math::Matrix44 ViewProjection;
			Math::Matrix44 ViewProjectionInverse;
			Math::Vector3  Position;
		};
	};

	namespace Vertex
	{
		struct PositionTexcoord
		{
			Math::Vector3 Position;
			Math::Vector2 Texcoord;
		};

		struct PositionNormalTexcoord
		{
			Math::Vector3 Position;
			Math::Vector3 Normal;
			Math::Vector2 Texcoord;
		};		
	};

	namespace Geometry
	{
		struct Batch
		{
			DECLARE_RENDER_BUFFER(Batch)

			Renderer::Material*		Material;
			Renderer::Geometry*		Geometry;
			Math::Matrix44			Transform;
			uint8					Group;
		};
		
		struct Instance
		{
			Math::Matrix44	World;
		};
	};

	namespace Decal
	{
		struct Batch
		{
			DECLARE_RENDER_BUFFER(Decal::Batch)

			Renderer::Material*		Material;
			Math::Matrix44			Transform;
		};
		
		struct Instance
		{
			Math::Matrix44	World;
			Math::Matrix44	WorldInverse;
		};
	};

	namespace Light
	{
		struct Environment
		{
			DECLARE_RENDER_BUFFER(Light::Environment)

			Math::Vector3			Position;
			Renderer::Texture2D*	Texture;
		};
	}
	
	namespace Operation
	{
		enum Type
		{
			Texture2Texture		= 0,
			Buffer2Texture,
			ABGR2RGBA,
		};

		struct TextureCopy
		{
			DECLARE_RENDER_BUFFER(Operation::TextureCopy)

			Math::Vector4			Box;
			void*					Source;
			void*					Destination;
			Type					Type;
		};
	}
}}}

#endif