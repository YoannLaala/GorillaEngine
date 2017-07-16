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
		printf("Autodesk FBX SDK version %s", pFbxManager->GetVersion());
		
		// Create an IOSettings object. This object holds all import/export settings.
		FbxIOSettings* pFbxIOSetting = FbxIOSettings::Create(pFbxManager, IOSROOT);
		pFbxManager->SetIOSettings(pFbxIOSetting);

		// Create an importer
	    FbxImporter* pFbxImporter = FbxImporter::Create(pFbxManager, "");
	
	    // Initialize the importer by providing a filename.
	    bool bSuccess = pFbxImporter->Initialize(_szInputPath, -1, pFbxIOSetting);
	    if(!bSuccess)
	    {
			printf("FbxImporter::Initialize() failed. %s", pFbxImporter->GetStatus().GetErrorString());
	        return false;
	    }

		// Create an FBX scene. This object holds most objects imported/exported from/to files.
		FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "");
		if(!pFbxScene)
		{
			printf("Unable to create FBX scene!");
		    return false;
		}
	
	    // Import the Document
	    bSuccess = pFbxImporter->Import(pFbxScene);
	    if(!bSuccess)
	    {
			printf("FbxImporter::Import() failed. %s", pFbxImporter->GetStatus().GetErrorString());
	        return false;
	    }
	
		// Convert mesh, NURBS and patch into triangle mesh
		FbxGeometryConverter kFbxGeometryConverter(pFbxManager);
		kFbxGeometryConverter.Triangulate(pFbxScene, true);
	
	    // Destroy the importer
	    pFbxImporter->Destroy();
		
		// System Unit
		FbxGlobalSettings& kFbxGlobalSetting = pFbxScene->GetGlobalSettings();
		FbxSystemUnit kSystemUnit = kFbxGlobalSetting.GetSystemUnit();
		float32 fScale = (float32)(1.0f / kSystemUnit.GetScaleFactor());

		// Transform to Gorilla format
		Geometry kGeometry;	
		GenerateGeometry(pFbxScene->GetRootNode(), &kGeometry, fScale);

		// Write it to a memory stream
		MemoryWriter kMemoryStream;
		kMemoryStream.Write(kGeometry.Vertices.GetSize());
		kMemoryStream.Write(&kGeometry.Vertices[0], kGeometry.Vertices.GetSize() * sizeof(Engine::RenderBuffer::Vertex::PositionNormalTexcoord));
		kMemoryStream.Write(kGeometry.Indices.GetSize());
		kMemoryStream.Write(&kGeometry.Indices[0], kGeometry.Indices.GetSize() * sizeof(uint32));
		
		// Sections
		const uint32 uiSectionCount = kGeometry.Sections.GetSize();
		kMemoryStream.Write(uiSectionCount);
		if(uiSectionCount)
		{
			// Compute Index Count per section
			uint32 uiLastIndex = 0;
			for (uint32 uiSection = 0; uiSection < uiSectionCount; ++uiSection)
			{
				uint32 uiIndexCount = kGeometry.Sections[uiSection] - uiLastIndex;
				uiLastIndex = kGeometry.Sections[uiSection];
				kGeometry.Sections[uiSection] = uiIndexCount;
			}

			kMemoryStream.Write(&kGeometry.Sections[0], uiSectionCount * sizeof(uint32));
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
	void FbxCooker::GenerateGeometry(FbxNode* _pNode, Geometry* _pGeometry, float32 _fScale)
	{
		FbxNodeAttribute* pNodeAttribute = _pNode->GetNodeAttribute();
		if(pNodeAttribute && pNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{	
			// Generate all vertices
			FbxMesh* pMesh = _pNode->GetMesh();
			GenerateVertices(pMesh, _pGeometry, _fScale);
		}

		// Childs
		for(int uiChild = 0; uiChild<_pNode->GetChildCount(); ++uiChild)
		{
			GenerateGeometry(_pNode->GetChild(uiChild), _pGeometry, _fScale);
		}
	}

	//!	@brief		GenerateVertices
	//!	@date		2015-11-22
	void FbxCooker::GenerateVertices(FbxMesh* _pMesh, Geometry* _pGeometry, float32 _fScale)
	{
		// Allocate Array 
		_pGeometry->Vertices.Resize(_pMesh->GetPolygonVertexCount());
		_pGeometry->Indices.Resize(_pMesh->GetPolygonVertexCount());

		// Each Polygons (Triangulated)
		uint32 uiVertexIndex = 0, uiLastMaterialId = 0;
		const uint32 uiPolygonCount = (uint32)_pMesh->GetPolygonCount();
		const FbxVector4* pControlPoints = _pMesh->GetControlPoints();
		for (uint32 uiPolygon = 0; uiPolygon < uiPolygonCount; ++uiPolygon)
		{
			// Get the MaterialId associated to this polygon
			// This id define a geometry section
			uint32 uiMaterialId =  _pMesh->GetLayer(0)->GetMaterials()->GetIndexArray().GetAt(uiPolygon);
			if(uiLastMaterialId != uiMaterialId)
			{
				_pGeometry->Sections.Add(uiVertexIndex);
				uiLastMaterialId = uiMaterialId;
			}			

			// Each Vertices of teh polygons
			const uint32 uiVertexCount = _pMesh->GetPolygonSize(uiPolygon);
			for (uint32 uiVertex = 0; uiVertex < uiVertexCount; ++uiVertex)
			{
				// Get The control point to find the proper vertex
				uint32 uiControlPoint = _pMesh->GetPolygonVertex(uiPolygon, uiVertex);
				const FbxVector4& kFbxPosition = pControlPoints[uiControlPoint];
			
				// Position
				Engine::RenderBuffer::Vertex::PositionNormalTexcoord& kVertex = _pGeometry->Vertices[uiVertexIndex];
				kVertex.Position.Set((float32)kFbxPosition[0], (float32)kFbxPosition[1], (float32)kFbxPosition[2]);
				kVertex.Position *= _fScale;

				// Normal
				FbxVector4 kFbxNormal;
				_pMesh->GetPolygonVertexNormal(uiPolygon, uiVertex, kFbxNormal);
				kVertex.Normal.Set((float32)kFbxNormal[0],  (float32)kFbxNormal[1], (float32)kFbxNormal[2]);
				kVertex.Normal.Normalize();

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

				// BBox
				_pGeometry->BBox.Min.SetX(Math::Min(_pGeometry->BBox.Min.GetX(), kVertex.Position.GetX()));
				_pGeometry->BBox.Min.SetY(Math::Min(_pGeometry->BBox.Min.GetY(), kVertex.Position.GetY()));
				_pGeometry->BBox.Min.SetZ(Math::Min(_pGeometry->BBox.Min.GetZ(), kVertex.Position.GetZ()));
				_pGeometry->BBox.Max.SetX(Math::Max(_pGeometry->BBox.Max.GetX(), kVertex.Position.GetX()));
				_pGeometry->BBox.Max.SetY(Math::Max(_pGeometry->BBox.Max.GetY(), kVertex.Position.GetY()));
				_pGeometry->BBox.Max.SetZ(Math::Max(_pGeometry->BBox.Max.GetZ(), kVertex.Position.GetZ()));
				
				// Increment index for each vertex processed
				_pGeometry->Indices[uiVertexIndex] = uiVertexIndex;
				uiVertexIndex++;	
			}			
		}

		_pGeometry->Sections.Add(uiVertexIndex);
	}
}