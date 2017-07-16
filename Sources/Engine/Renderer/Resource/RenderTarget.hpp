#ifndef _RENDERER_RENDER_TARGET_HPP_
#define _RENDERER_RENDER_TARGET_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/Resource/Resource.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Renderer;
	class Texture2D;
	class Texture3D;
	class Buffer;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class RenderTarget : public Resource
	{
		friend class Renderer;

		struct EEntry
		{
			enum Type : uint8
			{
				Target = 0,
				DepthStencil,
				Output,

				Count,
			};
		};

	public:
		DECLARE_RESOURCE(EResource::RenderTarget);

	private:
		RenderTarget();
		~RenderTarget();

		void Initiliaze(Renderer* _pRenderer);
		virtual void Release () override;

	public:
		inline uint32		GetTargetCount		() const { return m_aResource[EEntry::Target].GetSize(); }
		void				AddTarget			(Texture2D* _pTexture);
		inline Texture2D*	GetTarget			(uint32 _uiIndex) { return static_cast<Texture2D*>(m_aResource[EEntry::Target][_uiIndex]); }

		inline Texture2D*	GetDepthStencil		() { return static_cast<Texture2D*>(m_aResource[EEntry::DepthStencil][0]); }
		void				SetDepthStencil		(Texture2D* _pTexture);

		inline uint32		GetOutputCount		() const { return m_aResource[EEntry::Output].GetSize(); }
		void				AddOutput			(Buffer* _pBuffer);
		void				AddOutput			(Texture2D* _pTexture);
		void				AddOutput			(Texture3D* _pTexture);
		inline void*		GetOutput			(uint32 _uiIndex) { return m_aResource[EEntry::Output][_uiIndex]; }

		void				Clear				();

	private:
		inline Vector<void*>& GetVecTargetView	() { return m_aView[EEntry::Target]; }
		inline void*		GetDepthStencilView () { return m_aView[EEntry::DepthStencil].GetSize() ? m_aView[EEntry::DepthStencil][0] : NULL; }
		inline Vector<void*>& GetVecOutputView	() { return m_aView[EEntry::Output]; }

	private:
		Vector<void*>		m_aResource[EEntry::Count];
		Vector<void*>		m_aView[EEntry::Count];
		Renderer*			m_pRenderer;
	};
}}

#endif