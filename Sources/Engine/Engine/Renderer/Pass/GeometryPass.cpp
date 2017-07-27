/******************************************************************************
**	Includes
******************************************************************************/
#include "GeometryPass.hpp"

#include <Engine/Engine.hpp>
#include <Engine/Asset/AssetManager.hpp>
#include <Engine/Asset/Texture/Texture.hpp>
#include <Engine/Renderer/ResourceShared.hpp>

#include <Renderer/Renderer.hpp>
#include <Renderer/RenderContext.hpp>
#include <Renderer/Viewport.hpp>
#include <Renderer/Frustum.hpp>
#include <Renderer/Node.hpp>
#include <Renderer/IRenderable.hpp>
#include <Renderer/Resource/RenderTarget.hpp>
#include <Renderer/Resource/Geometry.hpp>
#include <Renderer/Effect.hpp>
#include <Renderer/Material.hpp>
#include <Renderer/Camera.hpp>

#include <Renderer/Helper/Gizmo.hpp>

/******************************************************************************
**	Defines
******************************************************************************/
#define VOXEL_GRID_SIZE 128
#define VOXEL_GRID_VOLUME (VOXEL_GRID_SIZE * VOXEL_GRID_SIZE * VOXEL_GRID_SIZE)
#define VOXEL_OCTREE_DEPTH_LITMIT 4

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	GeometryPass::GeometryPass()
		: m_pRenderTarget (NULL)
		, m_pSceneBuffer (NULL)
		, m_pInstanceBuffer (NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	GeometryPass::~GeometryPass()
	{
		// Nothing to do
	}

	//!	@brief		Execute 
	//!	@date		2016-06-25
	struct cbVoxel
	{
		Math::Matrix44	View;
		Math::Matrix44	Projection;
		Math::Vector3	GridCenter;
		float32			GridSize;
		float32			VoxelHalfSize;
		uint32			VoxelOctreeDepthLimit;
	};

	struct VoxelGridState
	{
		uint32	FragmentCount;
		uint32	FragmentPadding[2];
		uint32	NodeCount;
		uint32	NodePadding[2];
	};

	struct VoxelFragment
	{
		Math::Vector3	Position;
		Math::Vector3	Normal;
	};

	class VoxelNode
	{
		uint32 Offset;
		uint32 brickPtr;
		uint32 neighborX;
		uint32 neighborY;
		uint32 neighborZ;
	};


	//!	@brief		Initialize 
	//!	@date		2015-11-11
	AssetHandle<Texture> hAlbedo;
	AssetHandle<Texture> hMetallic;
	AssetHandle<Texture> hRoughness;
	void GeometryPass::Initialize(Renderer::RenderContext* _pContext)
	{
		/*hAlbedo = GetAssetManager()->Get<Texture>("pbr/test_001/albedo.png");
		hMetallic = GetAssetManager()->Get<Texture>("pbr/test_001/metallic.png");
		hRoughness = GetAssetManager()->Get<Texture>("pbr/test_001/roughness.png");*/

		Renderer::Texture2D* pRenderTexture = _pContext->GetViewport()->GetRenderTarget()->GetTarget(0);
		uint32 uiWidth = pRenderTexture->GetWidth();
		uint32 uiHeight = pRenderTexture->GetHeight();

		// Texture
		Renderer::Texture2D* pAlbedoTexture = _pContext->CreateTexture2D(RENDERER_TEXTURE_GEOMETRY_ALBEDO, uiWidth, uiHeight, 1, Renderer::EFormat::R8G8B8A8_UNORM, Renderer::EBind::RenderTarget | Renderer::EBind::ShaderResource);
		Renderer::Texture2D* pNormalTexture = _pContext->CreateTexture2D(RENDERER_TEXTURE_GEOMETRY_NORMAL, uiWidth, uiHeight, 1, Renderer::EFormat::R8G8B8A8_UNORM, Renderer::EBind::RenderTarget | Renderer::EBind::ShaderResource);
		Renderer::Texture2D* pDepthStencilTexture = _pContext->CreateTexture2D(RENDERER_TEXTURE_GEOMETRY_DEPTH_STENCIL, uiWidth, uiHeight, 1, Renderer::EFormat::R24G8_TYPELESS, Renderer::EBind::DepthStencil | Renderer::EBind::ShaderResource);

		// RenderTarget
		m_pRenderTarget = _pContext->CreateRenderTarget(RENDERER_TARGET_GEOMETRY);
		m_pRenderTarget->AddTarget(pAlbedoTexture);
		m_pRenderTarget->AddTarget(pNormalTexture);
		m_pRenderTarget->SetDepthStencil(pDepthStencilTexture);
		
		Renderer::Buffer* pconstant_buffer = _pContext->CreateConstantBuffer(RENDERER_BUFFER_VOXEL_GRID, sizeof(cbVoxel), 1);
		pconstant_buffer;

		Renderer::Buffer* pVoxelFragmentBuffer = _pContext->CreateShaderResourceBuffer(RENDERER_BUFFER_VOXEL_FRAMENT, sizeof(VoxelFragment), VOXEL_GRID_VOLUME, Renderer::EBind::ShaderResource | Renderer::EBind::UnorderedAccess);
		pVoxelFragmentBuffer;

		uint32 uiMipCount = (uint32)(Math::Log2((float32)VOXEL_GRID_SIZE)) + 1;
		Renderer::Texture3D* pVoxelColor = _pContext->CreateTexture3D(RENDERER_TEXTURE_VOXEL_VOLUME_COLOR, VOXEL_GRID_SIZE, VOXEL_GRID_SIZE, VOXEL_GRID_SIZE, uiMipCount, Renderer::EFormat::R8G8B8A8_UNORM, Renderer::EBind::ShaderResource | Renderer::EBind::RenderTarget | Renderer::EBind::UnorderedAccess);
		pVoxelColor;

		Renderer::Texture3D* pVoxelNormal = _pContext->CreateTexture3D(RENDERER_TEXTURE_VOXEL_VOLUME_NORMAL, VOXEL_GRID_SIZE, VOXEL_GRID_SIZE, VOXEL_GRID_SIZE, uiMipCount, Renderer::EFormat::R8G8B8A8_UNORM, Renderer::EBind::ShaderResource | Renderer::EBind::RenderTarget | Renderer::EBind::UnorderedAccess);
		pVoxelNormal;

		// Buffer
		m_pSceneBuffer = _pContext->GetBuffer(RENDERER_BUFFER_SCENE);
		m_pInstanceBuffer = _pContext->GetBuffer(RENDERER_BUFFER_INSTANCE);
		m_hEffect = GetAssetManager()->Get<Effect>("@Effect/Geometry.fx");
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void GeometryPass::Release(Renderer::RenderContext* _pContext)
	{
		_pContext->DestroyResource(m_pRenderTarget->GetTarget(0));
		_pContext->DestroyResource(m_pRenderTarget->GetTarget(1));
		_pContext->DestroyResource(m_pRenderTarget->GetDepthStencil());
		_pContext->DestroyResource(m_pRenderTarget);

		_pContext->DestroyResource(m_pSceneBuffer);
		_pContext->DestroyResource(m_pInstanceBuffer);
	}	

	Renderer::Texture3D* pVoxelColor;
	Renderer::Texture3D* pVoxelNormal;
	Renderer::Texture2D* pShadow = NULL;
	Renderer::RenderTarget* prt = NULL;
	Renderer::RenderTarget* prt2 = NULL;
	AssetHandle<Effect>* hEffect_create_fragment_list;
	AssetHandle<Effect>* hEffect_display_fragment_list;
	AssetHandle<Shader>* hShader_clear_fragment_list;
	AssetHandle<Effect>* hEffect3;
	AssetHandle<Shader>* hFlagVoxelNode;
	AssetHandle<Shader>* hSubdivideVoxelNode;
	AssetHandle<Shader>* hInjectLightDirect;
	Renderer::Buffer* pconstant_buffer = NULL;
	Renderer::Buffer* pVoxelFragment = NULL;
	Renderer::Buffer* pVoxelNode = NULL;
	Renderer::Buffer* pVoxelGridState = NULL;
	Renderer::Camera cam;
	bool test = false;
	void GeometryPass::Execute(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, Renderer::RenderBuffer* _pBuffer)
	{	
		// init debug
		if(!prt)
		{
			//pShadow = _pContext->GetTexture2D(RENDERER_TEXTURE_SHADOW_DEPTH_STENCIL);

			hEffect_create_fragment_list = new AssetHandle<Effect>();
			*hEffect_create_fragment_list = GetAssetManager()->Get<Effect>("@Effect/GlobalIllumination/CreateVoxelFragmentList.fx");
			hEffect_display_fragment_list = new AssetHandle<Effect>();
			*hEffect_display_fragment_list = GetAssetManager()->Get<Effect>("@Effect/GlobalIllumination/DisplayVoxelFragmentList.fx"); 
			hShader_clear_fragment_list = new AssetHandle<Shader>();
			*hShader_clear_fragment_list = GetAssetManager()->Get<Shader>("@Effect/GlobalIllumination/ClearVoxelFragmentList.cs"); 

			/*hEffect3 = new AssetHandle<Effect>();
			*hEffect3 = GetAssetManager()->Get<Effect>("Effect/GlobalIllumination/DisplaySparseVoxelOctree.fx"); */
			hInjectLightDirect = new AssetHandle<Shader>();
			*hInjectLightDirect = GetAssetManager()->Get<Shader>("@Effect/GlobalIllumination/InjectDirectLight.cs");

			hFlagVoxelNode = new AssetHandle<Shader>();
			*hFlagVoxelNode = GetAssetManager()->Get<Shader>("@Effect/GlobalIllumination/FlagVoxelNode.cs");

			hSubdivideVoxelNode = new AssetHandle<Shader>();
			*hSubdivideVoxelNode = GetAssetManager()->Get<Shader>("@Effect/GlobalIllumination/SubdivideVoxelNode.cs");
					
			
			pconstant_buffer = _pContext->GetBuffer(RENDERER_BUFFER_VOXEL_GRID);
			pVoxelFragment = _pContext->GetBuffer(RENDERER_BUFFER_VOXEL_FRAMENT);
			pVoxelNode = _pRenderer->CreateShaderResourceBuffer(sizeof(VoxelNode), VOXEL_GRID_VOLUME, Renderer::EBind::ShaderResource | Renderer::EBind::UnorderedAccess);
			pVoxelGridState = _pRenderer->CreateIndirectBuffer(sizeof(VoxelGridState), 1);

			pVoxelColor = _pContext->GetTexture3D(RENDERER_TEXTURE_VOXEL_VOLUME_COLOR);
			pVoxelNormal = _pContext->GetTexture3D(RENDERER_TEXTURE_VOXEL_VOLUME_NORMAL);

			Renderer::Texture2D* ptex = _pRenderer->CreateTexture2D(VOXEL_GRID_SIZE, VOXEL_GRID_SIZE, 1, Renderer::EFormat::R8G8B8A8_UNORM, Renderer::EBind::RenderTarget);
			prt = _pRenderer->CreateRenderTarget();
			prt->AddTarget(ptex);
			prt->AddOutput(pVoxelFragment);
			prt->AddOutput(pVoxelNode);
			prt->AddOutput(pVoxelGridState);
			prt->AddOutput(pVoxelColor);
			prt->AddOutput(pVoxelNormal);
			
			Renderer::RenderTarget* pRenderTarget = _pContext->GetViewport()->GetRenderTarget();
			prt2 = _pRenderer->CreateRenderTarget();
			prt2->AddTarget(pRenderTarget->GetTarget(0));
			//prt2->AddBuffer(pvoxel_buffer);
			prt2->SetDepthStencil(m_pRenderTarget->GetDepthStencil());
		}
		
		// update voxel constant buffer
		RenderBuffer::Constant::Scene* heyyyyyy; uint32 hooooo;
		if(_pBuffer->Get(&heyyyyyy, hooooo))
		{
			
			Math::Matrix44 mTest = heyyyyyy->View.Inverse();
			Math::Vector3 vPosition(mTest[3][0], mTest[3][1], mTest[3][2]);
			Math::Vector3 vRight(heyyyyyy->View[0][0], heyyyyyy->View[0][1], heyyyyyy->View[0][2]); vRight.Normalize();
			Math::Vector3 vUp(heyyyyyy->View[1][0], heyyyyyy->View[1][1], heyyyyyy->View[1][2]); vUp.Normalize();
			Math::Vector3 vForward(heyyyyyy->View[2][0], heyyyyyy->View[2][1], heyyyyyy->View[2][2]); vForward.Normalize();

			Math::Vector3 vOrigin(-1.0f, 0.0f,0.0);
			/*_pRenderer->GetGizmo()->PushLine(vOrigin, vOrigin+(vForward * -1.0f), Renderer::Color::Red);
			_pRenderer->GetGizmo()->PushLine(vOrigin, vOrigin+(vUp), Renderer::Color::Green);
			_pRenderer->GetGizmo()->PushLine(vOrigin, vOrigin+(vRight), Renderer::Color::Blue);*/

			static float32 aValue[4] = {2.0f, 2.0f, -1.0f, 1.0f};
			cam.SetOrthographic(aValue[0], aValue[1], aValue[2], aValue[3]);
			//float32 fDistanceToCenter = (aValue[2] + aValue[3]) * 0.5f;
			//Math::Vector3 vCenter = vPosition + vForward * 0.5f;

			/*_pRenderer->GetGizmo()->SetPosition(vCenter - vRight * fDistanceToCenter);
			_pRenderer->GetGizmo()->PushCube(Renderer::Color::Green);*/

			//if(test == false)
			{
				test = true;
				cbVoxel* pcbDestination = reinterpret_cast<cbVoxel*>(_pRenderer->Map(_pContext, pconstant_buffer));
				cam.SetView(Math::Vector3::Zero, Math::Vector3::UnitX, Math::Vector3::UnitY, Math::Vector3::UnitZ);
				pcbDestination->View = cam.GetView();
				pcbDestination->Projection = cam.GetProjection();
				pcbDestination->GridCenter = Math::Vector3::Zero;
				pcbDestination->GridSize = VOXEL_GRID_SIZE;
				pcbDestination->VoxelHalfSize =  (1.0f / VOXEL_GRID_SIZE) * 0.5;
				pcbDestination->VoxelOctreeDepthLimit = VOXEL_OCTREE_DEPTH_LITMIT;
				_pRenderer->Unmap(_pContext, pconstant_buffer);
			}
		}
	

		// Update ConstantBuffer
		RenderBuffer::Constant::Scene* pSceneBufferSource; uint32 uiSceneCount;
		if(_pBuffer->Get(&pSceneBufferSource, uiSceneCount))
		{
			RenderBuffer::Constant::Scene* pSceneBufferDestination = reinterpret_cast<RenderBuffer::Constant::Scene*>(_pRenderer->Map(_pContext, m_pSceneBuffer));
			memcpy_s(pSceneBufferDestination, sizeof(RenderBuffer::Constant::Scene), pSceneBufferSource, sizeof(RenderBuffer::Constant::Scene));
			_pRenderer->Unmap(_pContext, m_pSceneBuffer);
		}
		

		// default draw
		{
			//MARKER_GPU(_pContext, "Geometry");

			// Clear
			Renderer::Texture2D* pColorTexture = m_pRenderTarget->GetTarget(0);
			_pRenderer->SetRenderTarget(_pContext, m_pRenderTarget);
			_pRenderer->SetViewport(_pContext, 0, 0, pColorTexture->GetWidth(), pColorTexture->GetHeight());
			_pRenderer->Clear(_pContext, m_pRenderTarget);

			RenderBuffer::Geometry::Batch* pBatchArray; uint32 uiBatchCount;
			if(_pBuffer->Get(&pBatchArray, uiBatchCount))
			{
				// Set RenderState
				Renderer::Effect* pEffect = m_hEffect->GetData();
				_pRenderer->SetRenderState(_pContext, pEffect->GetRenderState());
				_pRenderer->SetTopology(_pContext, pEffect->GetTopology());

				// Set Vertex Shader and resources
				Renderer::Shader* pVertexShader = pEffect->GetShader(Renderer::EShader::Vertex, Renderer::ETechnique::Instancing);
				_pRenderer->SetVertexShader(_pContext, pVertexShader);
				_pRenderer->SetVertexShaderConstantBuffer(_pContext, 0, m_pSceneBuffer);
				_pRenderer->SetVertexShaderResource(_pContext, 0, m_pInstanceBuffer);

				Renderer::Shader* pPixelShader = m_hEffect->GetData()->GetShader(Renderer::EShader::Pixel, Renderer::ETechnique::Instancing);

				/*_pRenderer->SetPixelShaderResource(_pContext, 0, hAlbedo->GetData());
				_pRenderer->SetPixelShaderResource(_pContext, 1, hMetallic->GetData());
				_pRenderer->SetPixelShaderResource(_pContext, 2, hRoughness->GetData());*/

				

				DrawAllGeometry(_pRenderer, _pContext, pBatchArray, uiBatchCount, &m_pInstanceBuffer, pPixelShader);
			}
		}
		

		//{
		//	MARKER_GPU(_pContext, "Voxelization");

		//	// Clear Voxel Fragment List
		//	{
		//		MARKER_GPU(_pContext, "Clear");

		//		_pRenderer->Clear(_pContext, prt);

		//		_pRenderer->SetComputeShader(_pContext, (*hShader_clear_fragment_list)->GetData());
		//		_pRenderer->SetComputeShaderUnorderedAccess(_pContext, 0, pVoxelNode);
		//		_pRenderer->SetComputeShaderUnorderedAccess(_pContext, 1, pVoxelGridState);
		//		_pRenderer->Compute(_pContext, 1, 1, 1);

		//		_pRenderer->UnsetComputeShaderUnorderedAccess(_pContext, 0, 2);
		//	}
		//
		//	// Create Voxel Fragment list
		//	{
		//		MARKER_GPU(_pContext, "Create Fragment List");

		//		RenderBuffer::Geometry::Batch* pBatchArray; uint32 uiBatchCount;
		//		if(_pBuffer->Get(&pBatchArray, uiBatchCount))
		//		{
		//			_pRenderer->SetRenderTarget(_pContext, prt);
		//			_pRenderer->SetViewport(_pContext, 0, 0, VOXEL_GRID_SIZE, VOXEL_GRID_SIZE);

		//			Renderer::Effect* pEffect = (*hEffect_create_fragment_list)->GetData();
		//			_pRenderer->SetRenderState(_pContext, pEffect->GetRenderState());
		//			_pRenderer->SetTopology(_pContext, pEffect->GetTopology());

		//			// Set Shaders and resources
		//			Renderer::Shader* pVertexShader = pEffect->GetShader(Renderer::EShader::Vertex, Renderer::ETechnique::Instancing);
		//			_pRenderer->SetVertexShader(_pContext, pVertexShader);
		//			_pRenderer->SetVertexShaderResource(_pContext, 0, m_pInstanceBuffer);

		//			Renderer::Shader* pGeometryShader = pEffect->GetShader(Renderer::EShader::Geometry, Renderer::ETechnique::Instancing);
		//			_pRenderer->SetGeometryShader(_pContext, pGeometryShader);
		//			_pRenderer->SetGeometryShaderConstantBuffer(_pContext, 0, pconstant_buffer);

		//			Renderer::Shader* pPixelShader = pEffect->GetShader(Renderer::EShader::Pixel);
		//			_pRenderer->SetPixelShader(_pContext, pPixelShader);
		//			_pRenderer->SetPixelShaderConstantBuffer(_pContext, 0, pconstant_buffer);

		//			// Handle Instancing
		//			DrawAllGeometry(_pRenderer, _pContext, pBatchArray, uiBatchCount, &m_pInstanceBuffer);
		//		}
		//		_pRenderer->SetGeometryShader(_pContext, NULL);
		//	}

		//	// CLEAN RENTER TARGET WITH UAV ON OUTPUT
		//	//_pRenderer->SetRenderTarget(_pContext, m_pRenderTarget);
		//	// CLEAN RENTER TARGET WITH UAV ON OUTPUT
		//
		//	//{
		//	//	MARKER_GPU(_pContext, "Create Sparse Voxel Octree");

		//	//	for(uint32 uiDepth = 0; uiDepth < VOXEL_OCTREE_DEPTH_LITMIT; ++uiDepth)
		//	//	{
		//	//		MARKER_GPU(_pContext, (String("Depth") + uiDepth).GetBuffer());

		//	//		// Flag node
		//	//		{
		//	//			_pRenderer->SetComputeShader(_pContext, (*hFlagVoxelNode)->GetData());
		//	//			//_pRenderer->SetComputeShaderConstantBuffer(_pContext, 0, m_pSceneBuffer);
		//	//			_pRenderer->SetComputeShaderConstantBuffer(_pContext, 0, pconstant_buffer);			
		//	//			_pRenderer->SetComputeShaderResource(_pContext, 0, pVoxelFragment);
		//	//			_pRenderer->SetComputeShaderResource(_pContext, 1, pVoxelGridState);
		//	//			_pRenderer->SetComputeShaderUnorderedAccess(_pContext, 0, pVoxelNode);
		//	//			_pRenderer->Compute(_pContext, pVoxelGridState);

		//	//			_pRenderer->UnsetComputeShaderResource(_pContext, 0, 2);
		//	//			_pRenderer->UnsetComputeShaderUnorderedAccess(_pContext, 0, 1);
		//	//		}

		//	//		// Subdivide node
		//	//		{
		//	//			_pRenderer->SetComputeShader(_pContext, (*hSubdivideVoxelNode)->GetData());		
		//	//			_pRenderer->SetComputeShaderUnorderedAccess(_pContext, 0, pVoxelNode);
		//	//			_pRenderer->SetComputeShaderUnorderedAccess(_pContext, 1, pVoxelGridState);
		//	//			_pRenderer->Compute(_pContext, pVoxelGridState, 12);

		//	//			_pRenderer->UnsetComputeShaderUnorderedAccess(_pContext, 0, 2);
		//	//		}
		//	//	}
		//	//}
		//}

		//// Inject Light
		//{
		//	_pRenderer->SetRenderTarget(_pContext, m_pRenderTarget);

		//	// Update ConstantBuffer
		//	RenderBuffer::Constant::Scene* pSceneBufferSource; uint32 uiSceneCount;
		//	if(_pBuffer->Get(&pSceneBufferSource, uiSceneCount))
		//	{
		//		RenderBuffer::Light::Directional* pLightBufferSource; uint32 uiLightCount;
		//		if(_pRenderer->GetFrameBuffer()->Get(&pLightBufferSource, uiLightCount))
		//		{
		//			RenderBuffer::Constant::Scene* pSceneBuffer = reinterpret_cast<RenderBuffer::Constant::Scene*>(_pRenderer->Map(_pContext, m_pSceneBuffer));
		//			pSceneBuffer->View = pLightBufferSource->View.Inverse();
		//			pSceneBuffer->Projection = pLightBufferSource->Projection.Inverse();				
		//			_pRenderer->Unmap(_pContext, m_pSceneBuffer);
		//		}
		//	}

		//	// Inject direct light
		//	_pRenderer->SetComputeShader(_pContext, (*hInjectLightDirect)->GetData());
		//	_pRenderer->SetComputeShaderConstantBuffer(_pContext, 0, m_pSceneBuffer);
		//	_pRenderer->SetComputeShaderConstantBuffer(_pContext, 1, pconstant_buffer);
		//	//_pRenderer->SetComputeShaderResource(_pContext, 0, pShadow);
		//	_pRenderer->SetComputeShaderResource(_pContext, 1, pVoxelFragment);
		//	_pRenderer->SetComputeShaderUnorderedAccess(_pContext, 0, pVoxelColor);
		//	_pRenderer->Compute(_pContext, 32, 32, 1);
		//	_pRenderer->UnsetComputeShaderUnorderedAccess(_pContext, 0, 1);
		//}

		//_pRenderer->UpdateMipMap(_pContext, pVoxelColor);

		//// Update ConstantBuffer
		//if(pSceneBufferSource)
		//{
		//	RenderBuffer::Constant::Scene* pSceneBufferDestination = reinterpret_cast<RenderBuffer::Constant::Scene*>(_pRenderer->Map(_pContext, m_pSceneBuffer));
		//	memcpy_s(pSceneBufferDestination, sizeof(RenderBuffer::Constant::Scene), pSceneBufferSource, sizeof(RenderBuffer::Constant::Scene));
		//	_pRenderer->Unmap(_pContext, m_pSceneBuffer);
		//}

		// Display Voxel Fragment List
		/*{
			// Update ConstantBuffer
			RenderBuffer::Constant::Scene* pSceneBufferSource; uint32 uiSceneCount;
			_pBuffer->Get(&pSceneBufferSource, uiSceneCount);
			if(pSceneBufferSource)
			{
				RenderBuffer::Constant::Scene* pSceneBufferDestination = reinterpret_cast<RenderBuffer::Constant::Scene*>(_pRenderer->Map(_pContext, m_pSceneBuffer));
				memcpy_s(pSceneBufferDestination, sizeof(RenderBuffer::Constant::Scene), pSceneBufferSource, sizeof(RenderBuffer::Constant::Scene));
				_pRenderer->Unmap(_pContext, m_pSceneBuffer);
			}

			_pRenderer->Clear(_pContext, prt2);
			_pRenderer->SetRenderTarget(_pContext, prt2);
			_pRenderer->SetViewport(_pContext, 0, 0, prt2->GetTexture(0)->GetWidth(), prt2->GetTexture(0)->GetHeight());

			// Set RenderState
			Renderer::Effect* pEffect = (*hEffect_display_fragment_list)->GetData();
			_pRenderer->SetRenderState(_pContext, pEffect->GetRenderState());
			_pRenderer->SetTopology(_pContext, pEffect->GetTopology());

			// Set Vertex Shader and resources
			Renderer::Shader* pvtxShader = pEffect->GetShader(Renderer::EShader::Vertex, Renderer::ETechnique::Instancing);
			_pRenderer->SetVertexShader(_pContext, pvtxShader);
			_pRenderer->SetVertexShaderConstantBuffer(_pContext, 0, m_pSceneBuffer);
			_pRenderer->SetVertexShaderConstantBuffer(_pContext, 1, pconstant_buffer);
			_pRenderer->SetVertexShaderResource(_pContext, 0, pvoxel_buffer);

			Renderer::Shader* ppxShader = pEffect->GetShader(Renderer::EShader::Pixel);
			_pRenderer->SetPixelShader(_pContext, ppxShader);
			_pRenderer->SetPixelShaderResource(_pContext, 0, pvoxel_buffer);

			Renderer::Geometry* pGeometry = GeometryHelper::GetOrCreate(_pRenderer, Renderer::EPrimitive::Cube);
			Renderer::Buffer* pVertexBuffer = pGeometry->GetVertexBuffer();
			Renderer::Buffer* pIndexBuffer = pGeometry->GetIndexBuffer();
			_pRenderer->SetVertexBuffer(_pContext, pVertexBuffer);
			_pRenderer->SetIndexBuffer(_pContext, pIndexBuffer);

			// Draw all voxels
			uint32 uiIndexOffset = pGeometry->GetIndexOffset();
			uint32 uiIndexCount = pGeometry->GetIndexCount();
			_pRenderer->Draw(_pContext, uiIndexOffset, uiIndexCount, VOXEL_GRID_VOLUME);
		}*/

		//dispaly voxel grid
		//{
		//	Renderer::RenderTarget* pRenderTarget = _pContext->GetViewport()->GetRenderTarget();
		//	_pRenderer->Clear(_pContext, pRenderTarget);
		//	_pRenderer->SetRenderTarget(_pContext, pRenderTarget);
		//	_pRenderer->SetViewport(_pContext, 0, 0, pRenderTarget->GetTexture(0)->GetWidth(), pRenderTarget->GetTexture(0)->GetHeight());

		//	// Set RenderState
		//	Renderer::Effect* pEffect = (*hEffect2)->GetData();
		//	_pRenderer->SetRenderState(_pContext, pEffect->GetRenderState());
		//	_pRenderer->SetTopology(_pContext, pEffect->GetTopology());

		//	// Set Vertex Shader and resources
		//	Renderer::Shader* pvtxShader = pEffect->GetShader(Renderer::EShader::Vertex, Renderer::ETechnique::Instancing);
		//	Renderer::Shader* pgeoShader = pEffect->GetShader(Renderer::EShader::Geometry, Renderer::ETechnique::Instancing);
		//	Renderer::Shader* ppxShader = pEffect->GetShader(Renderer::EShader::Pixel);
		//	_pRenderer->SetVertexShader(_pContext, pvtxShader);
		//	_pRenderer->SetGeometryShader(_pContext, pgeoShader);
		//	_pRenderer->SetPixelShader(_pContext, ppxShader);
		//
		//	_pRenderer->SetVertexShaderConstantBuffer(_pContext, 0, pconstant_buffer);
		//	_pRenderer->SetGeometryShaderConstantBuffer(_pContext, 0, m_pSceneBuffer);
		//	_pRenderer->SetGeometryShaderResource(_pContext, 0, pvoxel_buffer);

		//	Renderer::Geometry* pGeometry = GeometryHelper::GetOrCreate(_pRenderer, Renderer::EPrimitive::Point);
		//	Renderer::Buffer* pVertexBuffer = pGeometry->GetVertexBuffer();
		//	Renderer::Buffer* pIndexBuffer = pGeometry->GetIndexBuffer();
		//	_pRenderer->SetVertexBuffer(_pContext, pVertexBuffer);
		//	_pRenderer->SetIndexBuffer(_pContext, pIndexBuffer);

		//	// Draw the section
		//	uint32 uiIndexOffset = pGeometry->GetIndexOffset();
		//	uint32 uiIndexCount = pGeometry->GetIndexCount();
		//	_pRenderer->Draw(_pContext, uiIndexOffset, uiIndexCount, 32*32*32);
		//	_pRenderer->SetGeometryShader(_pContext, NULL);
		//}
		
		





		// Update ConstantBuffer
		if(pSceneBufferSource)
		{
			RenderBuffer::Constant::Scene* pSceneBufferDestination = reinterpret_cast<RenderBuffer::Constant::Scene*>(_pRenderer->Map(_pContext, m_pSceneBuffer));
			pSceneBufferDestination->View = pSceneBufferSource->View.Inverse();
			pSceneBufferDestination->Projection = pSceneBufferSource->Projection;
			_pRenderer->Unmap(_pContext, m_pSceneBuffer);
		}


		
	}

	//!	@brief		DrawAllGeometry 
	//!	@date		2016-06-25
	void GeometryPass::DrawAllGeometry(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, RenderBuffer::Geometry::Batch* _pBatchArray, uint32 _uiBatchCount, Renderer::Buffer** _ppInstanceBufferOut, Renderer::Shader* _pPixelShader /*= NULL*/)
	{
		// HACK
		for(uint32 uiCommand = 0; uiCommand < _uiBatchCount; ++uiCommand)
		{
			DrawOneGeometry(_pRenderer, _pContext, &_pBatchArray[uiCommand], 1, _ppInstanceBufferOut, _pPixelShader);
		}

		//// Handle Instancing
		//uint32 uiCommandStart = 0, uiCommand = 1;
		//Renderer::Geometry* pGeometry = _pBatchArray->Geometry;
		//for(; uiCommand < _uiBatchCount; ++uiCommand)
		//{
		//	RenderBuffer::Geometry::Batch* pBatch = &_pBatchArray[uiCommand];
		//	
		//	// Draw only if we changed of geometry 
		//	if(pGeometry != pBatch->Geometry)
		//	{
		//		DrawOneGeometry(_pRenderer, _pContext, &_pBatchArray[uiCommandStart], uiCommand - uiCommandStart, _ppInstanceBufferOut, _pPixelShader);

		//		uiCommandStart = uiCommand;
		//		pGeometry = pBatch->Geometry;
		//	}
		//}

		//DrawOneGeometry(_pRenderer, _pContext, &_pBatchArray[uiCommandStart], uiCommand - uiCommandStart, _ppInstanceBufferOut, _pPixelShader);
	}

	//!	@brief		DrawOneGeometry 
	//!	@date		2016-06-25
	void GeometryPass::DrawOneGeometry(Renderer::Renderer* _pRenderer, Renderer::RenderContext* _pContext, RenderBuffer::Geometry::Batch* _pBatchArray, uint32 _uiInstanceCount, Renderer::Buffer** _ppInstanceBufferOut, Renderer::Shader* _pPixelShader)
	{
		// Allocate enough memory in buffer
		if((*_ppInstanceBufferOut)->GetElementCount() < _uiInstanceCount)
		{
			_pRenderer->DestroyResource(*_ppInstanceBufferOut);
			*_ppInstanceBufferOut = _pRenderer->CreateShaderResourceBuffer(sizeof(RenderBuffer::Geometry::Instance), _uiInstanceCount, Renderer::EBind::ShaderResource, Renderer::EUsage::Dynamic);
		}

		// Update Instance buffer
		RenderBuffer::Geometry::Instance* pInstanceBuffer = reinterpret_cast<RenderBuffer::Geometry::Instance*>(_pRenderer->Map(_pContext, *_ppInstanceBufferOut));
		for(uint32 uiInstance = 0; uiInstance < _uiInstanceCount; ++uiInstance)
		{
			RenderBuffer::Geometry::Batch& kBatch = _pBatchArray[uiInstance];
			pInstanceBuffer[uiInstance].World = kBatch.Transform;
		}
		_pRenderer->Unmap(_pContext, *_ppInstanceBufferOut);

		Renderer::Geometry* pGeometry = _pBatchArray->Geometry;
		Renderer::Buffer* pVertexBuffer = pGeometry->GetVertexBuffer();
		Renderer::Buffer* pIndexBuffer = pGeometry->GetIndexBuffer();
		_pRenderer->SetVertexBuffer(_pContext, pVertexBuffer);
		_pRenderer->SetIndexBuffer(_pContext, pIndexBuffer);

		// Retrieve proper Pixel shader
		Renderer::Material* pMaterial = _pBatchArray->Material;
		if(pMaterial)
		{
			// Replace Default Shader
			if(_pPixelShader)
			{
				_pPixelShader = pMaterial->GetShader();
			}

			// Synchronize Material if needed
			if(pMaterial->HasChanged())
			{
				_pRenderer->Synchronize(_pContext, pMaterial);
			}
			
			// Float Shader resource
			Renderer::Buffer* pNumberBuffer = pMaterial->GetNumberBuffer();
			if(pNumberBuffer)
			{
				_pRenderer->SetPixelShaderResource(_pContext, 0, pNumberBuffer);
			}

			// Texture shader resource
			const Vector<Renderer::Texture2D*>& vTexture = pMaterial->GetVecTexture();
			const uint32 uiTextureCount = vTexture.GetSize();
			for(uint32 uiTexture = 0; uiTexture < uiTextureCount; ++uiTexture)
			{
				_pRenderer->SetPixelShaderResource(_pContext, uiTexture + 1, vTexture[uiTexture]);
			}
		}

		if(_pPixelShader)
		{
			_pRenderer->SetPixelShader(_pContext, _pPixelShader);
		}
		
		// Draw the section
		uint32 uiIndexOffset = pGeometry->GetIndexOffset(_pBatchArray->Section);
		uint32 uiIndexCount = pGeometry->GetIndexCount(_pBatchArray->Section);
		_pRenderer->Draw(_pContext, uiIndexOffset, uiIndexCount, _uiInstanceCount);
	}
}}
