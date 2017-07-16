/******************************************************************************
**	Includes
******************************************************************************/
#include "Effect.hpp"

#include <Engine/Engine.hpp>
#include <Engine/Asset/AssetHandle.hpp>
#include <Renderer/Effect.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/Resource/RenderState.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	ASSET_REGISTER(Gorilla::Engine::Effect);

	//!	@brief		Contructor
	//!	@date		2015-10-18
	Effect::Effect()
		: m_pEffect(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-10-18
	Effect::~Effect()
	{
		// Nothing to do
	}

	//!	@brief		Initialize
	//!	@date		2015-10-18
	void Effect::Initialize(StreamReader* _pStream)
	{
		// Read topology 
		uint8 eTopology;
		_pStream->Read(eTopology);

		// RenderState key
		Renderer::RenderState::Key kKey;
		_pStream->Read(&kKey, sizeof(Renderer::RenderState::Key));

		// Create Effect
		m_pEffect = GetRenderer()->CreateEffect(kKey, (Renderer::ETopology::Type)eTopology);

		// Read shader count
		uint8 uiShaderCount;
		_pStream->Read(uiShaderCount);

		// Read all shader
		for(uint32 uiShader = 0; uiShader < uiShaderCount; ++uiShader)
		{
			uint8 eTechnique;
			_pStream->Read(eTechnique);

			uint8 eShader;
			_pStream->Read(eShader);

			// Read dependency
			AssetHandle<Shader>& hShader = m_vShader.Add(); 
			ReadAsset(_pStream, hShader);
			m_pEffect->SetShader((Renderer::EShader::Type)eShader, hShader->GetData(), (Renderer::ETechnique::Type)eTechnique);
		}
	}

	//!	@brief		Release
	//!	@date		2015-10-18
	void Effect::Release()
	{
		const uint32 uiShaderCount = m_vShader.GetSize();
		for(uint32 uiShader = 0; uiShader < uiShaderCount; ++uiShader)
		{
			m_vShader[uiShader].Release();
		}

		if(m_pEffect)
		{
			GetRenderer()->DestroyResource(m_pEffect);
			m_pEffect = NULL;
		}
	}
}}