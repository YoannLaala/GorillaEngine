#ifndef _ENGINE_GUI_PASS_HPP_
#define _ENGINE_GUI_PASS_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/RenderPass.hpp>
#include <Renderer/Resource/RenderTarget.hpp>
#include <Engine/Common.hpp>

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
	class Texture2D;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class GuiPass : public Renderer::RenderPass
	{
		struct EFilter
		{
			enum Type : uint8
			{
				Identity = 0,
				ABGR2RGBA,
				Count,
			};
		};

	public:
		GuiPass();
		~GuiPass();
		DECLARE_RENDER_PASS(GuiPass);

		virtual void Initialize		(Renderer::RenderContext* _pRenderContext) override;
		virtual void Release		(Renderer::RenderContext* _pRenderContext) override;

		virtual void Execute		(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pFrameBuffer) override;

	private:
		RenderFilter*	m_aFilter[EFilter::Count];
		Renderer::RenderTarget* m_pRenderTarget;
	};
}}

#endif