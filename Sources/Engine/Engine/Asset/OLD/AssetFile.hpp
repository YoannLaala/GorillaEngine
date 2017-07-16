#ifndef _ENGINE_FILE_HPP_
#define _ENGINE_FILE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>
#include <Common/JsonHelper.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace PantherRenderer
{
	// Handle an asset as a file
	class AssetFile : public Common::ISerialize
	{
	protected:
		AssetFile() {}

	public:
		~AssetFile() {}

		// Used to make dependency in files
		bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>& _kWriter)
		{
			Common::JsonHelper::Serialize(_kWriter, "FilePath", m_sRelativeFilePath);
			Common::JsonHelper::Serialize(_kWriter, "Params", m_sParams);

			return true;
		}

		bool Deserialize(const rapidjson::Value& _kDocument)
		{
			Common::JsonHelper::Deserialize(_kDocument, "FilePath", m_sRelativeFilePath);
			Common::JsonHelper::Deserialize(_kDocument, "Params", m_sParams);
			return true;
		}

		const String&	GetRelativeFilePath		() const { return m_sRelativeFilePath; }
		const String&	GetParams				() const { return m_sParams; }

	protected:
		String			m_sRelativeFilePath;
		String			m_sParams;	
	};
}

#endif