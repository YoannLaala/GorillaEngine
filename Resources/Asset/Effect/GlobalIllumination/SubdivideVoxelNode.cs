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
**	Helper
******************************************************************************/
uint GetNextIndexVoxelNode()
{
	uint uiIndex = 0;
	InterlockedAdd(aVoxelGridState[VOXEL_GRID_STATE_NODE], 1, uiIndex);
	
	return 1 + uiIndex * 8;
}

/******************************************************************************
**	Compute Entry
******************************************************************************/
[numthreads(9, 1, 1)]
void ComputeDefault(in uint _uiGroupId : SV_GroupID, in uint _uiGroupIndex : SV_GroupIndex)
{
	uint uiNodeIndex = _uiGroupId.x * 9 + _uiGroupIndex;
	if(aVoxelOctree[uiNodeIndex].Offset == VOXEL_NODE_SUBDIVISE)
	{
        uint uiValue = 0;
		uint uiNodeIndexChild = GetNextIndexVoxelNode();
		InterlockedExchange(aVoxelOctree[uiNodeIndex].Offset, uiNodeIndexChild, uiValue);
		
		// Prepare child
		for(uint uiNode = 0; uiNode < 9; ++uiNode)
		{
			InterlockedExchange(aVoxelOctree[uiNodeIndexChild + uiNode].Offset, VOXEL_NODE_UNDEFINED, uiValue);
		}
	}
}