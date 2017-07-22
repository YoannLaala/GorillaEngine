/******************************************************************************
**	Includes
******************************************************************************/
#include "Module.hpp"
#include <Core/Process/Process.hpp>
#include <Core/Stream/Dictionary/DictionaryStreamJson.hpp>
#include <Engine/Engine.hpp>
#include <Engine/System/Module.hpp>
#include <Windows.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	ASSET_REGISTER(Gorilla::Engine::Module);

	//!	@brief		Contructor
	//!	@date		2015-10-18
	Module::Module()
		: m_hModule(nullptr)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-10-18
	Module::~Module()
	{
		// Nothing to do
	}

	//!	@brief		Initialize
	//!	@date		2015-10-18
	void Module::Initialize(StreamReader* /*_pStream*/)
	{
		// Need to read library from memory
		// Copy from temp so we can build the library before releasing it (dll + pdb)
		String sLibrarySource;
		Path sLibraryDestination(GetFilePath());
		sLibrarySource.Set(sLibraryDestination.GetDirectory()).Append("Temp\\").Append(sLibraryDestination.GetFileNameWithExtension());
		FileManager::CopyAFile(sLibrarySource.GetBuffer(), sLibraryDestination.GetFull().GetBuffer());
		// Need to read library from memory

		// Load the Module
		m_hModule = ::LoadLibrary(sLibraryDestination.GetFull().GetBuffer());
		if (!m_hModule) 
		{
			return;
		}
		
		// Initialize the Engine Instance
		fnInitializeEngineModule pFunction0 = (fnInitializeEngineModule)GetProcAddress((HMODULE)m_hModule, "InitializeEngineModule");
		if (!pFunction0) 
		{
			LOG_ERROR("[Module::Initialize] Could not locate the function InitializeScriptModule in Module %s", GetFilePath().GetBuffer()); 
			return;
		}
		pFunction0(GetEngine());

		// Retrieve all component class in module 
		fnGetAllComponentClass pFunction1 = (fnGetAllComponentClass)GetProcAddress((HMODULE)m_hModule, "GetAllComponentClass");
		if (!pFunction1) 
		{
			LOG_ERROR("Could not locate the function CreateInstance in Script %s", GetFilePath().GetBuffer()); 
			return;
		}

		// Update User Component Descriptor
		Path sPath(GetFilePath());
		String sArgument;
		sArgument.Set("-input \"").Append(GetSourcePath().GetBuffer()).Append("\" -output \"").Append(sPath.GetDirectory()).Append("Temp\\ComponentDescriptor.json\"");
		Process kProcess("Cooker/Reflection/Reflection.exe");
		kProcess.SetArgument(sArgument.GetBuffer());
		kProcess.Execute();
			
		// Register all component class in the current module
		Gorilla::HashMap<Gorilla::uint32, Gorilla::Class*>& mComponentClass = pFunction1();
		HashMap<uint32, Gorilla::Class*>::Iterator it = mComponentClass.GetFirst();
		HashMap<uint32, Gorilla::Class*>::Iterator itEnd = mComponentClass.GetLast();
		while(it != itEnd)
		{
			Gorilla::ClassManager::GetInstance()->Set(Gorilla::Engine::EClass::Component, *it);
			++it;
		}

		// Read Module to get all files
		Dictionary dModule;
		dModule.Read<DictionaryStreamJson>(GetSourcePath().GetBuffer());
		
		Vector<String> vLink;
		dModule["files"].GetBufferString(vLink);

		// Link all file as dependencies
		const uint32 uiLinkCount = vLink.GetSize();
		for(uint32 uiLink = 0; uiLink < uiLinkCount; ++uiLink)
		{
			AddLink(vLink[uiLink].GetBuffer());
		}

		GetEngine()->LoadDescriptor();
	}

	//!	@brief		Release
	//!	@date		2015-10-18
	void Module::Release()
	{
		::FreeModule((HMODULE)m_hModule);
	}
}}