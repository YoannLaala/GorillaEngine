#include <Core/Core.hpp>
#include <Core/File/FileManager.hpp>
#include <Core/Process/ArgumentParser.hpp>
#include <Core/Process/Process.hpp>

#include "FbxCooker.hpp"

#define VERSION				"1.0.0"
#define ARGUMENT_INPUT		"-input"
#define ARGUMENT_OUTPUT		"-output"

int main(int argc, const char** argv)
{
	Gorilla::ArgumentParser kParser("Mesh Cooker", "Serialize a geometry file", VERSION);
	kParser.Add(ARGUMENT_INPUT, "Define which geometry will be serialized", true);
	kParser.Add(ARGUMENT_OUTPUT, "Define where the asset file will be serialized", true);

	Gorilla::String sUsage;
	if(!kParser.Parse(argc, argv, &sUsage))
	{
		printf(sUsage.GetBuffer());
		return -1;
	}

	// Retrieve main information
	Gorilla::String sInput = kParser.Get<Gorilla::String>(ARGUMENT_INPUT);
	Gorilla::String sOutput = kParser.Get<Gorilla::String>(ARGUMENT_OUTPUT);

	// Serialize the file
	Gorilla::FbxCooker kCooker;
	if(!kCooker.Execute(sInput.GetBuffer(), sOutput.GetBuffer()))
	{
		printf("Failed to serialize geometry");
		return -1;
	}

	return 0;
}