#ifndef _SCRIPT_OBJECT_HPP_
#define _SCRIPT_OBJECT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "Object.hpp"
#include "../Common.hpp"

/******************************************************************************
**	Forward Declarations
******************************************************************************/
#define REGISTER_SCRIPT(tType)								\
extern "C" __declspec(dllexport) void* CreateInstance()		\
{															\
	return new tType();										\
}															\

namespace PantherEngine
{
	namespace EScriptState
	{
		enum Type
		{
			Start	= 0,
			Update	= 1,
		};
	}
}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace PantherEngine
{
	class ScriptObject : public Object
	{
		friend class Script;

	public:

		ScriptObject()
			: Object()
		{
			m_eInternalState = EScriptState::Start;
		}

		~ScriptObject()
		{
			//m_pGameObject = 0;
		}

		virtual void	Start			() {}
		virtual void	Update			() {}

	private:
		EScriptState::Type	GetInternalState	() const { return m_eInternalState; }
		void				SetInternalState	(EScriptState::Type _eScriptState) { m_eInternalState = _eScriptState; }

	private:
		EScriptState::Type	m_eInternalState;
	};
}

#endif