#ifndef _COOKER_ENUM_HPP_
#define _COOKER_ENUM_HPP_

/******************************************************************************
**	Include
******************************************************************************/
#include <Core/Core.hpp>
#include <Renderer/Common.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	namespace Helper
	{
		uint8 GetEnumValue(const char* _szName, const uint8* _pArrayValue, const char** _ppArrayName, uint32 _uiCount);
	}

	namespace Renderer
	{
		namespace ETechnique
		{
			static const char* Name[ETechnique::Count] =
			{
				"Default"							,
				"Instancing"						,
			};
		};

		namespace EShader
		{
			static uint8 Value[EShader::Count] =
			{
				EShader::Vertex						,
				EShader::Geometry					,
				EShader::Pixel						,
				EShader::Compute					,
			};

			static const char* Name[EShader::Count] =
			{
				"Vertex"							,
				"Geometry"							,
				"Pixel"								,
				"Compute"							,
			};

			static const char* Extension[EShader::Count] =
			{
				"vs"								,
				"gs"								,
				"ps"								,
				"cs"								,
			};

			static const char* Target[EShader::Count] =
			{
				"vs_5_0"							,
				"gs_5_0"							,
				"ps_5_0"							,
				"cs_5_0"							,
			};
		};

		namespace ETopology
		{
			static uint8 Value[ETopology::Count] =
			{
				ETopology::PointList				,
				ETopology::LineList					,
				ETopology::LineStrip				,
				ETopology::TriangleList				,
				ETopology::TriangleAdjacent			,
				ETopology::LineListAdjacent			,
				ETopology::LineStripAdjacent		,
				ETopology::TriangleListAdjacent		,
				ETopology::TriangleStripAdjacent	,
				ETopology::ControlPointList			,
			};

			static const char* Name[ETopology::Count] =
			{
				"PointList"							,
				"LineList"							,
				"LineStrip"							,
				"TriangleList"						,
				"TriangleAdjacent"					,
				"LineListAdjacent"					,
				"LineStripAdjacent"					,
				"TriangleListAdjacent"				,
				"TriangleStripAdjacent"				,
				"ControlPointList"					,
			};
		}
	
		namespace EDepthComparaison
		{
			static uint8 Value[EDepthComparaison::Count] =
			{
				EDepthComparaison::Never			,
				EDepthComparaison::Less				,
				EDepthComparaison::Equal			,
				EDepthComparaison::LessEqual		,
				EDepthComparaison::Greater			,
				EDepthComparaison::NotEqual			,
				EDepthComparaison::GeaterEqual		,
				EDepthComparaison::Always			,
			};

			static const char* Name[EDepthComparaison::Count] =
			{
				"Never"								,
				"Less"								,
				"Equal"								,
				"LessEqual"							,
				"Greater"							,
				"NotEqual"							,
				"GeaterEqual"						,
				"Always"							,
			};
		}

		namespace EBlend
		{
			static uint8 Value[EBlend::Count] =
			{
				EBlend::Zero						,
				EBlend::One							,
				EBlend::SourceColor					,
				EBlend::SourceColorInversed			,
				EBlend::SourceAlpha					,
				EBlend::SourceAlphaInversed			,
				EBlend::DestinationAlpha			,
				EBlend::DestinationAlphaInversed	,
				EBlend::DestinationColor			,
				EBlend::DestinationColorInversed	,
				EBlend::SourceAlphaSaturated		,
			};

			static const char* Name[EBlend::Count] =
			{
				"Zero"								,
				"One"								,
				"SourceColor"						,
				"SourceColorInversed"				,
				"SourceAlpha"						,
				"SourceAlphaInversed"				,
				"DestinationAlpha"					,
				"DestinationAlphaInversed"			,
				"DestinationColor"					,
				"DestinationColorInversed"			,
				"SourceAlphaSaturated"				,
			};
		}

		namespace EBlendOperation
		{
			static uint8 Value[EBlendOperation::Count] =
			{
				EBlendOperation::Add				,
				EBlendOperation::Substract			,
				EBlendOperation::SubstractRev		,
				EBlendOperation::Min				,
				EBlendOperation::Max				,
			};

			static const char* Name[EBlendOperation::Count] =
			{
				"Add"								,
				"Substract"							,
				"SubstractRev"						,
				"Min"								,
				"Max"								,
			};	
		};
	}
}
#endif