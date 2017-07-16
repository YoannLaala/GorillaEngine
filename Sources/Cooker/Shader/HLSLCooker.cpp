/******************************************************************************
**	Includes
******************************************************************************/
#include "HLSLCooker.hpp"
#include <Core/File/FileManager.hpp>
#include <Renderer/Resource/RenderState.hpp>
#include <Renderer/Resource/Shader.hpp>
#include <d3dcompiler.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Serialize
	//!	@date		2015-11-22
	bool HLSLCooker::Serialize(const char* _szInputPath, FileWriter& _kOutputStream)
	{
		FileReader kStream(_szInputPath);
		if(!kStream.CanRead())
		{
			return false;
		}

		// Read compiled Shader
		uint32 uiSize = kStream.GetSize();
		uint8* pBuffer = new uint8[uiSize];
		kStream.Read(pBuffer, uiSize);

		// Write to output
		_kOutputStream.Write(&m_eShader, sizeof(uint8));
		_kOutputStream.Write(uiSize);
		_kOutputStream.Write(pBuffer, uiSize);

		// Specific process depending on shader type
		switch(m_eShader)
		{
			case Renderer::EShader::Vertex:
			{
				// Use reflection to create the input layout associated
				ID3D11ShaderReflection* pD3D11ShaderReflection = NULL;
				HRESULT hResult = D3DReflect(pBuffer, uiSize, IID_ID3D11ShaderReflection, (void**)&pD3D11ShaderReflection);
				if(FAILED(hResult))
				{
					printf("[HLSLCooker] D3DReflect failed (HRESULT = 0x%0X)", hResult);
					return false;
				}

				// Get Shader Description
				D3D11_SHADER_DESC kD3D11ShaderDesc;
				pD3D11ShaderReflection->GetDesc(&kD3D11ShaderDesc);

				// Write the number of parameter
				_kOutputStream.Write(kD3D11ShaderDesc.InputParameters);

				// Input Layout Parameters
				for(uint32 uiInputParameter = 0; uiInputParameter < kD3D11ShaderDesc.InputParameters; ++uiInputParameter)
				{
					D3D11_SIGNATURE_PARAMETER_DESC kD3D11SignatureParameterDesc;
					hResult = pD3D11ShaderReflection->GetInputParameterDesc(uiInputParameter, &kD3D11SignatureParameterDesc);
					if(FAILED(hResult))
					{
						printf("[HLSLCooker] ID3D11ShaderReflection::GetInputParameterDesc(%d) failed (HRESULT = 0x%0X)", uiInputParameter, hResult);
						continue;
					}

					// Write the parameter
					Renderer::Shader::Parameter kParameter;
					memset(&kParameter, 0, sizeof(Renderer::Shader::Parameter));

					// Check name Size
					uint32 uiLength = (uint32)strlen(kD3D11SignatureParameterDesc.SemanticName);
					if(sizeof(kParameter.Name) <= uiLength)
					{
						printf("[HLSLCooker] SementicName %s too long with %d character (%d max)", kD3D11SignatureParameterDesc.SemanticName, uiLength, sizeof(kParameter.Name) - 1);
						return false;
					}

					strcpy_s(kParameter.Name, sizeof(kParameter.Name), kD3D11SignatureParameterDesc.SemanticName);
					kParameter.Instancing = strstr(kParameter.Name, "INSTANCE_") != NULL;
					kParameter.Index = (uint8)kD3D11SignatureParameterDesc.SemanticIndex;
					kParameter.Format = Renderer::EFormat::Unknown;
					switch(kD3D11SignatureParameterDesc.ComponentType)
					{
						case D3D_REGISTER_COMPONENT_UINT32:
						{
							switch(kD3D11SignatureParameterDesc.Mask)
							{
								case 0x01:
								{
									kParameter.Format = Renderer::EFormat::R32_UINT;
									break;
								}

								case 0x03:
								{
									kParameter.Format = Renderer::EFormat::R32G32_UINT;
									break;
								}

								case 0x07:
								{
									kParameter.Format = Renderer::EFormat::R32G32B32_UINT;
									break;
								}

								case 0x0f:
								{
									kParameter.Format = Renderer::EFormat::R32G32B32A32_UINT;
									break;
								}
							}
							break;
						}

						case D3D_REGISTER_COMPONENT_SINT32:
						{
							switch(kD3D11SignatureParameterDesc.Mask)
							{
								case 0x01:
								{
									kParameter.Format = Renderer::EFormat::R32_SINT;
									break;
								}

								case 0x03:
								{
									kParameter.Format = Renderer::EFormat::R32G32_SINT;
									break;
								}

								case 0x07:
								{
									kParameter.Format = Renderer::EFormat::R32G32B32_SINT;
									break;
								}

								case 0x0f:
								{
									kParameter.Format = Renderer::EFormat::R32G32B32A32_SINT;
									break;
								}
							}
							break;
						}

						case D3D_REGISTER_COMPONENT_FLOAT32:
						{
							switch(kD3D11SignatureParameterDesc.Mask)
							{
								case 0x01:
								{
									kParameter.Format = Renderer::EFormat::R32_FLOAT;
									break;
								}

								case 0x03:
								{
									kParameter.Format = Renderer::EFormat::R32G32_FLOAT;
									break;
								}

								case 0x07:
								{
									kParameter.Format = Renderer::EFormat::R32G32B32_FLOAT;
									break;
								}

								case 0x0f:
								{
									kParameter.Format = Renderer::EFormat::R32G32B32A32_FLOAT;
									break;
								}
							}
							break;
						}
					}

					_kOutputStream.Write(&kParameter, sizeof(Renderer::Shader::Parameter));
				}
				SAFE_RELEASE(pD3D11ShaderReflection);

				break;
			}
		}
		
		_kOutputStream.Close();

		return true;
	}
}
