/******************************************************************************
**	Includes
******************************************************************************/
#include "../Common.sl"
#include "Vertex.sl"

/******************************************************************************
**	Buffers
******************************************************************************/
struct Quad
{
	float4x4 	World;
	float3		Color;
};
StructuredBuffer<Quad> aQuad : register(t0);

/******************************************************************************
**	Vertex Shader Instancing
******************************************************************************/
VS_OUTPUT_INSTANCE VertexInstancing(VS_INPUT _input)
{
	VS_OUTPUT_INSTANCE output = (VS_OUTPUT_INSTANCE)0;

	Quad kQuad = aQuad[_input.InstanceId];
	
	output.Position = mul(float4(_input.Position.x, _input.Position.y, _input.Position.z, 1.0f), kQuad.World);
	output.Position = mul(output.Position, VIEW_PROJECTION);
	output.Color = float4(kQuad.Color, 1.0f);
	
	return output;
}