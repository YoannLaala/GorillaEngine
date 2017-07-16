///******************************************************************************
//**	Includes
//******************************************************************************/
//#include "ScriptComponent.hpp"
//#include "../Object/GameObject.hpp"
//#include "../Object/ScriptObject.hpp"
//#include <Renderer/Asset/Script.hpp>
//
///******************************************************************************
//**	Class Definition
//******************************************************************************/
//namespace PantherEngine
//{
//	/******************************************************************************
//	**	Script Info
//	******************************************************************************/
//	ScriptInfo::ScriptInfo()
//	{
//		m_pScript = NULL;
//		m_uiIndex = (uint32)-1;
//	}
//		
//	void ScriptInfo::Init(const PantherRenderer::Script* _pScript, uint32 _uiIndex)
//	{
//		m_pScript = _pScript;
//		m_uiIndex = _uiIndex;
//	}
//
//	ScriptObject* ScriptInfo::GetInstance()
//	{
//		return (ScriptObject*)m_pScript->GetInstance(m_uiIndex);
//	}
//
//	ScriptInfo::~ScriptInfo()
//	{
//	}
//
//	/******************************************************************************
//	**	Script Component
//	******************************************************************************/
//	ScriptComponent::ScriptComponent()
//	{
//	}
//
//	void ScriptComponent::AddScript(const char* _szFilePath)
//	{
//		/*PantherRenderer::Script* pScript = const_cast<PantherRenderer::Script*>(GetAssetManager()->GetAsset<PantherRenderer::Script>(_szFilePath));
//		if(pScript)
//		{
//			// Create an instance of the script
//			uint32 uiScriptIndex = pScript->AddInstance();
//			if(uiScriptIndex != (uint32)-1)
//			{
//				ScriptInfo& kScriptInfo = m_vecScript.Add();
//				kScriptInfo.Init(pScript, uiScriptIndex);
//			}
//		}*/
//	}
//
//	// Update all instance related to this GameObject
//	void ScriptComponent::Update(Engine* _pEngine)
//	{
//		for(uint32 uiScript = 0; uiScript < m_vecScript.GetSize(); uiScript++)
//		{
//			// Handle all script instanced
//			ScriptObject* pScriptObject = m_vecScript[uiScript].GetInstance();
//			if(pScriptObject && pScriptObject->IsActive())
//			{
//				/*switch(pScriptObject->GetInternalState())
//				{
//					// Starting the Script
//					case EScriptState::Start:
//					{
//						//pScriptObject->SetGameObject(m_pGameObject);
//						pScriptObject->Start();
//						pScriptObject->SetInternalState(EScriptState::Update);
//						break;
//					}
//
//					// Updating the Script
//					case EScriptState::Update:
//					{
//						pScriptObject->Update();
//						break;
//					}
//				}*/
//				
//			}
//		}
//	}
//
//	void ScriptComponent::Release()
//	{
//		m_vecScript.Clear();
//	}
//}