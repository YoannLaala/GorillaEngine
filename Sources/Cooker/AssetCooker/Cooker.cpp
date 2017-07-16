#include <Core/Core.hpp>
#include <Core/Process/ArgumentParser.hpp>
#include <Core/Process/Process.hpp>
#include <Core/File/FileManager.hpp>
#include <Core/Container/Dictionary.hpp>
#include <Core/Stream/Dictionary/DictionaryStreamJson.hpp>
#include <Engine/Database/Database.hpp>
#include <Engine/Database/Table.hpp>
#include <Engine/Database/AssetDescriptor.hpp>
#include <Engine/Asset/AssetProcess.hpp>
#include <string>

#define VERSION						"1.0.0"
#define ARGUMENT_TYPE				"-type"
#define ARGUMENT_INPUT				"-input"
#define ARGUMENT_CONFIGURATION		"-configuration"

using namespace Gorilla;

//!	@brief		Entry
//!	@date		2015-11-22
int main(int argc, const char** argv) 
{
	ArgumentParser kParser("Asset Cooker", "Generic cooker using AssetDescriptor.json", VERSION);
	kParser.Add(ARGUMENT_TYPE, "Define the type to cook", true);
	kParser.Add(ARGUMENT_INPUT, "Define which file to cook", true);
	kParser.Add(ARGUMENT_CONFIGURATION, "Define the configuration", true);

	String sUsage;
	if(!kParser.Parse(argc, argv, &sUsage))
	{
		printf(sUsage.GetBuffer());
		return -1;
	}

	// Input and Output file
	String sType = kParser.Get<Gorilla::String>(ARGUMENT_TYPE).Prepend("Gorilla::Engine::");
	String sInput = kParser.Get<Gorilla::String>(ARGUMENT_INPUT).Remove("..\\..\\..\\Resources\\Asset\\");	// Due to relative visual studio path 
	String sConfiguration = kParser.Get<Gorilla::String>(ARGUMENT_CONFIGURATION);

	// Executable path
	String sDirectory;
	FileManager::GetDirectory(FileManager::Directory::Executable, sDirectory);
	sDirectory.Append("..\\..\\"); // Gorilla Engine Directory

	// Output Path
	String sProjectPath(sDirectory);
	sProjectPath.Append("Resources\\Asset\\");
	Path::Format(sProjectPath);

	// Prepare Asset Path
	Path::Format(sInput);
	String sSourcePath(sDirectory), sParameter;
	sSourcePath.Append("Resources\\Asset\\").Append(sInput);
	Path::Format(sSourcePath);

	uint32 uiPosition = sInput.Find(" ");
	if(uiPosition != (uint32)-1)
	{
		sSourcePath.Resize(sSourcePath.GetLength() - (sInput.GetLength() - uiPosition));
		sParameter.Set(&sInput[uiPosition], sInput.GetLength() - uiPosition);
	}
	
	// Check input file is present
	if(!FileManager::IsFileExist(sSourcePath.GetBuffer()))
	{
		printf("File '%s' not found!", sSourcePath.GetBuffer());
		return -1;
	}

	// Asset Descriptor FilePath
	String sAssetDescriptor(sDirectory);
	sAssetDescriptor.Append("Resources\\Database\\AssetDescriptor.json"); 

	// Asset Descriptor
	Dictionary dAssetDescriptor;
	if(!dAssetDescriptor.Read<DictionaryStreamJson>(sAssetDescriptor.GetBuffer()))
	{		
		printf("Failed to read AssetDescriptor.json");
		return -1;
	}

	// Create Database
	Engine::Database<Engine::AssetDescriptor>* pDbDescriptor = new Engine::Database<Engine::AssetDescriptor>();
	Node kNode = dAssetDescriptor.GetFirstChild();
	while(kNode.IsValid())
	{
		Engine::AssetDescriptor* pDescriptor = pDbDescriptor->GetOrCreateTable(kNode.GetId());
		pDescriptor->Deserialize(kNode);
		++kNode;
	}

	// Search for the proper descriptor
	uint32 uiClassId = Hash::Generate(sType);
	Engine::AssetDescriptor* pDescriptor = pDbDescriptor->GetTable(uiClassId);
	if(!pDescriptor)
	{
		printf("Asset of type '%s' is not supported", sType.GetBuffer());
		return -1;
	}

	// Compute AssetId
	uint32 uiAssetId = Hash::Generate(sInput);

	// Build path for the asset
	Path kRelativeInputPath = sInput;
	String sAssetPath(sDirectory);
	sAssetPath.Append("Binaries\\").Append(sConfiguration).Append("\\Resources\\Asset\\").Append(kRelativeInputPath.GetDirectory()).Append(kRelativeInputPath.GetFileName()).Append('_').Append(uiAssetId).Append(".asset");
	Path::Format(sAssetPath);

	// Create Path if needed
	FileManager::CreateADirectory(sAssetPath.GetBuffer());

	// if there isn't converter, just copy to the output
	Engine::AssetProcess* pProcess = pDescriptor->GetProcess();
	if(!pProcess) 
	{
		if (!FileManager::CopyAFile(sSourcePath.GetBuffer(), sAssetPath.GetBuffer()))
		{
			printf("Failed to copy copy '%s' to '%s'", sSourcePath.GetBuffer(), sAssetPath.GetBuffer());
			return -1;
		}
	}
	// Convert properly the asset
	else
	{
		// Set Process Path as absolute
		String sProcessPath(sDirectory);
		sProcessPath.Append("Binaries\\").Append(sConfiguration).Append("\\").Append(pProcess->GetPath());
		pProcess->Initialize(sProcessPath.GetBuffer(), pProcess->GetParameter().GetBuffer());

		// Generic Arguments
		String sError;
		Vector<Argument> vArgument(4);
		vArgument[0].Initialize("$[INPUT]", sSourcePath.GetBuffer());
		vArgument[1].Initialize("$[PARAMETER]", sParameter.GetBuffer());
		vArgument[2].Initialize("$[OUTPUT]", sAssetPath.GetBuffer());
		vArgument[3].Initialize("$[PROJECT_PATH]", sProjectPath.GetBuffer()); 

		// Execute the process
		if(!pProcess->Execute(vArgument, sError))
		{
			// Generate command line for debug purpose
			printf("Failed to cook %s\n%s", sSourcePath.GetBuffer(), sError.GetBuffer());
			return -1;
		}
	}

	return 0;
}