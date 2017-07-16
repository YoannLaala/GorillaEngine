///******************************************************************************
//**	Includes
//******************************************************************************/
//#include "Script.hpp"
//#include <Common/FileSystem.hpp>
//
///******************************************************************************
//**	Class Definition
//******************************************************************************/
//namespace PantherRenderer
//{
//	Script::Script()
//	{
//		m_hModule = NULL;
//		m_pCreateInstance = NULL;
//	}
//
//	void Script::PreBuild(const Common::FileSystem* _pFileSystem, Common::ArgumentList& _kArgumentList)
//	{
//		// Build TempPath
//		String sFilePathTemp = _pFileSystem->GetPath(EEnginePath::Temporary).GetBuffer();
//		sFilePathTemp += m_kOutput.GetFile();
//		sFilePathTemp += (uint32)(rand() % 100);
//
//		// Ass Argument for Script Converter
//		_kArgumentList.Add("$ENGINE_SOURCE", _pFileSystem->GetPath(EEnginePath::Source).GetBuffer());
//		_kArgumentList.Add("$VISUAL_STUDIO", _pFileSystem->GetPath(EEnginePath::VisualStudio).GetBuffer());
//		_kArgumentList.Add("$TEMP_FILE", sFilePathTemp.GetBuffer());
//	}
//
//	void Script::PostBuild()
//	{
//		// Load the Module
//		m_hModule = LoadLibrary(m_kOutput.GetPath().GetBuffer());
//		if (!m_hModule) 
//		{
//			LOG_WARNING("Could not load Script %s", m_kOutput.GetPath().GetBuffer()); 
//			return;
//		}
//
//		// Create an instance of the script
//		m_pCreateInstance = (fnCreateInstance)GetProcAddress(m_hModule, "CreateInstance");
//		if (!m_pCreateInstance) 
//		{
//			LOG_WARNING("Could not locate the function CreateInstance in Script %s",m_kOutput.GetPath().GetBuffer()); 
//			return;
//		}
//
//		// Recreate the instance
//		for(uint32 uiInstance = 0; uiInstance < m_vecInstance.GetSize(); uiInstance++)
//		{
//			m_vecInstance[uiInstance] = m_pCreateInstance();
//		}
//	}
//
//	uint32 Script::AddInstance()
//	{
//		uint32 uiIndex = m_vecInstance.GetSize();
//		m_vecInstance.Add(m_pCreateInstance());
//		return uiIndex;
//	}
//
//	void Script::Release()
//	{
//		//Asset::Release();
//		for(uint32 uiInstance = 0; uiInstance < m_vecInstance.GetSize(); uiInstance++)
//		{
//			SAFE_DELETE(m_vecInstance[uiInstance])
//		}
//
//		m_pCreateInstance = NULL;
//		FreeLibrary(m_hModule);
//	}
//
//	Script::~Script()
//	{
//		Release();
//	}
//
//	REGISTER_CLASS_SERIALIZABLE(PantherRenderer::Script)
//}