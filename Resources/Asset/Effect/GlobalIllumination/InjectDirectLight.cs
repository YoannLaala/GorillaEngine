/******************************************************************************
**	Includes
******************************************************************************/
#include "../Common.sl"
#include "Struct.sl"
#include "Helper.sl"

/******************************************************************************
**	Resources
******************************************************************************/
Texture2D tShadow								: register(t0);
StructuredBuffer<VoxelFragment> aVoxelFragment 	: register(t1);
//Texture3D<float4> tVoxelNormal 					: register(t1);
//RWTexture3D<float4> tVoxelColor 				: register(u0); 

// RWBuffer<uint> aVirtualLight : register(u0); 
// RWBuffer<uint> aVoxelGridState : register(u1);

// uint GetNextIndexVirtualLight()
// {
	// uint uiIndex = 0;
	// InterlockedAdd(aVoxelGridState[0], 1, uiIndex);
	
	// return uiIndex;
// }

/******************************************************************************
**	Compute Shader
******************************************************************************/
[numthreads(32, 32, 1)]
void ComputeDefault(in uint3 _vDispatchThreadId : SV_DispatchThreadID)
{
	float2 vTexcoord = _vDispatchThreadId.xy / 1024.0f;
	float fDepth = tShadow.SampleLevel(sPointSampler, vTexcoord.xy, 0).r;
	//if(fDepth > 0.0f)
	{	
		//float fDepthLinear = ComputeLinearDepth(fDepth);
		float4 vPosition = float4((2.0f * vTexcoord.x) - 1.0f, -((2.0f * vTexcoord.y) - 1.0f), fDepth, 1.0f);
		/*vPosition.x /= PROJECTION._m00;
		vPosition.y /= PROJECTION._m11;
		vPosition.xyz *= fDepthLinear;*/
		vPosition = mul(vPosition, PROJECTION);
		vPosition = mul(vPosition, VIEW);
		vPosition.xyz /= vPosition.w;
		
		//vPosition.z = (-2.0f * vPosition.z) + 1.0;
		
		//vPosition.xyz += float3(0.0f, -1.0f, -0.0f);
		//vPosition.xyz /= 1.0f;*/
		//vPosition.xyz /= vPosition.w;

		// float4 vPosition = float4(((2.0f * vTexcoord.x) - 1.0f), ((2.0f * vTexcoord.y) - 1.0f), -fDepth, 1.0f);
		// //vPosition.xyz *= fDepth;
		// vPosition = mul(vPosition, PROJECTION);
		// vPosition.xyz /= vPosition.w;
		// vPosition = mul(vPosition, VIEW);
		//vPosition.y *= 0.5f;
		
		//uint3 vCoordinate = PackVoxelIndex(vPosition.xyz);
		//uint3 vCoordinate = PackVoxelCoordinate(vPosition.xyz);
		
		//uint uiVoxelIndex = PackVoxelIndex(vPosition.xyz);
		//uint uiVirtuaLightIndex = GetNextIndexVirtualLight();
		//aVirtualLight[uiVirtuaLightIndex] = uiVoxelIndex;
		
		
		
		//float3 vTexcoord = PackVoxelTexcoord(vPosition.xyz);
		
		//if(vColor.a)
		// {
			// float4 vColor = float4(normalize(float3(aVoxelFragment[uiVoxelIndex].Color)), 1.0);
			// float3 vNormal = normalize(float3(aVoxelFragment[uiVoxelIndex].Normal));
			// float3 vLight = float3(0.0f, 1.0f, 0.0f);
			// float fNdotL = dot(vNormal, vLight);
			// vColor.xyz *= fNdotL;
			// tVoxelColor[vCoordinate] = vColor;
			
			// tVoxelColor[vCoordinate + int3(-1, 0, 0)] = vColor;
		// }	
	}
}