/******************************************************************************
**	Define
******************************************************************************/
#define VOXEL_GRID_STATE_FRAGMENT 	0
#define VOXEL_GRID_STATE_NODE 		3

#define VOXEL_NODE_UNDEFINED 	-1
#define VOXEL_NODE_SUBDIVISE 	-2
#define VOXEL_NODE_LEAF 		-3

/******************************************************************************
**	Struct
******************************************************************************/
cbuffer ConstantBufferVoxel
{
	float4x4 	GRID_VIEW;
	float4x4 	GRID_PROJECTION;
	float3 		GRID_CENTER;
	float		GRID_SIZE;
	float 		VOXEL_HALF_SIZE;
	uint 		VOXEL_OCTREE_DEPTH_LIMIT;
};

struct VoxelFragment
{
	uint3 	Normal;
	uint3 	Color;
};

struct VoxelNode
{
	uint Offset;
	uint brickPtr;
	uint neighborX;
	uint neighborY;
	uint neighborZ;
};