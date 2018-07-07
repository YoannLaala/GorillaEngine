#include <Core/Core.hpp>
#include <Core/String/StringHelper.hpp>
#include <Core/File/FileManager.hpp>
#include <Core/Process/ArgumentParser.hpp>
#include <Core/Process/Process.hpp>

#include "DDSCooker.hpp"

#define VERSION					"1.0.0"
#define ARGUMENT_INPUT			"--input"
#define ARGUMENT_OUTPUT			"--output"
#define ARGUMENT_IRRADIANCE		"--irradiance"

using namespace Gorilla;

//!	@brief		ConvertToDDS
//!	@date		2015-11-22
bool ConvertToDDS(const char* _szInput, const char* _szOutput, bool _bCubeMap = false)
{
	// Build parameters for nvtxt
	String sArgument;
	if(_bCubeMap) sArgument.Append("-cubeMap -list ");
	else sArgument.Append("-file ");
	sArgument.Append(_szInput).Append(" -output ").Append(_szOutput).Append(" -quick -overwrite");

	// Launch process
	String sError;
	Process kProcess;
	kProcess.Initialize("nvdxt.exe", sArgument.GetBuffer());
	if(!kProcess.Execute(&sError))
	{
		printf("Failed to generate a dds %s", _szOutput);
		return false;
	}

	return true;
}

//!	@brief		Entry
//!	@date		2015-11-22
int main(int argc, const char** argv) 
{
	ArgumentParser kParser("Texture2Asset", "Serialize a texture file", VERSION);
	kParser.Add(ARGUMENT_INPUT, "Define which texture will be serialized", true);
	kParser.Add(ARGUMENT_OUTPUT, "Define where the asset file will be serialized", true);
	kParser.Add(ARGUMENT_IRRADIANCE, "Define if an irradiance map will also be generated the asset file will be serialized", false);

	String sUsage;
	if(!kParser.Parse(argc, argv, &sUsage))
	{
		printf(sUsage.GetBuffer());
		return -1;
	}

	String sTemporaryPath;
	FileManager::CreateTemporaryDirectory(sTemporaryPath);

	// Retrieve main information
	Path sInput = kParser.Get<String>(ARGUMENT_INPUT);
	String sOutput = kParser.Get<String>(ARGUMENT_OUTPUT);

	bool bCubeMap = false;
	String sInputConverted;
	if(sInput.GetExtension() == "dds") sInputConverted = sInput.GetFull();
	else if(sInput.GetExtension() == "cmb") 
	{
		String sContent;
		FileReader kReader(sInput.GetFull().GetBuffer());
		sContent.Resize(kReader.GetSize());
		kReader.Read(&sContent[0], kReader.GetSize());

		Vector<String> vTexture;
		StringHelper::Split(sContent.GetBuffer(), "\r\n", vTexture);
		if(vTexture.GetSize() < 6)
		{
			printf("Failed to generate cubemap for %s", sInput.GetFull().GetBuffer());
			return -1;
		}

		// Create list file
		String sList;
		sList.Set(sTemporaryPath).Append(sInput.GetFileName()).Append(".lst");
		FileWriter kWriter(sList.GetBuffer());
		if(!kWriter.CanWrite())
		{
			printf("Failed writer list file for cubemap ", sList.GetBuffer());
			return -1;
		}

		// Convert all faces
		String sFace;
		for(int iTexture = 0; iTexture < 6; ++iTexture)
		{
			sFace.Set(sInput.GetDirectory()).Append(vTexture[iTexture]);
			kWriter.Write(sFace.GetBuffer(), sFace.GetLength());
			kWriter.Write('\n');
		}
		kWriter.Close();

		sInput = sList;
		bCubeMap = true;
	}

	// Convert texture to dds
	if(sInputConverted.IsEmpty())
	{
		sInputConverted.Append(sTemporaryPath).Append(sInput.GetFileName()).Append(".dds");
		if(!ConvertToDDS(sInput.GetFull().GetBuffer(), sInputConverted.GetBuffer(), bCubeMap))
		{
			printf("Failed to convert dds file %s", sInputConverted.GetBuffer());
			FileManager::DeleteADirectory(sTemporaryPath.GetBuffer());
			return -1;
		}

		// Generate irradiance map
		/*if(kParser.Has(ARGUMENT_IRRADIANCE))
		{
			
		}*/
	}

	// Create final dds file
	DDSCooker kCooker;
	if(!kCooker.Execute(sInputConverted.GetBuffer(), sOutput.GetBuffer()))
	{
		printf("Failed to serialize texture");
		FileManager::DeleteADirectory(sTemporaryPath.GetBuffer());
		return -1;
	}

	FileManager::DeleteADirectory(sTemporaryPath.GetBuffer());

	return 0;
}
