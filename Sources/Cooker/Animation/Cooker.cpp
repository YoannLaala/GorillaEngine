#include <Core/Core.hpp>
#include <Core/File/FileManager.hpp>
#include <Core/Process/ArgumentParser.hpp>
#include <Core/Process/Process.hpp>

#include "FbxCooker.hpp"

#define VERSION				"1.0.0"
#define ARGUMENT_INPUT		"--input"
#define ARGUMENT_OUTPUT		"--output"

using namespace Gorilla;

int main(int argc, const char** argv)
{
	ArgumentParser kParser("Animation Cooker", "Serialize a animation file", VERSION);
	kParser.Add(ARGUMENT_INPUT, "Define which animation will be cooked", true);
	kParser.Add(ARGUMENT_OUTPUT, "Define where the asset file will be generated", true);

	String sUsage;
	if(!kParser.Parse(argc, argv, &sUsage))
	{
		printf(sUsage.GetBuffer());
		return -1;
	}

	// Retrieve main information
	Path sInput = kParser.Get<String>(ARGUMENT_INPUT);
	String sOutput = kParser.Get<String>(ARGUMENT_OUTPUT);

	AssetCooker* pCooker = nullptr;
	if(sInput.GetExtension() == "fbx") pCooker = new FbxCooker();
	
	// Format not supported
	if(!pCooker)
	{
		printf("Format not supported");
		return -1;
	}

	// Serialize the file
	if(!pCooker->Execute(sInput.GetFull().GetBuffer(), sOutput.GetBuffer()))
	{
		printf("Failed to serialize geometry");
		return -1;
	}
	delete pCooker;	

	return 0;
}