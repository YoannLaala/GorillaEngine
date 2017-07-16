/******************************************************************************
**	Includes
******************************************************************************/
#include "../Common.sl"

/******************************************************************************
**	Vertex Shader Input
******************************************************************************/
struct VS_INPUT
{
	float3	Position 		: POSITION0;
	float2  Texcoord 		: TEXCOORD0;
};

/******************************************************************************
**	Vertex Shader Output
******************************************************************************/
struct VS_OUTPUT
{
	float4	Position 		: SV_POSITION;
	float2	Texcoord 		: TEXCOORD0;
};

/******************************************************************************
**	Vertex Shader Default
******************************************************************************/
VS_OUTPUT VertexDefault(VS_INPUT _input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Position = float4(_input.Position, 1.0f);
	output.Texcoord = _input.Texcoord;	
	
	return output;
}