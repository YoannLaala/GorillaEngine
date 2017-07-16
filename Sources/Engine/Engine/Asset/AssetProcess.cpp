/******************************************************************************
**	Includes
******************************************************************************/
#include "AssetProcess.hpp"
#include <Core/File/FileManager.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	//!	@brief		Constructor
	//!	@date		2015-11-21
	AssetProcess::AssetProcess()
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-11-21
	AssetProcess::~AssetProcess()
	{
		// Nothing to do
	}

	//!	@brief		Initialize
	//!	@date		2015-11-21
	void AssetProcess::Initialize(const char* _szExecutablePath, const char* _szParameter)
	{
		m_kProcess.Initialize(_szExecutablePath);
		m_sParameter = _szParameter;
	}

	//!	@brief		Execute
	//!	@date		2015-11-21
	bool AssetProcess::Execute(const Vector<Argument>& _vArgument, String& sErrorOut)
	{
		// Complete the command line
		String sArgument = m_sParameter;
		const uint32 uiArgumentCount = _vArgument.GetSize();
		for(uint32 uiArgument = 0; uiArgument < uiArgumentCount; ++uiArgument)
		{
			const Argument& kArgument = _vArgument[uiArgument];
			sArgument.Replace(kArgument.GetPattern().GetBuffer(), kArgument.Get().GetBuffer());
		}
		Path::Format(sArgument);

		m_kProcess.SetArgument(sArgument.GetBuffer());
		return m_kProcess.Execute(&sErrorOut);
	}
}}