#ifndef _RENDERER_GIZMO_HPP_
#define _RENDERER_GIZMO_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>
#include <Renderer/Node.hpp>
#include <Renderer/Color.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Renderer;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Gizmo
	{
	public:
		Gizmo(Renderer* _pRenderer);
		~Gizmo();

		inline void						SetIdentity		() { m_mTransform = Math::Matrix44::Identity; }

		// Position
		inline void						SetPosition		(const Math::Vector3& _vPosition) { m_mTransform.SetTranslation(_vPosition); }

		// Orientaion
		void							SetOrientation	(const Math::Quaternion& _qOrientation) { m_mTransform.SetRotation(_qOrientation); }

		// Scale
		inline void						SetScale		(const Math::Vector3& _vScale) { m_mTransform.SetScale(_vScale); }
		inline void						SetScale		(float32 _fScale) { m_mTransform.SetScale(Math::Vector3(_fScale, _fScale, _fScale)); }
		void							SetScale		(float32 _fX, float32 _fY, float32 _fZ) { m_mTransform.SetScale(Math::Vector3(_fX, _fY, _fZ)); }

		// Draw Simple Primitive from anywhere
		void							PushLine		(const Math::Vector3& _vStart, const Math::Vector3& _vEnd, Color _kColor = Color::White);
		void							PushQuad		(Color _kColor = Color::White);
		void							PushCube		(Color _kColor = Color::White);
		void							PushFrustum		(float32 _fFov = 90.0f, float32 _fNear = 0.1f, float32 _fFar = 1.0f, float32 _fAspect = 1.0f, Color _kColor = Color::White);

	private:
		Math::Matrix44	m_mTransform;
		Renderer*		m_pRenderer;
	};
}}

#endif