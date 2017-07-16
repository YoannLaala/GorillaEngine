//#ifndef _ENGINE_POST_PROCESS_PASS_HPP_
//#define _ENGINE_POST_PROCESS_PASS_HPP_
//
///******************************************************************************
//**	Includes
//******************************************************************************/
//#include <Renderer/RenderPass.hpp>
//#include <Engine/Asset/AssetHandle.hpp>
//#include <Engine/Asset/Effect/Effect.hpp>
//#include <Engine/Common.hpp>
//
///******************************************************************************
//**	Forward Declarations
//******************************************************************************/
//namespace Gorilla { namespace Renderer
//{
//	class Renderer;
//	class RenderContext;
//	class RenderTarget;
//	class Texture2D;
//	class Buffer;
//	class Geometry;
//	class Material;
//}}
//
///******************************************************************************
//**	Class Declaration
//******************************************************************************/
//namespace Gorilla { namespace Engine
//{
//	class PostProcessPass : public Renderer::RenderPass
//	{
//	public:
//		PostProcessPass();
//		~PostProcessPass();
//		DECLARE_RENDER_PASS(PostProcessPass);
//
//		virtual void Initialize		(Renderer::RenderContext* _pRenderContext) override;
//		virtual void Release		(Renderer::RenderContext* _pRenderContext) override;
//
//		virtual void Execute		(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, void* _pCommandArray, uint32 _uiCommandCount) override;
//	};
//}}
//
//#endif