/******************************************************************************
**	Includes
******************************************************************************/
#include "Shader.hpp"

#include <Engine/Engine.hpp>
#include <Renderer/Renderer.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	ASSET_REGISTER(Gorilla::Engine::Shader);

	//!	@brief		Contructor
	//!	@date		2015-10-18
	Shader::Shader()
		: m_pShader(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-10-18
	Shader::~Shader()
	{
		// Nothing to do
	}

	//!	@brief		Initialize
	//!	@date		2015-10-18
	void Shader::Initialize(StreamReader* _pStream)
	{
		// Read Shader 
		uint8 eShader;
		_pStream->Read(eShader);

		uint32 uiSize;
		_pStream->Read(uiSize);

		uint8* pBuffer = new uint8[uiSize];
		_pStream->Read(pBuffer, uiSize);

		// Create shader
		switch(eShader)
		{
			case Renderer::EShader::Vertex:
			{
				// Read Parameter
				uint32 uiParameterCount;
				_pStream->Read(uiParameterCount);
				Renderer::Shader::Parameter* pParameterArray = new Renderer::Shader::Parameter[uiParameterCount];
				_pStream->Read(pParameterArray, uiParameterCount * sizeof(Renderer::Shader::Parameter));

				m_pShader = GetRenderer()->CreateVertexShader(pBuffer, uiSize, pParameterArray, uiParameterCount);

				SAFE_DELETE(pParameterArray);
				break;
			}

			case Renderer::EShader::Geometry:
			{
				m_pShader = GetRenderer()->CreateGeometryShader(pBuffer, uiSize);
				break;
			}

			case Renderer::EShader::Pixel:
			{
				m_pShader = GetRenderer()->CreatePixelShader(pBuffer, uiSize);
				break;
			}

			case Renderer::EShader::Compute:
			{
				m_pShader = GetRenderer()->CreateComputeShader(pBuffer, uiSize);
				break;
			}
		}
		SAFE_DELETE(pBuffer);
	}

	//!	@brief		Release
	//!	@date		2015-10-18
	void Shader::Release()
	{
		if(m_pShader)
		{
			GetRenderer()->DestroyResource(m_pShader);
			m_pShader = NULL;
		}
	}
}}