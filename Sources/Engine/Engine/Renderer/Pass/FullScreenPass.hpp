#ifndef _ENGINE_FULLSCREEN_PASS_HPP_
#define _ENGINE_FULLSCREEN_PASS_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/RenderPass.hpp>
#include <Renderer/RenderBuffer.hpp>
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
	class RenderTarget;
	class Texture2D;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class FullScreenPass : public Renderer::RenderPass
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
		FullScreenPass();
		~FullScreenPass();
		DECLARE_RENDER_PASS(FullScreenPass);

		struct Command
		{
			DECLARE_RENDER_BUFFER(FullScreenPass::Command)
			Renderer::Texture2D*	Source;
		};

		virtual void Initialize		(Renderer::RenderContext* _pRenderContext) override;
		virtual void Release		(Renderer::RenderContext* _pRenderContext) override;

		virtual void Execute		(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pFrameBuffer) override;
		void			ExecuteCommand(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Command* pCommand);

	private:
		RenderFilter*	m_aFilter[EFilter::Count];
		Renderer::RenderTarget* m_pRenderTarget;
	};
}}

#endif