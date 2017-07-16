#ifndef _RENDERER_EFFECT_HPP_
#define _RENDERER_EFFECT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/Resource/Resource.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Renderer;
	class RenderContext;
	class RenderState;
	class Shader;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Effect : public Resource
	{
		friend class Renderer;
		friend class RenderContext;

	private:
		Effect();
		~Effect();
		DECLARE_RESOURCE(EResource::Effect);

		void Initialize	(RenderState* _pRenderState, ETopology::Type _eTopology);
		void Release	();

	public:
		inline Shader*	GetShader(EShader::Type _eShader, ETechnique::Type _eTechnique = ETechnique::Default) { return m_aShader[_eTechnique][_eShader]; }
		inline void		SetShader(EShader::Type _eShader, Shader* _pShader, ETechnique::Type _eTechnique = ETechnique::Default) { m_aShader[_eTechnique][_eShader] = _pShader; }

		inline RenderState*			GetRenderState		() { return m_pRenderState; }
		inline ETopology::Type		GetTopology			() { return m_eTopology; }	

	private:
		Shader* m_aShader[ETechnique::Count][EShader::Count];
		RenderState* m_pRenderState;
		ETopology::Type m_eTopology;
	};
}}

#endif