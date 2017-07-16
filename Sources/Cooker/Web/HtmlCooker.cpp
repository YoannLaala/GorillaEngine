/******************************************************************************
**	Includes
******************************************************************************/
#include "HtmlCooker.hpp"
#include <Core/File/FileManager.hpp>
#include <regex>
#include <iterator>
/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
/* 
   base64.cpp and base64.h

   Copyright (C) 2004-2017 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/

//#include "base64.h"

static const String base64_chars("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

void base64_encode(const char* bytes_to_encode, unsigned int in_len, String& sValueOut) 
{
	sValueOut.Set(""); 
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        sValueOut += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] =   char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      sValueOut += base64_chars[char_array_4[j]];

    while((i++ < 3))
      sValueOut += '=';

  }
}

	//!	@brief		EncodeFile
	//!	@date		2015-12-05
	bool EncodeFile(const char* _szFilePath, String& _sEncodedOut)
	{
		// Read the file
		FileReader kFileToEncode(_szFilePath);
		if(!kFileToEncode.CanRead())
		{
			printf("Failed to encode file '%s'", _szFilePath);
			return false;
		}

		// Convert it
		uint32 uiDecodedSize = kFileToEncode.GetSize();
		char* pBufferToEncode = new char[uiDecodedSize];
		kFileToEncode.Read(pBufferToEncode, uiDecodedSize);
		kFileToEncode.Close();

		// Encode file
		base64_encode(pBufferToEncode, uiDecodedSize, _sEncodedOut);
		SAFE_DELETE_ARRAY(pBufferToEncode);

		// Check extension file
		Path sUrlPath(_szFilePath);
		if(sUrlPath.GetExtension() == "png")		_sEncodedOut.Prepend("data:image/png;base64,");
		else if(sUrlPath.GetExtension() == "gif")	_sEncodedOut.Prepend("data:image/gif;base64,");
		else if(sUrlPath.GetExtension() == "svg")	_sEncodedOut.Prepend("data:image/svg+xml;base64,");
		else if(sUrlPath.GetExtension() == "jpg")	_sEncodedOut.Prepend("data:image/jpg;base64,");
		else if(sUrlPath.GetExtension() == "jpeg")	_sEncodedOut.Prepend("data:image/jpg;base64,");
		else if(sUrlPath.GetExtension() == "ttf")	_sEncodedOut.Prepend("data:application/x-font-ttf;base64,");
		else if(sUrlPath.GetExtension() == "otf")	_sEncodedOut.Prepend("data:application/x-font-opentype;base64,");
		else if(sUrlPath.GetExtension() == "woff")	_sEncodedOut.Prepend("data:application/font-woff;base64,");
		else if(sUrlPath.GetExtension() == "woff2") _sEncodedOut.Prepend("data:application/font-woff2;base64,");
		else if(sUrlPath.GetExtension() == "eot")	_sEncodedOut.Prepend("data:application/vnd.ms-fontobject;base64,");
		else if(sUrlPath.GetExtension() == "sfnt")	_sEncodedOut.Prepend("data:application/font-sfnt;base64,");
		
		return true;
	}

	//!	@brief		IncludeFile
	//!	@date		2015-12-05
	void IncludeFile(const char* _szFilePath, FileWriter& _kOutputStream)
	{
		FileReader kFile(_szFilePath);
		if(!kFile.CanRead())
		{
			printf("Failed to include file '%s'", _szFilePath);
			return;
		}

		// Read Script file
		Path kPath = _szFilePath;
		uint32 uiSize = kFile.GetSize();
		char* pBuffer = new char[uiSize];
		kFile.Read(pBuffer, uiSize);
		kFile.Close();

		// Encode all dependencies to base64
		char* pCursor = pBuffer;
		std::regex base_regex("url\\('*(.*?)'*\\)");
		std::regex_iterator<char*> it = std::regex_iterator<char*>(pBuffer, pBuffer + uiSize, base_regex);
		std::regex_iterator<char*> itEnd = std::regex_iterator<char*>();
		while(it != itEnd)
		{
			std::match_results<char*> match = *it;
			String sPattern = match[0].str().c_str();
			String sFileName = match[1].str().c_str();

			// Avoid dependencies already encoded
			bool bIsEncoded = sFileName.Find("data:") != (uint32)-1;
			bool bIsVariable = sFileName.Find("+") != (uint32)-1;
			if(!bIsEncoded && !bIsVariable)
			{
				const uint32 uiPosition = Math::Min<uint32>(sFileName.Find("#"), sFileName.Find("?")); // Sometime character '?' can define some behavior with the url (css ?#iefix)
				if(uiPosition != (uint32)-1) sFileName.Resize(uiPosition);

				// Build absolute path
				String sFilePath(kPath.GetDirectory());
				sFilePath.Append(sFileName);
				Path::Format(sFilePath);

				String sEncoded;
				if(!EncodeFile(sFilePath.GetBuffer(), sEncoded))
				{
					++it; 
					continue;
				}

				// Write encoded file
				uint32 uiOriginalLength = (uint32)match.position() - (uint32)(pCursor - pBuffer);
				_kOutputStream.Write(pCursor, uiOriginalLength);
				_kOutputStream.Write("url(", 4);
				_kOutputStream.Write(sEncoded.GetBuffer(), sEncoded.GetLength());
				_kOutputStream.Write(')');
				pCursor = pCursor + uiOriginalLength + sPattern.GetLength();
			}

			++it;
		}

		// Write it to output
		uiSize -= (uint32)(pCursor-pBuffer);
		_kOutputStream.Write(pCursor, uiSize);
		SAFE_DELETE_ARRAY(pBuffer);
	}


	//!	@brief		Serialize
	//!	@date		2015-12-05
	bool HtmlCooker::Serialize(const char* _szInputPath, FileWriter& _kOutputStream)
	{
		// Open html file
		FileReader kFileReader(_szInputPath);
		if(!kFileReader.CanRead())
		{
			return false;
		}

		// Copy it to a buffer
		uint32 uiSize = kFileReader.GetSize();
		char* pBuffer = new char[uiSize+1];
		kFileReader.Read(pBuffer, uiSize);
		pBuffer[uiSize] = 0;
		kFileReader.Close();

		// Parse this buffer
		GumboOutput* pGumbo = gumbo_parse(pBuffer);
		bool bResult = Serialize(pGumbo->document, _kOutputStream);
		gumbo_destroy_output(&kGumboDefaultOptions, pGumbo);
		SAFE_DELETE(pBuffer);

		return bResult;
	}

	//!	@brief		GetTagName
	//!	@date		2015-12-05
	void GetTagName(GumboNode* _pGumboNode, String& sTagNameOut)
	{
		// Work around lack of proper name for document node
		if (_pGumboNode->type == GUMBO_NODE_DOCUMENT) 
		{
			sTagNameOut = "document";
		} 
		else 
		{
			sTagNameOut = gumbo_normalized_tagname(_pGumboNode->v.element.tag);
		}

		// Final check
		if (sTagNameOut.IsEmpty()) 
		{
			GumboStringPiece& sOriginalTagName = _pGumboNode->v.element.original_tag;
			if (sOriginalTagName.data) 
			{
				gumbo_tag_from_original_text(&sOriginalTagName);
				sTagNameOut.Set(sOriginalTagName.data, (uint32)sOriginalTagName.length);
			}
		}
	}

	//!	@brief		Serialize
	//!	@date		2015-12-05
	bool HtmlCooker::Serialize(GumboNode* _pGumboNode, FileWriter& _kOutputStream)
	{
		// Special case the document node
		if (_pGumboNode->type == GUMBO_NODE_DOCUMENT) 
		{
			SerializeDocType(_pGumboNode, _kOutputStream);
			SerializeContent(_pGumboNode, _kOutputStream);
			return true;
		}

		// Get Information about the tag
		String sTagName, sTagKey;
		GetTagName(_pGumboNode, sTagName);
		sTagKey.Set('|').Append(sTagName).Append('|');

		//bool bIsTagSpecial = HtmlTag::Is(HtmlTag::Special, sTagKey.GetBuffer());
		bool bIsTagEmpty = EHtmlTag::Is(EHtmlTag::Empty, sTagKey.GetBuffer());
		bool bIsTagScript = EHtmlTag::Is(EHtmlTag::Script, sTagKey.GetBuffer());
		bool bIsTagStyle = EHtmlTag::Is(EHtmlTag::Style, sTagKey.GetBuffer());
		bool bIsTagImage = EHtmlTag::Is(EHtmlTag::Image, sTagKey.GetBuffer());

		// Force tag style for style element
		if(bIsTagStyle)
		{
			sTagName = "style";
		}

		_kOutputStream.Write("<", 1);
		_kOutputStream.Write(sTagName.GetBuffer(), sTagName.GetLength());

		// Serialize Attributes
		GumboVector& vGumbottribute = _pGumboNode->v.element.attributes;
		uint32 uiAttributeCount = vGumbottribute.length;
		for(uint32 uiAttribute = 0; uiAttribute < uiAttributeCount; ++uiAttribute) 
		{
			GumboAttribute* pGumboAttribute = static_cast<GumboAttribute*>(vGumbottribute.data[uiAttribute]);
			SerializeAttribute(pGumboAttribute, bIsTagScript, bIsTagStyle, bIsTagImage, _kOutputStream);
		}

		// Determine closing tag type - Force Script & Style to be not empty
		if (!bIsTagScript && !bIsTagStyle && bIsTagEmpty) 
		{
			_kOutputStream.Write("/", 1);
		} 
		else 
		{
			_kOutputStream.Write(">", 1);

			// Serialize Content
			bool bResult = false;
			if(bIsTagScript)
			{
				bResult = SerializeScript(_pGumboNode, _kOutputStream);
			}
			else if(bIsTagStyle)
			{
				bResult = SerializeStyle(_pGumboNode, _kOutputStream);
			}
			else
			{
				bResult = SerializeContent(_pGumboNode, _kOutputStream);
			}

			// Check result
			if(!bResult)
			{
				printf("Failed to Serialize Tag '%s'", sTagName.GetBuffer());
				return false;
			}

			_kOutputStream.Write("</", 2);
			_kOutputStream.Write(sTagName.GetBuffer(), sTagName.GetLength());
		}
		_kOutputStream.Write(">", 1);

		return true;
	}

	//!	@brief		SerializeDocType
	//! @details	Serialize the DOCTYPE tag
	//!	@date		2015-12-05
	bool HtmlCooker::SerializeDocType(GumboNode* _pGumboNode, FileWriter& _kOutputStream)
	{
		static String sDocType;

		const GumboDocument& kGumboDocument = _pGumboNode->v.document;
		if (kGumboDocument.has_doctype) 
		{
			sDocType.Set("<!DOCTYPE ");
			sDocType.Append(kGumboDocument.name);
			if (strlen(kGumboDocument.public_identifier) > 0) 
			{
				sDocType.Append(kGumboDocument.public_identifier);
				sDocType.Append(" PUBLIC \"");
				sDocType.Append(kGumboDocument.public_identifier);
				sDocType.Append("\" \"");
				sDocType.Append(kGumboDocument.system_identifier);
				sDocType.Append("\"");
			}
			sDocType.Append(">");
			_kOutputStream.Write(sDocType.GetBuffer(), sDocType.GetLength());
		}

		return true;
	}

	//!	@brief		SerializeContent
	//! @details	Serialize content of the balise
	//!	@date		2015-12-05
	bool HtmlCooker::SerializeContent(GumboNode* _pGumboNode, FileWriter& _kOutputStream)
	{
		static String sContent;

		// build up result for each child, recursively if need be
		GumboVector& vGumboChild = _pGumboNode->v.element.children;
		uint32 uiChildCount = vGumboChild.length;
		for (uint32 uiChild = 0; uiChild < uiChildCount; ++uiChild) 
		{
			GumboNode* pChildNode = static_cast<GumboNode*>(vGumboChild.data[uiChild]);

			if (pChildNode->type == GUMBO_NODE_TEXT) 
			{
				sContent.Set(pChildNode->v.text.text);
				_kOutputStream.Write(sContent.GetBuffer(), sContent.GetLength());

			} 
			else if (pChildNode->type == GUMBO_NODE_ELEMENT || pChildNode->type == GUMBO_NODE_TEMPLATE) 
			{
				Serialize(pChildNode, _kOutputStream);
			} 
			else if (pChildNode->type == GUMBO_NODE_WHITESPACE) 
			{
				// Keep all whitespace to keep as close to original as possible
				sContent.Set(pChildNode->v.text.text);
				_kOutputStream.Write(sContent.GetBuffer(), sContent.GetLength());

			}
			else if (pChildNode->type != GUMBO_NODE_COMMENT) 
			{
				printf("Unknown element of type %d", pChildNode->type);
				return false;
			}
		}

		return true;
	}

	//!	@brief		SerializeScript
	//! @details	Read the script file and includes it
	//!	@date		2015-12-05
	bool HtmlCooker::SerializeScript(GumboNode* _pGumboNode, FileWriter& _kOutputStream)
	{
		// Search for src attribute Attributes
		GumboVector& vGumbottribute = _pGumboNode->v.element.attributes;
		uint32 uiAttributeCount = vGumbottribute.length;
		for(uint32 uiAttribute = 0; uiAttribute < uiAttributeCount; ++uiAttribute) 
		{
			GumboAttribute* pGumboAttribute = static_cast<GumboAttribute*>(vGumbottribute.data[uiAttribute]);
			if(_strnicmp(pGumboAttribute->name, "src", 3) == 0)
			{
				// Write Script File to main file
				String sScriptPath;
				sScriptPath.Set(GetInputPath().GetDirectory()).Append(pGumboAttribute->value);

				// Only include files present on disk (skip all reference with http://)
				if(FileManager::IsFileExist(sScriptPath.GetBuffer()))
				{
					IncludeFile(sScriptPath.GetBuffer(), _kOutputStream);
				}
				
				return true;
			}
		}

		// No file to include
		return SerializeContent(_pGumboNode, _kOutputStream);
	}

	//!	@brief		SerializeStyle
	//! @details	Read the css file and includes it
	//!	@date		2015-12-05
	bool HtmlCooker::SerializeStyle(GumboNode* _pGumboNode, FileWriter& _kOutputStream)
	{
		// Search for src attribute Attributes
		GumboVector& vGumbottribute = _pGumboNode->v.element.attributes;
		uint32 uiAttributeCount = vGumbottribute.length;
		for(uint32 uiAttribute = 0; uiAttribute < uiAttributeCount; ++uiAttribute) 
		{
			GumboAttribute* pGumboAttribute = static_cast<GumboAttribute*>(vGumbottribute.data[uiAttribute]);
			if(_strnicmp(pGumboAttribute->name, "href", 4) == 0)
			{
				// Write Script File to main file
				String sStylePath;
				sStylePath.Set(GetInputPath().GetDirectory()).Append(pGumboAttribute->value);

				// Only include files present on disk (skip all reference with http://)
				if(FileManager::IsFileExist(sStylePath.GetBuffer()))
				{
					IncludeFile(sStylePath.GetBuffer(), _kOutputStream);
				}

				break;
			}
		}

		// No file to include
		return SerializeContent(_pGumboNode, _kOutputStream);
	}

	//!	@brief		SerializeAttribute
	//! @details	Serialize all the attribute of the balise
	//!	@date		2015-12-05
	bool HtmlCooker::SerializeAttribute(GumboAttribute* _pGumboAttribute, bool _bIsTagScript, bool _bIsTagStyle, bool _bIsTagImage, FileWriter& _kOutputStream)
	{
		static String sAttribute;

		// Avoid src attribute for script tag
		bool bEncode = false;
		if(_bIsTagScript || _bIsTagImage)
		{
			bEncode = _strnicmp(_pGumboAttribute->name, "src", 3) == 0;
			if(_bIsTagScript && bEncode) return true;
		}

		// Avoid href attribute for style tag
		if(_bIsTagStyle)
		{
			if(_strnicmp(_pGumboAttribute->name, "href", 4) == 0)
			{
				return true;
			}
		}

		// Process Attribute
		sAttribute.Set(" ");
		sAttribute.Append(_pGumboAttribute->name);
		if (_pGumboAttribute->value ||  _pGumboAttribute->original_value.data[0] == '"' ||  _pGumboAttribute->original_value.data[0] == '\'') 
		{
			sAttribute.Append('=');

			// Determine original quote character used if it exists
			char cQuote = _pGumboAttribute->original_value.data[0];
			sAttribute.Append(cQuote);

			// Encore Data to Base64
			if(bEncode) 
			{
				String sDataPath, sEncoded;
				sDataPath.Set(GetInputPath().GetDirectory()).Append(_pGumboAttribute->value);
				EncodeFile(sDataPath.GetBuffer(), sEncoded);
				sAttribute.Append(sEncoded);
			}
			else
			{	
				sAttribute.Append(_pGumboAttribute->value);
			}
			sAttribute.Append(cQuote);
		}

		_kOutputStream.Write(sAttribute.GetBuffer(),sAttribute.GetLength());

		return true;
	}
}