#ifndef _RENDER_CANVAS_CONTEXT_HPP_
#define _RENDER_CANVAS_CONTEXT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Hash.hpp>
#include <Renderer/Common.hpp>
#include <Renderer/Color.hpp>
#include <Renderer/Canvas/Resource/CanvasResource.hpp>

/******************************************************************************
**	Define
******************************************************************************/
#undef CreateFont
#undef DrawText

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class CanvasRenderer;
	class CanvasQueue;

	class CanvasSurface;
	class CanvasImage;
	class CanvasGeometry;
	class CanvasBrush;
	class CanvasStyle;
	class CanvasFont;
	class CanvasText;

	class Texture2D;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class CanvasContext
	{
		friend class CanvasRenderer;

	public:
		CanvasContext	();
		~CanvasContext	();

		void			Initialize				(CanvasRenderer* _pRenderer);
		void			Release					();

		// Creation / Destruction
		CanvasSurface*	CreateSurface			(Texture2D* _pTexture);
		CanvasSurface*	CreateSurface			(const char* _szName, Texture2D* _pTexture);
		CanvasSurface*	GetSurface				(const char* _szName);
		void			DestroySurface			(const char* _szName);

		CanvasImage*	CreateImage				(const char* _szName, Texture2D* _pTexture);
		CanvasImage*	CreateImage				(const char* _szName, CanvasSurface* _pSurface, const char* szFilePath);
		CanvasImage*	GetImage				(const char* _szName);
		void			DestroyImage			(const char* _szName);

		CanvasBrush*	CreateBrush				(CanvasSurface* _pSurface, Color _kColor);
		CanvasBrush*	GetBrush				(Color _kColor);
		void			DestroyBrush			(const char* _szName);

		CanvasStyle*	CreateStyle				(CanvasSurface* _pSurface, ELineStyle::Type _eLine);
		CanvasStyle*	GetStyle				(ELineStyle::Type _eLine);
		void			DestroyStyle			(const char* _szName);

		CanvasGeometry*	CreateRectangle			(const char* _szName, CanvasSurface* _pSurface, float32 _fWidth, float32 _fHeight, float32 _fRadiusX = 0.0f, float32 _fRadiusY = 0.0f);
		CanvasGeometry*	GetRectangle			(const char* _szName);
		void			DestroyRectangle		(const char* _szName);

		CanvasGeometry*	CreateElipse			(const char* _szName, CanvasSurface* _pSurface, float32 _fRadiusX, float32 _fRadiusY);
		CanvasGeometry*	GetElipse				(const char* _szName);
		void			DestroyElipse			(const char* _szName);

		bool			HasFont					(const wchar_t* _szFamilyName) const;
		CanvasFont*		CreateFont				(const char* _szName, const wchar_t* _szFamilyName, float32 _fSize, uint32 _uiWeight, EFontStyle::Type _eFont);
		CanvasFont*		GetFont					(const char* _szName);
		void			DestroyFont				(const char* _szName);

		CanvasText*		CreateText				(const char* _szName, const wchar_t* _szText, float32 _fWidthMax, float32 _fHeightMax, CanvasFont* _pFont);
		CanvasText*		GetText					(const char* _szName);
		void			DestroyText				(const char* _szName);

		template <class T>
		void			DestroyResource			(T*& _pResource);

		// Push to CanvasQueue
		void			PushGeometry			(CanvasSurface* _pSurface, CanvasGeometry* _pGeometry, CanvasBrush* _pBrush, const Math::Matrix33& _mTransform);
		void			PushGeometryFilled		(CanvasSurface* _pSurface, CanvasGeometry* _pGeometry, CanvasBrush* _pBrush, const Math::Matrix33& _mTransform);
		void			PushLine				(CanvasSurface* _pSurface, CanvasBrush* _pBrush, const Math::Vector2& _vPointA, const Math::Vector2& _vPointB, CanvasStyle* _pStyle = NULL, float32 _fWidth = 1.0f);
		void			PushText				(CanvasSurface* _pSurface, CanvasText* _pText, CanvasBrush* _pBrush, const Math::Matrix33& _mTransform);

		static void		ExecuteGeometry			(CanvasRenderer* _pRenderer, CanvasSurface* _pSurface, void* _pParameter);
		static void		ExecuteGeometryFilled	(CanvasRenderer* _pRenderer, CanvasSurface* _pSurface, void* _pParameter);
		static void		ExecuteLine				(CanvasRenderer* _pRenderer, CanvasSurface* _pSurface, void* _pParameter);
		static void		ExecuteText				(CanvasRenderer* _pRenderer, CanvasSurface* _pSurface, void* _pParameter);

	private:
		static void		RenderThreadEntry		(void* _pData);
		void			RenderLoop				();

	private:
		template <class T>
		void			AddResource				(const char* _szName, T* _pResource) { return AddResource<T>(Hash::Generate(_szName), _pResource); }

		template <class T>
		void			AddResource				(uint32 _uiResourceId, T* _pResource);

		template <class T>
		inline T*		GetResource				(const char* _szName) { return GetResource<T>(Hash::Generate(_szName)); }

		template <class T>
		T*				GetResource				(uint32 _uiResourceId);

		template <class T>
		inline void		DestroyResource			(const char* _szName) { return DestroyResource<T>(Hash::Generate(_szName)); }

		template <class T>
		void			DestroyResource			(uint32 _uiResourceId);

	private:
		inline CanvasQueue* GetQueue() { return m_pQueue; }

	private:
		HashMap<uint32, CanvasResource*>	m_aResource[EResource::Count];
		CanvasRenderer*						m_pRenderer;
		CanvasQueue*						m_pQueue;

		bool								m_bIsRunning;
	};
}}

#endif