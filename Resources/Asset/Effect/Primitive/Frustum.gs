/******************************************************************************
**	Includes
******************************************************************************/
#include "../Common.sl"
#include "Vertex.sl"

/******************************************************************************
**	Buffers
******************************************************************************/
struct Frustum
{
	float4x4 World;
	float3	Color;
	float 	Fov;
	float 	Near;
	float 	Far;
	float 	Aspect;
};
StructuredBuffer<Frustum> aFrustum : register(t0);

/******************************************************************************
**	AddVertex
******************************************************************************/
VS_OUTPUT_INSTANCE AddVertex(in VS_OUTPUT_INSTANCE _input, in float _fOffsetX, in float _fOffsetY, in float _fOffsetZ, inout LineStream<VS_OUTPUT_INSTANCE> _kStream)
{
	Frustum kFrustum = aFrustum[_input.InstanceId];
	
	VS_OUTPUT_INSTANCE output = (VS_OUTPUT_INSTANCE)0;
	output.InstanceId = _input.InstanceId;
	output.Color = float4(kFrustum.Color.xyz, 1.0f);
	output.Position = mul(float4(_input.Position.x + _fOffsetX, _input.Position.y + _fOffsetY, _input.Position.z + _fOffsetZ, 1.0f), kFrustum.World);
	output.Position = mul(output.Position, VIEW_PROJECTION);
	_kStream.Append(output);
	
	return output;
}

/******************************************************************************
**	Geometry Shader Instancing
******************************************************************************/
[maxvertexcount(18)]
void GeometryInstancing(in point VS_OUTPUT_INSTANCE _input[1], inout LineStream<VS_OUTPUT_INSTANCE> _kStream)
{
	VS_OUTPUT_INSTANCE input = _input[0];
	Frustum kFrustum = aFrustum[input.InstanceId];
	
	float fHeightScale = tan(kFrustum.Fov * 0.5f);
	float2 vNearSize = float2(kFrustum.Near * kFrustum.Aspect, kFrustum.Near * fHeightScale);
	float2 vFarSize = float2(kFrustum.Far * kFrustum.Aspect, kFrustum.Far * fHeightScale);
	
	// Near face
	VS_OUTPUT_INSTANCE kVertexN_LB = AddVertex(input, -vNearSize.x, -vNearSize.y, +kFrustum.Near, _kStream);
	VS_OUTPUT_INSTANCE kVertexN_LT = AddVertex(input, -vNearSize.x, +vNearSize.y, +kFrustum.Near, _kStream);
	VS_OUTPUT_INSTANCE kVertexN_RT = AddVertex(input, +vNearSize.x, +vNearSize.y, +kFrustum.Near, _kStream);
	VS_OUTPUT_INSTANCE kVertexN_RB = AddVertex(input, +vNearSize.x, -vNearSize.y, +kFrustum.Near, _kStream);
	_kStream.Append(kVertexN_LB);
	_kStream.RestartStrip();
	
	// Far face
	VS_OUTPUT_INSTANCE kVertexF_LB = AddVertex(input, -vFarSize.x, -vFarSize.y, +kFrustum.Far, _kStream);
	VS_OUTPUT_INSTANCE kVertexF_LT = AddVertex(input, -vFarSize.x, +vFarSize.y, +kFrustum.Far, _kStream);
	VS_OUTPUT_INSTANCE kVertexF_RT = AddVertex(input, +vFarSize.x, +vFarSize.y, +kFrustum.Far, _kStream);
	VS_OUTPUT_INSTANCE kVertexF_RB = AddVertex(input, +vFarSize.x, -vFarSize.y, +kFrustum.Far, _kStream);
	_kStream.Append(kVertexF_LB);
	_kStream.RestartStrip();
	
	// Close Frustum
	VS_OUTPUT_INSTANCE kVertex_Apex =  AddVertex(input, 0.0f, 0.0f, 0.0f, _kStream);
	_kStream.Append(kVertexF_LB);
	_kStream.RestartStrip();
	_kStream.Append(kVertex_Apex);
	_kStream.Append(kVertexF_LT);
	_kStream.RestartStrip();
	_kStream.Append(kVertex_Apex);
	_kStream.Append(kVertexF_RT);
	_kStream.RestartStrip();
	_kStream.Append(kVertex_Apex);
	_kStream.Append(kVertexF_RB);
	_kStream.RestartStrip();
}