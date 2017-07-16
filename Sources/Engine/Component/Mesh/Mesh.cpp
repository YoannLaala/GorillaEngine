/******************************************************************************
**	Includes
******************************************************************************/
#include "Mesh.hpp"

#include <Component/Node.hpp>
#include <Engine/Renderer/ResourceShared.hpp>
#include <Engine/Renderer/Pass/GeometryPass.hpp>
#include <Renderer/Material.hpp>	
#include <Renderer/RenderContext.hpp>	
#include <Renderer/Resource/Geometry.hpp>	

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Component
{	
	COMPONENT_REGISTER(Gorilla::Component::Mesh)

	//!	@brief		Constructor
	//!	@date		2015-04-04
	Mesh::Mesh()
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	Mesh::~Mesh()
	{
		// Nothing to do
	}

	//!	@brief		Start
	//!	@date		2016-06-19
	void Mesh::Start()
	{
		//GetOrCreate<Material>();
		RenderableNode::Start();
	}

	//!	@brief		Update
	//!	@date		2016-06-19
	void Mesh::Update()
	{
		// Display BoundingBox
		//GetRenderer()->PushCube(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, Renderer::Color::Red);
	}

	//!	@brief		Stop
	//!	@date		2016-06-19
	void Mesh::Stop()
	{
		RenderableNode::Stop();
	}

	//!	@brief		PushCommand
	//!	@date		2016-06-19
	void Mesh::PushCommand(Renderer::RenderBuffer* _pBuffer)
	{
		if(Asset.IsLoaded())
		{
			Renderer::Geometry* pGeometry = Asset->GetData();

			// Prepare Materials
			const uint8 uiSectionCount = pGeometry->GetSectionCount();
			//Materials.ExpandTo(uiSectionCount);

			// Push all section to rendering with proper material
			for(uint8 uiSection = 0; uiSection < uiSectionCount; ++uiSection)
			{
				Engine::RenderBuffer::Geometry::Batch* pBatch = _pBuffer->Push<Engine::RenderBuffer::Geometry::Batch>();
				pBatch->Geometry = pGeometry;
				pBatch->Section = uiSection;

				Node* pCpnNode = GetOrCreate<Node>();
				pBatch->Transform.SetRotation(pCpnNode->GetOrientation());
				pBatch->Transform.SetTranslation(pCpnNode->GetPosition());
				pBatch->Transform.ApplyScale(pCpnNode->GetScale());


				Math::Matrix44 test;
				test.SetRotation(pCpnNode->GetOrientation());
				test.SetTranslation(pCpnNode->GetPosition());

				Math::Matrix44 scale;
				scale.SetScale(pCpnNode->GetScale());
				
				pBatch->Transform = test * scale;
				pBatch->Transform.SetTranslation(pCpnNode->GetPosition());

				pBatch->Material = nullptr; 
			}
		}
	}
}}	
