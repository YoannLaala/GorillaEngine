#include <Core/Core.hpp>
#include <Core/File/FileManager.hpp>
#include <Core/Stream/FileReader.hpp>
#include <Core/Stream/FileWriter.hpp>
#include <Core/Process/ArgumentParser.hpp>
#include <Core/Process/Process.hpp>
#include <Core/Container/Dictionary.hpp>
#include <Core/Stream/Dictionary/DictionaryStream.hpp>
#include <Core/Stream/Dictionary/DictionaryStreamJson.hpp>
#include <string>

#define VERSION				"1.0.0"
#define ARGUMENT_INPUT		"--input"
#define ARGUMENT_OUTPUT		"--output"

int main(int argc, const char** argv)
{
	Gorilla::ArgumentParser kParser("Json Cooker", "Cook an json file", VERSION);
	kParser.Add(ARGUMENT_INPUT, "Define which json will be serialized", true);
	kParser.Add(ARGUMENT_OUTPUT, "Define where the asset file will be serialized", true);

	Gorilla::String sUsage;
	if(!kParser.Parse(argc, argv, &sUsage))
	{
		printf(sUsage.GetBuffer());
		return -1;
	}

	Gorilla::String sInput = kParser.Get<Gorilla::String>(ARGUMENT_INPUT);
	Gorilla::String sOutput = kParser.Get<Gorilla::String>(ARGUMENT_OUTPUT);

	Gorilla::Dictionary dFile;
	if(!dFile.Read<Gorilla::DictionaryStreamJson>(sInput.GetBuffer()))
	{
		printf("Failed to read json file");	
		return -1;
	}

	if(!dFile.Write<Gorilla::DictionaryStream>(sOutput.GetBuffer()))
	{
		printf("Failed to cook json file");
		return -1;
	}

	return 0;
}