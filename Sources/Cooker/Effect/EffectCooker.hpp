#ifndef _COOKER_EFFECT_COOKER_HPP_
#define _COOKER_EFFECT_COOKER_HPP_

/******************************************************************************
**	Include
******************************************************************************/
#include <Helper/AssetCooker.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class EffectCooker : public AssetCooker
	{
	public:
		inline void		SetProjectPath(const char* _szProjectPath) { m_sProjectPath = _szProjectPath; }
		virtual bool	Serialize	(const char* _szInputPath, FileWriter& _kOutputStream) override;

	private:
		String m_sProjectPath;
	};
}
#endif