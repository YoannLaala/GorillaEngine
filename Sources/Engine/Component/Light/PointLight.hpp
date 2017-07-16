#ifndef _COMPONENT_POINT_LIGHT_HPP_
#define _COMPONENT_POINT_LIGHT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Component/RenderableNode.hpp>
#include <Renderer/Color.hpp>
#include <Renderer/Light/PointLight.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Component
{
	class PointLight : public RenderableNode // Rendering simple geometry
	{
	public:
		PointLight();
		~PointLight();
		COMPONENT_DECLARE(Gorilla::Component::PointLight);

		inline void SetColor(Renderer::Color _kColor) { m_pLight->SetColor(_kColor.GetRed() / 255.0f, _kColor.GetGreen() / 255.0f, _kColor.GetBlue() / 255.0f); }

	protected:
		//virtual bool Serialize		(FileWriter& /*_kStream*/) override { return true; }
		//virtual bool Deserialize	(FileReader& /*_kStream*/) override { return true; }
		virtual void PushCommand(Renderer::RenderBuffer* _pBuffer) override;

	private:
		Renderer::PointLight* m_pLight;
	};
}}

#endif