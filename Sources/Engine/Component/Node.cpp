/******************************************************************************
**	Includes
******************************************************************************/
#include "Node.hpp"

#include <Engine/Object/GameObject.hpp>
#include <Engine/World.hpp>
#include <Renderer/Scene.hpp>
#include <Renderer/Node.hpp>

#include <Engine/Engine.hpp>
#include <Renderer/Renderer.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Component
{	
	COMPONENT_REGISTER(Gorilla::Component::Node)

	//!	@brief		Constructor
	//!	@date		2015-04-04
	Node::Node()
		: m_vRight(1.0f, 0.0f, 0.0f)
		, m_vUp(0.0f, 1.0f, 0.0f)
		, m_vForward(0.0f, 0.0f, 1.0f)
		, m_vPosition(0.0f, 0.0f, 0.0f)
		, m_vRotation(0.0f, 0.0f, 0.0f)
		, m_vScale(1.0f, 1.0f, 1.0f)
		, m_pNode(NULL)
	{
		SetFlag(EFlag::Changed);
		m_pNode = new Renderer::Node();
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	Node::~Node()
	{
		SAFE_DELETE(m_pNode);
	}

	//!	@brief		Start
	//!	@date		2015-04-04
	void Node::Start()
	{
		GetGameObject()->GetWorld()->GetScene()->AddNode(m_pNode);
	}

	//!	@brief		Update
	//!	@date		2015-04-04
	void Node::Update()
	{	
		if(HasFlag(EFlag::Changed))
		{
			GetGameObject()->GetWorld()->GetScene()->UpdateNode(m_pNode);
			RemoveFlag(EFlag::Changed);
		}
	}

	//!	@brief		Stop
	//!	@date		2015-04-04
	void Node::Stop()
	{
		GetGameObject()->GetWorld()->GetScene()->RemoveNode(m_pNode);
	}

	//!	@brief		LookAt
	//!	@date		2015-04-04
	void Node::LookAt(const Math::Vector3& _vTarget)
	{
		Math::Vector3 vForwardNew = (_vTarget - m_pNode->GetPosition()); vForwardNew.Normalize();
		m_vRight = m_vForward.Cross(vForwardNew);
		m_vForward = vForwardNew;
		m_vUp = m_vForward.Cross(m_vRight);

		Math::Quaternion qRotation(m_vRight.GetX(), m_vUp.GetY(), m_vForward.GetZ(), 1.0f);
		m_pNode->SetOrientation(qRotation);

		SetFlag(EFlag::Changed);
	}

	//!	@brief		LookAt
	//!	@date		2015-04-04
	void Node::LookAt(float32 _fX, float32 _fY, float32 _fZ)
	{
		Math::Vector3 vTarget(_fX, _fY, _fZ);
		LookAt(vTarget);
	}

	//!	@brief		LookAt
	//!	@date		2015-04-04
	void Node::Rotate(const Math::Vector3& _vAxis, float32 _fAngle)
	{
		// Compute Rotation
		Math::Quaternion qRotation;
		qRotation.Rotate(_vAxis, Math::ToRadian(-_fAngle));
		m_pNode->SetOrientation(qRotation);

		// Apply rotation
		m_vUp = qRotation * m_vUp;	
		m_vRight = qRotation * m_vRight;	
		m_vForward = qRotation * m_vForward;

		SetFlag(EFlag::Changed);
	}
}}	
