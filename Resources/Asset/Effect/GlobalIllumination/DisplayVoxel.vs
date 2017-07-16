/******************************************************************************
**	Includes
******************************************************************************/
#include "../Common.sl"
#include "Buffer.sl"

/******************************************************************************
**	Vertex Shader Input
******************************************************************************/
struct VS_INPUT
{
	float3	Position 	: POSITION0;
	float2	Texcoord	: TEXCOORD0;
	uint	InstanceId	: SV_InstanceID;
};

/******************************************************************************
**	Vertex Shader Output
******************************************************************************/
struct VS_OUTPUT_INSTANCE
{
	float4 	Position 	: SV_POSITION;
	uint	InstanceId	: SV_InstanceID;
};

static float3x3 SCALE =
{
	1.0f/32.0f, 0.0f, 0.0f,
	0.0f, 1.0f/32.0f, 0.0f,
	0.0f, 0.0f, 1.0f/32.0f,
};

/******************************************************************************
**	Vertex Shader Instancing
******************************************************************************/
VS_OUTPUT_INSTANCE VertexInstancing(VS_INPUT _input)
{
	int3 vIndex = int3(_input.InstanceId % 32, (_input.InstanceId / 32) % 32, _input.InstanceId / 1024) - (int3)16;
	
	VS_OUTPUT_INSTANCE output = (VS_OUTPUT_INSTANCE)0;
	output.Position = float4((vIndex / 16.0f) + (_input.Position / 32.0f), 1.0f);
	output.Position = mul(output.Position, VIEW);
	output.Position = mul(output.Position, PROJECTION);
	output.InstanceId = _input.InstanceId;
	
	return output;
}