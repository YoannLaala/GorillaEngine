/******************************************************************************
**	Defines
******************************************************************************/
#define FXAA_PC 1
#define FXAA_HLSL_5 1
#define FXAA_QUALITY__PRESET 12
#define FXAA_QUALITY__SUBPIX 0.75f
#define FXAA_QUALITY__EDGE_THRESHOLD 0.166f
#define FXAA_QUALITY__EDGE_THRESHOLD_MIN 0.0833f

/******************************************************************************
**	Includes
******************************************************************************/
#include "Common.hlsl"
#include "FXAA3_11.hlsl"

/******************************************************************************
**	Resources
******************************************************************************/
Texture2D tTex		: register(t0);
SamplerState sFXAA;

/******************************************************************************
**	Vertex Shader Input
******************************************************************************/
struct VS_INPUT
{
	float3	Position 		: POSITION0;
	float2  Texcoord 		: TEXCOORD0;
};

/******************************************************************************
**	Pixel Shader Input
******************************************************************************/
struct PS_INPUT
{
	float4	Position 		: SV_POSITION;
	float2	Texcoord 		: TEXCOORD0;
};

/******************************************************************************
**	Vertex Shader Default
******************************************************************************/
PS_INPUT VSMain(VS_INPUT _input)
{
	PS_INPUT output = (PS_INPUT)0;

	output.Position = float4(_input.Position, 1.0f);
	output.Position = mul(output.Position, PROJECTION);
	output.Texcoord = _input.Texcoord;

	return output;
}


//#define FXAA_PRESET 3
//#define FXAA_HLSL_4 1
//#include “FxaaShader.h”
//cbuffer cbFxaa : register(b1) { 
//float4 rcpFrame : packoffset(c0); };
//struct FxaaVS_Output 
//{ 
//	float4 Pos : SV_POSITION; 
//	float2 Tex : TEXCOORD0; 
//};
//
//FxaaVS_Output FxaaVS(uint id : SV_VertexID) 
//{
//	FxaaVS_Output Output; Output.Tex = float2((id << 1) & 2, id & 2);
//	Output.Pos = float4(Output.Tex * float2(2.0f, -2.0f) + 
//	float2(-1.0f, 1.0f), 0.0f, 1.0f);
//	return Output; 
//}
//
//SamplerState anisotropicSampler : register(s0);
//Texture2D inputTexture : register(t0);
//
//float4 FxaaPS(FxaaVS_Output Input) : SV_TARGET 
//{
// FxaaTex tex = { anisotropicSampler, inputTexture };
// return float4(FxaaPixelShader(Input.Tex.xy, tex, rcpFrame.xy), 1.0f); 
//}

/******************************************************************************
**	Pixel Shader Default
******************************************************************************/
float4 PSMain(PS_INPUT _input) : SV_TARGET
{
	FxaaTex test;
	test.smpl = sFXAA;
	test.tex = tTex;

	FxaaTex test2;
	FxaaTex test3;

	//return tTex.Sample(sFXAA, _input.Texcoord);

	return FxaaPixelShader(_input.Texcoord, (float4)0, test, test2, test3, float2(1.0f/1280.0f, 1.0f/720.0f), (float4)0, (float4)0, (float4)0, 
		FXAA_QUALITY__SUBPIX, FXAA_QUALITY__EDGE_THRESHOLD, FXAA_QUALITY__EDGE_THRESHOLD_MIN, 0, 0, 0, (float4)0);
}