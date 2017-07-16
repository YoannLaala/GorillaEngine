#ifndef _ENGINE_EDITOR_PASS_HPP_
#define _ENGINE_EDITOR_PASS_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/RenderPass.hpp>
#include <Engine/Common.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Renderer;
	class RenderTarget;
	class Viewport;
	class Texture2D;
}}

namespace Gorilla { namespace Engine
{
	class RenderFilter;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Editor
{
	class EditorPass : public Renderer::RenderPass
	{
	public:
		EditorPass();
		~EditorPass();
		DECLARE_RENDER_PASS(EditorPass);

		virtual void Initialize		(Renderer::RenderContext* _pRenderContext) override;
		virtual void Release		(Renderer::RenderContext* _pRenderContext) override;

		virtual void Execute		(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pBuffer) override;

	public:
		inline void SetRenderTarget(Renderer::RenderTarget* _pRenderTarget) { m_pRenderTarget = _pRenderTarget; }
		inline void SetTexture(Renderer::Texture2D* _pTexture) { m_pTexture = _pTexture; }
		inline void AddViewport(Renderer::Viewport* _pViewport) { m_vViewport.Add(_pViewport); }
		inline void RemoveDisplay(Renderer::Viewport* _pViewport) { m_vViewport.Remove(_pViewport); }
		inline void Clear() { m_vViewport.Clear(); m_pRenderTarget = nullptr;  }

	private:
		Vector<Renderer::Viewport*>	m_vViewport;
		Renderer::RenderTarget*		m_pRenderTarget;
		Renderer::Texture2D*		m_pTexture;
		Engine::RenderFilter*		m_pFilter;
	};
}}

#endif