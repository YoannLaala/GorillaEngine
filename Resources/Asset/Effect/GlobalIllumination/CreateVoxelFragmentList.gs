/******************************************************************************
**	Includes
******************************************************************************/
#include "CreateVoxelFragmentList.vs"

/******************************************************************************
**	Geometry Shader Output
******************************************************************************/
struct GS_OUTPUT
{
	float4 	Position 	: SV_POSITION;
	float4 	PositionWS	: POSITION;
	float3 	Normal		: NORMAL;
	float2	Texcoord	: TEXCOORD0;
	uint	View		: INDEX0;
};

/******************************************************************************
**	Helpers
******************************************************************************/
uint ComputeViewIndex(in float3 _vNormal0, in float3 _vNormal1, in float3 _vNormal2)
{
	// Maximum dot product correspond to the most visible direction 
	float3 vFaceNormal = abs(normalize(_vNormal0 + _vNormal1 + _vNormal2));
	float fMaximum = max(max(vFaceNormal.x, vFaceNormal.y), vFaceNormal.z);
	
	if(fMaximum == vFaceNormal.x) 		return 0;		// Right
	else if(fMaximum == vFaceNormal.y) 	return 1; 		// Top

	// Forward
	return 2;
}

static float4 aPosition[3];

/******************************************************************************
**	Geometry Shader
******************************************************************************/
[maxvertexcount(3)]
void GeometryInstancing(in triangle VS_OUTPUT _input[3], inout TriangleStream<GS_OUTPUT> _kStream)
{	
	// Compute the view where the triangle is the most visible
	uint uiViewIndex = ComputeViewIndex(_input[0].Normal, _input[1].Normal, _input[2].Normal);

	// Create vertices with proper VIEW*PROJECTION
	GS_OUTPUT output[3];
	[unroll]
	for(uint i=0; i < 3; ++i)
	{		
		aPosition[0] = _input[i].Position.zyxw;	// Right
		aPosition[1] = _input[i].Position.xzyw;	// Top
		aPosition[2] = _input[i].Position.xyzw;	// Forward

		output[i].Position = mul(aPosition[uiViewIndex], GRID_VIEW);
		output[i].Position = mul(output[i].Position, GRID_PROJECTION);
		output[i].PositionWS = _input[i].Position;
		output[i].Normal = _input[i].Normal;
		output[i].Texcoord = _input[i].Texcoord;
		output[i].View = uiViewIndex;
		
		_kStream.Append(output[i]);
	}
}