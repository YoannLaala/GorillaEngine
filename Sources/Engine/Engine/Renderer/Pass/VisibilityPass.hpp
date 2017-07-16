#ifndef _ENGINE_VISIBILITY_PASS_HPP_
#define _ENGINE_VISIBILITY_PASS_HPP_

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
	class RenderContext;
	class RenderTarget;
	class RenderBuffer;
	class Buffer;
	class Camera;
	class Octree;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class VisibilityPass : public Renderer::RenderPass
	{

	public:
		VisibilityPass();
		~VisibilityPass();
		DECLARE_RENDER_PASS(VisibilityPass);

		virtual void Initialize		(Renderer::RenderContext* _pRenderContext) override;
		virtual void Release		(Renderer::RenderContext* _pRenderContext) override;

		virtual void Prepare(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, const Renderer::Camera* _pCamera, const Renderer::Octree* _pTree, Renderer::RenderBuffer* _pBuffer) override;
		virtual void Execute(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pBuffer) override;

	private:
		struct EBuffer
		{
			enum Type : uint8
			{
				Scene	= 0,
				Instance,

				Count,
			};
		};

	private:
		Renderer::Buffer*		m_aBuffer[EBuffer::Count];
	};
}}

#endif