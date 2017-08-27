/******************************************************************************
**	Includes
******************************************************************************/
#include "FbxCooker.hpp"
#include <Core/Stream/MemoryWriter.hpp>
#include <Core/Helper/Compression.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	struct Animation
	{
		uint32 FrameCount;
		String Name;
		Vector<String> Channels;
		Vector<Math::Matrix44> Frames;
	};

	Vector<Animation> Animations;

	//!	@brief		GenerateStructure
	//!	@date		2015-11-22
	void GenerateStructure(FbxNode* pNode, Animation* _pAnimationOut)
	{
		_pAnimationOut->Channels.Add(pNode->GetName());
		const int32 iChildCount = pNode->GetChildCount();
		for(int32 iChild = 0; iChild < iChildCount; ++iChild)
		{
			GenerateStructure(pNode->GetChild(iChild), _pAnimationOut);
		}
	}

	//!	@brief		GenerateFrame
	//!	@date		2015-11-22
	void GenerateFrame(FbxAnimLayer* _pFbxAnimLayer, FbxNode* pNode, uint32 _uiFrame,  Animation* _pAnimationOut)
	{
		FbxAnimCurve* pFbxAnimCurve = nullptr;

		// Translation
		Math::Vector3 vTranslation;
		pFbxAnimCurve = pNode->LclTranslation.GetCurve(_pFbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
        if (pFbxAnimCurve) vTranslation.SetX(pFbxAnimCurve->KeyGetValue(_uiFrame));
		pFbxAnimCurve = pNode->LclTranslation.GetCurve(_pFbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
        if (pFbxAnimCurve) vTranslation.SetY(pFbxAnimCurve->KeyGetValue(_uiFrame));
		pFbxAnimCurve = pNode->LclTranslation.GetCurve(_pFbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
        if (pFbxAnimCurve) vTranslation.SetZ(pFbxAnimCurve->KeyGetValue(_uiFrame));

		// Rotation
		Math::Vector3 vRotation;
		pFbxAnimCurve = pNode->LclRotation.GetCurve(_pFbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
        if (pFbxAnimCurve) vRotation.SetX(pFbxAnimCurve->KeyGetValue(_uiFrame));
		pFbxAnimCurve = pNode->LclRotation.GetCurve(_pFbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
        if (pFbxAnimCurve) vRotation.SetY(pFbxAnimCurve->KeyGetValue(_uiFrame));
		pFbxAnimCurve = pNode->LclRotation.GetCurve(_pFbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
        if (pFbxAnimCurve) vRotation.SetZ(pFbxAnimCurve->KeyGetValue(_uiFrame));

		// Scale
		Math::Vector3 vScale;
		pFbxAnimCurve = pNode->LclScaling.GetCurve(_pFbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
        if (pFbxAnimCurve) vScale.SetX(pFbxAnimCurve->KeyGetValue(_uiFrame));
		pFbxAnimCurve = pNode->LclScaling.GetCurve(_pFbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
        if (pFbxAnimCurve) vScale.SetY(pFbxAnimCurve->KeyGetValue(_uiFrame));
		pFbxAnimCurve = pNode->LclScaling.GetCurve(_pFbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
        if (pFbxAnimCurve) vScale.SetZ(pFbxAnimCurve->KeyGetValue(_uiFrame));

		// Add FrameValue
		Math::Matrix44& mValue = _pAnimationOut->Frames.Add();
		mValue.SetOrientation(vRotation);
		mValue.Scale(vScale);
		mValue.SetTranslation(vTranslation);

		// Child
		const int32 iChildCount = pNode->GetChildCount();
		for(int32 iChild = 0; iChild < iChildCount; ++iChild)
		{
			GenerateFrame(_pFbxAnimLayer, pNode->GetChild(iChild), _uiFrame, _pAnimationOut);
		}
	}


	//!	@brief		GenerateAnimation
	//!	@date		2015-11-22
	void GenerateAnimation(FbxScene* _pFbxScene)
	{
		FbxGlobalSettings& kFbxGlobalSettings = _pFbxScene->GetGlobalSettings();

		// Get User Root Node
		FbxNode* pFbxUserRootNode = _pFbxScene->GetRootNode()->GetChild(0);

		const int32 iAnimStackCount = _pFbxScene->GetSrcObjectCount<FbxAnimStack>();
		for (int32 iAnimStack = 0; iAnimStack < iAnimStackCount; ++iAnimStack)
		{
			FbxAnimStack* pFbxAnimStack = _pFbxScene->GetSrcObject<FbxAnimStack>(iAnimStack);

			// Choose Timeline (Specific in layers or global)
			FbxTimeSpan kFbxTimeSpan;
			FbxTakeInfo* pFbxTakeInfo = _pFbxScene->GetTakeInfo(pFbxAnimStack->GetName());
			if (pFbxTakeInfo) kFbxTimeSpan = pFbxTakeInfo->mLocalTimeSpan;
			else kFbxGlobalSettings.GetTimelineDefaultTimeSpan(kFbxTimeSpan);
		
			float64 fStartTime = kFbxTimeSpan.GetStart().GetSecondDouble();
			float64 fEndTime  = kFbxTimeSpan.GetStop().GetSecondDouble();

			FbxTime::EMode eFbxTimeMode = kFbxGlobalSettings.GetTimeMode();
			float64 fFrameRate = FbxTime::GetFrameRate(eFbxTimeMode);
			uint32 uiFrameCount = (uint32)((fEndTime - fStartTime) * fFrameRate) + 1;

			// Iterate all animations
			const int32 iAnimLayerCount = pFbxAnimStack->GetMemberCount<FbxAnimLayer>();
			for (int32 iAnimLayer = 0; iAnimLayer < iAnimLayerCount; ++iAnimLayer)
			{
				FbxAnimLayer* pFbxAnimLayer = pFbxAnimStack->GetMember<FbxAnimLayer>(iAnimLayer);

				// Create Animation
				Animation& kAnimation = Animations.Add();
				kAnimation.Name = pFbxAnimLayer->GetName();
				kAnimation.FrameCount = uiFrameCount;

				// Structure	
				GenerateStructure(pFbxUserRootNode, &kAnimation);

				// Data
				for (uint32 uiFrame = 0; uiFrame < uiFrameCount; ++uiFrame)
				{
					GenerateFrame(pFbxAnimLayer, pFbxUserRootNode, uiFrame, &kAnimation);
				}
			}
		}
	}

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
	
		// Iterate all anim stack
		GenerateAnimation(pFbxScene);

	    // Destroy the importer
		printf("Cleaning...\n");
	    pFbxImporter->Destroy();
		

		// Write it to a memory stream
		MemoryWriter kMemoryStream;
		const uint32 uiAnimationCount = Animations.GetSize();
		kMemoryStream.Write(uiAnimationCount);
		for (uint32 uiAnimation = 0; uiAnimation < uiAnimationCount; ++uiAnimation)
		{
			Animation& kAnimation = Animations[uiAnimation];
			kMemoryStream.Write(kAnimation.Name);

			// Channels
			const uint32 uiChannelCount = kAnimation.Channels.GetSize(); 
			kMemoryStream.Write(uiChannelCount);
			for (uint32 uiChannel = 0; uiChannel < uiChannelCount; ++uiChannel)
			{
				kMemoryStream.Write(kAnimation.Channels[uiChannel]);
			}
			
			// Frames
			kMemoryStream.Write(kAnimation.FrameCount);
			kMemoryStream.Write(&kAnimation.Frames[0], uiChannelCount * sizeof(Math::Matrix44) * kAnimation.FrameCount);
		}

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
}