/******************************************************************************
**	Defines
******************************************************************************/
#define K_PI	3.14159265359

#define MATERIAL_WATER float3(0.02,0.02,0.02)
#define MATERIAL_PLASTIC float3(0.03,0.03,0.03)
#define MATERIAL_PLASTIC_HIGH float3(0.05,0.05,0.05)
#define MATERIAL_GLASS float3(0.08,0.08,0.08)
#define MATERIAL_DIAMOND float3(0.17,0.17,0.17)
#define MATERIAL_IRON float3(0.56,0.57,0.58)
#define MATERIAL_COPPER float3(0.95,0.64,0.54)
#define MATERIAL_GOLD float3(1.00,0.71,0.29)
#define MATERIAL_ALUMINIUM float3(0.91,0.92,0.92)
#define MATERIAL_SILVER float3(0.95,0.93,0.88)

#define POINT_LIGHT_VIEW_COUNT 6

/******************************************************************************
**	Struct
******************************************************************************/
cbuffer ConstantBufferScene : register(cb0)
{
	float4x4	VIEW;
	float4x4	PROJECTION;
	float4 		VIEWPORT;
};

cbuffer ConstantBufferPointLight : register(cb1)
{
	float4x4 POINT_LIGHT_VIEW[POINT_LIGHT_VIEW_COUNT];
	float4x4 POINT_LIGHT_PROJECTION;
};

struct GeometryInstance
{
	float4x4 	World;
};

struct DirectionalLight
{
	float4x4	View;
	float4x4	Projection;
	float3		Color;
	float		Lux;		
};

struct PointLight
{
	float3 		Position;
	float3		Color;
	float		Radius;
	float		Lumen;
};

/******************************************************************************
**	Samplers
******************************************************************************/
SamplerState sPointSampler 		: register(s0);
SamplerState sLinearSampler 	: register(s1);
SamplerState sBiLinearSampler 	: register(s2);
SamplerState sTriLinearSampler 	: register(s3);
SamplerState sAnisotropicSampler: register(s4);
SamplerComparisonState sShadowSampler : register(s5);

//!	@brief		GetWidth 
//!	@date		2016-07-30
float GetWidth()
{
	return VIEWPORT.x; 
}

//!	@brief		GetHeight 
//!	@date		2016-07-30
float GetHeight()
{
	return VIEWPORT.y; 
}

//!	@brief		GetNear 
//!	@date		2016-07-30
float GetNear()
{
	return VIEWPORT.z; 
}

//!	@brief		GetFar 
//!	@date		2016-07-30
float GetFar()
{
	return VIEWPORT.w; 
}

//!	@brief		GetCameraPosition 
//!	@date		2016-06-25
float3 GetCameraPosition()
{
	return float3(VIEW._m30, VIEW._m31, VIEW._m32); 
}

//!	@brief		GetCameraDirection 
//!	@date		2016-06-25
float3 GetCameraDirection()
{
	return float3(VIEW._m20, VIEW._m21, VIEW._m22); 
}

//!	@brief		ComputeLinearDepth 
//!	@date		2016-06-25
float ComputeLinearDepth(in float _fDepth)
{
	return PROJECTION._m32 / (_fDepth - PROJECTION._m22);
}

//!	@brief		ComputeViewPosition
//!	@date		2016-06-25
float3 ComputeViewPosition(in float2 _vTexcoord, in float _fDepth)
{
	float fDepthLinear = ComputeLinearDepth(_fDepth);
	float3 vPosition = float3(((2.0f * _vTexcoord.x) - 1.0f), -((2.0f * _vTexcoord.y) - 1.0f), 1.0f);
	vPosition.x /= PROJECTION._m00;
	vPosition.y /= PROJECTION._m11;
	vPosition *= fDepthLinear;
	
	return vPosition;
}

//!	@brief		ComputeViewDirection 
//!	@date		2016-06-25
float3 ComputeViewDirection(in float2 _vTexcoord, in float _fDepth)
{
	return normalize(ComputeViewPosition(_vTexcoord, _fDepth));
}

//!	@brief		ComputeWorldPosition 
//!	@date		2016-06-25
float3 ComputeWorldPosition(in float2 _vTexcoord, in float _fDepth)
{	
	return GetCameraPosition() + ComputeViewPosition(_vTexcoord, _fDepth);
}