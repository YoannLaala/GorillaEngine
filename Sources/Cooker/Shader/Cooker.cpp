#include <Core/Core.hpp>
#include <Core/File/FileManager.hpp>
#include <Core/Process/ArgumentParser.hpp>
#include <Core/Process/Process.hpp>
#include <Core/Thread/Thread.hpp>

#include "HLSLCooker.hpp"

#define VERSION				"1.0.0"
#define ARGUMENT_INPUT		"-input"
#define ARGUMENT_OUTPUT		"-output"
#define ARGUMENT_TYPE		"-type"
#define ARGUMENT_TECHNIQUE	"-technique"

//!	@brief		Entry
//!	@date		2015-11-22
int main(int argc, const char** argv) 
{
	Gorilla::ArgumentParser kParser("Shader Cooker", "Serialize a shader file", VERSION);
	kParser.Add(ARGUMENT_INPUT, "Define which effect will be serialized", true);
	kParser.Add(ARGUMENT_OUTPUT, "Define where the asset file will be serialized", true);
	kParser.Add(ARGUMENT_TECHNIQUE, "Define the technique of the shader (Default, Instancing, ...)", false);

	Gorilla::String sUsage;
	if(!kParser.Parse(argc, argv, &sUsage))
	{
		printf(sUsage.GetBuffer());
		return -1;
	}

	// Retrieve argument
	Gorilla::Path kInput = kParser.Get<Gorilla::String>(ARGUMENT_INPUT);
	Gorilla::String sOutput = kParser.Get<Gorilla::String>(ARGUMENT_OUTPUT);

	// Handle default value for the technique
	Gorilla::String sTechnique = kParser.Get<Gorilla::String>(ARGUMENT_TECHNIQUE);
	if(sTechnique.IsEmpty())
	{
		sTechnique.Set(Gorilla::Renderer::ETechnique::Name[Gorilla::Renderer::ETechnique::Default]);
	}
	
	// Create temporary directory
	Gorilla::String sTemporaryPath;
	Gorilla::FileManager::CreateTemporaryDirectory(sTemporaryPath);

	Gorilla::String sInputConverted;
	sInputConverted.Append(sTemporaryPath).Append(kInput.GetFileName());

	// Build parameters for fxc
	Gorilla::uint8 eShader = Gorilla::Helper::GetEnumValue(kInput.GetExtension().GetBuffer(), Gorilla::Renderer::EShader::Value, Gorilla::Renderer::EShader::Extension, Gorilla::Renderer::EShader::Count);
	
	Gorilla::String sArgument;
	const char* szType = Gorilla::Renderer::EShader::Name[eShader];
	const char* szTarget = Gorilla::Renderer::EShader::Target[eShader];
	const char* szTechnique = sTechnique.GetBuffer();
	sArgument.Append(kInput.GetFull()).Append(" /Fo ").Append(sInputConverted.GetBuffer()).Append(" /E ").Append(szType).Append(szTechnique).Append(" /T ").Append(szTarget).Append(" /Zpr /nologo");

	// Compile the shader
	Gorilla::String sError;
	Gorilla::Process kProcess;
	kProcess.Initialize("fxc.exe", sArgument.GetBuffer());
	if(!kProcess.Execute(&sError))
	{
		printf(sError.GetBuffer());
		Gorilla::FileManager::DeleteADirectory(sTemporaryPath.GetBuffer());
		return -1;
	}

	// Serialize it
	Gorilla::HLSLCooker kCooker;
	kCooker.SetType((Gorilla::Renderer::EShader::Type)eShader);
	if(!kCooker.Execute(sInputConverted.GetBuffer(), sOutput.GetBuffer()))
	{
		printf("Failed to serialize shader");
		Gorilla::FileManager::DeleteADirectory(sTemporaryPath.GetBuffer());
		return -1;
	}

	Gorilla::FileManager::DeleteADirectory(sTemporaryPath.GetBuffer());

	return 0;
}
