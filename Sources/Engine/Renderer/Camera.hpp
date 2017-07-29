#ifndef _RENDERER_CAMERA_HPP_
#define _RENDERER_CAMERA_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Scene;
	class Viewport;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Camera
	{
		friend class Scene;

	public:
		Camera();
		~Camera();

	public:
		inline bool						HasLayer		(uint32 _uiLayer) const { return (m_uiLayer & _uiLayer) != 0; }
		inline void						SetLayer		(uint32 _uiLayer) {  m_uiLayer |= _uiLayer; }
		inline void						RemoveLayer		(uint32 _uiLayer) {  m_uiLayer &= ~_uiLayer; }

		inline const Math::Matrix44&	GetProjection	() const { return m_mProjection; }
		void							SetPerspective	(float32 _fFOV, float32 _fAspect, float32 _fZNear, float32 _fZFar);
		void							SetOrthographic	(float32 _fWidth, float32 _fHeight, float32 _fZNear, float32 _fZFar);

		inline const Math::Vector3&		GetPosition		() const { return m_vPosition;}						
		
		inline const Math::Matrix44&	GetView			() const { return m_mView; }
		void							SetView			(const Math::Vector3& _vPosition, const Math::Vector3& _vRight, const Math::Vector3& _vUp, const Math::Vector3& _vForward);

		inline Viewport*				GetViewport		() const { return m_pViewport; }
		inline void						SetViewport		(Viewport* _pViewport) { m_pViewport = _pViewport; }

	public:
		static void						ComputePerspective(Math::Matrix44& _mProjection, float32 _fFOV, float32 _fAspect, float32 _fZNear, float32 _fZFar);
		static void						ComputeOthographic(Math::Matrix44& _mProjection, float32 _fWidth, float32 _fHeight, float32 _fZNear, float32 _fZFar);
		static void						ComputeView(Math::Matrix44& _mView, const Math::Vector3& _vPosition, const Math::Vector3& _vRight, const Math::Vector3& _vUp, const Math::Vector3& _vForward);

	private:
		Math::Matrix44		m_mView;
		Math::Matrix44		m_mProjection;
		Math::Vector3		m_vPosition;
		Viewport*			m_pViewport;
		uint32				m_uiLayer;
	};
}}

#endif