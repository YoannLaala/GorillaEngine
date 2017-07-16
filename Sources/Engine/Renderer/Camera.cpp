/******************************************************************************
**	Includes
******************************************************************************/
#include "Camera.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	Camera::Camera()
		: m_pViewport(nullptr)
		, m_uiLayer(0)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	Camera::~Camera()
	{
		// Nothing to do
	}

	//!	@brief		SetPerspective 
	//!	@date		2015-11-11
	void Camera::SetPerspective(float32 _fFOV, float32 _fAspect, float32 _fZNear, float32 _fZFar)
	{
		ComputePerspective(m_mProjection, _fFOV, _fAspect, _fZNear, _fZFar);
	}

	//!	@brief		SetOrthographic 
	//!	@date		2015-11-11
	void Camera::SetOrthographic(float32 _fWidth, float32 _fHeight, float32 _fZNear, float32 _fZFar)
	{
		ComputeOthographic(m_mProjection, _fWidth, _fHeight, _fZNear, _fZFar);
	}

	//!	@brief		SetView
	//!	@date		2015-11-13
	void Camera::SetView(const Math::Vector3& _vPosition, const Math::Vector3& _vRight, const Math::Vector3& _vUp, const Math::Vector3& _vForward)
	{
		ComputeView(m_mView, _vPosition, _vRight, _vUp, _vForward);
	}


	//!	@brief		ComputePerspective
	//!	@date		2015-11-13
	void Camera::ComputePerspective(Math::Matrix44& _mProjection, float32 _fFOV, float32 _fAspect, float32 _fZNear, float32 _fZFar)
	{
		float32 fCos = Math::Cos(0.5f * Math::ToRadian(_fFOV));
		float32 fSin = Math::Sin(0.5f * Math::ToRadian(_fFOV));
		float32 fH = fCos / fSin;
		float32 fW = fH / _fAspect;


		//float32 fScale = 1.0f / tanf(Math::ToRadian(_fFOV) * 0.5f); 
		float32 fZRange = _fZFar / (_fZFar - _fZNear);

		_mProjection[0][0] = fW;//_fAspect * fScale;	
		//_mProjection[0][0] = _fAspect * fScale;	
		_mProjection[1][0] = 0.0f;	
		_mProjection[2][0] = 0.0f;	
		_mProjection[3][0] = 0.0f;	

		_mProjection[0][1] = 0.0f;	
		_mProjection[1][1] = fH;//fScale;	
		//_mProjection[1][1] = fScale;	
		_mProjection[2][1] = 0.0f;	
		_mProjection[3][1] = 0.0f;	

		_mProjection[0][2] = 0.0f;											
		_mProjection[1][2] = 0.0f;											
		_mProjection[2][2] = fZRange;								
		_mProjection[3][2] = -_fZNear * fZRange;	

		_mProjection[0][3] = 0.0f;
		_mProjection[1][3] = 0.0f;
		_mProjection[2][3] = 1.0f;
		_mProjection[3][3] = 0.0f;
	}

	//!	@brief		ComputeOthographic
	//!	@date		2015-11-13
	void Camera::ComputeOthographic(Math::Matrix44& _mProjection, float32 _fWidth, float32 _fHeight, float32 _fZNear, float32 _fZFar)
	{
		float32 fZRange = 1.0f / (_fZFar - _fZNear);

		_mProjection[0][0] = _fWidth * 0.5f;	
		_mProjection[1][0] = 0.0f;				
		_mProjection[2][0] = 0.0f;				
		_mProjection[3][0] = 0.0f;
					   
		_mProjection[0][1] = 0.0f;			
		_mProjection[1][1] = _fHeight * 0.5f;
		_mProjection[2][1] = 0.0f;			
		_mProjection[3][1] = 0.0f;			
					   
		_mProjection[0][2] = 0.0f;
		_mProjection[1][2] = 0.0f;
		_mProjection[2][2] = fZRange;
		_mProjection[3][2] = -_fZNear * fZRange;
					   
		_mProjection[0][3] = 0.0f;
		_mProjection[1][3] = 0.0f;
		_mProjection[2][3] = 0.0f;
		_mProjection[3][3] = 1.0f;
	}

	//!	@brief		ComputeView
	//!	@date		2015-11-13
	void Camera::ComputeView(Math::Matrix44& _mView, const Math::Vector3& _vPosition, const Math::Vector3& _vRight, const Math::Vector3& _vUp, const Math::Vector3& _vForward)
	{
		_mView[0][0] = _vRight.GetX();
		_mView[1][0] = _vRight.GetY();
		_mView[2][0] = _vRight.GetZ();
		_mView[3][0] = -(_vRight.GetX() * _vPosition.GetX() + _vRight.GetY() * _vPosition.GetY() + _vRight.GetZ() * _vPosition.GetZ());

		_mView[0][1] = _vUp.GetX();
		_mView[1][1] = _vUp.GetY();
		_mView[2][1] = _vUp.GetZ();
		_mView[3][1] = -(_vUp.GetX() * _vPosition.GetX() + _vUp.GetY() * _vPosition.GetY() + _vUp.GetZ() * _vPosition.GetZ());

		_mView[0][2] = _vForward.GetX();
		_mView[1][2] = _vForward.GetY();
		_mView[2][2] = _vForward.GetZ();
		_mView[3][2] = -(_vForward.GetX() * _vPosition.GetX() + _vForward.GetY() * _vPosition.GetY() + _vForward.GetZ() * _vPosition.GetZ());

		_mView[0][3] = 0.0f;
		_mView[1][3] = 0.0f;
		_mView[2][3] = 0.0f;
		_mView[3][3] = 1.0f;
	}
}}