/******************************************************************************
**	Includes
******************************************************************************/
#include "../Common.sl"
#include "Vertex.sl"

/******************************************************************************
**	Vertex Shader Instancing
******************************************************************************/
VS_OUTPUT_INSTANCE VertexInstancing(VS_INPUT _input)
{
	VS_OUTPUT_INSTANCE output = (VS_OUTPUT_INSTANCE)0;
	output.InstanceId = _input.InstanceId;
	
	return output;
}