#include <Core/Core.hpp>
#include <Core/File/FileManager.hpp>
#include <Core/Process/ArgumentParser.hpp>
#include <Core/Process/Process.hpp>

#include "DDSCooker.hpp"

#define VERSION				"1.0.0"
#define ARGUMENT_X_NEGATIVE	"-xn"
#define ARGUMENT_X_POSITIVE	"-xp"
#define ARGUMENT_Y_NEGATIVE	"-yn"
#define ARGUMENT_Y_POSITIVE	"-yp"
#define ARGUMENT_Z_NEGATIVE	"-zn"
#define ARGUMENT_Z_POSITIVE	"-zp"
#define ARGUMENT_INPUT		"-input"
#define ARGUMENT_OUTPUT		"-output"

//!	@brief		Entry
//!	@date		2015-11-22
int main(int argc, const char** argv) 
{
	Gorilla::ArgumentParser kParser("Texture2Asset", "Serialize a texture file", VERSION);
	kParser.Add(ARGUMENT_INPUT, "Define which texture will be serialized", true);
	kParser.Add(ARGUMENT_OUTPUT, "Define where the asset file will be serialized", true);

	Gorilla::String sUsage;
	if(!kParser.Parse(argc, argv, &sUsage))
	{
		printf(sUsage.GetBuffer());
		return -1;
	}

	Gorilla::String sTemporaryPath;
	Gorilla::FileManager::CreateTemporaryDirectory(sTemporaryPath);

	// Retrieve main information
	Gorilla::Path sInput = kParser.Get<Gorilla::String>(ARGUMENT_INPUT);

	Gorilla::String sInputConverted;
	sInputConverted.Append(sTemporaryPath).Append(sInput.GetFileName()).Append(".dds");
	Gorilla::String sOutput = kParser.Get<Gorilla::String>(ARGUMENT_OUTPUT);

	// Build parameters for nvtxt
	Gorilla::String sArgument;
	sArgument.Append("-cubeMap -file ").Append(sInput.GetFull()).Append(" -output ").Append(sInputConverted).Append(" -overwrite");

	// Launch process
	Gorilla::String sError;
	Gorilla::Process kProcess;
	kProcess.Initialize("nvdxt.exe", sArgument.GetBuffer());
	if(!kProcess.Execute(&sError))
	{
		printf("Failed to generate a dds: ", sOutput.GetBuffer());
		Gorilla::FileManager::DeleteADirectory(sTemporaryPath.GetBuffer());
		return -1;
	}

	Gorilla::DDSCooker kCooker;
	if(!kCooker.Execute(sInputConverted.GetBuffer(), sOutput.GetBuffer()))
	{
		printf("Failed to serialize texture");
		Gorilla::FileManager::DeleteADirectory(sTemporaryPath.GetBuffer());
		return -1;
	}

	Gorilla::FileManager::DeleteADirectory(sTemporaryPath.GetBuffer());

	return 0;
}
