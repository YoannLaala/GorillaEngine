#ifndef _COMPONENT_CAMERA_HPP_
#define _COMPONENT_CAMERA_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Component/Component.hpp>
#include <Renderer/Camera.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class RenderContext;
	class Camera;
	class Viewport;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Component
{
	class Camera : public Engine::Component
	{
	public:
		struct EProjection
		{
			enum Type : uint8
			{
				Perspective = 0,
				Orthocgraphic
			};
		};

		struct EFlag
		{
			enum Type
			{
				Changed = 1 << 0,
			};
		};

		Camera();
		~Camera();
		COMPONENT_DECLARE(Gorilla::Component::Camera);

	protected:
		virtual void Start			() override;
		virtual void Update			() override;
		virtual void Stop			() override;

	public:
		inline bool		HasLayer	(uint32 _uiLayer) const { return (m_uiLayer & _uiLayer) != 0; }
		inline void		SetLayer	(uint32 _uiLayer) {  m_uiLayer |= _uiLayer; SetFlag(EFlag::Changed); }
		inline void		RemoveLayer	(uint32 _uiLayer) {  m_uiLayer &= ~_uiLayer; SetFlag(EFlag::Changed); }

		inline float32	GetFOV		() const { return m_fFOV; }
		inline void		SetFOV		(float32 _fFOV) { m_fFOV = _fFOV; SetFlag(EFlag::Changed); }

		inline float32	GetZNear	() const { return m_fZNear; }
		inline void		SetZNear	(float32 _fZNear) { m_fZNear = _fZNear; SetFlag(EFlag::Changed); }

		inline float32	GetZFar		() const { return m_fZFar; }
		inline void		SetZFar		(float32 _fZFar) { m_fZFar = _fZFar; SetFlag(EFlag::Changed); }

		Renderer::Viewport*	Viewport;

	private:
		Renderer::Camera*	m_pCamera;
		float32				m_fFOV;
		float32				m_fZNear;
		float32				m_fZFar;
		uint32				m_uiLayer;
		EProjection::Type	m_eProjection;
	};
}}

#endif