/******************************************************************************
**	Includes
******************************************************************************/
#include "../Common.sl"
#include "Struct.sl"

/******************************************************************************
**	Resources
******************************************************************************/
StructuredBuffer<VoxelFragment> aVoxelFragment : register(t0);
Buffer<uint> aVoxelGridState : register(t1);
RWStructuredBuffer<VoxelNode> aVoxelOctree : register(u0);

/******************************************************************************
**	Helper
******************************************************************************/
uint FindNodeIndex(in float3 _vPosition)
{
	float3 pos;
	float3 boxMin = (float3)0;
	float3 boxDim = (float3)1.0f;
	uint uiIndex = 0;
	uint uiDepth = 0;
	
	// Traverse octree until undivided node is found
	while (aVoxelOctree[uiIndex].Offset != VOXEL_NODE_UNDEFINED && aVoxelOctree[uiIndex].Offset != VOXEL_NODE_SUBDIVISE && uiDepth < VOXEL_OCTREE_DEPTH_LIMIT)
	{
		boxDim *= 0.5;
		uint uiChildIndex = dot(step(boxMin + boxDim, _vPosition), float3(1, 2, 4)); // Range child index between 0-7
		uiIndex = aVoxelOctree[uiIndex].Offset + uiChildIndex;
		boxMin += (boxDim * uiChildIndex);
		++uiDepth;
	}
	
	return uiIndex;
}

/******************************************************************************
**	Compute Shader
******************************************************************************/
[numthreads(1, 1, 1)]
void ComputeDefault(in uint3 _uiGroupId : SV_GroupID)
{
	VoxelFragment kFragment = aVoxelFragment[_uiGroupId.x];
	uint uiIndex = 0;//FindNodeIndex(kFragment.Position);

    uint uiValue = 0;
	InterlockedCompareExchange(aVoxelOctree[uiIndex].Offset, VOXEL_NODE_LEAF, VOXEL_NODE_SUBDIVISE, uiValue);
	InterlockedCompareExchange(aVoxelOctree[uiIndex].Offset, VOXEL_NODE_UNDEFINED, VOXEL_NODE_LEAF, uiValue);
}