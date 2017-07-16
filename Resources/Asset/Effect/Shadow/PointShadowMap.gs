/******************************************************************************
**	Includes
******************************************************************************/
#include "PointShadowMap.vs"

/******************************************************************************
**	Resources
******************************************************************************/
StructuredBuffer<PointLight> aPointLight : register(t1);

/******************************************************************************
**	Geometry Shader Instancing
******************************************************************************/
[maxvertexcount(18)]
void GeometryDefault(in triangle VS_OUTPUT_INSTANCE _input[3], inout TriangleStream<GS_OUTPUT> _kStream)
{
	uint uiCount, uiSize;
	aPointLight.GetDimensions(uiCount, uiSize);
	
	GS_OUTPUT aVertex[3];
	for(uint uiLight = 0; uiLight < uiCount; ++uiLight)
	{
		PointLight kLight = aPointLight[uiLight];		
		for(uint uiView = 0; uiView < POINT_LIGHT_VIEW_COUNT; ++uiView)
		{
			for(uint uiVertex = 0; uiVertex < 3; ++uiVertex)
			{
				aVertex[uiVertex].Position = mul(_input[uiVertex].Position, POINT_LIGHT_VIEW[uiView]);
				aVertex[uiVertex].Position = mul(aVertex[uiVertex].Position, POINT_LIGHT_PROJECTION);
				aVertex[uiVertex].PositionWorld = _input[uiVertex].Position.xyz;
				aVertex[uiVertex].ViewIndex = uiView;
				aVertex[uiVertex].LightIndex = uiLight;
				
				_kStream.Append(aVertex[uiVertex]);
			}
			_kStream.RestartStrip();
		}
	}
}