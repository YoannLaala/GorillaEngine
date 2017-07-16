#ifndef _RENDERER_FRUSTUM_HPP_
#define _RENDERER_FRUSTUM_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Frustum
	{
		enum Type : uint8
		{
			Left = 0,
			Right,
			Top, 
			Bottom, 
			Near,
			Far,

			Count
		};

	public:
		void Set(float32 _fFov, float32 _fAspect, float32 _fZNear, float32 _fZFar);
		void Set(const Math::Matrix44& _mProjection);

		bool IsCubeVisible(const Math::Vector3& /*_vPosition*/, float32 /*_fSize*/) const;

	private:
		Math::Vector4 m_aPlane[Type::Count];
	};
}}

#endif