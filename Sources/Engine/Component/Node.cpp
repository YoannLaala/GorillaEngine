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

	//!	@brief		SetOrientation
	//!	@date		2015-04-04
	void Node::SetOrientation(const Math::Quaternion& _qOrientation) 
	{
		m_vUp = _qOrientation * Math::Vector3::UnitY;
		m_vRight = _qOrientation * Math::Vector3::UnitX;
		m_vForward = _qOrientation * Math::Vector3::UnitZ;

		m_pNode->GetTransform().SetOrientation(_qOrientation); 
		SetFlag(EFlag::Changed); 
	}

	//!	@brief		LookAt
	//!	@date		2015-04-04
	void Node::Rotate(const Math::Vector3& _vAxis, float32 _fAngle)
	{
		if(_fAngle == 0.0f) return;

		// Compute Rotation
		Math::Quaternion qOrientation;
		qOrientation.Rotate(_vAxis, Math::ToRadian(-_fAngle));
		m_pNode->GetTransform().Rotate(qOrientation); 

		// Apply rotation
		m_vUp = qOrientation * m_vUp;	
		m_vRight = qOrientation * m_vRight;	
		m_vForward = qOrientation * m_vForward;

		SetFlag(EFlag::Changed);
	}

	//!	@brief		LookAt
	//!	@date		2015-04-04
	void Node::LookAt(const Math::Vector3& _vTarget)
	{
		Math::Vector3 vNewForward(_vTarget - GetPosition()); vNewForward.Normalize();
		if(vNewForward == m_vForward) return;		
		if(Math::Abs(vNewForward.Dot(m_vUp)) > 0.999999f) m_vUp = m_vRight;
		
		m_vRight = m_vUp.Cross(vNewForward); m_vRight.Normalize();
		m_vUp = vNewForward.Cross(m_vRight); m_vUp.Normalize();
		m_vForward = vNewForward;

		Math::Quaternion qOrientation(m_vRight.GetX(), m_vUp.GetY(), m_vForward.GetZ(), 1.0f);
		m_pNode->GetTransform().SetOrientation(qOrientation);

		SetFlag(EFlag::Changed);
	}

	//!	@brief		LookAt
	//!	@date		2015-04-04
	void Node::LookAt(float32 _fX, float32 _fY, float32 _fZ)
	{
		Math::Vector3 vTarget(_fX, _fY, _fZ);
		LookAt(vTarget);
	}
}}	
