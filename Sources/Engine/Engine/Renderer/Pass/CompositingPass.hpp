#ifndef _ENGINE_COMPOSITING_PASS_HPP_
#define _ENGINE_COMPOSITING_PASS_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/RenderPass.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class RenderFilter;
}}

namespace Gorilla { namespace Renderer
{
	class Renderer;
	class RenderTarget;
	class RenderBuffer;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class CompositingPass : public Renderer::RenderPass
	{
	public:
		CompositingPass();
		~CompositingPass();
		DECLARE_RENDER_PASS(CompositingPass);

		virtual void Initialize		(Renderer::RenderContext* _pRenderContext) override;
		virtual void Release		(Renderer::RenderContext* _pRenderContext) override;

		virtual void Execute		(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pBuffer) override;

	private:
		RenderFilter*	m_pFilter;
	};
}}

#endif