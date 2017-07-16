#pragma once

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Component/Component.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Component
{
	class ${NAME} : public Engine::Component
	{
	public:
		${NAME}();
		~${NAME}();
		DECLARE_COMPONENT(Gorilla::Component::${NAME});

	protected:
		virtual void Start			() override;
		virtual void Update			() override;
		virtual void Stop			() override;
	};
}}