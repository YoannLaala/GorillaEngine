#include <Core/Core.hpp>
#include <Core/File/FileManager.hpp>
#include <Core/Stream/FileReader.hpp>
#include <Core/Stream/FileWriter.hpp>
#include <Core/String/StringHelper.hpp>
#include <Core/Process/ArgumentParser.hpp>
#include <Core/Process/Process.hpp>
#include <Core/Container/Dictionary.hpp>
#include <Core/Stream/Dictionary/DictionaryStreamJson.hpp>
#include <string>

#define VERSION				"1.0.0"
#define ARGUMENT_INPUT		"--input"
#define ARGUMENT_OUTPUT		"--output"
#define ARGUMENT_IDE		"--ide"

#if defined(DEBUG)
	#define BUILD_CONFIGURATION "Debug"
#elif defined(RELEASE)
	#define BUILD_CONFIGURATION "Release"
#elif defined(MASTER)
	#define BUILD_CONFIGURATION "Master"
#endif

using namespace Gorilla;

int main(int argc, const char** argv)
{
	ArgumentParser kParser("Module Cooker", "Cook a dll for a specific project", VERSION);
	kParser.Add(ARGUMENT_INPUT, "Define which project will be cooked", true);
	kParser.Add(ARGUMENT_OUTPUT, "Define where the asset file will be cooked", true);
	kParser.Add(ARGUMENT_IDE, "Define the ide to use", false);

	String sUsage;
	if(!kParser.Parse(argc, argv, &sUsage))
	{
		printf(sUsage.GetBuffer());
		return -1;
	}

	// Retrieve main information
	Path sInput = kParser.Get<String>(ARGUMENT_INPUT);
	Path sOutput = kParser.Get<String>(ARGUMENT_OUTPUT);
	String sIde = kParser.Get<String>(ARGUMENT_IDE);
	if(sIde.IsEmpty()) sIde = "vs2013";

	String sSolution(sInput.GetDirectory());
	sSolution.Append("..\\");

	// Check if there file to compile
	Dictionary dModule;
	if(!dModule.Read<DictionaryStreamJson>(sInput.GetFull().GetBuffer()))
	{
		printf("Failed to read module %s", sInput.GetFull().GetBuffer());
		return -1;
	}
	Vector<String> vFile;
	dModule.GetValue("files", vFile);
	if(vFile.IsEmpty()) return true;

	String sCookerPath, sEditorPath;
	FileManager::GetDirectory(FileManager::Directory::Executable, sCookerPath);
	sEditorPath.Set(sCookerPath).Append("..\\..\\");
	Path::Format(sEditorPath);

	// Generate premake file from template
	String sTemplatePath(sCookerPath);
	sTemplatePath.Append("Application.lua");

	// Read template application lua
	FileReader kReader;
	if(!kReader.Open(sTemplatePath.GetBuffer()))
	{
		printf("Failed to open %s", sTemplatePath.GetBuffer());
		return -1;
	}

	// Replace variable
	const uint32 uiFileSize = kReader.GetSize();
	String sContent;
	sContent.Resize(uiFileSize);
	kReader.Read(&sContent[0], uiFileSize);
	sContent.Replace("${SOLUTION_NAME}", sInput.GetFileName().GetBuffer());
	sContent.Replace("${SOLUTION_DIR}", sSolution.GetBuffer());
	sContent.Replace("${TARGET_DIR}", sOutput.GetDirectory().GetBuffer());
	sContent.Replace("${TARGET_NAME}", sOutput.GetFileName().GetBuffer());
	
	// Generate file include
	String sIncludePath, sInclude;
	sIncludePath.Set(sEditorPath).Append("Resources\\Include\\");
	const uint32 uiFileCount = vFile.GetSize();
	for(uint32 uiFile = 0; uiFile < uiFileCount; ++uiFile) sInclude.Append("\"").Append(sInput.GetDirectory()).Append(vFile[uiFile]).Append("\",");
	sInclude.Append("\"").Append(sIncludePath).Append("Engine\\System\\Module.hpp").Append("\",");
	sInclude.Append("\"").Append(sIncludePath).Append("Engine\\System\\Module.cpp").Append("\",");
	sContent.Replace("${SCRIPT_FILE}", sInclude.GetBuffer());

	// Generate include lib
	sInclude.Set("\"").Append(sEditorPath).Append("..\\..\\Sources\\Engine\\\"");
	Path::Format(sInclude);
	sContent.Replace("${ENGINE_INCLUDE}", sInclude.GetBuffer());

	// Generate Library to link with
	String sLibrary, sLink;
	sLibrary.Set(sEditorPath).Append("Resources\\Libraries\\");
	Path::Format(sLibrary);
	sLink.Append("\"").Append(sLibrary).Append("Core.lib\",");
	sLink.Append("\"").Append(sLibrary).Append("Renderer.lib\",");
	sLink.Append("\"").Append(sLibrary).Append("Engine.lib\",");
	sLink.Append("\"").Append(sLibrary).Append("Web.lib\",");
	sLink.Append("\"d3d11.lib\",");
	sLink.Append("\"Shlwapi.lib\"");
	sContent.Replace("${ENGINE_LINK}", sLink.GetBuffer());
	sContent.Replace("\\", "/");
	
	// Configuration file
	String sConfiguration;
	FileManager::GetDirectory(FileManager::Directory::Temporary, sConfiguration);
	sConfiguration.Append("Application.lua");

	// Write premake file
	FileWriter kWriter;
	if(!kWriter.Open(sConfiguration.GetBuffer()))
	{
		printf("Failed to open %s", sConfiguration.GetBuffer());
		return -1;
	}
	kWriter.Write(sContent.GetBuffer(), sContent.GetLength());
	kWriter.Close();
	
	String sArgument, sError;
	
	// Generate solution
	Process kPremake;
	sArgument.Set("--file=").Append(sConfiguration.GetBuffer()).Append(" ").Append(sIde);
	kPremake.Initialize("premake5.exe", sArgument.GetBuffer());
	if(!kPremake.Execute(&sError))
	{
		printf(sError.GetBuffer());
		return -1;
	}

	// Build solution
	Process sDevEnv;
	sArgument.Set("\"").Append(sSolution.GetBuffer()).Append(sInput.GetFileName()).Append(".vcxproj\" /build ").Append(BUILD_CONFIGURATION);
	sDevEnv.Initialize("C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\Common7\\IDE\\devenv.exe", sArgument.GetBuffer());
	if(!sDevEnv.Execute())
	{
		String sLogFile;
		sLogFile.Set(sOutput.GetDirectory()).Append("Temp\\x64\\").Append(BUILD_CONFIGURATION).Append("\\Script.log");

		// Read log to display errors
		FileReader kReader(sLogFile.GetBuffer());
		if(kReader.CanRead())
		{
			Vector<String> vLine;
			
			// Split content by lines
			String sContent;
			sContent.Resize(kReader.GetSize());
			kReader.Read(&sContent[0], kReader.GetSize());
			StringHelper::Split(sContent.GetBuffer(), "\n", vLine);

			// Check each line
			const uint32 uiElementCount = vLine.GetSize();
			for(uint32 uiElement = 0; uiElement < uiElementCount; ++uiElement) 
			{
				String& sLine = vLine[uiElement];
				if(sLine.Find(": error ") != (uint32)-1)
				{
					uint32 uiStartPosition = sLine.Find(">")+1;
					sError.Append(&sLine[uiStartPosition], sLine.GetLength() - uiStartPosition).Append("\n");
				}
			}
			String sInputDirectoryToLower(sInput.GetDirectory());
			sInputDirectoryToLower.Remove("Asset\\");
			sInputDirectoryToLower.ToLower();
			sError.Remove(sInputDirectoryToLower.GetBuffer());
		}
		
		printf(sError.GetBuffer());
		return -1;
	}

	// Copy to real output (HACK waiting for reading dll from memory)
	String sLibrarySource;
	sLibrarySource.Set(sOutput.GetDirectory()).Append("Temp\\").Append(sOutput.GetFileNameWithExtension());
	FileManager::CopyAFile(sLibrarySource.GetBuffer(), sOutput.GetFull().GetBuffer());

	String sPdbFile;
	sPdbFile.Set(sOutput.GetDirectory()).Append("Temp\\").Append(sOutput.GetFileName()).Append(".pdb");
	FileManager::DeleteAFile(sPdbFile.GetBuffer());


	return 0;
}