#ifndef _ENGINE_SHADER_HPP_
#define _ENGINE_SHADER_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Asset/Asset.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Shader;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class Shader : public Asset
	{
	public:
		Shader();
		~Shader();
		ASSET_DECLARE(Gorilla::Engine::Shader);

	protected:
		virtual void Initialize	(StreamReader* _pStream) override;
		virtual void Release	() override;
		
	public:
		inline Renderer::Shader*		GetData() { return m_pShader; }

	private:
		Renderer::Shader*		m_pShader;
	};
}}

#endif