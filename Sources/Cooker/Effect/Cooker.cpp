#include <Core/Core.hpp>
#include <Core/File/FileManager.hpp>
#include <Core/Process/ArgumentParser.hpp>
#include <Core/Process/Process.hpp>

#include "EffectCooker.hpp"

#define VERSION					"1.0.0"
#define ARGUMENT_INPUT			"-input"
#define ARGUMENT_OUTPUT			"-output"
#define ARGUMENT_PROJECT_PATH	"-project_path"

//!	@brief		Entry
//!	@date		2015-11-22
int main(int argc, const char** argv) 
{
	Gorilla::ArgumentParser kParser("Effect Cooker", "Cook an effect file", VERSION);
	kParser.Add(ARGUMENT_INPUT, "Define which effect will be serialized", true);
	kParser.Add(ARGUMENT_OUTPUT, "Define where the asset file will be serialized", true);
	kParser.Add(ARGUMENT_PROJECT_PATH, "Define where the project is to create relative dependency", true);

	Gorilla::String sUsage;
	if(!kParser.Parse(argc, argv, &sUsage))
	{
		printf(sUsage.GetBuffer());
		return -1;
	}

	Gorilla::String sInput = kParser.Get<Gorilla::String>(ARGUMENT_INPUT);
	Gorilla::String sOutput = kParser.Get<Gorilla::String>(ARGUMENT_OUTPUT);
	Gorilla::String sProjectPath = kParser.Get<Gorilla::String>(ARGUMENT_PROJECT_PATH);

	Gorilla::EffectCooker kCooker;
	kCooker.SetProjectPath(sProjectPath.GetBuffer());
	if(!kCooker.Execute(sInput.GetBuffer(), sOutput.GetBuffer()))
	{
		printf("Failed to serialize effect");
		return -1;
	}

	return 0;
}
