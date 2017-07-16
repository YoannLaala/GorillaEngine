/******************************************************************************
**	Helpers
******************************************************************************/
uint3 PackVoxelCoordinate(in float3 _vPosition)
{
	uint3 vCoordinate = (_vPosition * (GRID_SIZE-1) * 0.5f) + (int3)(GRID_SIZE-1)*0.5f;
	vCoordinate.y = (GRID_SIZE-1) - vCoordinate.y;
	
	return vCoordinate;
}

float3 UnpackVoxelCoordinate(in uint3 _vCoordinate)
{
	_vCoordinate.y += (GRID_SIZE-1);
	return (_vCoordinate - (GRID_SIZE-1) * 0.5f) / ((GRID_SIZE-1)*0.5f) - (int3)(GRID_SIZE-1)*0.5f;
}

float3 PackVoxelTexcoord(in float3 _vPosition)
{
	uint3 vCoordinate = (_vPosition * (GRID_SIZE-1) * 0.5f) + (int3)(GRID_SIZE-1)*0.5f;
	vCoordinate.y = (GRID_SIZE-1) - vCoordinate.y;
	
	return vCoordinate / ((GRID_SIZE-1)*0.5f);
}

float3 PackVoxelTexcoord2(in float3 _vPosition, in float _fSize)
{
	uint3 vTexcoord = (_vPosition * _fSize * 0.5f) + (int3)_fSize*0.5f;
	vTexcoord.y = (_fSize-1) - vTexcoord.y;
	
	return vTexcoord / _fSize;
}

uint PackVoxelIndex(in float3 _vPosition)
{
	int3 vIndex = (_vPosition * (GRID_SIZE-1) * 0.5f) + (int3)(GRID_SIZE-1)*0.5f;
	return dot(vIndex, int3(1.0f, GRID_SIZE, GRID_SIZE * GRID_SIZE));
}

float3 UnpackVoxelIndex(in uint _uiIndex)
{
	int3 vPosition = int3(_uiIndex % GRID_SIZE, (_uiIndex / GRID_SIZE) % GRID_SIZE, (_uiIndex / GRID_SIZE / GRID_SIZE)) - (int3)GRID_SIZE*0.5f;
	return vPosition / GRID_SIZE;
}

uint ToUInt(in float _fValue)
{
	return uint(_fValue * 255.0f);
}

float ToFloat(in uint _uiValue)
{
	return _uiValue / 255.0f;
}

uint PackColor(in float3 _vColor)
{
	return ToUInt(_vColor.r) | (ToUInt(_vColor.g) << 8) | (ToUInt(_vColor.b) << 16);
}

float3 UnpackColor(in uint _uiColor)
{
	return float3(ToFloat(_uiColor & 0x000000ff), ToFloat((_uiColor & 0x0000ff00) >> 8), ToFloat((_uiColor & 0x00ff0000) >> 16));
}

/*uint PackNormal(in float3 _vNormal)
{
	return ToUInt(_vNormal.x) | (ToUInt(_vNormal.y) << 8) | (ToUInt(_vNormal.z) << 16) | ((sign(_vNormal.x) + 1) << 24) | ((sign(_vNormal.y) + 1) << 26) | ((sign(_vNormal.z) + 1) << 28);
}

float3 UnpackNormal(in uint _uiNormal)
{
	float3 vValue = float3(ToFloat(_uiNormal & 0x000000ff), ToFloat((_uiNormal & 0x0000ff00) >> 8), ToFloat((_uiNormal & 0x00ff0000) >> 16));
	int3 vSign = int3(((_uiNormal & 0x02000000) >> 24) - 1,  ((_uiNormal & 0x06000000) >> 26) - 1, ((_uiNormal & 0x18000000) >> 28) - 1);
	return vValue * vSign;
}*/