#ifndef _ENGINE_EFFECT_HPP_
#define _ENGINE_EFFECT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Asset/Effect/Shader.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Effect;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class Effect : public Asset
	{
	public:
		Effect();
		~Effect();
		ASSET_DECLARE(Gorilla::Engine::Effect);

	protected:
		virtual void Initialize	(StreamReader* _pStream) override;
		virtual void Release	() override;
	
	public:
		inline Renderer::Effect*	GetData() { return m_pEffect; }

	private:
		Renderer::Effect*			m_pEffect;
		Vector<AssetHandle<Shader>>	m_vShader;
	};
}}

#endif