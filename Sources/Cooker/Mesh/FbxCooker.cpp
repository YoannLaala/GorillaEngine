/******************************************************************************
**	Includes
******************************************************************************/
#include "FbxCooker.hpp"
#include "Geometry.hpp"
#include <Core/Stream/MemoryWriter.hpp>
#include <Core/Helper/Compression.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Serialize
	//!	@date		2015-11-22
	bool FbxCooker::Serialize(const char* _szInputPath, FileWriter& _kOutputStream)
	{
		// The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
		FbxManager* pFbxManager = FbxManager::Create();
		if(!pFbxManager)
		{
			printf("Unable to create FBX Manager!");
		    return false;
		}
		printf("Autodesk FBX SDK version %s\n", pFbxManager->GetVersion());
		
		// Create an IOSettings object. This object holds all import/export settings.
		FbxIOSettings* pFbxIOSetting = FbxIOSettings::Create(pFbxManager, IOSROOT);
		pFbxManager->SetIOSettings(pFbxIOSetting);

		// Create an importer
	    FbxImporter* pFbxImporter = FbxImporter::Create(pFbxManager, "");
	
	    // Initialize the importer by providing a filename.
	    bool bSuccess = pFbxImporter->Initialize(_szInputPath, -1, pFbxIOSetting);
	    if(!bSuccess)
	    {
			printf("FbxImporter::Initialize() failed. %s\n", pFbxImporter->GetStatus().GetErrorString());
	        return false;
	    }

		// Create an FBX scene. This object holds most objects imported/exported from/to files.
		FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "");
		if(!pFbxScene)
		{
			printf("Unable to create FBX scene!\n");
		    return false;
		}
	
	    // Import the Document
		printf("Importing...\n");
	    bSuccess = pFbxImporter->Import(pFbxScene);
	    if(!bSuccess)
	    {
			printf("FbxImporter::Import() failed. %s\n", pFbxImporter->GetStatus().GetErrorString());
	        return false;
	    }		
	
		// Convert mesh, NURBS and patch into triangle mesh
		printf("Triangulating...\n");
		FbxGeometryConverter kFbxGeometryConverter(pFbxManager);
		kFbxGeometryConverter.Triangulate(pFbxScene, true);
	
	    // Destroy the importer
		printf("Cleaning...\n");
	    pFbxImporter->Destroy();
		
		// Transform to Gorilla format
		Geometry kGeometry;	
		GenerateGeometry(pFbxScene->GetRootNode(), &kGeometry);
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

		// Cleanup
		pFbxScene->Destroy();
		pFbxManager->Destroy();
		
		return true;
	}

	//!	@brief		GenerateGeometry
	//!	@date		2015-11-22
	void FbxCooker::GenerateGeometry(FbxNode* _pNode, Geometry* _pGeometry)
	{
		FbxNodeAttribute* pNodeAttribute = _pNode->GetNodeAttribute();
		if(pNodeAttribute && pNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{	
			// Generate all vertices
			FbxMesh* pMesh = _pNode->GetMesh();
			GenerateVertices(_pNode, pMesh, _pGeometry);
		}

		// Childs
		for(int uiChild = 0; uiChild<_pNode->GetChildCount(); ++uiChild)
		{
			GenerateGeometry(_pNode->GetChild(uiChild), _pGeometry);
		}
	}

	//!	@brief		GenerateVertices
	//!	@date		2015-11-22
	void FbxCooker::GenerateVertices(FbxNode* _pNode, FbxMesh* _pMesh, Geometry* _pGeometry)
	{
		FbxAMatrix mGlobalMatrix = _pNode->EvaluateGlobalTransform();

		// Allocate Array 
		uint32 uiVertexIndex = _pGeometry->Vertices.GetSize();
		const uint32 uiMeshVertexCount = _pMesh->GetPolygonVertexCount();
		_pGeometry->Vertices.Expand(uiMeshVertexCount);
		_pGeometry->Indices.Expand(uiMeshVertexCount);
		_pGeometry->Groups.Add(uiVertexIndex + uiMeshVertexCount);

		// Each Polygons (Triangulated)
		const uint32 uiPolygonCount = (uint32)_pMesh->GetPolygonCount();
		const FbxVector4* pControlPoints = _pMesh->GetControlPoints();
		for (uint32 uiPolygon = 0; uiPolygon < uiPolygonCount; ++uiPolygon)
		{
			// Each Vertices of teh polygons
			const uint32 uiVertexCount = _pMesh->GetPolygonSize(uiPolygon);
			for (uint32 uiVertex = 0; uiVertex < uiVertexCount; ++uiVertex)
			{
				// Get The control point to find the proper vertex
				uint32 uiControlPoint = _pMesh->GetPolygonVertex(uiPolygon, uiVertex);
				FbxVector4 vFbxPosition = mGlobalMatrix.MultT(pControlPoints[uiControlPoint]);
			
				// Position
				Engine::RenderBuffer::Vertex::PositionNormalTexcoord& kVertex = _pGeometry->Vertices[uiVertexIndex];
				kVertex.Position.Set((float32)vFbxPosition[0], (float32)vFbxPosition[1], (float32)vFbxPosition[2]);

				// Normal
				FbxVector4 kFbxNormal;
				_pMesh->GetPolygonVertexNormal(uiPolygon, uiVertex, kFbxNormal);
				kFbxNormal = mGlobalMatrix.MultT(kFbxNormal);
				kFbxNormal.Normalize();
				kVertex.Normal.Set((float32)kFbxNormal[0],  (float32)kFbxNormal[1], (float32)kFbxNormal[2]);

				// Texcoord
				const uint32 uiUVCount = (uint32)_pMesh->GetElementUVCount();
				for(uint32 uiUV = 0; uiUV < uiUVCount; ++uiUV)
				{
					const FbxGeometryElementUV* pUVElement = _pMesh->GetElementUV(uiUV);

					// Get the proper UV index
					uint32 uiUVIndex =  pUVElement->GetIndexArray().GetAt(uiVertexIndex);
					FbxVector2 kFbxUV = pUVElement->GetDirectArray().GetAt(uiUVIndex);
					kVertex.Texcoord.Set((float32)kFbxUV[0], (float32)kFbxUV[1]);
				}
				
				// Increment index for each vertex processed
				_pGeometry->Indices[uiVertexIndex] = uiVertexIndex;
				++uiVertexIndex;	
			}			
		}
	}
}