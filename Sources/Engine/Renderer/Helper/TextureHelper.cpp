/******************************************************************************
**	Includes
******************************************************************************/
#include "TextureHelper.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		GetMipMapInfo
	//!	@param		_uiWidth the width mip
	//!	@param		_uiHeight the height mip
	//!	@param		_eFormat the image format
	//!	@param		_uiRowByteOut the number of byte per row
	//!	@param		_uiMipMapByteOut the number byte for the whole mip
	//!	@date		2015-11-22
	void TextureHelper::GetSurfaceInfo(uint32 _uiWidth, uint32 _uiHeight, EFormat::Type _eFormat, uint32& _uiRowByteOut, uint32& _uiSurfaceByteOut)
	{
		// Get Information on Block Compression
		bool bIsBlockCompresion = false;
		uint32 uiBytePerBlock = 0;
		bool bIsPacked = false;
		switch (_eFormat)
		{
			case EFormat::BC1_TYPELESS:
			case EFormat::BC1_UNORM:
			case EFormat::BC1_UNORM_SRGB:
			case EFormat::BC4_TYPELESS:
			case EFormat::BC4_UNORM:
			case EFormat::BC4_SNORM:
			{
				bIsBlockCompresion = true;
				uiBytePerBlock = 8;
				break;
			}

			case EFormat::BC2_TYPELESS:
			case EFormat::BC2_UNORM:
			case EFormat::BC2_UNORM_SRGB:
			case EFormat::BC3_TYPELESS:
			case EFormat::BC3_UNORM:
			case EFormat::BC3_UNORM_SRGB:
			case EFormat::BC5_TYPELESS:
			case EFormat::BC5_UNORM:
			case EFormat::BC5_SNORM:
			case EFormat::BC6H_TYPELESS:
			case EFormat::BC6H_UF16:
			case EFormat::BC6H_SF16:
			case EFormat::BC7_TYPELESS:
			case EFormat::BC7_UNORM:
			case EFormat::BC7_UNORM_SRGB:
			{
				bIsBlockCompresion = true;
				uiBytePerBlock = 16;
				break;
			}

			case EFormat::R8G8_B8G8_UNORM:
			case EFormat::G8R8_G8B8_UNORM:
			{
				bIsPacked = true;
				break;
			}
		}

		// Fill all size needed
		uint32 uiRowCount;
		if (bIsBlockCompresion)
		{
			uint32 uiBlockWide = Math::Max<uint32>(1, (_uiWidth + 3) / 4);
			_uiRowByteOut = uiBlockWide * uiBytePerBlock;
			uiRowCount = Math::Max<uint32>(1, (_uiHeight + 3) / 4);
		}
		else if (bIsPacked)
		{
			_uiRowByteOut = ((_uiWidth + 1) >> 1) * 4;
			uiRowCount = _uiHeight;
		}
		else
		{
			uint32 uiBitPerPixel = GetBitPerPixel(_eFormat);

			// Round up to nearest byte
			_uiRowByteOut = (_uiWidth * uiBitPerPixel + 7) / 8;
			uiRowCount = _uiHeight;
		}

		_uiSurfaceByteOut = _uiRowByteOut * uiRowCount;
	}	

	//!	@brief		GetBitPerPixel
	//!	@param		_eFormat the image format
	//!	@return		the number of bit per pixel
	//!	@date		2015-11-22
	uint32 TextureHelper::GetBitPerPixel(EFormat::Type _eFormat)
	{
		switch(_eFormat)
		{
			case EFormat::R32G32B32A32_TYPELESS:
			case EFormat::R32G32B32A32_FLOAT:
			case EFormat::R32G32B32A32_UINT:
			case EFormat::R32G32B32A32_SINT:
			{
				return 128;
			}

			case EFormat::R32G32B32_TYPELESS:
			case EFormat::R32G32B32_FLOAT:
			case EFormat::R32G32B32_UINT:
			case EFormat::R32G32B32_SINT:
			{
				return 96;
			}

			case EFormat::R16G16B16A16_TYPELESS:
			case EFormat::R16G16B16A16_FLOAT:
			case EFormat::R16G16B16A16_UNORM:
			case EFormat::R16G16B16A16_UINT:
			case EFormat::R16G16B16A16_SNORM:
			case EFormat::R16G16B16A16_SINT:
			case EFormat::R32G32_TYPELESS:
			case EFormat::R32G32_FLOAT:
			case EFormat::R32G32_UINT:
			case EFormat::R32G32_SINT:
			case EFormat::R32G8X24_TYPELESS:
			case EFormat::D32_FLOAT_S8X24_UINT:
			case EFormat::R32_FLOAT_X8X24_TYPELESS:
			case EFormat::X32_TYPELESS_G8X24_UINT:
			{
				return 64;
			}

			case EFormat::R10G10B10A2_TYPELESS:
			case EFormat::R10G10B10A2_UNORM:
			case EFormat::R10G10B10A2_UINT:
			case EFormat::R11G11B10_FLOAT:
			case EFormat::R8G8B8A8_TYPELESS:
			case EFormat::R8G8B8A8_UNORM:
			case EFormat::R8G8B8A8_UNORM_SRGB:
			case EFormat::R8G8B8A8_UINT:
			case EFormat::R8G8B8A8_SNORM:
			case EFormat::R8G8B8A8_SINT:
			case EFormat::R16G16_TYPELESS:
			case EFormat::R16G16_FLOAT:
			case EFormat::R16G16_UNORM:
			case EFormat::R16G16_UINT:
			case EFormat::R16G16_SNORM:
			case EFormat::R16G16_SINT:
			case EFormat::R32_TYPELESS:
			case EFormat::D32_FLOAT:
			case EFormat::R32_FLOAT:
			case EFormat::R32_UINT:
			case EFormat::R32_SINT:
			case EFormat::R24G8_TYPELESS:
			case EFormat::D24_UNORM_S8_UINT:
			case EFormat::R24_UNORM_X8_TYPELESS:
			case EFormat::X24_TYPELESS_G8_UINT:
			case EFormat::R9G9B9E5_SHAREDEXP:
			case EFormat::R8G8_B8G8_UNORM:
			case EFormat::G8R8_G8B8_UNORM:
			case EFormat::B8G8R8A8_UNORM:
			case EFormat::B8G8R8X8_UNORM:
			case EFormat::R10G10B10_XR_BIAS_A2_UNORM:
			case EFormat::B8G8R8A8_TYPELESS:
			case EFormat::B8G8R8A8_UNORM_SRGB:
			case EFormat::B8G8R8X8_TYPELESS:
			case EFormat::B8G8R8X8_UNORM_SRGB:
			{
				return 32;
			}

			case EFormat::R8G8_TYPELESS:
			case EFormat::R8G8_UNORM:
			case EFormat::R8G8_UINT:
			case EFormat::R8G8_SNORM:
			case EFormat::R8G8_SINT:
			case EFormat::R16_TYPELESS:
			case EFormat::R16_FLOAT:
			case EFormat::D16_UNORM:
			case EFormat::R16_UNORM:
			case EFormat::R16_UINT:
			case EFormat::R16_SNORM:
			case EFormat::R16_SINT:
			case EFormat::B5G6R5_UNORM:
			case EFormat::B5G5R5A1_UNORM:
			case EFormat::B4G4R4A4_UNORM:
			{
				return 16;
			}

			case EFormat::R8_TYPELESS:
			case EFormat::R8_UNORM:
			case EFormat::R8_UINT:
			case EFormat::R8_SNORM:
			case EFormat::R8_SINT:
			case EFormat::A8_UNORM:
			{
				return 8;
			}

			case EFormat::R1_UNORM:
			{
				return 1;
			}

			case EFormat::BC1_TYPELESS:
			case EFormat::BC1_UNORM:
			case EFormat::BC1_UNORM_SRGB:
			case EFormat::BC4_TYPELESS:
			case EFormat::BC4_UNORM:
			case EFormat::BC4_SNORM:
			{
				return 4;
			}

			case EFormat::BC2_TYPELESS:
			case EFormat::BC2_UNORM:
			case EFormat::BC2_UNORM_SRGB:
			case EFormat::BC3_TYPELESS:
			case EFormat::BC3_UNORM:
			case EFormat::BC3_UNORM_SRGB:
			case EFormat::BC5_TYPELESS:
			case EFormat::BC5_UNORM:
			case EFormat::BC5_SNORM:
			case EFormat::BC6H_TYPELESS:
			case EFormat::BC6H_UF16:
			case EFormat::BC6H_SF16:
			case EFormat::BC7_TYPELESS:
			case EFormat::BC7_UNORM:
			case EFormat::BC7_UNORM_SRGB:
			{
				return 8;
			}

			default:
			{
				return 0;
			}
		}
	}
}}