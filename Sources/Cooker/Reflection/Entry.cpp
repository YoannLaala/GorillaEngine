#include <Core/Core.hpp>
#include <Core/Process/ArgumentParser.hpp>
#include "ClangCooker.hpp"

#define VERSION				"1.0.0"
#define ARGUMENT_INPUT		"-input"
#define ARGUMENT_OUTPUT		"-output"

using namespace Gorilla;

//!	@brief		Entry
//!	@date		2015-11-22
int main(int argc, const char** argv) 
{
	ArgumentParser kParser("Reflection Cooker", "Serialize a shader file", VERSION);
	kParser.Add(ARGUMENT_INPUT, "Define which unit to extract", true);
	kParser.Add(ARGUMENT_OUTPUT, "Define where extracted informations will be written", true);

	String sUsage;
	if(!kParser.Parse(argc, argv, &sUsage))
	{
		printf(sUsage.GetBuffer());
		return -1;
	}

	// Retrieve argument
	String sInput = kParser.Get<String>(ARGUMENT_INPUT);
	String sOutput = kParser.Get<String>(ARGUMENT_OUTPUT);

	// Cook it
	Gorilla::ClangCooker kCooker;
	if(!kCooker.Execute(sInput.GetBuffer(), sOutput.GetBuffer()))
	{
		printf("Failed to make reflection");
		return -1;
	}

	return 0;
}
