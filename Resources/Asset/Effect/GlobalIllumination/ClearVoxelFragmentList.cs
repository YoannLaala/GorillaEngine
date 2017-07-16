/******************************************************************************
**	Includes
******************************************************************************/
#include "../Common.sl"
#include "Struct.sl"

/******************************************************************************
**	Resources
******************************************************************************/
RWStructuredBuffer<VoxelNode> aVoxelOctree : register(u0);
RWBuffer<uint> aVoxelGridState : register(u1);

/******************************************************************************
**	Compute Shader
******************************************************************************/
[numthreads(1, 1, 1)]
void ComputeDefault()
{
	uint uiValue = 0;
	
	InterlockedExchange(aVoxelOctree[0].Offset, 1, uiValue);
	for(uint uiIndex = 1; uiIndex < 9; ++uiIndex)
	{
		InterlockedExchange(aVoxelOctree[uiIndex].Offset, VOXEL_NODE_UNDEFINED, uiValue);
	}
	
	// Fragment & Node Dispatch
	for(uint uiIndex = 0; uiIndex < 3; ++uiIndex)
	{
		InterlockedExchange(aVoxelGridState[VOXEL_GRID_STATE_FRAGMENT + uiIndex], 1, uiValue);
		InterlockedExchange(aVoxelGridState[VOXEL_GRID_STATE_NODE + uiIndex], 1, uiValue);
	}
	InterlockedExchange(aVoxelGridState[VOXEL_GRID_STATE_FRAGMENT], 0, uiValue);
}