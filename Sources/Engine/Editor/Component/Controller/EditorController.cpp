/******************************************************************************
**	Includes
******************************************************************************/
#include "EditorController.hpp"
#include <Core/String/StringHelper.hpp>
#include <Core/Container/Dictionary.hpp>
#include <Core/File/FileManager.hpp>
#include <Core/Stream/MemoryWriter.hpp>
#include <Core/Stream/FileReader.hpp>
#include <Core/Stream/FileWriter.hpp>
#include <Core/Stream/Dictionary/DictionaryStreamJson.hpp>
#include <Core/Process/Process.hpp>
#include <Core/Time/Profiler.hpp>
#include <Core/Helper/Window.hpp>

// Engine
#include <Engine/Engine.hpp>
#include <Engine/Asset/AssetManager.hpp>
#include <Engine/Database/AssetDescriptor.hpp>
#include <Renderer/Renderer.hpp>
#include <Engine/Renderer/ResourceShared.hpp>
#include <Engine/Renderer/Pass/GuiPass.hpp>
#include <Engine/World.hpp>
#include <Engine/Object/GameObject.hpp>
#include <Engine/View.hpp>
#include <Renderer/Viewport.hpp>
#include <Renderer/RenderContext.hpp>

// Editor
#include <Editor/Renderer/Pass/EditorPass.hpp>

// Component
#include <Component/Node.hpp>
#include <Component/Camera/Camera.hpp>
#include <Component/Script/CameraFree.hpp>
#include <Component/Light/DirectionalLight.hpp>
#include <Component/Mesh/Mesh.hpp>
#include <Component/Gui/WebView.hpp>

/******************************************************************************
**	Define
******************************************************************************/
#define GORILLA_EDITOR_VERSION	"1.0.1-alpha"
#define GORILLA_EDITOR_TITLE	"Gorilla Editor"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Editor 
{	
	COMPONENT_REGISTER(Gorilla::Editor::EditorController)

	//!	@brief		Constructor
	//!	@date		2015-04-04
	EditorController::EditorController()
		: m_pSelection(nullptr)
		, m_pWorld(nullptr)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	EditorController::~EditorController()
	{
		// Nothing to do
	}

	void ConvertRecursive(const FileManager::Directory& _input, Node _kParent)
	{
		static String sDirectoryName;
		sDirectoryName.Set(_input.GetPath());
		sDirectoryName[sDirectoryName.GetLength()-1] = '\0';

		Node kNode = _kParent.Add();
		kNode["id"] =  _input.GetPath();
		kNode["name"] = &sDirectoryName[sDirectoryName.FindLast("\\") + 1];

		Node kChilds = kNode["childs"];
		const uint32 uiDirectoryCount = _input.GetDirectoryCount();
		for(uint32 uiDirectory = 0; uiDirectory < uiDirectoryCount; ++uiDirectory)
		{
			ConvertRecursive(_input.GetDirectory(uiDirectory), kChilds);
		}

		static String sFilePath;
		const uint32 uiFileCount = _input.GetFileCount();
		for(uint32 uiFile = 0; uiFile < uiFileCount; ++uiFile)
		{
			const String& sFileName = _input.GetFile(uiFile);
			sFilePath.Set(_input.GetPath()).Append(sFileName);

			Node kFile = kChilds.Add();
			kFile["id"] = sFilePath;
			kFile["name"] = sFileName;
		}		
	}

	Gorilla::Component::WebView* pWebView = nullptr;
	void EditorController::OnLogChanged(ELog::Type _eLog, const char* _szMessage)
	{
		static Vector<String> vLine; vLine.Clear();

		// Check each line
		StringHelper::Split(_szMessage, "\n", vLine);
		const uint32 uiElementCount = vLine.GetSize();
		switch(_eLog)
		{
			case ELog::Normal:
			{
				for(uint32 uiElement = 0; uiElement < uiElementCount; ++uiElement) pWebView->GetPage()->ExecuteFunction("Editor.panels.console.onPrint", vLine[uiElement].GetBuffer());
				break;
			}

			case ELog::Warning:
			{
				for(uint32 uiElement = 0; uiElement < uiElementCount; ++uiElement) pWebView->GetPage()->ExecuteFunction("Editor.panels.console.onWarning", vLine[uiElement].GetBuffer());
				break;
			}

			case ELog::Error:
			{
				for(uint32 uiElement = 0; uiElement < uiElementCount; ++uiElement) pWebView->GetPage()->ExecuteFunction("Editor.panels.console.onError", vLine[uiElement].GetBuffer());
				break;
			}
		}
	}

	Dictionary& GetDictionary()
	{
		static Dictionary dValue; dValue.Clear();
		return dValue;
	}

	const char* JsonToString(const Dictionary& _dValue)
	{
		static MemoryWriter kWriter; kWriter.Seek(0);
		const_cast<Dictionary&>(_dValue).Write<DictionaryStreamJson>(&kWriter);
		kWriter.Write('\0');

		return (const char*)&kWriter[0];
	}

	void SendJson(const char* _szFunctionName, const char* _szJson)
	{
		pWebView->GetPage()->ExecuteFunction(_szFunctionName, _szJson);
	}

	void SendJson(const char* _szFunctionName, const Dictionary& _dDictionary)
	{
		SendJson(_szFunctionName, JsonToString(_dDictionary));
	}

	void SendNotification(const char* _szType, const char* _szMessage)
	{
		Dictionary& dNotification = GetDictionary();
		dNotification["type"] =  _szType;
		dNotification["message"] = _szMessage;
		SendJson("Editor.onNotification", dNotification);
	}

	void SendWorldTree(Engine::World* _pWorld)
	{
		Dictionary& dWorld = GetDictionary();
		const uint32 uiGameObjectCount = _pWorld->GetGameObjectCount();
		for(uint32 uiGameObject = 0; uiGameObject < uiGameObjectCount; ++uiGameObject)
		{
			Engine::GameObject* pGameObject = _pWorld->GetGameObject(uiGameObject);
			if(pGameObject->GetName()[0] == '@') continue; // Avoid intrinsic GameObject

			Engine::GameObject* pParent = pGameObject->GetParent();
			Node kGameObject = dWorld.Add();
			kGameObject["id"] = (uint64)pGameObject;
			kGameObject["text"] = pGameObject->GetName();
			if(pParent) kGameObject["parent"] = (uint64)pParent;
			else kGameObject["parent"] = "#";
		}	
		SendJson("Editor.panels.world.onChanged", dWorld);
	}		

	//!	@brief		SelectGameObject
	//!	@date		2015-04-04
	void EditorController::SelectGameObject(Engine::GameObject* _pGameObject)
	{
		m_pSelection = nullptr;
		if(_pGameObject)
		{
			m_pSelection = _pGameObject;

			// Retrieve all component on the GameObject
			Dictionary& dAllComponent = GetDictionary();
			m_pSelection->Serialize(dAllComponent);
			SendJson("Editor.panels.property.onSelectionChanged", dAllComponent);
		}
	}

	//!	@brief		OnAssetChanged
	//!	@date		2015-04-04
	void EditorController::OnAssetChanged(uint8 _eEvent, Engine::Asset* _pAsset, bool _bSucceeded)
	{	
		static String sMessage;
		switch(_eEvent)
		{
			case Engine::AssetManager::EEvent::CookingStarted:
			{
				sMessage.Set("Cooking ").Append(_pAsset->GetClass()->GetName()).Append(' ').Append(_pAsset->GetName());
				SendNotification("info", sMessage.GetBuffer());				
				if(_pAsset->GetClass() == Engine::Module::Class::GetInstance())
				{
					m_pWorld->Pause();
				}
				break;
			}

			case Engine::AssetManager::EEvent::CookingFinished:
			{
				sMessage.Set(_pAsset->GetClass()->GetName()).Append(' ').Append(_pAsset->GetName());
				SendNotification(_bSucceeded ? "success" : "danger", sMessage.GetBuffer());
				break;
			}

			case Engine::AssetManager::EEvent::LoadingFinished:
			{
				// Module Changed
				if(_pAsset->GetClass() == Engine::Module::Class::GetInstance())
				{
					m_pWorld->Play();
					SendJson("Editor.panels.property.onModuleChanged", GetEngine()->GetAllComponentDescriptor());
					if(m_pSelection && m_sScript.GetLength())
					{
						Gorilla::Class* pClass = COMPONENT_CLASS(m_sScript.GetBuffer());
						m_pSelection->AddComponent(pClass);
						SelectGameObject(m_pSelection);
					}
				}
				break;
			}
		}
	}

	EditorPass* pEditorPass = nullptr;

	//!	@brief		Start
	//!	@date		2015-04-04
	void EditorController::Start()
	{
		Renderer::Viewport* pViewport = View->GetViewport();
		pEditorPass = pViewport->GetRenderContext()->CreatePassBefore<EditorPass>(Engine::GuiPass::Class::GetType());
		pEditorPass->SetRenderTarget(pViewport->GetRenderTarget());

		pWebView = GetOrCreate<Gorilla::Component::WebView>();
		pWebView->Document = GetAssetManager()->Get<Gorilla::Engine::WebDocument>("@Gui/Editor/_game_view_.html");
		pWebView->Transparent = true;

		// Load current project
		LoadProject();
		Engine::World* pWorld = m_pWorld;
		Web::WebPage* pPage = pWebView->GetPage();

		SIGNAL_CONNECT(GetLogManager(), this, LogChanged);
		SIGNAL_CONNECT(GetAssetManager(), this, AssetChanged);

		pPage->CreateCallback("viewLoaded", [this, pPage, pWorld](const Web::WebArgument& /*_vArgument*/, Web::WebValueList& /*_vOutput*/)
		{
			// Select / Create a Project
			if(GetAssetManager()->GetPath().IsEmpty())
			{
				pPage->ExecuteFunction2("Editor.selectProject", true, false);
				this->View->Show();
				return;
			}

			// Retrieve file structure
			FileManager::Directory kDirectory(GetAssetManager()->GetPath().GetBuffer());
			FileManager::GetTree(kDirectory, true);

			// Convert to Json
			Dictionary& dTree = GetDictionary();
			const uint32 uiDirectoryCount = kDirectory.GetDirectoryCount();
			for(uint32 uiDirectory = 0; uiDirectory < uiDirectoryCount; ++uiDirectory)
			{
				Node dDirectory = dTree.Add();
				ConvertRecursive(kDirectory.GetDirectory(uiDirectory), dDirectory);
			}

			// Notify the workspace only if a project has been set
			SendJson("Editor.panels.workspace.onChanged", dTree);

			// World
			SendWorldTree(m_pWorld);

			// Component
			SendJson("Editor.panels.property.onModuleChanged", GetEngine()->GetAllComponentDescriptor());
			pWorld->Stop();

			this->View->Show();
		});

		pPage->CreateCallback("Gorilla.Project.getName", [this](const Web::WebArgument& /*_vArgument*/, Web::WebValueList& _vOutput)
		{
			const char* szProjectName = this->GetConfig(Config::Project)["Name"];
			_vOutput.SetString(0, szProjectName);
		}, true);

		// Project
		pPage->CreateCallback("Gorilla.Project.create", [this](const Web::WebArgument& _vArgument, Web::WebValueList& /*_vOutput*/)
		{
			bool bResult = false;

			const String& sProjectName = _vArgument.GetString(0);
			const String& sProjecPath = _vArgument.GetString(1);
			if(sProjectName.GetLength() && sProjecPath.GetLength())
			{
				Dictionary& dProject = GetDictionary();

				// Create project
				String sProject;
				sProject.Append(sProjecPath).Append(sProjectName).Append(REPERTORY_SEPARATOR).Append(sProjectName).Append(".gorilla");
				dProject["Name"] = sProjectName;
				if(!dProject.Write<DictionaryStreamJson>(sProject.GetBuffer())) 
				{
					bResult = false;
					return;
				}

				bResult = this->LoadProject(sProject.GetBuffer());
			}

			//_vOutput.SetBool(0, bResult);
		});

		pPage->CreateCallback("Gorilla.Project.open", [this](const Web::WebArgument& _vArgument, Web::WebValueList& /*_vOutput*/)
		{
			const String& sProjecPath = _vArgument.GetString(0);
			this->LoadProject(sProjecPath.GetBuffer());
		});

		pPage->CreateCallback("Gorilla.Project.build", [this](const Web::WebArgument& _vArgument, Web::WebValueList& /*_vOutput*/)
		{
			const String& sBuild = _vArgument.GetString(0);
			FileManager::DeleteADirectory(sBuild.GetBuffer());

			// Generic Path
			String sDirectory, sSource, sDestination;
			FileManager::GetDirectory(FileManager::Directory::Executable, sDirectory);

			// Cef Dependencies 
			Vector<String> vFile;		
			FileManager::GetAllFiles(sDirectory.GetBuffer(), vFile, false, "dll;dat;bin;pak");
			sSource.Set(sDirectory).Append("Resources\\");
			FileManager::GetAllFiles(sSource.GetBuffer(), vFile, true, "dll;dat;bin;pak");

			uint32 uiElementCount = vFile.GetSize();
			for(uint32 uiElement = 0; uiElement < uiElementCount; ++uiElement)
			{
				String& sFileSource = vFile[uiElement];
				sDestination.Set(sFileSource);
				sDestination.Replace(sDirectory.GetBuffer(), sBuild.GetBuffer());
				FileManager::CreateADirectory(sDestination.GetBuffer());
				FileManager::CopyAFile(sFileSource.GetBuffer(), sDestination.GetBuffer());
			}

			// HACK
			vFile.Clear();	
			FileManager::GetAllFiles(GetAssetManager()->GetPath().GetBuffer(), vFile, true, "world");
			uiElementCount = vFile.GetSize();
			for(uint32 uiElement = 0; uiElement < uiElementCount; ++uiElement)
			{
				String& sAssetSource = vFile[uiElement];
				GetAssetManager()->FormatToRelative(sAssetSource);
				Engine::AssetHandle<Engine::World> hWorld = GetAssetManager()->Get<Engine::World>(sAssetSource.GetBuffer());
				while(hWorld.IsLoading()) { Thread::Sleep(100); }
			}
			// HACK

			// Asset cooked path
			String sCookedOld, sCookedNew;
			sCookedOld.Set(GetAssetManager()->GetPath()).Append("..\\Cooked\\");
			sCookedNew.Set(sBuild).Append("Resources\\Cooked\\");

			// Copy user Asset
			vFile.Clear();
			FileManager::GetAllFiles(sCookedOld.GetBuffer(), vFile, true);
			uiElementCount = vFile.GetSize();
			for(uint32 uiElement = 0; uiElement < uiElementCount; ++uiElement)
			{
				String& sAssetSource = vFile[uiElement];
				sDestination.Set(sAssetSource);
				sDestination.Replace(sCookedOld.GetBuffer(), sCookedNew.GetBuffer());
				FileManager::CreateADirectory(sDestination.GetBuffer());
				FileManager::CopyAFile(sAssetSource.GetBuffer(), sDestination.GetBuffer());
			}

			// Search all intrinsic assets
			static const uint32 uiDirectoryCount = 2;
			const char* aDirectory[uiDirectoryCount] = 
			{
				"Effect",
				"Mesh",
			};

			vFile.Clear();
			sCookedOld.Set(sDirectory).Append("Resources\\Asset\\");
			for(uint32 uiElement = 0; uiElement < uiDirectoryCount; ++uiElement)
			{
				sSource.Set(sCookedOld).Append(aDirectory[uiElement]).Append(REPERTORY_SEPARATOR);
				FileManager::GetAllFiles(sSource.GetBuffer(), vFile, true);
			}

			// Copy Intrinsic Asset
			uiElementCount = vFile.GetSize();
			for(uint32 uiElement = 0; uiElement < uiElementCount; ++uiElement)
			{
				String& sAssetSource = vFile[uiElement];
				sDestination.Set(sAssetSource);
				sDestination.Replace(sCookedOld.GetBuffer(), sCookedNew.GetBuffer());
				FileManager::CreateADirectory(sDestination.GetBuffer());
				FileManager::CopyAFile(sAssetSource.GetBuffer(), sDestination.GetBuffer());
			}

			// Copy Component Descriptor
			sDestination.Set(sBuild).Append("Component.bin");
			if(!GetEngine()->GetAllComponentDescriptor().Write<DictionaryStream>(sDestination.GetBuffer()))
			{
				LOG_ERROR("Failed to write project configration file %s", sDestination.GetBuffer());
			}

			// Copy Project config
			Dictionary& dProject = this->GetConfig(Config::Project);
			sDestination.Set(sBuild).Append("Core.bin");
			if(!dProject.Write<DictionaryStream>(sDestination.GetBuffer()))
			{
				LOG_ERROR("Failed to write project configration file %s", sDestination.GetBuffer());
			}

			// Copy Executable
			sSource.Set(sDirectory).Append("Game64.exe");
			sDestination.Set(sBuild).Append(dProject["Name"].GetString()).Append(".exe");
			FileManager::CopyAFile(sSource.GetBuffer(), sDestination.GetBuffer());
		});

		// Engine
		pPage->CreateCallback("Gorilla.createViewport", [this](const Web::WebArgument& _vArgument, Web::WebValueList& /*_vOutput*/)
		{
			uint32 uiLeft = _vArgument.GetUInt32(0);
			uint32 uiTop = _vArgument.GetUInt32(1);
			uint32 uiWidth = _vArgument.GetUInt32(2);
			uint32 uiHeight = _vArgument.GetUInt32(3);
			this->CreateViewport(uiLeft, uiTop, uiWidth, uiHeight);
		});

		pPage->CreateCallback("Gorilla.openScript", [this](const Web::WebArgument& /*_vArgument*/, Web::WebValueList& /*_vOutput*/)
		{
			//const String& sScript = _vArgument.GetString(0);
			//const char* szProjectName = GetConfig(Config::Project)["Name"].GetString();

			String sArgument;
			//sArgument.Set("\"").Append(GetAssetManager()->GetPath()).Append("..\\").Append(szProjectName).Append(".sln\" /command \"File.OpenFile ").Append(sScript).Append("\"");
			sArgument.Set("/c start \"C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\Common7\\IDE\\devenv.exe\" \"").Append(GetAssetManager()->GetPath()).Append("..\\Script.sln\"");
			Process kProcess("cmd", sArgument.GetBuffer());
			kProcess.Execute();
		}, true);

		pPage->CreateCallback("Gorilla.createScript", [this](const Web::WebArgument& _vArgument, Web::WebValueList& /*_vOutput*/)
		{
			static const char* szName = "Script";
			static const char* aExtension[2] = { ".hpp", ".cpp" };

			const String& sRelativePath = _vArgument.GetString(0);
			String sAbsolutePath(sRelativePath);
			GetAssetManager()->FormatToAbsolute(sAbsolutePath);

			// Retirve the tree from the path
			Vector<String> vFile;
			FileManager::GetAllFiles(sAbsolutePath.GetBuffer(), vFile, false, "hpp");
			
			// Finalize the list name and check if the current filename is valid
			uint32 uiInstance = 0;
			String sFileNameNew(szName);
			const uint32 uiNameCount = vFile.GetSize();
			for(uint32 uiName = 0; uiName < uiNameCount; ++uiName)
			{
				String& sFileName = vFile[uiName];
				sFileName.Remove(sAbsolutePath.GetBuffer()).Remove(aExtension[0]);
				if(sFileName == sFileNameNew)
				{
					sFileNameNew.Set(szName).Append(++uiInstance);
				}
			}
		
			String sInput, sOutput, sDirectory, sContent;
			FileManager::GetDirectory(FileManager::Directory::Executable, sDirectory);

			// Copy cpp & hpp files
			FileReader kReader;
			FileWriter kWriter;
			for(uint32 uiFile = 0; uiFile < 2; ++uiFile)
			{
				// Read input file
				sInput.Set(sDirectory).Append("Resources\\Template\\Script\\").Append(szName).Append(aExtension[uiFile]);
				if(!kReader.Open(sInput.GetBuffer()))
				{
					LOG_ERROR("Failed to read script template %s", sInput.GetBuffer());
					return;
				}
				uint32 uiFileSize = kReader.GetSize();
				sContent.Resize(uiFileSize);
				kReader.Read(&sContent[0], uiFileSize);
				kReader.Close();
				sContent.Replace("${NAME}", sFileNameNew.GetBuffer());

				// Write ouput file
				sOutput.Set(GetAssetManager()->GetPath()).Append(sFileNameNew).Append(aExtension[uiFile]);
				if(!kWriter.Open(sOutput.GetBuffer()))
				{
					LOG_ERROR("Failed to write script %s", sOutput.GetBuffer());
					return;
				}
				kWriter.Write(sContent.GetBuffer(), sContent.GetLength());
				kWriter.Close();
			}
			GetEngine()->LoadDescriptor();










			//// Check filename
			//const String& sFileName = _vArgument.GetString(0);
			//if(sFileName.IsEmpty())
			//{
			//	LOG_ERROR("err");
			//	return;
			//}
			//
			//// Check output file
			//String sOutput;
			//sOutput.Set(GetAssetManager()->GetPath()).Append(sFileName).Append(".hpp");
			//if(FileManager::IsFileExist(sOutput.GetBuffer()))
			//{
			//	LOG_ERROR("err");
			//	return;
			//}

			//String sDirectory, sInput, sContent;
			//FileManager::GetDirectory(FileManager::Directory::Executable, sDirectory);

			//// Copy cpp & hpp files
			//FileReader kReader;
			//FileWriter kWriter;
			//for(uint32 uiFile = 0; uiFile < 2; ++uiFile)
			//{
			//	// Read input file
			//	sInput.Set(sDirectory).Append("Resources\\Template\\Script\\").Append(aTemplate[uiFile]);
			//	if(!kReader.Open(sInput.GetBuffer()))
			//	{
			//		LOG_ERROR("err");
			//		return;
			//	}
			//	uint32 uiFileSize = kReader.GetSize();
			//	sContent.Resize(uiFileSize);
			//	kReader.Read(&sContent[0], uiFileSize);
			//	kReader.Close();
			//	sContent.Replace("${NAME}", sFileName.GetBuffer());

			//	// Write ouput file
			//	const char* szExtension = &aTemplate[uiFile][6];
			//	sOutput.Set(GetAssetManager()->GetPath()).Append(sFileName).Append(szExtension);
			//	if(!kWriter.Open(sOutput.GetBuffer()))
			//	{
			//		LOG_ERROR("err");
			//		return;
			//	}
			//	kWriter.Write(sContent.GetBuffer(), sContent.GetLength());
			//	kWriter.Close();
			//}
			//RefreshModule();

			//this->m_sScript.Set("Gorilla::Component::").Append(sFileName);
		});

		pPage->CreateCallback("Gorilla.play", [pWorld](const Web::WebArgument& /*_vArgument*/, Web::WebValueList& /*_vOutput*/)
		{
			pWorld->Play();
		});

		pPage->CreateCallback("Gorilla.pause", [pWorld](const Web::WebArgument& /*_vArgument*/, Web::WebValueList& /*_vOutput*/)
		{
			pWorld->Pause();
		});

		pPage->CreateCallback("Gorilla.stop", [pWorld](const Web::WebArgument& /*_vArgument*/, Web::WebValueList& /*_vOutput*/)
		{
			pWorld->Stop();
		});

		pPage->CreateCallback("Gorilla.exit", [](const Web::WebArgument& /*_vArgument*/, Web::WebValueList& /*_vOutput*/)
		{
			exit(0);
		});

		pPage->CreateCallback("Gorilla.getAssetExtension", [pWorld](const Web::WebArgument& _vArgument, Web::WebValueList& _vOutput)
		{
			const String& sAssetType = _vArgument.GetString(0);

			String sResult;
			const Engine::AssetDescriptor* pDescriptor = GetAssetManager()->GetDescriptor(sAssetType.GetBuffer());
			if(pDescriptor)
			{
				const Vector<String>& vInput = pDescriptor->GetVecInput();
				const uint32 uiInputCount = vInput.GetSize();
				for(uint32 uiInput = 0; uiInput < uiInputCount; ++uiInput)
				{
					sResult.Append('.').Append(vInput[uiInput]).Append(';');
				}
				sResult.Resize(sResult.GetLength()-1);
			}
			_vOutput.SetString(0, sResult.GetBuffer());
		}, true);

		pPage->CreateCallback("Gorilla.World.create", [this](const Web::WebArgument& /*_vArgument*/, Web::WebValueList& /*_vOutput*/)
		{
			this->LoadWorld(nullptr);
		});

		pPage->CreateCallback("Gorilla.World.open", [this, pPage](const Web::WebArgument& /*_vArgument*/, Web::WebValueList& /*_vOutput*/)
		{
			pPage->OpenFileDialog(Web::WebPage::EDialog::File, [this](const Vector<String>& _vFile)
			{
				if(_vFile.GetSize()) this->LoadWorld(_vFile[0].GetBuffer());
			}, nullptr, ".world", GetAssetManager()->GetPath().GetBuffer());
		});

		pPage->CreateCallback("Gorilla.World.save", [this, pPage, pWorld](const Web::WebArgument& _vArgument, Web::WebValueList& /*_vOutput*/)
		{
			bool bSaveAs = _vArgument.GetBool(0);
			const char* szWorld = GetConfig(Config::Project)["World"];
			if(*szWorld == '\0') bSaveAs = true;

			if(!bSaveAs)
			{
				this->SaveWorld(szWorld);
			}
			else
			{
				// Save As
				pPage->OpenFileDialog(Web::WebPage::EDialog::Save, [this](const Vector<String>& _vFile)
				{
					if(_vFile.GetSize()) this->SaveWorld(_vFile[0].GetBuffer());
				}, nullptr, ".world", GetAssetManager()->GetPath().GetBuffer());
			}
		});

		// GameObject Manager
		pPage->CreateCallback("Gorilla.GameObject.create", [this, pPage, pWorld](const Web::WebArgument& _vArgument, Web::WebValueList& /*_vOutput*/)
		{
			uint64 uiParentId = _vArgument.GetUInt64(0);
			Engine::GameObject* pParent = reinterpret_cast<Engine::GameObject*>(uiParentId);
			Engine::GameObject* pGameObject = pWorld->AddGameObject(nullptr, pParent);
			
			Dictionary& dValue = GetDictionary();
			dValue["id"] = (uint64)pGameObject;
			dValue["text"] = pGameObject->GetName();
			if(pParent) dValue["parent"] = (uint64)pParent;
			else dValue["parent"] = "#";	
			SendJson("Editor.panels.world.onGameObjectCreated", dValue);
		});

		pPage->CreateCallback("Gorilla.GameObject.destroy", [pWorld](const Web::WebArgument& _vArgument, Web::WebValueList& /*_vOutput*/)
		{
			uint64 uiGameObjectId = _vArgument.GetUInt64(0);
			Engine::GameObject* pGameObject = reinterpret_cast<Engine::GameObject*>(uiGameObjectId);
			pWorld->RemoveGameObject(pGameObject);
		});

		pPage->CreateCallback("Gorilla.GameObject.rename", [](const Web::WebArgument& _vArgument, Web::WebValueList& /*_vOutput*/)
		{
			uint64 uiGameObjectId = _vArgument.GetUInt64(0);
			const String& sName = _vArgument.GetString(1);
			Engine::GameObject* pGameObject = reinterpret_cast<Engine::GameObject*>(uiGameObjectId);
			pGameObject->SetName(sName.GetBuffer());
		});

		pPage->CreateCallback("Gorilla.GameObject.select", [this, pPage](const Web::WebArgument& _vArgument, Web::WebValueList& /*_vOutput*/)
		{
			uint64 uiGameObjectId = _vArgument.GetUInt64(0);

			// Send the GameObject info
			Engine::GameObject* pGameObject = reinterpret_cast<Engine::GameObject*>(uiGameObjectId);
			SelectGameObject(pGameObject);
		});

		// Component
		pPage->CreateCallback("Gorilla.Component.add", [this, pPage](const Web::WebArgument& _vArgument, Web::WebValueList& /*_vOutput*/)
		{
			uint64 uiGameObjectId = _vArgument.GetUInt64(0);
			const String& sComponentName = _vArgument.GetString(1);

			// Create the Component
			Engine::GameObject* pGameObject = reinterpret_cast<Engine::GameObject*>(uiGameObjectId);
			Gorilla::Class* pClass = COMPONENT_CLASS(sComponentName.GetBuffer());
			if(pGameObject && pClass)
			{
				pGameObject->AddComponent(pClass);
			}
		});

		pPage->CreateCallback("Gorilla.Component.remove", [this, pPage](const Web::WebArgument& _vArgument, Web::WebValueList& /*_vOutput*/)
		{
			uint64 uiGameObjectId = _vArgument.GetUInt64(0);
			const String& sComponentName = _vArgument.GetString(1);

			// Create the Component
			Engine::GameObject* pGameObject = reinterpret_cast<Engine::GameObject*>(uiGameObjectId);
			Gorilla::Class* pClass = COMPONENT_CLASS(sComponentName.GetBuffer());
			if(pGameObject && pClass)
			{
				pGameObject->AddComponent(pClass);
			}
		});

		pPage->CreateCallback("Gorilla.Component.set", [this, pPage](const Web::WebArgument& _vArgument, Web::WebValueList& /*_vOutput*/)
		{
			enum class JS_TYPE
			{
				Bool	= 0,
				UInt8	= 1,
				UInt16	= 2,
				UInt32	= 3,
				UInt64	= 4,
				Int8	= 5,
				Int16	= 6,
				Int32	= 7,
				Int64	= 8,
				Float32	= 9,
				Float64	= 10,
				String	= 11,
				Asset	= 12
			};
			
			const String& sComponentClass = _vArgument.GetString(0);
			Gorilla::Class* pComponentClass = COMPONENT_CLASS(sComponentClass.GetBuffer());
			if(this->m_pSelection && pComponentClass)
			{
				Component* pComponent = this->m_pSelection->GetComponent(pComponentClass);
				if(pComponent)
				{
					uint32 uiOffset = _vArgument.GetUInt32(1);
					JS_TYPE eType = static_cast<JS_TYPE>(_vArgument.GetUInt32(2));
			
					// Apply value
					uint8* pBuffer = reinterpret_cast<uint8*>(pComponent) + uiOffset;
					switch(eType)
					{
						case JS_TYPE::Float32:
						{
							*reinterpret_cast<float32*>(pBuffer) = _vArgument.GetFloat32(3);
							break;
						}

						case JS_TYPE::Asset:
						{
							// Retreieve value composed as [ASSET_TYPE@ABSOLUTE_PPATH]
							const String& sTypeAndPath = _vArgument.GetString(3);
							uint32 uiPosition = sTypeAndPath.Find("@");

							// Extract values
							String sType(sTypeAndPath.GetBuffer(), uiPosition);
							String sRelativePath(&sTypeAndPath[++uiPosition]);
							GetAssetManager()->FormatToRelative(sRelativePath);

							// Load Asset
							Gorilla::Class* pClass = ASSET_CLASS(sType.GetBuffer());
							Engine::AssetHandle<Engine::Asset> hAsset = GetAssetManager()->Get(pClass, sRelativePath.GetBuffer());
							*reinterpret_cast<Engine::AssetHandle<Engine::Asset>*>(pBuffer) = hAsset;
							break;
						}
					}	
				}
			}
		});

		//// FileManager
		//pPage->CreateCallback("Gorilla.File.openDirectory", [](const Web::WebArgument& /*_vArgument*/, Web::WebValueList& /*_vOutput*/)
		//{
		//	String sArg;
		//	sArg.Set("\"").Append(GetAssetManager()->GetPath()).Append("\"");
		//	Process kProcess("explorer.exe", sArg.GetBuffer());
		//	String sError;
		//	kProcess.Execute(&sError);
		//});

		//pPage->CreateCallback("Gorilla.File.createDirectory", [](const Web::WebArgument& _vArgument, Web::WebValueList& /*_vOutput*/)
		//{
		//	FileManager::CreateADirectory(_vArgument[0]->GetString().GetBuffer());
		//});

		//pPage->CreateCallback("Gorilla.File.rename", [](const Web::WebArgument& _vArgument, Web::WebValueList& /*_vOutput*/)
		//{
		//	FileManager::CreateADirectory(_vArgument[0]->GetString().GetBuffer());
		//});

		pPage->CreateCallback("Gorilla.File.getPath", [](const Web::WebArgument& _vArgument, Web::WebValueList& _vOutput)
		{
			enum class Path
			{
				Document = 0,
				Asset,
			};
			Path ePath = static_cast<Path>(_vArgument.GetUInt32(0));

			String sResult;
			switch(ePath)
			{
				case Path::Document: FileManager::GetDirectory(FileManager::Directory::User, sResult); break;
				case Path::Asset: sResult = GetAssetManager()->GetPath(); break;
			}
			_vOutput.SetString(0, sResult.GetBuffer());			
		}, true);

		pPage->CreateCallback("Gorilla.File.getDefaultFileName", [](const Web::WebArgument& _vArgument, Web::WebValueList& _vOutput)
		{
			String sFileNameNew;
			const String& sFileName = _vArgument.GetString(0);
			String sExtension = _vArgument.GetString(1);

			// Retirve the tree from the path
			Vector<String> vList;
			FileManager::GetAllFiles(GetAssetManager()->GetPath().GetBuffer(), vList, false, sExtension.GetBuffer());
			sExtension.Prepend('.');
			
			// Finalize the list name and check if the current filename is valid
			uint32 uiInstance = 0;
			sFileNameNew.Set(sFileName);
			const uint32 uiNameCount = vList.GetSize();
			for(uint32 uiName = 0; uiName < uiNameCount; ++uiName)
			{
				String& sName = vList[uiName];
				sName.Remove(GetAssetManager()->GetPath().GetBuffer());
				sName.Remove(sExtension.GetBuffer());
				if(sName == sFileNameNew)
				{
					++uiInstance;
					sFileNameNew.Set(sFileName).Append(uiInstance);
				}
			}

			// Write to json 
			Dictionary& dResult = GetDictionary();
			dResult["name"] = sFileNameNew;
			dResult["list"] = vList;
			_vOutput.SetString(0, JsonToString(dResult));
		}, true);

		// Dialog
		pPage->CreateCallback("Gorilla.openDialog", [pPage](const Web::WebArgument& _vArgument, Web::WebValueList& /*_vOutput*/)
		{
			uint32 eMode = _vArgument.GetUInt32(0);
			if(eMode < Web::WebPage::EDialog::Count)
			{
				void* pCallback = _vArgument.GetCallback(1);
				const String& sExtension = _vArgument.GetString(2);
				const String& sDefaultPath = _vArgument.GetString(3);
				pPage->OpenFileDialog((Web::WebPage::EDialog::Type)eMode, [pPage, pCallback](const Vector<String>& _vFile)
				{
					pPage->ExecuteCallback(pCallback, _vFile);
				}, nullptr, sExtension.GetBuffer(), sDefaultPath.GetBuffer());
			}
		}, true);
	}

	//!	@brief		Update
	//!	@date		2015-04-04
	
	void EditorController::Update()
	{	
		/*static uint32 uiFrame = 0;
		Dictionary& dValue = Profiler::GetInstance()->GetValue();
		if(dValue.Get("fps").IsValid())
		{
			SendJson("Editor.panels.profiler.onChanged", dValue);
		}*/	
	}

	//!	@brief		Stop
	//!	@date		2015-04-04
	void EditorController::Stop()
	{

	}

	//!	@brief		CreateViewport
	//!	@date		2015-04-08
	uint32 EditorController::CreateViewport(uint32 _uiLeft, uint32 _uiTop, uint32 _uiWidth, uint32 _uiHeight)
	{
		// Create new Viewport
		Renderer::Viewport* pViewport = GetRenderer()->CreateViewport(_uiWidth, _uiHeight, Renderer::EFormat::R8G8B8A8_UNORM);	
		pViewport->SetPosition(_uiLeft, _uiTop);
		m_vViewport.Add(pViewport);
		pEditorPass->AddViewport(pViewport);

		// Update Viewport on Camera
		const uint32 uiLastIndex = m_vViewport.GetSize() - 1;
		Engine::GameObject* pGoEditor = m_pWorld->GetGameObject(0);
		Gorilla::Component::Camera* pCpnCamera = pGoEditor->GetComponent<Gorilla::Component::Camera>();
		pCpnCamera->Viewport = m_vViewport[uiLastIndex];
		
		return uiLastIndex;
	}

	//!	@brief		RefreshModule
	//!	@date		2015-04-04
	void EditorController::RefreshModule()
	{
		// Retrieve all files with proper extension and make them relative
		Vector<String> vFile;
		FileManager::GetAllFiles(GetAssetManager()->GetPath().GetBuffer(), vFile, true, "hpp;cpp");
		const uint32 uiFileCount = vFile.GetSize();
		if(uiFileCount)
		{
			for(uint32 uiFile = 0; uiFile < uiFileCount; ++uiFile) GetAssetManager()->FormatToRelative(vFile[uiFile]);

			// Build path
			String sModule, sModuleRelative;
			sModuleRelative.Set("Script").Append(".module");
			sModule.Set(sModuleRelative);
			GetAssetManager()->FormatToAbsolute(sModule);

			// Refresh Module file
			Dictionary dFile;
			dFile["files"] = vFile;
			dFile.Write<DictionaryStreamJson>(sModule.GetBuffer());
		}
	}

	//!	@brief		LoadProject
	//!	@date		2015-04-04
	bool EditorController::LoadProject(const char* _szProjectPath /*= nullptr*/)
	{
		String sProject;
		
		// Get the document path
		String sDocument;
		FileManager::GetDirectory(FileManager::Directory::User, sDocument);
		sDocument.Append("GorillaEngine").Append(REPERTORY_SEPARATOR);

		// Editor Setting
		String sEditor;
		sEditor.Append(sDocument).Append("Editor.json");
		Dictionary& dEditor = GetConfig(Config::Editor);
		dEditor.Read<DictionaryStreamJson>(sEditor.GetBuffer());

		// Load saved Project
		if(!_szProjectPath) sProject = dEditor["Project"];
		else sProject.Set(_szProjectPath);

		// Load Project
		if(FileManager::IsFileExist(sProject.GetBuffer()))
		{
			dEditor["Project"] = sProject;
			if(!dEditor.Write<DictionaryStreamJson>(sEditor.GetBuffer()))
			{
				return false;
			}
			
			// Load Current World for the project
			Dictionary& dProject = GetConfig(Config::Project);
			if(!dProject.Read<DictionaryStreamJson>(sProject.GetBuffer()))
			{
				return false;
			}

			// Apply the project path to the asset manager
			Path sPath(sProject);
			GetAssetManager()->SetPath(sPath.GetDirectory().GetBuffer());

			// Retrieve file structure
			FileManager::Directory kDirectory(GetAssetManager()->GetPath().GetBuffer());
			FileManager::GetTree(kDirectory, true);

			// Convert to Json
			Dictionary& dTree = GetDictionary();
			const uint32 uiDirectoryCount = kDirectory.GetDirectoryCount();
			for(uint32 uiDirectory = 0; uiDirectory < uiDirectoryCount; ++uiDirectory)
			{
				Node dDirectory = dTree.Add();
				ConvertRecursive(kDirectory.GetDirectory(uiDirectory), dDirectory);
			}

			// Notify the workspace only if a project has been set
			SendJson("Editor.panels.workspace.onChanged", dTree);

			// Refresh module
			RefreshModule();
			String sModule;
			sModule.Set(GetAssetManager()->GetPath().GetBuffer()).Append("Script.module");
			if(FileManager::IsFileExist(sModule.GetBuffer()))
			{
				GetEngine()->AddModule("Script.module");
			}

			// Load Current world for this project
			const char* szWorld = dProject["World"];
			LoadWorld(szWorld);

			// Update Window title
			String sTitle;
			sTitle.Append(GORILLA_EDITOR_TITLE).Append(" - ").Append(GORILLA_EDITOR_VERSION).Append(" - ").Append(sPath.GetFileName());
			View->GetWindow()->SetTitle(sTitle.GetBuffer());

			return true;
		}
		LoadWorld(nullptr); // Default world

		return false;
	}

	//!	@brief		LoadWorld
	//!	@date		2015-04-04
	void EditorController::LoadWorld(const char* _szWorldPath /*= nullptr*/)
	{
		String sWorld;
		if(_szWorldPath && *_szWorldPath != '\0')
		{
			// World not in current project
			sWorld.Set(_szWorldPath);
			if(!GetAssetManager()->FormatToAbsolute(sWorld))
			{
				LOG_ERROR("World %s does not belong to current project", _szWorldPath);
				return;
			}
		}

		GetEngine()->DestroyWorld(m_pWorld);
		m_pWorld = GetEngine()->CreateWorld();
		m_pWorld->Pause();

		// Intrinsic GameObject
		Engine::GameObject* pGoEditor = m_pWorld->AddGameObject("@Editor");
		pGoEditor->SetFlag(Engine::GameObject::EFlag::UpdateDuringPause);
		pGoEditor->AddComponent<Gorilla::Component::Node>()->SetPosition(0.0f, 0.0f, -3.0f);
		Gorilla::Component::Camera* pCpnCamera = pGoEditor->AddComponent<Gorilla::Component::Camera>();
		pGoEditor->AddComponent<Gorilla::Component::CameraFree>();
		if(m_vViewport.GetSize()) pCpnCamera->Viewport = m_vViewport[m_vViewport.GetSize()-1];

		// User World Object
		if(sWorld.GetLength())
		{
			Dictionary dWorld;
			bool bResult = dWorld.Read<DictionaryStreamJson>(sWorld.GetBuffer());
			if(bResult) bResult = m_pWorld->Deserialize(dWorld);

			if(!bResult)
			{
				LOG_ERROR("Failed to load world %s", sWorld.GetBuffer());
			}			
		}
		else
		{
			// Default Game 
			Engine::GameObject* pGoDirectionalLight = m_pWorld->AddGameObject("Directional Light");
			pGoDirectionalLight->AddComponent<Gorilla::Component::DirectionalLight>();

			Engine::GameObject* pGoCube = m_pWorld->AddGameObject("Cube");
			Gorilla::Component::Mesh* pCpnMesh = pGoCube->AddComponent<Gorilla::Component::Mesh>();
			pCpnMesh->Asset = GetAssetManager()->Get<Engine::Mesh>("@Mesh/Cube.fbx");

			Engine::GameObject* pGoCamera = m_pWorld->AddGameObject("Camera");
			pGoCamera->AddComponent<Gorilla::Component::Camera>();
		}
		SendWorldTree(m_pWorld);
	}

	//!	@brief		SaveWorld
	//!	@date		2015-04-04
	void EditorController::SaveWorld(const char* _szWorldPath)
	{
		// Check path to save
		String sWorld(_szWorldPath);
		if(!GetAssetManager()->FormatToAbsolute(sWorld))
		{
			return;
		}

		// Serialize world
		Dictionary dWorld;
		bool bResult = m_pWorld->Serialize(dWorld);
		if(bResult) bResult = dWorld.Write<DictionaryStreamJson>(sWorld.GetBuffer());
		
		// Error notification
		if(bResult)
		{
			// Make relative path
			GetAssetManager()->FormatToRelative(sWorld);

			// Check if we need to update editor config
			Dictionary& dProject = GetConfig(Config::Project);
			const char* szWorldSaved = dProject["World"];
			if(sWorld == szWorldSaved) return;
			dProject["World"] = sWorld.GetBuffer();

			// Update Project config
			const char* szProjectPath = GetConfig(Config::Editor)["Project"];
			dProject.Write<DictionaryStreamJson>(szProjectPath);
		}
		else
		{
			LOG_ERROR("Failed to save world %s", _szWorldPath);
		}
	}
}}