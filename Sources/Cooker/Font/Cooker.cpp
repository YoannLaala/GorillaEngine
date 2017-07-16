#include <Common/Common.hpp>
#include <Common/FileSystem.hpp>
#include <Common/Misc/JsonHelper.hpp>

#include <Renderer/Asset/Font.hpp>

#include <regex>

// Convert int from String
uint32 ConvertSubStringToUInt32(const char* _szText, uint32 _uiLength)
{
	char szInteger[10] = { 0 };
	memcpy_s(szInteger, 10, _szText, _uiLength);
	return (uint32)atoi(szInteger);
}

// Used to build the CharacterMap
void AddCharacterWithSlashes(String& _sText, char _cValue)
{
	switch(_cValue)
	{
		// Need escapde character
		case '@':
		case '%':
		case '"':
		{
			_sText += "\\";
			_sText += _cValue;
			break;
		}

		// ImageMagick needs 4 '\' to process one...
		case '\\':
		{
			_sText += "\\\\\\";
			_sText += _cValue;
			break;
		}

		default:
		{
			_sText += _cValue;
			break;
		}
	}
}

int main(int argc, const char** argv) 
{
	ThreadManager kThreadManager;

	// File System
	Common::FileSystem kFileSystem;
	String sImageMagicConvert = kFileSystem.GetDirectory() + "../ImageMagick/convert.exe";
	String sTextureConverter = kFileSystem.GetDirectory() + "../Texture/nvdxt.exe";
	Path kOutput =  kFileSystem.GetDirectory();

	// Build full param without -output argument
	String sParams = "";
	Common::ArgumentList kArgumentList;
	kArgumentList.Parse(argc, argv);
	for(uint32 uiArgument = 0; uiArgument < kArgumentList.GetSize(); ++uiArgument)
	{
		const Common::ArgumentList::Argument& kArgument = kArgumentList[uiArgument];
		if(kArgument.Match("-output"))
		{
			kOutput = kArgument.GetString();
		}
		else
		{
			sParams += kArgument.m_sPattern + " " + kArgument.GetString() + " ";
		}
	}

	// Prepare Convert from ImageMagick
	Common::Process kImageMagickConvert;
	kImageMagickConvert.Init(sImageMagicConvert.GetStr(), "$ALL_CHARACTER $PARAMS -trim info:-");
	kImageMagickConvert.GetArgumentList().Add("$PARAMS", sParams.GetStr());
	kImageMagickConvert.GetArgumentList().Add("$ALL_CHARACTER", "");
	String& sAllCharacter = kImageMagickConvert.GetArgumentList()[1].m_sValue;

	// Get information about all character
	uint32 uiCharacter = 32, uiCharacterEnd = 127, uiCharacterProcess = 0; 
	Vector<PantherRenderer::GlyphInfo> vecGlyphInfo;
	for(; uiCharacter < uiCharacterEnd; ++uiCharacter)
	{ 
		// Character '"' is specific - ImageMagick need to have this one at the end to process
		if(uiCharacter != '"')
		{
			sAllCharacter += " label:\"";
			AddCharacterWithSlashes(sAllCharacter, uiCharacter);
			sAllCharacter += "\"";
		}

		// Only 64 character at a time due to ImageMagick...
		uiCharacterProcess++;
		if(uiCharacterProcess == 64 || uiCharacter == uiCharacterEnd-1)
		{
			// Character '\\' is specific - ImageMagick need to have this one at the end to process
			if(uiCharacter == uiCharacterEnd-1)
			{
				sAllCharacter += " label:\"";
				AddCharacterWithSlashes(sAllCharacter, '"');
				sAllCharacter += "\"";
			}

			// Get information about the character
			kImageMagickConvert.Execute();
	
			uint32 uiStartLineIndex = 0;
			uint32 uiEndLineIndex = 0;
			while(uiStartLineIndex != kImageMagickConvert.GetOutput().GetLength())
			{
				// Find end of line
				uiEndLineIndex = kImageMagickConvert.GetOutput().Find("\n", uiStartLineIndex);
				uiEndLineIndex = (uiEndLineIndex == (uint32)-1) ? kImageMagickConvert.GetOutput().GetLength() : uiEndLineIndex-1;

				// Search pattern (label:a[1] LABEL 6x5 8x12+0+4 16-bit sRGB 0.031u 0:00.010)
				// label:(.*)[[] : Represent the character 'a'
				// .*[ ]([0-9]+)[x]([0-9]+) : Represent the character with trim (6x5)
				// [ ]([0-9]+)[x]([0-9]+).* : Represent the character without trim(8x12)
				std::cmatch kMatchResult;
				const char* szStart = &kImageMagickConvert.GetOutput()[uiStartLineIndex];
				std::regex_match(szStart, szStart + (uiEndLineIndex - uiStartLineIndex), kMatchResult, std::regex("label:(.*)[[].*[ ]([0-9]+)[x]([0-9]+)[ ]([0-9]+)[x]([0-9]+).*"));
				uiStartLineIndex = uiEndLineIndex + 2; // Add 2 character (\r\n)

				if(kMatchResult.size())
				{
					PantherRenderer::GlyphInfo& kGlyphInfo = vecGlyphInfo.Add();
					kGlyphInfo.m_uiId = (uint32)kMatchResult[1].str().c_str()[kMatchResult[1].str().length()-1];
					kGlyphInfo.m_vSize.Set((float32)ConvertSubStringToUInt32(kMatchResult[2].str().c_str(), kMatchResult[2].length()), (float32)ConvertSubStringToUInt32(kMatchResult[5].str().c_str(), kMatchResult[5].length()));

					// More Space for ' ' (space character)
					if(kGlyphInfo.m_uiId == ' ')
					{
						kGlyphInfo.m_vSize.x *= 5;
					}
				}
			}

			uiCharacterProcess = 0;
			sAllCharacter = "";
		}
	}	

	// Compute pixel Square needed and Row Height
	uint32 uiLineHeight = 0;
	uint32 uiPixelSquared = 0;
	for(uint32 uiGlyphInfo = 0; uiGlyphInfo < vecGlyphInfo.GetSize(); ++uiGlyphInfo)
	{
		PantherRenderer::GlyphInfo& kGlyphInfo = vecGlyphInfo[uiGlyphInfo];
		uiPixelSquared += (uint32)kGlyphInfo.m_vSize.x * (uint32)kGlyphInfo.m_vSize.y;

		// Find row height
		if(uiLineHeight < kGlyphInfo.m_vSize.y)
		{
			uiLineHeight = (uint32)kGlyphInfo.m_vSize.y;
		}
	}

	// Determine Texture Size 128 * 128 by Default
	uint32 uiTextureBitOffset = 6, uiTextureSize = 0, uiTextureSizeSquared = 0;
	while(uiTextureSizeSquared < uiPixelSquared)
	{
		uiTextureSize = 1 << ++uiTextureBitOffset;
		uiTextureSizeSquared = uiTextureSize*uiTextureSize;
	}

	// Build argument for converter and texcoord
	Math::Vector2 vCurcor;
	String sCharacterMap;
	for(uint32 uiGlyphInfo = 0; uiGlyphInfo < vecGlyphInfo.GetSize(); ++uiGlyphInfo)
	{
		PantherRenderer::GlyphInfo& kGlyphInfo = vecGlyphInfo[uiGlyphInfo];

		// Handle end line
		if(vCurcor.x + kGlyphInfo.m_vSize.x > uiTextureSize)
		{
			vCurcor.x = 0;
			vCurcor.y += uiLineHeight;
		}

		// Finalize GlyphInfo and continue to build the character map
		kGlyphInfo.m_vTexcoord.Set(vCurcor.x, vCurcor.y, kGlyphInfo.m_vSize.x, kGlyphInfo.m_vSize.y);
		kGlyphInfo.ScaleTexcoord(uiTextureSize);

		// Build Charachter Offset
		sCharacterMap += " -page +";
		sCharacterMap += (uint32)vCurcor.x;
		sCharacterMap += "+";
		sCharacterMap += (uint32)vCurcor.y;
		sCharacterMap += " label:\"";
		AddCharacterWithSlashes(sCharacterMap, kGlyphInfo.m_uiId);
		sCharacterMap += "\"";

		vCurcor.x += kGlyphInfo.m_vSize.x + 1; 
	}

	// Final texture name
	String sTextureOutputDDS = kOutput.GetDirectory() + kOutput.GetFile() + ".dds";
	String sTextureOutputPNG = kOutput.GetDirectory() + kOutput.GetFile() + ".png";

	// Build png image
	kImageMagickConvert.Init(sImageMagicConvert.GetStr(), "$PARAMS -background none $CHARACTER_MAP -layers merge -background none -extent $SIZEx$SIZE \"$OUTPUT\"");
	kImageMagickConvert.GetArgumentList().Add("$PARAMS", sParams.GetStr());
	kImageMagickConvert.GetArgumentList().Add("$SIZE", String(uiTextureSize).GetStr());
	kImageMagickConvert.GetArgumentList().Add("$CHARACTER_MAP", sCharacterMap.GetStr());
	kImageMagickConvert.GetArgumentList().Add("$OUTPUT", sTextureOutputPNG.GetStr());
	kImageMagickConvert.Execute();

	// Convert to DDS (Better Quality than using direcly ImageMagick)
	Common::Process kNVDXT;
	kNVDXT.Init(sTextureConverter.GetStr(), "-file $INPUT -outdir $OUTPUT_DIRECTORY -overwrite");
	kNVDXT.GetArgumentList().Add("$INPUT", sTextureOutputPNG.GetStr());
	kNVDXT.GetArgumentList().Add("$OUTPUT_DIRECTORY", kOutput.GetDirectory().GetStr());
	kNVDXT.Execute();

	//Delete the .png File
	kFileSystem.Delete(sTextureOutputPNG.GetStr());

	// Write JSON
	rapidjson::StringBuffer kBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> kWriter(kBuffer);
	kWriter.StartObject();

		rapidjson::Writer<rapidjson::StringBuffer>& kAllCharacterJson = kWriter.String("AllGlyph").StartArray();
		for(uint32 uiCharacterInfo = 0; uiCharacterInfo < vecGlyphInfo.GetSize(); ++uiCharacterInfo)
		{
			PantherRenderer:: GlyphInfo& kGlyphInfo = vecGlyphInfo[uiCharacterInfo];

			// Serialize Character Info
			rapidjson::Writer<rapidjson::StringBuffer>& kCharacterJson = kAllCharacterJson.StartObject();
				kGlyphInfo.Serialize(kCharacterJson);
			kAllCharacterJson.EndObject();
		}
		kAllCharacterJson.EndArray();

		// Line Height, Space
		kWriter.String("LineHeight").Uint((uint32)(uiLineHeight));
		kWriter.String("LineSpace").Uint((uint32)(uiLineHeight * 0.75f));

		// Texture Dependency
		kWriter.String("Texture").String(sTextureOutputDDS.GetStr());

	kWriter.EndObject();

	// Save Font File describing each CharacterInfo
	Common::JsonHelper::WriteJson(kOutput.GetPath().GetStr(), kBuffer, false);

	return 0;
}
