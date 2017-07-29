/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Renderer/Pass/VisibilityPass.hpp>

#include <Engine/Engine.hpp>
#include <Engine/Asset/AssetManager.hpp>
#include <Engine/Renderer/ResourceShared.hpp>

#include <Renderer/Renderer.hpp>
#include <Renderer/RenderContext.hpp>
#include <Renderer/Camera.hpp>
#include <Renderer/Resource/RenderTarget.hpp>
#include <Renderer/Resource/Geometry.hpp>
#include <Renderer/Effect.hpp>
#include <Renderer/Frustum.hpp>
#include <Renderer/Tree/Octree.hpp>
#include <Renderer/IRenderable.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	VisibilityPass::VisibilityPass()
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	VisibilityPass::~VisibilityPass()
	{
		// Nothing to do
	}

	//!	@brief		Initialize 
	//!	@date		2015-11-11
	void VisibilityPass::Initialize(Renderer::RenderContext* _pContext)
	{		
		// Buffer
		m_aBuffer[EBuffer::Scene] = _pContext->CreateConstantBuffer(RENDERER_BUFFER_SCENE, sizeof(RenderBuffer::Constant::Scene), 1);
		m_aBuffer[EBuffer::Instance]= _pContext->CreateShaderResourceBuffer(RENDERER_BUFFER_INSTANCE, sizeof(RenderBuffer::Geometry::Instance), 1, Renderer::EBind::ShaderResource, Renderer::EUsage::Dynamic);
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void VisibilityPass::Release(Renderer::RenderContext* _pContext)
	{
		for(uint8 eBuffer = 0; eBuffer < EBuffer::Count; ++eBuffer)
		{
			_pContext->DestroyResource(m_aBuffer[eBuffer]);
		}
	}

	//!	@brief		Prepare 
	//!	@date		2016-06-25
	void VisibilityPass::Prepare(Renderer::Renderer* /*_pRenderer*/, Renderer::RenderContext* /*_pContext*/, const Renderer::Camera* _pCamera, const Renderer::Octree* _pTree, Renderer::RenderBuffer* _pBuffer)
	{
		RenderBuffer::Constant::Scene* pSceneBuffer = _pBuffer->Push<RenderBuffer::Constant::Scene>();
		pSceneBuffer->ViewProjection = _pCamera->GetView() * _pCamera->GetProjection();
		pSceneBuffer->ViewProjectionInverse = pSceneBuffer->ViewProjection.Inverse();
		pSceneBuffer->Position = _pCamera->GetPosition();

		Renderer::Frustum kFrustum;
		kFrustum.Set(_pCamera->GetProjection());

		Vector<Renderer::Node*> vNode;
		_pTree->FindVisible(kFrustum, vNode);

		// Push Render command for each render component of visible nodes
		const uint32 uiNodecount = vNode.GetSize();
		for(uint32 uiNode = 0; uiNode < uiNodecount; ++uiNode)
		{
			// Retrieve information on the node
			Renderer::Node* pNode = vNode[uiNode];
			if(pNode->GetLayer() && !_pCamera->HasLayer(pNode->GetLayer()))
			{
				continue;
			}

			// Iterate all rendering object on the node
			const Vector<Renderer::IRenderable*>& vRenderable = pNode->GetVecRenderable();
			const uint32 uiRenderableCount = vRenderable.GetSize();
			for(uint32 uiRenderable = 0; uiRenderable < uiRenderableCount; ++uiRenderable)
			{
				// Push the command
				Renderer::IRenderable* pObject = vRenderable[uiRenderable];
				pObject->PushCommand(_pBuffer);
			}
		}
	}

	//!	@brief		Prepare 
	//!	@date		2016-06-25
	void VisibilityPass::Execute(Renderer::Renderer* /*_pRenderer*/, Renderer::RenderContext* /*_pContext*/, Renderer::RenderBuffer* /*_pBuffer*/)
	{
		// Nothing to do
	}
}}
