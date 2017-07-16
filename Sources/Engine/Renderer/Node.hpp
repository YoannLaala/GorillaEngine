#ifndef _RENDERER_NODE_HPP_
#define _RENDERER_NODE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Octree;
	class IRenderable;
	class Gizmo;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Node
	{
		friend class Octree;
		friend class Gizmo;

	public:
		Node();
		~Node();

	public:
		inline uint32					GetLayer		() const { return m_uiLayer; };
		inline void						SetLayer		(uint32 _uiLayer) {  m_uiLayer = _uiLayer; }
		
		// Position
		inline const Math::Vector3&		GetPosition		() const { return m_vPosition; }
		inline void						SetPosition		(const Math::Vector3& _vPosition) { m_vPosition = _vPosition; }
		inline void						SetPosition		(float32 _fX, float32 _fY, float32 _fZ) { m_vPosition.Set(_fX, _fY, _fZ); }
		inline void						Move			(const Math::Vector3& _vTranslation) { m_vPosition += _vTranslation; }

		// Orientaion
		inline const Math::Quaternion&	GetOrientation	() const { return m_qOrientation; }
		inline void						SetOrientation	(const Math::Quaternion& _qOrientation) { m_qOrientation = _qOrientation; }
		inline void						SetOrientation	(const Math::Vector3& _vEulerOrientation) { m_qOrientation.Rotate(_vEulerOrientation.GetX(), _vEulerOrientation.GetY(), _vEulerOrientation.GetZ()); }
		inline void						SetOrientation	(float32 _fX, float32 _fY, float32 _fZ) { m_qOrientation.Rotate(_fX, _fY, _fZ);}
		inline void						Rotate			(const Math::Quaternion& _qOrientation) { m_qOrientation *= _qOrientation; }

		// Scale
		inline const Math::Vector3&		GetScale		() const { return m_vScale; }
		inline void						SetScale		(const Math::Vector3& _vScale) { m_vScale = _vScale; }
		inline void						SetScale		(float32 _fScale) { m_vScale.Set(_fScale, _fScale, _fScale); }
		inline void						SetScale		(float32 _fX, float32 _fY, float32 _fZ) { m_vScale.Set(_fX, _fY, _fZ); }

		// Component
		inline void							AddRenderable		(IRenderable* _pObject) { m_vRenderable.Add(_pObject); }
		inline void							RemoveRenderable	(IRenderable* _pObject) { m_vRenderable.Remove(_pObject); }
		inline const Vector<IRenderable*>&	GetVecRenderable	() const { return m_vRenderable; }

	private:
		Vector<IRenderable*>		m_vRenderable;		
		Math::Vector3				m_vPosition;
		Math::Vector3				m_vScale;
		Math::Quaternion			m_qOrientation;
		uint32						m_uiLayer;
	};
}}

#endif