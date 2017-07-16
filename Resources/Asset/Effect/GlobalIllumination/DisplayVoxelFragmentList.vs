/******************************************************************************
**	Includes
******************************************************************************/
#include "../Common.sl"
#include "Struct.sl"
#include "Helper.sl"

/******************************************************************************
**	Resources
******************************************************************************/
StructuredBuffer<VoxelFragment> aVoxelFragment : register(t0);

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

/******************************************************************************
**	Vertex Shader Instancing
******************************************************************************/
VS_OUTPUT_INSTANCE VertexInstancing(VS_INPUT _input)
{
	VS_OUTPUT_INSTANCE output = (VS_OUTPUT_INSTANCE)0;
	
	VoxelFragment kVoxel = aVoxelFragment[_input.InstanceId];
	/*if(kVoxel.Count > 0)
	{	
		float3 vPosition = UnpackVoxelIndex(_input.InstanceId);
		output.Position = float4(vPosition + (_input.Position * VOXEL_HALF_SIZE), 1.0f);
		output.Position.xyz *= 2.0f;
		output.Position = mul(output.Position, VIEW);
		output.Position = mul(output.Position, PROJECTION);
		output.InstanceId = _input.InstanceId;
	}	*/
	
	return output;
}