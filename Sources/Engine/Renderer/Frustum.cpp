/******************************************************************************
**	Includes
******************************************************************************/
#include "Frustum.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Set 
	//!	@details	iqFrustumF_CreatePerspective
	//!				Use trigonometry to find each plane's normal
	//!				Only plane are passing through origin so w is 0.0f
	//!	@date		2015-11-11
	void Frustum::Set(float32 _fFov, float32 _fAspect, float32 _fZNear, float32 _fZFar)
	{
		float32 fAngle = Math::ToRadian(_fFov * 0.5f);
		float32 fCos = Math::Cos(fAngle);
		float32 fSin = Math::Sin(fAngle);
			
		m_aPlane[Left].Set(fCos, 0.0f, fSin * _fAspect, 0.0f);
		m_aPlane[Right].Set(-fCos, 0.0f, fSin * _fAspect, 0.0f);
		m_aPlane[Top].Set(0.0f, -fCos, fSin, 0.0f);
		m_aPlane[Bottom].Set(0.0f, fCos, fSin, 0.0f);
		m_aPlane[Near].Set(0.0f, 0.0f, 1.0f, _fZNear);
		m_aPlane[Left].Set(0.0f, 0.0f, -1.0f, _fZFar);
	}

	//!	@brief		Set 
	//!	@date		2015-11-11
	void Frustum::Set(const Math::Matrix44& _mProjection)
	{
		m_aPlane[Left].Set(_mProjection[0][3] + _mProjection[0][0], _mProjection[1][3] + _mProjection[1][0], _mProjection[2][3] + _mProjection[2][0], _mProjection[3][3] + _mProjection[3][0]);
		m_aPlane[Right].Set(_mProjection[0][3] - _mProjection[0][0], _mProjection[1][3] - _mProjection[1][0], _mProjection[2][3] - _mProjection[2][0], _mProjection[3][3] - _mProjection[3][0]);
		m_aPlane[Top].Set(_mProjection[0][3] - _mProjection[0][1], _mProjection[1][3] - _mProjection[1][1], _mProjection[2][3] - _mProjection[2][1], _mProjection[3][3] - _mProjection[3][1]);
		m_aPlane[Bottom].Set(_mProjection[0][3] +_mProjection[0][1], _mProjection[1][3] + _mProjection[1][1], _mProjection[2][3] + _mProjection[2][1], _mProjection[3][3] + _mProjection[3][1]);
		m_aPlane[Near].Set(_mProjection[0][2], _mProjection[1][2], _mProjection[2][2], _mProjection[3][2]);
		m_aPlane[Far].Set(_mProjection[0][2], _mProjection[1][2], _mProjection[2][2], _mProjection[3][2]);
		m_aPlane[Bottom].Set(_mProjection[0][3] -_mProjection[0][2], _mProjection[1][3] - _mProjection[1][2], _mProjection[2][3] - _mProjection[2][2], _mProjection[3][3] - _mProjection[3][2]);
	}

	//!	@brief		IsCubeVisible 
	//!	@date		2015-11-11
	bool Frustum::IsCubeVisible(const Math::Vector3& /*_vPosition*/, float32 /*_fSize*/) const
	{
		return true;
	}
}}