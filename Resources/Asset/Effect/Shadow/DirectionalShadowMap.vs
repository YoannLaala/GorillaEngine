/******************************************************************************
**	Includes
******************************************************************************/
#include "../Common.sl"

/******************************************************************************
**	Resources
******************************************************************************/
StructuredBuffer<GeometryInstance> aGeometryInstance;

/******************************************************************************
**	Vertex Shader Input
******************************************************************************/
struct VS_INPUT
{
	float3	Position 	: POSITION0;
	float3	Normal		: NORMAL0;
	float2	Texcoord	: TEXCOORD0;
	uint 	InstanceId	: SV_InstanceID;
};

/******************************************************************************
**	Vertex Shader Output Instance
******************************************************************************/
struct VS_OUTPUT_INSTANCE
{
	float4 Position 	: SV_POSITION;
};

/******************************************************************************
**	Vertex Shader Instancing
******************************************************************************/
VS_OUTPUT_INSTANCE VertexInstancing(VS_INPUT _input)
{
	GeometryInstance kInstance = aGeometryInstance[_input.InstanceId];
	
	VS_OUTPUT_INSTANCE output = (VS_OUTPUT_INSTANCE)0;
	output.Position = float4(_input.Position, 1.0f);
	output.Position = mul(output.Position, kInstance.World);
	output.Position = mul(output.Position, VIEW);
	output.Position = mul(output.Position, PROJECTION);
	
	return output;
}