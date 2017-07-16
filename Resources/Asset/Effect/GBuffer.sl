//!	@brief		PackNormal 
//!	@details	Allow to use Normal value in R8G8B8 format
//!	@date		2016-07-30
float3 PackNormal(in float3 _vNormal)
{
	return 0.5f * _vNormal + 0.5f; 
}

//!	@brief		UnpackNormal 
//!	@details	Allow to use Normal value in R8G8B8 format
//!	@date		2016-07-30
float3 UnpackNormal(in float3 _vNormal)
{
	return 2.0f * _vNormal - 1.0f; 
}