#ifndef _ENGINE_MODULE_HPP_
#define _ENGINE_MODULE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Asset/Asset.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class Module : public Asset
	{
	public:
		Module();
		~Module();
		ASSET_DECLARE(Gorilla::Engine::Module);

	protected:
		virtual void Initialize	(StreamReader* _pStream) override;
		virtual void Release	() override;
		
	private:
		void* m_hModule;
	};
}}

#endif