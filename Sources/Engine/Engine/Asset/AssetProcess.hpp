#ifndef _ENGINE_ASSET_PROCESS_HPP_
#define _ENGINE_ASSET_PROCESS_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>
#include <Core/String/String.hpp>
#include <Core/Container/Vector.hpp>
#include <Core/Process/Argument.hpp>
#include <Core/Process/Process.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine 
{
	class AssetProcess
	{
	public:
		AssetProcess();
		~AssetProcess();

		void	Initialize	(const char* _szExecutablePath, const char* _szParameter);
		bool	Execute		(const Vector<Argument>& _vArgument, String& sErrorOut);

		inline const String& GetPath() const { return m_kProcess.GetPath().GetFull(); }
		inline const String& GetParameter() const { return m_sParameter; }

	private:
		Process m_kProcess;
		String m_sParameter;
	};
}}
#endif