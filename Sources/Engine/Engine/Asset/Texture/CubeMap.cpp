/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Asset/Texture/CubeMap.hpp>
#include <Engine/Engine.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/Resource/Texture2D.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	ASSET_REGISTER(Gorilla::Engine::CubeMap);

	//!	@brief		Contructor
	//!	@date		2015-10-18
	CubeMap::CubeMap()
		: m_pTexture(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-10-18
	CubeMap::~CubeMap()
	{
		// Nothing to do
	}

	//!	@brief		Initialize
	//!	@date		2015-10-18
	void CubeMap::Initialize(StreamReader* _pStream)
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

		m_pTexture = GetRenderer()->CreateTextureCube(uiWidth, uiHeight, uiMipMapcount, (Renderer::EFormat::Type)eFormat, Renderer::EBind::ShaderResource, Renderer::EUsage::Immutable, pBuffer);
		SAFE_DELETE(pBuffer);
	}

	//!	@brief		Release
	//!	@date		2015-10-18
	void CubeMap::Release()
	{
		if(m_pTexture)
		{
			GetRenderer()->DestroyResource(m_pTexture);
			m_pTexture = NULL;
		}
	}
}}	// Gorilla::Asset
