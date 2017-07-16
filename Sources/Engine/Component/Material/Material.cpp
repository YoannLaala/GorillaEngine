/******************************************************************************
**	Includes
******************************************************************************/
#include "Material.hpp"	
#include <Engine/Engine.hpp>
#include <Engine/Asset/AssetManager.hpp>

//#define COMPONENT_REGISTER2(_class_) COMPONENT_REGISTER(_class_)	\
//void _class_::ClassIdentifier::Initialize()
//
//#define REGISTER_ATTRIBUTE(_name_, _comment_, _test_)

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Component
{	
	COMPONENT_REGISTER(Gorilla::Component::Material,
	{
		/*REGISTER_ATTRIBUTE_CUSTOM(m_vInstance, "List", "Contains all material instance for this component",
		{
			REGISTER_ATTRIBUTE_ASSET(Shader, "Shader", "Shader used for the rendering");
		});*/

		//REGISTER_ATTRIBUTE(m_bMyBool, "MyBool", "Great Bool comment");

		////REGISTER_ATTRIBUTE(m_vMaterials, "MyMate", "Great Material")
		//{
		//	REGISTER_ATTRIBUTE(m_bMyBool2, "MyBool", "Great Bool comment");
		//	//REGISTER_ATTRIBUTE_ASSET(Shader, "MyShader", "Great Shader");
		//	//REGISTER_ATTRIBUTE(Parameter, "List", "Every Parameters")
		//	
		//	{
		//		//REGISTER_ATTRIBUTE_ASSET(m_kValue, "Value", "ParameterValue");
		//		//REGISTER_ATTRIBUTE_ASSET(m_eType, "Type", "Parameter TYpe");
		//	}
		//}
	})

	//!	@brief		Constructor
	//!	@date		2015-04-04
	Material::Material()
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	Material::~Material()
	{
		// Nothing to do
	}
}}	
