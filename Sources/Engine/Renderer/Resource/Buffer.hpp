#ifndef _RENDERER_BUFFER_HPP_
#define _RENDERER_BUFFER_HPP_

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
	class RenderContext;
	class RenderTarget;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Buffer : public Resource
	{
		friend class Renderer;
		friend class RenderContext;
		friend class RenderTarget;

		struct EView
		{
			enum Type : uint8
			{
				ShaderResource = 0,
				UnorderedAccess,

				Count,
			};
		};

	public:
		DECLARE_RESOURCE(EResource::Buffer);

	private:
		Buffer();
		~Buffer();

		void Initialize	(uint32 _uiElementSize, uint32 _uiElementCount, void* _pHandle);
		void Release	();
		
		inline void*	GetHandle		() const { return m_pHandle; }

		inline void*	GetView	(EView::Type _eView) { return m_aView[_eView]; }
		inline void		SetView	(EView::Type _eView, void* _pView) { m_aView[_eView] = _pView; }

	public:
		inline uint32	GetElementSize	() const { return m_uiElementSize; }
		inline uint32	GetElementCount	() const { return m_uiElementCount; }
		inline uint32	GetSize			() const { return m_uiElementSize * m_uiElementCount; }

	private:
		uint32		m_uiElementSize;
		uint32		m_uiElementCount;
		void*		m_pHandle;
		void*		m_aView[EView::Count];
	};
}}

#endif