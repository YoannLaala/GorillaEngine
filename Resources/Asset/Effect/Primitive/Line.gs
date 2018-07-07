/******************************************************************************
**	Includes
******************************************************************************/
#include "../Common.sl"
#include "Vertex.sl"

/******************************************************************************
**	Buffers
******************************************************************************/
struct Line
{
	float3 	Start;
	float3 	End;
	float3	Color;
};
StructuredBuffer<Line> aLine : register(t0);

/******************************************************************************
**	AddVertex
******************************************************************************/
VS_OUTPUT_INSTANCE AddVertex(in float3 _vPosition, in float3 _vColor, inout LineStream<VS_OUTPUT_INSTANCE> _kStream)
{	
	VS_OUTPUT_INSTANCE output = (VS_OUTPUT_INSTANCE)0;
	output.Color = float4(_vColor, 1.0f);
	output.Position = mul(float4(_vPosition, 1.0f), VIEW_PROJECTION);
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
	Line kLine = aLine[input.InstanceId];
	
	AddVertex(kLine.Start, kLine.Color, _kStream);
	AddVertex(kLine.End, kLine.Color, _kStream);
}