#include <Core/Core.hpp>
#include <Core/Process/ArgumentParser.hpp>
#include <Core/Process/Process.hpp>

#include "HtmlCooker.hpp"

#define VERSION				"1.0.0"
#define ARGUMENT_INPUT		"--input"
#define ARGUMENT_OUTPUT		"--output"

using namespace Gorilla;

//!	@brief		Entry
//!	@date		2015-11-22
int main(int argc, const char** argv) 
{
	ArgumentParser kParser("Web Cooker", "Serialize an Html file and his dependencies", VERSION);
	kParser.Add(ARGUMENT_INPUT, "Define which html will be serialized", true);
	kParser.Add(ARGUMENT_OUTPUT, "Define where the asset file will be serialized", true);

	String sUsage;
	if(!kParser.Parse(argc, argv, &sUsage))
	{
		printf(sUsage.GetBuffer());
		return -1;
	}

	// Input and Output file
	String sInput = kParser.Get<String>(ARGUMENT_INPUT);
	String sOutput = kParser.Get<String>(ARGUMENT_OUTPUT);

	Gorilla::HtmlCooker kCooker;
	if(!kCooker.Execute(sInput.GetBuffer(), sOutput.GetBuffer()))
	{
		printf("Failed to serialize html file");
		return -1;
	}
	
	return 0;
}