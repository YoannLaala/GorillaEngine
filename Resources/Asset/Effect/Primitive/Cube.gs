/******************************************************************************
**	Includes
******************************************************************************/
#include "../Common.sl"
#include "Vertex.sl"

/******************************************************************************
**	Buffers
******************************************************************************/
struct Cube
{
	float4x4 	World;
	float3		Color;
};
StructuredBuffer<Cube> aCube : register(t0);

/******************************************************************************
**	AddVertex
******************************************************************************/
VS_OUTPUT_INSTANCE AddVertex(in VS_OUTPUT_INSTANCE _input, in float _fOffsetX, in float _fOffsetY, in float _fOffsetZ, inout LineStream<VS_OUTPUT_INSTANCE> _kStream)
{
	Cube kCube = aCube[_input.InstanceId];
	
	VS_OUTPUT_INSTANCE output = (VS_OUTPUT_INSTANCE)0;
	output.InstanceId = _input.InstanceId;
	output.Color = float4(kCube.Color.xyz, 1.0f);
	output.Position = mul(float4(_input.Position.x + _fOffsetX, _input.Position.y + _fOffsetY, _input.Position.z + _fOffsetZ, 1.0f), kCube.World);
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
	
	// Near face
	VS_OUTPUT_INSTANCE kVertexN_LB = AddVertex(input, -0.25f, -0.25f, -0.25f, _kStream);
	VS_OUTPUT_INSTANCE kVertexN_LT = AddVertex(input, -0.25f, +0.25f, -0.25f, _kStream);
	VS_OUTPUT_INSTANCE kVertexN_RT = AddVertex(input, +0.25f, +0.25f, -0.25f, _kStream);
	VS_OUTPUT_INSTANCE kVertexN_RB = AddVertex(input, +0.25f, -0.25f, -0.25f, _kStream);
	_kStream.Append(kVertexN_LB);
	_kStream.RestartStrip();
	
	// Far face
	VS_OUTPUT_INSTANCE kVertexF_LB = AddVertex(input, -0.25f, -0.25f, +0.25f, _kStream);
	VS_OUTPUT_INSTANCE kVertexF_LT = AddVertex(input, -0.25f, +0.25f, +0.25f, _kStream);
	VS_OUTPUT_INSTANCE kVertexF_RT = AddVertex(input, +0.25f, +0.25f, +0.25f, _kStream);
	VS_OUTPUT_INSTANCE kVertexF_RB = AddVertex(input, +0.25f, -0.25f, +0.25f, _kStream);
	_kStream.Append(kVertexF_LB);
	_kStream.RestartStrip();
	
	// Close Cube
	_kStream.Append(kVertexN_LB);
	_kStream.Append(kVertexF_LB);
	_kStream.RestartStrip();
	_kStream.Append(kVertexN_LT);
	_kStream.Append(kVertexF_LT);
	_kStream.RestartStrip();
	_kStream.Append(kVertexN_RB);
	_kStream.Append(kVertexF_RB);
	_kStream.RestartStrip();
	_kStream.Append(kVertexN_RT);
	_kStream.Append(kVertexF_RT);
	_kStream.RestartStrip();
}