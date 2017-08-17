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
			Node* pCpnNode = GetOrCreate<Node>();
			Renderer::Geometry* pGeometry = Asset->GetGeometry();

			// Prepare Materials
			const uint8 uiGroupCount = pGeometry->GetGroupCount();
			//Materials.ExpandTo(uiSectionCount);

			// Push all section to rendering with proper material
			for(uint8 uiGroup = 0; uiGroup < uiGroupCount; ++uiGroup)
			{
				Engine::RenderBuffer::Geometry::Batch* pBatch = _pBuffer->Push<Engine::RenderBuffer::Geometry::Batch>();
				pBatch->Geometry = pGeometry;
				pBatch->Group = uiGroup;
				pBatch->Transform = pCpnNode->GetTransform();
				pBatch->Material = nullptr; 
			}
		}
	}
}}	
