#ifndef _BUFFER_TYPE_HPP_
#define _BUFFER_TYPE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Common/Common.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace PantherEngine
{
	/**************************************************************************
	**	Constant Buffer Per Object
	***************************************************************************/
	struct ConstantBufferPerObject
	{
		Math::Matrix m_mWorld;
		Math::Matrix m_mView;
		Math::Matrix m_mProjection;
	};

	/**************************************************************************
	**	Light
	***************************************************************************/
	struct DirectionalLightBuffer
	{
		Math::Vector3	m_vDirection;
		float32			m_fIntensity;
		Math::Vector4	m_vDiffuse;
		Math::Vector4	m_vAmbient;
		Math::Vector4	m_vSpecular;
		float32			m_fSpecularPower;
		Math::Vector3	m_vExt;
	};

	struct PointLightBuffer
	{
		Math::Vector3	m_vPosition;
		float32			m_fIntensity;
		Math::Vector4	m_vDiffuse;
		float32			m_fRadius;
		Math::Vector3	m_vExt;
	};

	//struct LightBuffer
	//{
	//	Vector<DirectionalLightBufferData> m_vecDirectionalLight;
	//	/*PointLight		 m_aPointLight[1024];
	//	PointLight		 m_aSpotLight[1024];*/
	//};

	/*struct TesselationBuffer
	{
		float32		Amount;
		XMFLOAT3	Padding;
	};*/
}

#endif