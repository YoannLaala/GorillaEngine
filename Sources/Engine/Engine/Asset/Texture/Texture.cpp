/******************************************************************************
**	Includes
******************************************************************************/
#include "Texture.hpp"
#include <Engine/Engine.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/Resource/Texture2D.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	ASSET_REGISTER(Gorilla::Engine::Texture);

	//!	@brief		Contructor
	//!	@date		2015-10-18
	Texture::Texture()
		: m_pTexture(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-10-18
	Texture::~Texture()
	{
		// Nothing to do
	}

	//!	@brief		Initialize
	//!	@date		2015-10-18
	void Texture::Initialize(StreamReader* _pStream)
	{
		uint32 uiWidth;
		_pStream->Read(uiWidth);

		uint32 uiHeight;
		_pStream->Read(uiHeight);

		uint32 uiMipMapcount;
		_pStream->Read(uiMipMapcount);

		uint8 eFormat = Renderer::EFormat::Unknown;
		_pStream->Read(eFormat);

		uint32 uiBufferSize;
		_pStream->Read(uiBufferSize);

		uint8* pBuffer = new uint8[uiBufferSize];
		_pStream->Read(pBuffer, uiBufferSize);

		m_pTexture = GetRenderer()->CreateTexture2D(uiWidth, uiHeight, uiMipMapcount, (Renderer::EFormat::Type)eFormat, Renderer::EBind::ShaderResource, Renderer::EUsage::Immutable, pBuffer);
		SAFE_DELETE(pBuffer);
	}

	//!	@brief		Release
	//!	@date		2015-10-18
	void Texture::Release()
	{
		if(m_pTexture)
		{
			GetRenderer()->DestroyResource(m_pTexture);
			m_pTexture = NULL;
		}
	}
}}	// Gorilla::Asset
