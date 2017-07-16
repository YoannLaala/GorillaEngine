#ifndef _HTML_COOKER_HPP_
#define _HTML_COOKER_HPP_

/******************************************************************************
**	Include
******************************************************************************/
#include <Helper/AssetCooker.hpp>
#include <gumbo-parser/src/gumbo.h>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class HtmlCooker : public AssetCooker
	{
	private:
		virtual bool	Serialize			(const char* _szInputPath, FileWriter& _kOutputStream) override;
		bool			Serialize			(GumboNode* _pGumboNode, FileWriter& _kOutputStream);
		bool			SerializeDocType	(GumboNode* _pGumboNode, FileWriter& _kOutputStream);
		bool			SerializeContent	(GumboNode* _pGumboNode, FileWriter& _kOutputStream);
		bool			SerializeScript		(GumboNode* _pGumboNode, FileWriter& _kOutputStream);
		bool			SerializeStyle		(GumboNode* _pGumboNode, FileWriter& _kOutputStream);
		bool			SerializeAttribute	(GumboAttribute* _pGumboAttribute, bool _bIsTagScript, bool _bIsTagStyle, bool _bIsTagImage, FileWriter& _kOutputStream);
	};

	namespace EHtmlTag
	{
		enum Type
		{
			Inline = 0,
			PreserveWhiteSpace,
			Empty,
			Image,
			Special,
			Script,
			Style,

			Count
		};

		static const char* Name[EHtmlTag::Count] = 
		{
			"|a|abbr|acronym|b|bdo|big|cite|code|dfn|em|font|i|img|kbd|nobr|s|small|span|strike|strong|sub|sup|tt|",
			"|pre|textarea|script|style|",
			"|area|base|basefont|bgsound|br|command|col|embed|event-source|frame|hr|input|keygen|link|menuitem|meta|param|source|spacer|track|wbr|",
			"|image|img|",
			"|html|body|",
			"|script|",
			"|link|",
		};

		static inline bool Is(EHtmlTag::Type _eTag, const char* _szTagName)
		{
			return strstr(Name[_eTag], _szTagName) != NULL;
		}
	}
}
#endif