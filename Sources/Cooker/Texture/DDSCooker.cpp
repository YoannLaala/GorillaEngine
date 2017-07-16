/******************************************************************************
**	Includes
******************************************************************************/
#include "DDSCooker.hpp"
#include <Renderer/Common.hpp>
#include <Renderer/Helper/TextureHelper.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		GetFormat
	//!	@date		2015-11-22
	Renderer::EFormat::Type GetTextureFormat(const DDSCooker::PixelFormat& _kFormat)
	{
		// RGB
		if(_kFormat.HasFlag(DDSCooker::EFormat::RGB))
		{
			// Check the total of bit occuped
			switch(_kFormat.GetRGBBitCount())
			{
				case 32:
				{
					if(_kFormat.HasMask(0x000000ff,0x0000ff00,0x00ff0000,0xff000000))
					{
						return Renderer::EFormat::R8G8B8A8_UNORM;
					}

					if(_kFormat.HasMask(0x00ff0000,0x0000ff00,0x000000ff,0xff000000))
					{
						return Renderer::EFormat::B8G8R8A8_UNORM;
					}

					if(_kFormat.HasMask(0x00ff0000,0x0000ff00,0x000000ff,0x00000000))
					{
						return Renderer::EFormat::B8G8R8X8_UNORM;
					}

					// No DXGI format maps to (0x000000ff,0x0000ff00,0x00ff0000,0x00000000) aka D3DFMT_X8B8G8R8
					if(_kFormat.HasMask(0x3ff00000,0x000ffc00,0x000003ff,0xc0000000))
					{
						return Renderer::EFormat::R10G10B10A2_UNORM;
					}

					// No DXGI format maps to (0x000003ff,0x000ffc00,0x3ff00000,0xc0000000) aka D3DFMT_A2R10G10B10
					if(_kFormat.HasMask(0x0000ffff,0xffff0000,0x00000000,0x00000000))
					{
						return Renderer::EFormat::R16G16_UNORM;
					}

					if(_kFormat.HasMask(0xffffffff,0x00000000,0x00000000,0x00000000))
					{
						return Renderer::EFormat::R32_FLOAT;
					}

					break;
				}

				// No 24bpp DXGI formats aka D3DFMT_R8G8B8
				case 24:
				{
					break;
				}

				case 16:
				{
					if(_kFormat.HasMask(0x7c00,0x03e0,0x001f,0x8000))
					{
						return Renderer::EFormat::B5G5R5A1_UNORM;
					}

					if(_kFormat.HasMask(0xf800,0x07e0,0x001f,0x0000))
					{
						return Renderer::EFormat::B5G6R5_UNORM;
					}

					// No DXGI format maps to (0x7c00,0x03e0,0x001f,0x0000) aka D3DFMT_X1R5G5B5
					if(_kFormat.HasMask(0x0f00,0x00f0,0x000f,0xf000))
					{
						return Renderer::EFormat::B4G4R4A4_UNORM;
					}

					break;
				}
			}
		}
		// Luminance
		else if(_kFormat.HasFlag(DDSCooker::EFormat::Luminance))
		{
			switch(_kFormat.GetRGBBitCount())
			{
				case 8:
				{
					if(_kFormat.HasMask(0x000000ff,0x00000000,0x00000000,0x00000000))
					{
						return Renderer::EFormat::R8_UNORM;
					}

					break;
				}

				case 16:
				{
					if (_kFormat.HasMask(0x0000ffff,0x00000000,0x00000000,0x00000000))
					{
						return Renderer::EFormat::R16_UNORM;
					}

					if (_kFormat.HasMask(0x000000ff,0x00000000,0x00000000,0x0000ff00))
					{
						return Renderer::EFormat::R8G8_UNORM;
					}

					break;
				}
			}
		}
		// Alpha
		else if(_kFormat.HasFlag(DDSCooker::EFormat::Alpha))
		{
			if(_kFormat.GetRGBBitCount())
			{
				return Renderer::EFormat::A8_UNORM;
			}
		}
		// FourCC
		else if(_kFormat.HasFlag(DDSCooker::EFormat::FourCC))
		{
			// Check first value
			switch(_kFormat.GetFourCC())
			{
				// D3DFMT_A16B16G16R16
				case 36: 
				{
					return Renderer::EFormat::R16G16B16A16_UNORM;
				}

				// D3DFMT_Q16W16V16U16
				case 110: 
				{
					return Renderer::EFormat::R16G16B16A16_SNORM;
				}

				// D3DFMT_R16F
				case 111: 
				{
					return Renderer::EFormat::R16_FLOAT;
				}

				// D3DFMT_G16R16F
				case 112: 
				{
					return Renderer::EFormat::R16G16_FLOAT;
				}

				// D3DFMT_A16B16G16R16F
				case 113: 
				{
					return Renderer::EFormat::R16G16B16A16_FLOAT;
				}

				// D3DFMT_R32F
				case 114: 
				{
					return Renderer::EFormat::R32_FLOAT;
				}

				// D3DFMT_G32R32F
				case 115: 
				{
					return Renderer::EFormat::R32G32_FLOAT;
				}

				// D3DFMT_A32B32G32R32F
				case 116: 
				{
					return Renderer::EFormat::R32G32B32A32_FLOAT;
				}
			}

			// Basic FourCC not defined
			if(_kFormat.HasFourCC('D', 'X', 'T', '1'))
			{
				return Renderer::EFormat::BC1_UNORM;
			}

			if(_kFormat.HasFourCC('D', 'X', 'T', '3'))
			{
				return Renderer::EFormat::BC2_UNORM;
			}

			if(_kFormat.HasFourCC('D', 'X', 'T', '5'))
			{
				return Renderer::EFormat::BC3_UNORM;
			}

			if(_kFormat.HasFourCC('D', 'X', 'T', '2'))
			{
				return Renderer::EFormat::BC2_UNORM;
			}

			if(_kFormat.HasFourCC('D', 'X', 'T', '4'))
			{
				return Renderer::EFormat::BC3_UNORM;
			}

			if(_kFormat.HasFourCC('A', 'T', 'I', '1'))
			{
				return Renderer::EFormat::BC4_UNORM;
			}

			if (_kFormat.HasFourCC('B', 'C', '4', 'U'))
			{
				return Renderer::EFormat::BC4_UNORM;
			}

			if(_kFormat.HasFourCC('B', 'C', '4', 'S'))
			{
				return Renderer::EFormat::BC4_SNORM;
			}

			if(_kFormat.HasFourCC('A', 'T', 'I', '2'))
			{
				return Renderer::EFormat::BC5_UNORM;
			}

			if(_kFormat.HasFourCC('B', 'C', '5', 'U'))
			{
				return Renderer::EFormat::BC5_UNORM;
			}

			if(_kFormat.HasFourCC('B', 'C', '5', 'S'))
			{
				return Renderer::EFormat::BC5_SNORM;
			}

			if(_kFormat.HasFourCC('R', 'G', 'B', 'G'))
			{
				return Renderer::EFormat::R8G8_B8G8_UNORM;
			}

			if(_kFormat.HasFourCC('G', 'R', 'G', 'B'))
			{
				return Renderer::EFormat::G8R8_G8B8_UNORM;
			}
		}

		return Renderer::EFormat::Unknown;
	}

	//!	@brief		Serialize
	//!	@date		2015-11-22
	bool DDSCooker::Serialize(const char* _szInputPath, FileWriter& _kOutputStream)
	{
		// Read input file and then conver it to Gorilla Format
		Gorilla::FileReader kFileInput(_szInputPath);
		if(!kFileInput.CanRead())
		{
			printf("Failed open '%' to read input", _szInputPath);
			return false;
		}

		bool bResult = Serialize(kFileInput, _kOutputStream);
		kFileInput.Close();

		return bResult;
	}


	//!	@brief		ConvertStream
	//!	@date		2015-11-22
	bool DDSCooker::Serialize(FileReader& _kInputStream, FileWriter& _kOutputStream)
	{
		// Check header
		uint32 uiFileExtension;
		_kInputStream.Read(uiFileExtension);
		if(uiFileExtension != 0x20534444)
		{
			printf("[DDSFile] Format not supported");
			return false;
		}

		// Verify header to validate DDS file
		Header kHeader;
		_kInputStream.Read(&kHeader, sizeof(Header));
		if (kHeader.GetSize() != sizeof(Header) || kHeader.GetPixelFormat().GetSize() != sizeof(PixelFormat))
		{
			printf("[DDSFile] Incoherency in size of Header");
			return false;
		}

		// Generic Information
		Renderer::EDimension::Type eDimension;
		uint32 uiWidth = kHeader.GetWidth();
		uint32 uiHeight = kHeader.GetHeight();
		uint32 uiMipMapCount = kHeader.GetMipMapCount() == 0 ? 1 : kHeader.GetMipMapCount();
		uint32 uiArrayCount = 1;
		uint32 uiDepth = kHeader.GetDepth();
		uint32 uiMiscFlag = 0;
		bool bIsCubeMap = false;

		// DXGI Format
		uint8 eFormat = GetTextureFormat(kHeader.GetPixelFormat());
		if(eFormat == Renderer::EFormat::Unknown)
		{
			printf("[DDSFile] Format unknown");
			return false;
		}

		// Volume
		if(kHeader.HasFlag(EHeader::Volume))
		{
			eDimension = Renderer::EDimension::Texture3D;
		}
		else 
		{
			// Cube Map
			if(kHeader.GetCaps2() & ECaps::CubeMap)
			{
				// We require all six faces to be defined
				if((kHeader.GetCaps2() & ECubeMap::AllFaces) == 0)
				{
					printf("[DDSFile] Texture flagged as CubeMap without all faces");
					return false;
				}

				uiArrayCount = 6;
				bIsCubeMap = true;
				uiMiscFlag |= D3D11_RESOURCE_MISC_TEXTURECUBE;
			}

			uiDepth = 1;
			eDimension = Renderer::EDimension::Texture2D;
		}

		// Check Error Size depending on the dimension
		switch(eDimension)
		{
			case Renderer::EDimension::Texture1D:
			{
				if((kHeader.GetWidth() > D3D11_REQ_TEXTURE1D_U_DIMENSION) || (uiArrayCount > D3D11_REQ_TEXTURE1D_ARRAY_AXIS_DIMENSION))
				{
					printf("[Texture] Texture2D CubeMap Dimension not supported [W:%d][A:%d] (MAX [W:%d][A:%d])", kHeader.GetWidth(), uiArrayCount, D3D11_REQ_TEXTURE1D_U_DIMENSION, D3D11_REQ_TEXTURE1D_ARRAY_AXIS_DIMENSION);
					return NULL;
				}
				
				break;
			}
			case Renderer::EDimension::Texture2D:
			{
				if(bIsCubeMap)
				{
					// This is the right bound because we set arraySize to (NumCubes*6) above
					if((kHeader.GetWidth() > D3D11_REQ_TEXTURECUBE_DIMENSION) || (kHeader.GetHeight() > D3D11_REQ_TEXTURECUBE_DIMENSION) || (uiArrayCount > D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION))
					{
						printf("[Texture] Texture2D as CubeMap Dimension not supported [W:%d][H:%d][A:%d] (MAX [W:%d][H:%d][A:%d])", kHeader.GetWidth(), kHeader.GetHeight(), uiArrayCount, D3D11_REQ_TEXTURECUBE_DIMENSION, D3D11_REQ_TEXTURECUBE_DIMENSION, D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION);
						return NULL;
					}
				}
				else if((kHeader.GetWidth() > D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION) || (kHeader.GetHeight() > D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION) || (uiArrayCount > D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION))
				{
					printf("[Texture] Texture2D Dimension not supported [W:%d][H:%d][A:%d] (MAX [W:%d][H:%d][A:%d])", kHeader.GetWidth(), kHeader.GetHeight(), uiArrayCount, D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION, D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION, D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION);
					return NULL;
				}

				break;
			}
			case Renderer::EDimension::Texture3D:
			{
				if((kHeader.GetWidth() > D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION) || (kHeader.GetHeight() > D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION) || (uiDepth > D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION) || (uiArrayCount != 6))
				{
					printf("[Texture] Texture3D Dimension not supported [W:%d][H:%d][D:%d][A:%d] (MAX [W:%d][H:%d][D:%d][A:%d])", kHeader.GetWidth(), kHeader.GetHeight(), kHeader.GetDepth(), uiArrayCount, D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION, D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION, D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION, 6);
					return NULL;
				}
				break;
			}
		}

		// Read DDS data
		const uint32 uiInputPosition = _kInputStream.GetPosition();
		const uint32 uiInputSize = _kInputStream.GetSize();
		const uint32 uiBufferSize = uiInputSize - uiInputPosition;
		uint8* pData = new uint8[uiBufferSize];
		_kInputStream.Read(pData, uiBufferSize);

		// Write to output stream		
		_kOutputStream.Write(uiWidth);
		_kOutputStream.Write(uiHeight);
		//_kOutputStream.Write(uiArrayCount);
		_kOutputStream.Write(uiMipMapCount);
		_kOutputStream.Write(eFormat);

		// Take in account
		// uiBufferSize represent the size of the complete image
		// uiMipMapInfoSize reprensent the 2 uint32 per mip to know the surface and 
		uint32 uiMipMapInfoSize = uiArrayCount * uiMipMapCount * (2 * sizeof(uint32));
		_kOutputStream.Write(uiBufferSize + uiMipMapInfoSize);

		uint32 uiIndex = 0;
		uint8* pDataStart = pData;
		for(uint32 uiArray = 0; uiArray < uiArrayCount; ++uiArray)
		{
			uint32 uiWidthMip = uiWidth, uiHeightMip = uiHeight;
			for(uint32 uiMipMap = 0; uiMipMap < uiMipMapCount; ++uiMipMap)
			{
				uint32 uiRowByte, uiSurfaceByte;
				Renderer::TextureHelper::GetSurfaceInfo(uiWidthMip, uiHeightMip, (Renderer::EFormat::Type)eFormat, uiRowByte, uiSurfaceByte);
				_kOutputStream.Write(uiSurfaceByte);
				_kOutputStream.Write(uiRowByte);
				_kOutputStream.Write(pDataStart, uiSurfaceByte);

				pDataStart += uiSurfaceByte;

				// Increment  MipMap
				uiWidthMip = uiWidthMip >> 1;
				uiHeightMip = uiHeightMip >> 1;
				if (uiWidthMip == 0)
				{
					uiWidthMip = 1;
				}
				if (uiHeightMip == 0)
				{
					uiHeightMip = 1;
				}

				++uiIndex;
			}
		}

		return true;
	}
}
