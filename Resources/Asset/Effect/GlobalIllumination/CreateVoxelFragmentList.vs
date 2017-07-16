/******************************************************************************
**	Includes
******************************************************************************/
#include "../Common.sl"
#include "Struct.sl"

/******************************************************************************
**	Resources
******************************************************************************/
StructuredBuffer<GeometryInstance> aGeometryInstance : register(t0);

/******************************************************************************
**	Vertex Shader Input
******************************************************************************/
struct VS_INPUT
{
	float3	Position 	: POSITION;
	float3	Normal		: NORMAL;
	float2	Texcoord	: TEXCOORD0;
	uint 	InstanceId	: SV_InstanceID;
};

/******************************************************************************
**	Vertex Shader Output
******************************************************************************/
struct VS_OUTPUT
{
	float4 Position 	: SV_POSITION;
	float3 Normal		: NORMAL;
	float2 Texcoord		: TEXCOORD0;
};

/******************************************************************************
**	Vertex Shader Instancing
******************************************************************************/
VS_OUTPUT VertexInstancing(VS_INPUT _input)
{
	GeometryInstance kInstance = aGeometryInstance[_input.InstanceId];
	
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Position = mul(float4(_input.Position, 1.0f), kInstance.World);
	output.Normal = normalize(mul(_input.Normal, (float3x3)kInstance.World));
	output.Texcoord = _input.Texcoord;
	
	return output;
}