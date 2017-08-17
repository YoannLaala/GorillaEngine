/******************************************************************************
**	Includes
******************************************************************************/
#include "ObjCooker.hpp"
#include "Geometry.hpp"
#include <Core/Stream/MemoryWriter.hpp>
#include <Core/Helper/Compression.hpp>
#include <Core/String/StringHelper.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Serialize
	//!	@date		2015-11-22
	bool ObjCooker::Serialize(const char* _szInputPath, FileWriter& _kOutputStream)
	{
		FileReader kReader(_szInputPath);
		if(!kReader.CanRead())
		{
			printf("Failed to read input file %s\n", _szInputPath);
			return false;
		}
		const uint32 uiFileSize = kReader.GetSize();
		String sContent; sContent.Resize(uiFileSize);
		kReader.Read(&sContent[0], uiFileSize);
	    
		Geometry kGeometry;	

		// Iterate on each line
		uint32 uiLineStart = 0;
		uint32 uiLineEnd = sContent.Find("\n");
		Vector<Math::Vector3> vPosition;
		Vector<Math::Vector2> vTexcoord;
		while(uiLineEnd != (uint32)-1)
		{
			switch(sContent[uiLineStart])
			{
				// group
				case 'g':
				{
					if(kGeometry.Vertices.GetSize())
					{
						kGeometry.Groups.Add(kGeometry.Vertices.GetSize());
					}

					break;
				}

				// vertex related
				case 'v':
				{
					switch(sContent[uiLineStart+1])
					{
						// vertex
						case ' ':
						{
							uint32 uiValueStart = uiLineStart+2;
							Math::Vector3& vValue = vPosition.Add();
							for(uint32 uiCoordinate = 0; uiCoordinate < 3; uiCoordinate++)
							{
								static const char* aPattern[3] = { " ", " ", "\n" };
								uint32 uiValueEnd = sContent.Find(aPattern[uiCoordinate], uiValueStart);
								vValue[uiCoordinate] = StringHelper::ToFloat32(&sContent[uiValueStart], uiValueEnd - uiValueStart);
								uiValueStart = ++uiValueEnd;
							}

							break;
						}

						// normal
						case 'n':
						{
							// directly computed in cooker
							break;
						}

						// texcoord
						case 't':
						{
							uint32 uiValueStart = uiLineStart+3;
							Math::Vector2& vValue = vTexcoord.Add();
							for(uint32 uiCoordinate = 0; uiCoordinate < 2; uiCoordinate++)
							{
								static const char* aPattern[2] = { " ", "\n" };
								uint32 uiValueEnd = sContent.Find(aPattern[uiCoordinate], uiValueStart);
								vValue[uiCoordinate] = StringHelper::ToFloat32(&sContent[uiValueStart], uiValueEnd - uiValueStart);
								uiValueStart = ++uiValueEnd;
							}
							break;
						}
					}

					break;
				}

				// face
				case 'f':
				{
					// Compute vertex start & end index
					const uint32 uiVertexStart = kGeometry.Vertices.GetSize();
					const uint32 uiVertexEnd = uiVertexStart + 3;

					uint32 uiValueStart;
					uint32 uiGroupStart = uiLineStart+2;
					for(uint32 uiCoordinate = 0; uiCoordinate < 3; uiCoordinate++)
					{
						Engine::RenderBuffer::Vertex::PositionNormalTexcoord& kVertex = kGeometry.Vertices.Add();
						uint32 uiGroupEnd = sContent.Find(" ", uiGroupStart);

						// Retrieve vertex
						uiValueStart = uiGroupStart;
						uint32 uiValueEnd = sContent.Find("/", uiValueStart);
						uint32 uiVertex = StringHelper::ToUint32(&sContent[uiValueStart], uiValueEnd - uiValueStart) - 1;
						kVertex.Position = vPosition[uiVertex] / 100.0f;
						LOG_INTERNAL_PRINT("%d", uiVertex);
						// Retrieve texcoord
						uiValueStart = ++uiValueEnd;
						uint32 uiTexcoord = StringHelper::ToUint32(&sContent[uiValueStart], uiGroupEnd - uiValueStart) - 1;
						kVertex.Texcoord = vTexcoord[uiTexcoord];

						uiGroupStart = ++uiGroupEnd;
					}

					// Compute normals
					Math::Vector3 v0 = kGeometry.Vertices[uiVertexStart+1].Position - kGeometry.Vertices[uiVertexStart].Position;
					Math::Vector3 v1 = kGeometry.Vertices[uiVertexStart+2].Position - kGeometry.Vertices[uiVertexStart].Position;
					Math::Vector3 vNormal = v0.Cross(v1); vNormal.Normalize();
					for(uint32 uiVertex = uiVertexStart; uiVertex < uiVertexEnd; ++uiVertex)
					{
						kGeometry.Vertices[uiVertex].Normal = vNormal;
					}

					break;
				}

				case '\n':
				{
					break;
				}
			}
			uiLineStart = ++uiLineEnd;
			uiLineEnd = sContent.Find("\n", uiLineStart);
		}
		kGeometry.Groups.Add(kGeometry.Vertices.GetSize());

		// Apply indices
		const uint32 uiIndexCount = kGeometry.Vertices.GetSize();
		kGeometry.Indices.Resize(uiIndexCount);
		for(uint32 uiIndex = 0; uiIndex < uiIndexCount; ++uiIndex)
		{
			kGeometry.Indices[uiIndex] = uiIndex;
		}
		kGeometry.ComputeBBox();
		
		// Write it to a memory stream
		MemoryWriter kMemoryStream;
		kMemoryStream.Write(kGeometry.Vertices.GetSize());
		kMemoryStream.Write(&kGeometry.Vertices[0], kGeometry.Vertices.GetSize() * sizeof(Engine::RenderBuffer::Vertex::PositionNormalTexcoord));
		kMemoryStream.Write(kGeometry.Indices.GetSize());
		kMemoryStream.Write(&kGeometry.Indices[0], kGeometry.Indices.GetSize() * sizeof(uint32));
		
		// Groups
		const uint32 uiGroupCount = kGeometry.Groups.GetSize();
		kMemoryStream.Write(uiGroupCount);
		if(uiGroupCount)
		{
			// Compute Index Count per section
			uint32 uiLastIndex = 0;
			for (uint32 uiSection = 0; uiSection < uiGroupCount; ++uiSection)
			{
				uint32 uiIndexCount = kGeometry.Groups[uiSection] - uiLastIndex;
				uiLastIndex = kGeometry.Groups[uiSection];
				kGeometry.Groups[uiSection] = uiIndexCount;
			}

			kMemoryStream.Write(&kGeometry.Groups[0], uiGroupCount * sizeof(uint32));
		}
		kMemoryStream.Write(&kGeometry.BBox, sizeof(Renderer::BoundingBox));

		// Compress the stream
		uint32 uiSizeCompressed;
		uint8* pBufferCompressed;
		Helper::Compress(&kMemoryStream[0], kMemoryStream.GetSize(), pBufferCompressed, uiSizeCompressed);
		_kOutputStream.Write(uiSizeCompressed);
		_kOutputStream.Write(pBufferCompressed, uiSizeCompressed);
		_kOutputStream.Close();
		delete pBufferCompressed;
		
		return true;
	}
}