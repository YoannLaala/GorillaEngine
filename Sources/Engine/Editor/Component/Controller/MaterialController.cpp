///******************************************************************************
//**	Includes
//******************************************************************************/
//#include "MaterialViewHandler.hpp"
//#include <Engine/Engine.hpp>
//#include <Engine/Asset/AssetManager.hpp>
//#include <Engine/Asset/Material/Material.hpp>
//#include <Engine/Database/AssetDescriptor.hpp>
//#include <Component/Gui/WebView.hpp>
//#include <Editor/Material/MaterialView.hpp>
//
///******************************************************************************
//**	Class Definition
//******************************************************************************/
//namespace Gorilla { namespace Editor 
//{	
//	COMPONENT_REGISTER(Gorilla::Editor::MaterialViewHandler)
//
//	//!	@brief		Constructor
//	//!	@date		2015-04-04
//	MaterialViewHandler::MaterialViewHandler()
//	: View(NULL)
//	{
//		// Nothing to do
//	}
//
//	//!	@brief		Destructor
//	//!	@date		2015-04-04
//	MaterialViewHandler::~MaterialViewHandler()
//	{
//		// Nothing to do
//	}
//
//	//bool EditorViewHandler::Serialize		(FileWriter& /*_kStream*/) { return true; };
//	//bool EditorViewHandler::Deserialize	(FileReader& /*_kStream*/) { return true; };
//
//	void OnCreateViewport(void* _pData, const Web::WebValueList* _pValueList, Web::WebValue* /*_pValueOut*/)
//	{
//		MaterialViewHandler* pHandler = static_cast<MaterialViewHandler*>(_pData);
//		
//		// Retrieve information
//		uint32 uiLeft, uiTop, uiWidth, uiHeight = 0;
//		_pValueList->Get(0)->Read(uiLeft);
//		_pValueList->Get(1)->Read(uiTop);
//		_pValueList->Get(2)->Read(uiWidth);
//		_pValueList->Get(3)->Read(uiHeight);
//
//		// Create the viewport
//		pHandler->View->CreateViewport(uiLeft, uiTop, uiWidth, uiHeight);
//	}
//
//	//!	@brief		OnFileSelected
//	//!	@date		2015-04-04
//	Web::WebObject* pEditorJS = NULL;
//	void OnFileSelected(void* _pData, const Vector<String>& _vFile)
//	{
//		Web::WebObject* pEditorJS = static_cast<Web::WebObject*>(_pData);
//		Web::WebValue* pValue = pEditorJS->CreateValue(_vFile[0].GetBuffer());
//		pEditorJS->ExecuteFunction("onOpenAsset", pValue);
//	}
//
//	//!	@brief		OnOpenAsset
//	//!	@date		2015-04-04
//	void OnOpenAsset(void* _pData, const Web::WebValueList* _pValueList, Web::WebValue* /*_pValueOut*/)
//	{
//		MaterialViewHandler* pHandler = static_cast<MaterialViewHandler*>(_pData);
//		
//		// Read Type
//		String sType;
//		_pValueList->Get(0)->Read(sType);
//		
//		// Find extension available for this asset
//		Class* pClass = COMPONENT_CLASS(sType.GetBuffer());
//		if(pClass)
//		{
//			// Create proper filter
//			const Engine::AssetDescriptor* pDescriptor = GetAssetManager()->GetDescriptor(pClass);
//
//			String sFilter;
//			const Vector<String>& vInput = pDescriptor->GetVecInput();
//			const uint32 uiInputCount = vInput.GetSize();
//			for(uint32 uiInput = 0; uiInput < uiInputCount; ++uiInput)
//			{
//				sFilter.Append('.').Append(vInput[uiInput]).Append(';');
//			}
//
//			// Build proper title
//			String sTitle(pClass->GetName());
//			uint32 uiPosition = sTitle.FindLast("::");
//			if(uiPosition != (uint32)-1)
//			{
//				uiPosition += 2;
//				sTitle.Set(&sTitle[uiPosition], sTitle.GetLength() - uiPosition);
//			}
//			sTitle.Prepend("Open ");
//			
//			// Open file dialog with title and specified extensions
//			Gorilla::Component::WebView* pView = pHandler->GetGameObject()->GetComponent<Gorilla::Component::WebView>();
//			pView->GetPage()->OpenFileDialog(OnFileSelected, pEditorJS, sTitle.GetBuffer(), sFilter.GetBuffer(), GetAssetManager()->GetPath().GetBuffer());
//		}
//	}
//
//	//!	@brief		OnUpdateShader
//	//!	@date		2015-04-04
//	void OnUpdateShader(void* /*_pData*/, const Web::WebValueList* _pValueList, Web::WebValue* /*_pValueOut*/)
//	{		
//		// Read shader
//		String sBuffer;
//		_pValueList->Get(0)->Read(sBuffer);
//
//		//GetAssetManager()->Create<Engine::Shader>();
//
//		// Write the shader to the disk
//		/*FileWriter kStream("F:/projects/GorillaEngine/Projects/MyGame/Asset/Effect/temp.ps");
//		kStream.Write(sBuffer.GetBuffer(), sBuffer.GetSize());
//		kStream.Close();*/
//	}
//
//	//!	@brief		OnUpdateShader
//	//!	@date		2015-04-04
//	void OnUpdateMaterial(void* _pData, const Web::WebValueList* _pValueList, Web::WebValue* /*_pValueOut*/)
//	{
//		MaterialViewHandler* pHandler = static_cast<MaterialViewHandler*>(_pData);
//		
//		// Create material if needed
//		Engine::GameObject* pGoGameObject = pHandler->GetGameObject()->GetWorld()->GetGameObject(2);
//		Component::Mesh* pCpnMesh = pGoGameObject->GetComponent<Component::Mesh>();
//		Engine::AssetHandle<Engine::Material>& hMaterial = pCpnMesh->Materials[0];		// --> Material not an asset
//		if(!hMaterial.IsLoaded())
//		{
//			Engine::AssetHandle<Engine::Shader> hShader = GetAssetManager()->Get<Engine::Shader>("Effect/temp.ps");
//			hMaterial = GetAssetManager()->Create<Engine::Material>();
//			hMaterial->SetShader(hShader);	
//		}
//		
//		// Read variables
//		float32 fValue = 0.0f;
//		const Web::WebValue* pNumberArray = _pValueList->Get(0);
//		const uint32 uiNumberCount = pNumberArray->GetSize();
//		for(uint32 uiNumber = 0; uiNumber < uiNumberCount; ++uiNumber)
//		{
//			pNumberArray->Get(uiNumber)->Read(fValue);
//			hMaterial->SetNumber(uiNumber, fValue);
//		}
//
//		// Read textures
//		/*float32 fValue = 0.0f;
//		const Web::WebValue* pNumberArray = _pValueList->Get(0);
//		uint32 uitest = _pValueList->GetSize();
//		uitest;
//		const uint32 uiNumberCount = pNumberArray->GetSize();
//		for(uint32 uiNumber = 0; uiNumber < uiNumberCount; ++uiNumber)
//		{
//			pNumberArray->Get(uiNumber)->Read(fValue);
//			hMaterial->SetNumber(uiNumber, fValue);
//		}*/
//	}
//
//	//!	@brief		Start
//	//!	@date		2015-04-04
//	void MaterialViewHandler::Start()
//	{
//		Gorilla::Component::WebView* pView = GetOrCreate<Gorilla::Component::WebView>();
//		pView->Url = "F:/projects/GorillaEngine/Projects/MyGame/Asset/GUI/Editor/material_view.html";
//
//		//SIGNAL_CONNECT(GetEngine()->GetWorld(), this, GameObjectCreated)
//
//		pEditorJS = pView->GetPage()->GetRoot()->CreateObject("Gorilla");
//		pEditorJS->CreateFunction("createViewport", &OnCreateViewport, this);
//		pEditorJS->CreateFunction("openAsset", &OnOpenAsset, this);
//		pEditorJS->CreateFunction("updateShader", &OnUpdateShader, this);
//		pEditorJS->CreateFunction("updateMaterial", &OnUpdateMaterial, this);
//		//pEditorJS->CreateFunction("CreateGameObject", &OnCreateGameObject, this);
//		//pView->CreateFunction("CreateGameObject", &OnCreateGameObject, this, pEditorJS);
//	}
//	
//
//	/*void EditorViewHandler::OnGameObjectCreated(Engine::GameObject* _pObject)
//	{
//		JsonWriter kStream;
//		JsonWriter* pResultDict = kWriter.CreateObject("result");
//		pResultDict->SetValue("parent", reinterpret_cast<uint64>(_pObject->GetParent()));
//		pResultDict->SetValue("id", reinterpret_cast<uint64>(_pObject));
//		pResultDict->SetValue("name", _pObject->GetName().GetBuffer());
//
//		kEditorJS.ExecuteFunction("OnGameObjectCreated", &kStream);
//	}*/
//
//	/*void EditorViewHandler::OnEvent(uint32 _uiEvent, const Dictionary* _pParameter)
//	{
//		GenerateEventId("OnGameObjectCreate")
//		switch(_uiEvent)
//		{
//			case EventId:
//			{
//				uint64 uiParentId = _pDictionary->GetMember("parent")->GetUint64();
//				GameObject* pParent = reinterpret_cast<GameObject*>(uiParentId);
//				GetWorld()->AddGameObject(pParent);
//			}
//
//			case OnGameObjectCreated:
//			{
//				uint64 uiObjectId = _pDictionary->GetMember("Value")->GetUint64();
//				GameObject* pGameObject = reinterpret_cast<GameObject*>(uiObjectId);
//
//				Dictionary* pResultDict = _pDictionary->CreateObject("result");
//				pResultDict->SetValue("parent", reinterpret_cast<uint64>(pObject->GetParent()));
//				pResultDict->SetValue("id", uiObjectId);
//				pResultDict->SetValue("name", pChild->GetName().GetBuffer());
//			}
//
//			case OnComponentCreate:
//			{
//				uint64 uiObjectId = _pDictionary->GetMember("id")->GetUint64();
//				uint32 uiComponentId = _pDictionary->GetMember("component")->GetUint32();
//				
//				GameObject* pGameObject = reinterpret_cast<GameObject*>(uiObjectId);
//
//				Dictionary* pResultDict = _pDictionary->CreateObject("result");
//				pResultDict->SetValue("parent", reinterpret_cast<uint64>(pObject->GetParent()));
//				pResultDict->SetValue("id", uiObjectId);
//				pResultDict->SetValue("name", pChild->GetName().GetBuffer());
//			}
//
//			case OnGameObjectSelect:
//			{
//				uint64 uiObjectId = _pDictionary->GetMember("id")->GetUint64();
//				GameObject* pGameObject = reinterpret_cast<GameObject*>(uiObjectId);
//			}
//		}
//	}*/
//
//	//!	@brief		Stop
//	//!	@date		2015-04-04
//	void MaterialViewHandler::Update()
//	{	
//		
//	}
//
//	//!	@brief		Stop
//	//!	@date		2015-04-04
//	void MaterialViewHandler::Stop()
//	{
//
//	}
//}}
