#ifndef _COOKER_ASSET_COOKER_HPP_
#define _COOKER_ASSET_COOKER_HPP_

/******************************************************************************
**	Include
******************************************************************************/
#include <Core/Core.hpp>
#include <Core/Stream/FileReader.hpp>
#include <Core/Stream/FileWriter.hpp>
#include <Core/Log/LogManager.hpp>
#include <Helper/Enum.hpp>
#include <string>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class AssetCooker
	{
	protected:
		AssetCooker();
		~AssetCooker();

	public:
		bool			Execute				(const char* _szInputPath, const char* _szOutputPath);

	private:
		virtual bool	Serialize			(const char* _szInputPath, FileWriter& _kOutputStream) = 0;

	protected:
		inline const Path& GetInputPath		() const { return m_kInputPath; }
		inline const Path& GetOutputPath	() const { return m_kOutputPath; }

	protected:
		void			WriteDependency		(const String& _sFilePath, const String& _sParam, FileWriter& _kOutputStream);

	private:
		Path m_kInputPath;
		Path m_kOutputPath;
	};
}
#endif