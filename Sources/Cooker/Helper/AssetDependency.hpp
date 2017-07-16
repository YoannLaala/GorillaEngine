//#ifndef _DDS_FILE_HPP_
//#define _DDS_FILE_HPP_
//
///******************************************************************************
//**	Include
//******************************************************************************/
//#include <Core/Core.hpp>
//#include <Core/Stream/FileReader.hpp>
//#include <Core/Stream/FileWriter.hpp>
//#include <Windows.h>
//
///******************************************************************************
//**	Class Declaration
//******************************************************************************/
//namespace Gorilla
//{
//	class DDSFile
//	{
//	public:
//		/******************************************************************************
//		**	Flags
//		******************************************************************************/
//		struct EFormat
//		{
//			enum Type
//			{
//				Header		= 0x20534444,	// DDS
//				FourCC		= 0x00000004,	// DDPF_FOURCC
//				RGB			= 0x00000040,	// DDPF_RGB
//				RGBA		= 0x00000041,	// DDPF_RGB | DDPF_ALPHAPIXELS
//				Luminance	= 0x00020000,	// DDPF_LUMINANCE
//				LuminanceA	= 0x00020001,	// DDPF_LUMINANCE | DDPF_ALPHAPIXELS
//				Alpha		= 0x00000002,	// DDPF_ALPHA
//				PAL8		= 0x00000020,	// DDPF_PALETTEINDEXED8
//			};
//		};
//
//		struct EHeader
//		{
//			enum Type
//			{
//				Texture		= 0x00001007,	// DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT 
//				MipMap		= 0x00020000,	// DDSD_MIPMAPCOUNT
//				Volume		= 0x00800000,	// DDSD_DEPTH
//				Pitch		= 0x00000008,	// DDSD_PITCH
//				Linearsize	= 0x00080000,	// DDSD_LINEARSIZE
//			};
//		};
//
//		struct ESurface
//		{
//			enum Type
//			{
//				Texture		= 0x00001000,	// DDSCAPS_TEXTURE
//				MipMap		= 0x00400008,	// DDSCAPS_COMPLEX | DDSCAPS_MIPMAP
//				CubeMap		= 0x00000008,	// DDSCAPS_COMPLEX
//			};
//		};
//
//		struct ECubeMap
//		{
//			enum Type
//			{
//				PositiveX	= 0x00000600,	// DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
//				NegativeX	= 0x00000a00,	// DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
//				PositiveY	= 0x00001200,	// DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
//				NegativeY	= 0x00002200,	// DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
//				PositiveZ	= 0x00004200,	// DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
//				NegativeZ	= 0x00008200,	// DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ
//				AllFaces	= PositiveX | NegativeX | PositiveY | NegativeY | PositiveZ | NegativeZ
//			};
//		};
//
//		struct ECaps
//		{
//			enum Type
//			{
//				CubeMap		= 0x00000200,	// DDSCAPS2_CUBEMAP
//				Volume		= 0x00200000,	// DDSCAPS2_VOLUME
//			};
//		};
//
//		/******************************************************************************
//		**	Texture Format
//		******************************************************************************/
//		class PixelFormat
//		{
//		public:
//
//			inline uint32 GetSize			() const { return m_uiSize; }
//			inline uint32 GetRGBBitCount	() const { return m_uiRGBBitCount; }
//			inline uint32 GetFourCC			() const { return m_uiFourCC; }
//
//			inline bool HasFlag(uint32 _uiFlag) const { return (m_uiFlag & _uiFlag) != 0; }
//			inline bool HasFourCC(char _ch0, char _ch1, char _ch2, char _ch3) const { return m_uiFourCC == MAKEFOURCC(_ch0, _ch1, _ch2, _ch3); }
//			inline bool HasMask(uint32 _uiRBitMask, uint32 _uiGBitMask, uint32 _uiBBitMask, uint32 _uiABitMask) const
//			{
//				return m_uiRBitMask == _uiRBitMask && m_uiGBitMask == _uiGBitMask && m_uiBBitMask == _uiBBitMask && m_uiABitMask == _uiABitMask;
//			}
//
//		private:
//			uint32 m_uiSize;
//			uint32 m_uiFlag;
//			uint32 m_uiFourCC;
//			uint32 m_uiRGBBitCount;
//			uint32 m_uiRBitMask;
//			uint32 m_uiGBitMask;
//			uint32 m_uiBBitMask;
//			uint32 m_uiABitMask;
//		};
//
//		/******************************************************************************
//		**	Texture Header
//		******************************************************************************/
//		class Header
//		{
//		public:
//			inline uint32 GetSize			() const { return m_uiSize; }
//			inline uint32 GetFlag			() const { return m_uiFlag; }
//			inline uint32 GetWidth			() const { return m_uiWidth; }
//			inline uint32 GetHeight			() const { return m_uiHeight; }
//			inline uint32 GetDepth			() const { return m_uiDepth; }
//			inline uint32 GetMipMapCount	() const { return m_uiMipMapCount; }
//			inline const PixelFormat& GetPixelFormat() const { return m_kPixelFormat; }
//			inline uint32 GetCaps2			() const { return m_uiCaps2; }
//
//			inline bool HasFlag(uint32 _uiFlag) const { return (m_uiFlag & _uiFlag) != 0; }
//
//		private:
//			uint32 m_uiSize;
//			uint32 m_uiFlag;
//			uint32 m_uiHeight;
//			uint32 m_uiWidth;
//			uint32 m_uiPitchOrLinearSize;
//			uint32 m_uiDepth;				// Only if DDS_HEADER_FLAGS_VOLUME is set in flags
//			uint32 m_uiMipMapCount;
//			uint32 m_uiReserved1[11];
//			PixelFormat m_kPixelFormat;
//			uint32 m_uiCaps;
//			uint32 m_uiCaps2;
//			uint32 m_uiCaps3;
//			uint32 m_uiCaps4;
//			uint32 m_uiReserved2;
//		};
//
//		/******************************************************************************
//		**	Core Functions
//		******************************************************************************/
//		static bool ConvertStream(FileReader& _kInputStream, FileWriter& _kOutputStream);
//	};
//}
//#endif