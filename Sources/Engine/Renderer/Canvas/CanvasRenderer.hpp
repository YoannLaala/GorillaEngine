#ifndef _RENDERER_CANVAS_RENDERER_HPP_
#define _RENDERER_CANVAS_RENDERER_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/Common.hpp>
#include <Renderer/Canvas/CanvasContext.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Renderer;
	class CanvasImageFactory;
	class CanvasTextFactory;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class CanvasRenderer
	{
		friend class Renderer;

	public:
		CanvasRenderer();
		~CanvasRenderer();
		bool										Initialize				(Renderer* _pRenderer);
		void										Release					();

		void										Present					();

		CanvasContext*								AddContext				()
		{
			CanvasContext* pContext = new CanvasContext();
			pContext->Initialize(this);
			m_vContext.Add(pContext);

			return pContext;
		}

		// Creation / Destruction
		CanvasSurface*								CreateSurface			(Texture2D* _pTexture);
		CanvasImage*								CreateImage				(CanvasSurface* _pSurface, const char* szFilePath);
		CanvasImage*								CreateImage				(Texture2D* _pTexture);
		CanvasBrush*								CreateBrush				(CanvasSurface* _pSurface, Color _kColor);
		CanvasStyle*								CreateStyle				(CanvasSurface* _pSurface, ELineStyle::Type _eBorder);
		CanvasGeometry*								CreateRectangle			(CanvasSurface* _pSurface, float32 _fWidth, float32 _fHeight, float32 _fRadiusX = 0.0f, float32 _fRadiusY = 0.0f);
		CanvasGeometry*								CreateElipse			(CanvasSurface* _pSurface, float32 _fRadiusX, float32 _fRadiusY);
		
		bool										HasFont					(const wchar_t* _szFamilyName) const;
		CanvasFont*									CreateFont				(const wchar_t* _szFamilyName, float32 _fSize, uint32 _uiWeight, EFontStyle::Type _eFont);
		CanvasText*									CreateText				(const wchar_t* _szText, float32 _fWidthMax, float32 _fHeightMax, CanvasFont* _pFont = NULL);

		static void									DestroyResource			(void* _pHandle);

		// Draw
		void										DrawBegin				(CanvasSurface* _pSurface);
		void										DrawEnd					(CanvasSurface* _pSurface);
		void										DrawLine				(CanvasSurface* _pSurface, CanvasBrush* _pBrush, const Math::Vector2& _vPointA, const Math::Vector2& _vPointB, CanvasStyle* _pStyle = NULL, float32 _fWidth = 1.0f);
		void										DrawText				(CanvasSurface* _pSurface, CanvasText* _pText, CanvasBrush* _pBrush, const Math::Matrix33& _mTransform);
		void										DrawImage				(CanvasSurface* _pSurface, CanvasImage* _pImage, const Math::Matrix33& _mTransform);
		void										DrawGeometry			(CanvasSurface* _pSurface, CanvasGeometry* _pGeometry, CanvasBrush* _pBrush, const Math::Matrix33& _mTransform, float32 _fWidth, CanvasStyle* _pStyle = NULL);
		void										DrawGeometry			(CanvasSurface* _pSurface, CanvasGeometry* _pGeometry, CanvasBrush* _pBrush, const Math::Matrix33& _mTransform);

	private:
		void*					m_pDevice;
		void*					m_pDeviceContext;
		CanvasImageFactory*		m_pImageFactory;
		CanvasTextFactory*		m_pTextFactory;		
		Vector<CanvasContext*>	m_vContext;
	};

	//!	@brief		AddContext
	//!	@date		2015-12-30
	/*CanvasContext* CanvasRenderer::AddContext()
	{
		CanvasContext* pContext = new CanvasContext();
		pContext->Initialize(this);
		m_vContext.Add(pContext);

		return pContext;
	}*/
}}

#endif