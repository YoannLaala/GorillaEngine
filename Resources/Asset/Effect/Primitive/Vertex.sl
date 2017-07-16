/******************************************************************************
**	Vertex Shader Input
******************************************************************************/
struct VS_INPUT
{
	float3 	Position 	: POSITION;
	float2  Texcoord	: TEXCOORD0;
	uint	InstanceId	: SV_InstanceID;
};

/******************************************************************************
**	Vertex Shader Output
******************************************************************************/
struct VS_OUTPUT_INSTANCE
{
	float4 	Position 	: SV_POSITION;
	float4	Color		: COLOR0;
	uint	InstanceId	: INSTANCEID;
};