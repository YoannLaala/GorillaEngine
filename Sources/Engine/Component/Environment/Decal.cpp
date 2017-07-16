/******************************************************************************
**	Includes
******************************************************************************/
#include <Component/Environment/Decal.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Renderer/ResourceShared.hpp>
#include <Renderer/RenderContext.hpp>	
#include <Engine/Object/GameObject.hpp>	
#include <Component/Node.hpp>	

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Component
{
	COMPONENT_REGISTER(Gorilla::Component::Decal)

	//!	@brief		Constructor
	//!	@date		2015-04-04
	Decal::Decal()
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	Decal::~Decal()
	{
		// Nothing to do
	}

	//!	@brief		PushCommand
	//!	@date		2016-06-19
	void Decal::PushCommand(Renderer::RenderBuffer* _pBuffer)
	{
		if(Asset.IsLoaded())
		{			
			Engine::RenderBuffer::Decal::Batch* pBatch = _pBuffer->Push<Engine::RenderBuffer::Decal::Batch>();
			pBatch->Material = Asset->GetData();

			Node* pCpnNode = GetOrCreate<Node>();
			pBatch->Transform.SetRotation(pCpnNode->GetOrientation());
			pBatch->Transform.SetTranslation(pCpnNode->GetPosition());
			pBatch->Transform.ApplyScale(pCpnNode->GetScale());

			GetRenderer()->GetGizmo()->SetIdentity();
			GetRenderer()->GetGizmo()->SetOrientation(pCpnNode->GetOrientation());
			GetRenderer()->GetGizmo()->SetPosition(pCpnNode->GetPosition());
			GetRenderer()->GetGizmo()->SetScale(pCpnNode->GetScale());
			GetRenderer()->GetGizmo()->PushCube();
		}
	}
}}