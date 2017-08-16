#ifndef _COMPONENT_NODE_HPP_
#define _COMPONENT_NODE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Component/Component.hpp>
#include <Renderer/Node.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class GameObject;
}}

namespace Gorilla { namespace Renderer
{
	class IRenderable;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Component
{
	class Node : public Engine::Component
	{
		struct EFlag
		{
			enum Type
			{
				Changed = 0x01,
			};
		};

	public:
		Node();
		~Node();
		COMPONENT_DECLARE_EXTENDED(Gorilla::Component::Node, 0);

	protected:
		virtual void Start			() override;
		virtual void Update			() override;
		virtual void Stop			() override;

	public:
		inline uint32					GetLayer	() const {  m_pNode->GetLayer();}
		inline void						SetLayer	(uint32 _uiLayer) {  m_pNode->SetLayer(_uiLayer);}

		Math::Matrix44& 				GetTransform	() { return m_pNode->GetTransform(); }
		const Math::Matrix44& 			GetTransform	() const { return m_pNode->GetTransform(); }

		// Position
		inline const Math::Vector3&		GetPosition		() const { return m_pNode->GetTransform().GetTranslation(); }
		inline void						SetPosition		(const Math::Vector3& _vPosition) { SetPosition(_vPosition.GetX(), _vPosition.GetY(), _vPosition.GetZ()); }
		inline void						SetPosition		(float32 _fX, float32 _fY, float32 _fZ) { m_pNode->GetTransform().SetTranslation(_fX, _fY, _fZ); SetFlag(EFlag::Changed); }

		// Move
		inline void						MoveRight		(float32 _fDistance = 1.0f) { Move(m_vRight * _fDistance); }
		inline void						MoveUp			(float32 _fDistance = 1.0f) { Move(m_vUp * _fDistance); }
		inline void						MoveForward		(float32 _fDistance = 1.0f) { Move(m_vForward * _fDistance); }
		inline void						Move			(const Math::Vector3& _vTranslation) { m_pNode->GetTransform().Translate(_vTranslation); SetFlag(EFlag::Changed); }
		
		// Orientation
		void							SetOrientation	(const Math::Vector3& _vEuler) { SetOrientation(_vEuler.GetX(), _vEuler.GetY(), _vEuler.GetZ()); }
		inline void						SetOrientation	(float32 _fEulerX, float32 _fEulerY, float32 _fEulerZ) { Math::Quaternion qOrientation; qOrientation.Rotate(_fEulerX, _fEulerY, _fEulerZ); SetOrientation(qOrientation); }
		void							SetOrientation	(const Math::Quaternion& _qOrientation);

		// Rotation
		inline void						RotateRight		(float32 _fAngle) { Rotate(m_vRight, _fAngle); }
		inline void						RotateUp		(float32 _fAngle) { Rotate(m_vUp, _fAngle); }
		inline void						RotateForward	(float32 _fAngle) { Rotate(m_vForward, _fAngle); }
		inline void						RotateX			(float32 _fAngle) { Rotate(Math::Vector3::UnitX, _fAngle); }
		inline void						RotateY			(float32 _fAngle) { Rotate(Math::Vector3::UnitY, _fAngle); }
		inline void						RotateZ			(float32 _fAngle) { Rotate(Math::Vector3::UnitZ, _fAngle); }
		void							Rotate			(const Math::Vector3& _vAxis, float32 _fAngle);

		// Scale
		inline const Math::Vector3&		GetScale		() const { return m_pNode->GetTransform().GetScale(); }
		inline void						SetScale		(float32 _fScale) { m_pNode->GetTransform().SetScale(_fScale, _fScale, _fScale); SetFlag(EFlag::Changed); }
		inline void						SetScale		(float32 _fX, float32 _fY, float32 _fZ) { m_pNode->GetTransform().SetScale(_fX, _fY, _fZ); SetFlag(EFlag::Changed); }
		inline void						SetScale		(const Math::Vector3& _vScale) { m_pNode->GetTransform().SetScale(_vScale); SetFlag(EFlag::Changed); }

		// Look
		void							LookAt			(const Math::Vector3& _vTarget);
		void							LookAt			(float32 _fX, float32 _fY, float32 _fZ);

		// Axis
		inline const Math::Vector3&		GetRight		() const { return m_vRight; }
		inline const Math::Vector3&		GetUp			() const { return m_vUp; }
		inline const Math::Vector3&		GetForward		() const { return m_vForward; }	

		// Render Component
		inline void						AddRendeable(Renderer::IRenderable* _pObject) { m_pNode->AddRenderable(_pObject); }
		inline void						RemoveRenderable(Renderer::IRenderable* _pObject) { m_pNode->RemoveRenderable(_pObject); }

	private:
		Math::Vector3		m_vUp;
		Math::Vector3		m_vRight;
		Math::Vector3		m_vForward;

	private:
		Renderer::Node*		m_pNode;
	};
}}

#endif