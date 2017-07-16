/******************************************************************************
**	Includes
******************************************************************************/
#include "AssetCooker.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Constructor
	//!	@date		2015-11-22
	AssetCooker::AssetCooker()
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-11-22
	AssetCooker::~AssetCooker()
	{
		// Nothing to do
	}

	//!	@brief		Execute
	//!	@date		2015-11-22
	bool AssetCooker::Execute(const char* _szInputPath, const char* _szOutputPath)
	{
		// Prepare output
		FileWriter kFileOutput(_szOutputPath);
		if(!kFileOutput.CanWrite())
		{
			printf("Failed open '%' to write asset", _szOutputPath);
			return false;
		}
		m_kInputPath = _szInputPath;
		m_kOutputPath = _szOutputPath;

		bool bResult = Serialize(_szInputPath, kFileOutput);
		kFileOutput.Close();

		return bResult;
	}
	
	//!	@brief		WriteDependency
	//!	@date		2015-12-05
	void AssetCooker::WriteDependency(const String& _sFilePath, const String& _sParam, FileWriter& _kOutputStream)
	{
		_kOutputStream.Write(_sFilePath);
		_kOutputStream.Write(_sParam);
	}
}
