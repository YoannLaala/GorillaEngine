#ifndef _COMPONENT_DIRECTIONAL_LIGHT_HPP_
#define _COMPONENT_DIRECTIONAL_LIGHT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Component/Renderable.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class DirectionalLight;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Component
{
	class DirectionalLight : public Renderable // Rendering simple geometry
	{
	public:
		DirectionalLight();
		~DirectionalLight();
		COMPONENT_DECLARE(Gorilla::Component::DirectionalLight);

	protected:
		//virtual bool Serialize		(FileWriter& /*_kStream*/) override { return true; }
		//virtual bool Deserialize	(FileReader& /*_kStream*/) override { return true; }
		virtual void PushCommand(Renderer::RenderBuffer* _pBuffer) override;

	private:
		Renderer::DirectionalLight* m_pLight;
	};
}}

#endif