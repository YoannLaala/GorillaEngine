#ifndef _COOKER_SHADER_COOKER_HPP_
#define _COOKER_SHADER_COOKER_HPP_

/******************************************************************************
**	Include
******************************************************************************/
#include <Helper/AssetCooker.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class HLSLCooker : public AssetCooker
	{
	public:
		inline void SetType(Renderer::EShader::Type _eShader) { m_eShader = _eShader; }

		virtual bool	Serialize	(const char* _szInputPath, FileWriter& _kOutputStream) override;

	private:
		Renderer::EShader::Type m_eShader;
	};
}
#endif