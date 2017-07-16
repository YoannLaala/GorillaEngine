/******************************************************************************
**	Includes
******************************************************************************/
#include "RenderableNode.hpp"

#include <Engine/Object/GameObject.hpp>
#include <Engine/Component/Component.hpp>
#include <Component/Node.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Component
{		
	//!	@brief		Constructor
	//!	@date		2015-12-29
	RenderableNode::RenderableNode()
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-12-29
	RenderableNode::~RenderableNode()
	{
		// Nothing to do
	}

	//!	@brief		Start
	//!	@date		2016-03-26
	void RenderableNode::Start()
	{
		Node* pCpnNode = GetOrCreate<Node>();
		pCpnNode->AddRendeable(this);
	}

	//!	@brief		Stop
	//!	@date		2016-03-26
	void RenderableNode::Stop()
	{
		Node* pCpnNode = GetOrCreate<Node>();
		pCpnNode->RemoveRenderable(this);
	}
}}	
