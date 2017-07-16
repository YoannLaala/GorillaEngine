#include <Core/Core.hpp>
#include <Core/File/FileManager.hpp>
#include <Core/Stream/FileReader.hpp>
#include <Core/Stream/FileWriter.hpp>
#include <Core/Process/ArgumentParser.hpp>
#include <Core/Process/Process.hpp>
#include <Core/Container/Dictionary.hpp>
#include <Core/Stream/Dictionary/DictionaryStreamJson.hpp>
#include <string>

#define VERSION				"1.0.0"
#define ARGUMENT_INPUT		"-input"
#define ARGUMENT_OUTPUT		"-output"
#define ARGUMENT_IDE		"-ide"

int main(int argc, const char** argv)
{
	Gorilla::ArgumentParser kParser("Module Cooker", "Cook a dll for a specific project", VERSION);
	kParser.Add(ARGUMENT_INPUT, "Define which project will be cooked", true);
	kParser.Add(ARGUMENT_OUTPUT, "Define where the asset file will be cooked", true);
	kParser.Add(ARGUMENT_IDE, "Define the ide to use", false);

	Gorilla::String sUsage;
	if(!kParser.Parse(argc, argv, &sUsage))
	{
		printf(sUsage.GetBuffer());
		return -1;
	}

	// Retrieve main information
	Gorilla::Path sInput = kParser.Get<Gorilla::String>(ARGUMENT_INPUT);
	Gorilla::Path sOutput = kParser.Get<Gorilla::String>(ARGUMENT_OUTPUT);
	Gorilla::String sIde = kParser.Get<Gorilla::String>(ARGUMENT_IDE);
	if(sIde.IsEmpty()) sIde = "vs2013";

	Gorilla::String sSolution(sInput.GetDirectory());
	sSolution.Append("..\\");

	// Check if there file to compile
	Gorilla::Dictionary dModule;
	if(!dModule.Read<Gorilla::DictionaryStreamJson>(sInput.GetFull().GetBuffer()))
	{
		printf("Failed to read module %s", sInput.GetFull().GetBuffer());
		return -1;
	}
	Gorilla::Vector<Gorilla::String> vFile;
	dModule.GetValue("files", vFile);
	if(vFile.IsEmpty()) return true;

	Gorilla::String sCookerPath, sEditorPath;
	Gorilla::FileManager::GetDirectory(Gorilla::FileManager::Directory::Executable, sCookerPath);
	sEditorPath.Set(sCookerPath).Append("..\\..\\");
	Gorilla::Path::Format(sEditorPath);

	// Generate premake file from template
	Gorilla::String sTemplatePath(sCookerPath);
	sTemplatePath.Append("Application.lua");

	// Read template application lua
	Gorilla::FileReader kReader;
	if(!kReader.Open(sTemplatePath.GetBuffer()))
	{
		printf("Failed to open %s", sTemplatePath.GetBuffer());
		return -1;
	}

	// Replace variable
	const Gorilla::uint32 uiFileSize = kReader.GetSize();
	Gorilla::String sContent;
	sContent.Resize(uiFileSize);
	kReader.Read(&sContent[0], uiFileSize);
	sContent.Replace("${SOLUTION_NAME}", sInput.GetFileName().GetBuffer());
	sContent.Replace("${SOLUTION_DIR}", sSolution.GetBuffer());
	sContent.Replace("${TARGET_DIR}", sOutput.GetDirectory().GetBuffer());
	sContent.Replace("${TARGET_NAME}", sOutput.GetFileName().GetBuffer());
	
	// Generate file include
	Gorilla::String sIncludePath, sInclude;
	sIncludePath.Set(sEditorPath).Append("Resources\\Include\\");
	const Gorilla::uint32 uiFileCount = vFile.GetSize();
	for(Gorilla::uint32 uiFile = 0; uiFile < uiFileCount; ++uiFile) sInclude.Append("\"").Append(sInput.GetDirectory()).Append(vFile[uiFile]).Append("\",");
	sInclude.Append("\"").Append(sIncludePath).Append("Engine\\System\\Module.hpp").Append("\",");
	sInclude.Append("\"").Append(sIncludePath).Append("Engine\\System\\Module.cpp").Append("\",");
	sContent.Replace("${SCRIPT_FILE}", sInclude.GetBuffer());

	// Generate include lib
	sInclude.Set("\"").Append(sEditorPath).Append("..\\..\\Sources\\Engine\\\"");
	Gorilla::Path::Format(sInclude);
	sContent.Replace("${ENGINE_INCLUDE}", sInclude.GetBuffer());

	// Generate Library to link with
	Gorilla::String sLibrary, sLink;
	sLibrary.Set(sEditorPath).Append("Resources\\Libraries\\");
	Gorilla::Path::Format(sLibrary);
	sLink.Append("\"").Append(sLibrary).Append("Core.lib\",");
	sLink.Append("\"").Append(sLibrary).Append("Renderer.lib\",");
	sLink.Append("\"").Append(sLibrary).Append("Engine.lib\",");
	sLink.Append("\"").Append(sLibrary).Append("Web.lib\",");
	sLink.Append("\"d3d11.lib\"");
	sContent.Replace("${ENGINE_LINK}", sLink.GetBuffer());
	sContent.Replace("\\", "/");
	
	// Configuration file
	Gorilla::String sConfiguration;
	Gorilla::FileManager::GetDirectory(Gorilla::FileManager::Directory::Temporary, sConfiguration);
	sConfiguration.Append("Application.lua");

	// Write premake file
	Gorilla::FileWriter kWriter;
	if(!kWriter.Open(sConfiguration.GetBuffer()))
	{
		printf("Failed to open %s", sConfiguration.GetBuffer());
		return -1;
	}
	kWriter.Write(sContent.GetBuffer(), sContent.GetLength());
	kWriter.Close();
	
	Gorilla::String sArgument, sError;
	
	// Generate solution
	Gorilla::Process kPremake;
	sArgument.Set("--file=").Append(sConfiguration.GetBuffer()).Append(" ").Append(sIde);
	kPremake.Initialize("premake5.exe", sArgument.GetBuffer());
	if(!kPremake.Execute(&sError))
	{
		printf(sError.GetBuffer());
		return -1;
	}

	// Build solution
	Gorilla::Process sDevEnv;
	sArgument.Set("\"").Append(sSolution.GetBuffer()).Append(sInput.GetFileName()).Append(".vcxproj\" /build Debug");
	sDevEnv.Initialize("C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\Common7\\IDE\\devenv.exe", sArgument.GetBuffer());
	if(!sDevEnv.Execute(&sError))
	{
		printf(sError.GetBuffer());
		return -1;
	}

	return 0;
}