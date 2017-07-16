/******************************************************************************
**	Includes
******************************************************************************/
#include "Gizmo.hpp"

#include <Renderer/Renderer.hpp>
#include <Renderer/Primitive.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	Gizmo::Gizmo(Renderer* _pRenderer)
		: m_pRenderer(_pRenderer)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	Gizmo::~Gizmo()
	{
		m_pRenderer = NULL;
	}

	////!	@brief		PushLine
	////!	@date		2015-11-07
	void Gizmo::PushLine(const Math::Vector3& _vStart, const Math::Vector3& _vEnd, Color _kColor /*= Color::White*/)
	{
		RenderBuffer* pFrameBruffer = m_pRenderer->GetFrameBuffer();

		Primitive::Instance<Primitive::Line>* pInstance = pFrameBruffer->Push<Primitive::Instance<Primitive::Line>>();
		pInstance->Start = _vStart;
		pInstance->End = _vEnd;
		pInstance->Color.Set(_kColor.GetRed() / 255.0f, _kColor.GetGreen() / 255.0f, _kColor.GetBlue() / 255.0f);
	}

	////!	@brief		PushQuad
	////!	@date		2015-11-07
	void Gizmo::PushQuad(Color _kColor /*= Color::White*/)
	{
		RenderBuffer* pFrameBruffer = m_pRenderer->GetFrameBuffer();

		Primitive::Instance<Primitive::Quad>* pInstance = pFrameBruffer->Push<Primitive::Instance<Primitive::Quad>>();
		pInstance->World = m_mTransform;
		pInstance->Color.Set(_kColor.GetRed() / 255.0f, _kColor.GetGreen() / 255.0f, _kColor.GetBlue() / 255.0f);
	}

	////!	@brief		PushCube
	////!	@date		2015-11-07
	void Gizmo::PushCube(Color _kColor /*= Color::White*/)
	{
		RenderBuffer* pFrameBruffer = m_pRenderer->GetFrameBuffer();

		Primitive::Instance<Primitive::Cube>* pInstance = pFrameBruffer->Push<Primitive::Instance<Primitive::Cube>>();
		pInstance->World = m_mTransform;
		pInstance->Color.Set(_kColor.GetRed() / 255.0f, _kColor.GetGreen() / 255.0f, _kColor.GetBlue() / 255.0f);
	}

	////!	@brief		PushFrustum
	////!	@date		2015-11-07
	void Gizmo::PushFrustum(float32 _fFov /*= 90.0f*/, float32 _fNear /*= 0.1f*/, float32 _fFar /*= 1.0f*/, float32 _fAspect /*= 1.0f*/, Color _kColor /*= Color::White*/)
	{
		RenderBuffer* pFrameBruffer = m_pRenderer->GetFrameBuffer();

		Primitive::Instance<Primitive::Frustum>* pInstance = pFrameBruffer->Push<Primitive::Instance<Primitive::Frustum>>();
		pInstance->World = m_mTransform;
		pInstance->Color.Set(_kColor.GetRed() / 255.0f, _kColor.GetGreen() / 255.0f, _kColor.GetBlue() / 255.0f);
		pInstance->Fov = Math::ToRadian(_fFov);
		pInstance->Near = _fNear;
		pInstance->Far = _fFar;
		pInstance->Aspect = _fAspect;
	}
}}