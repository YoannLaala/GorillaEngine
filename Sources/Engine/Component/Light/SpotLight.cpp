///******************************************************************************
//**	Includes
//******************************************************************************/
//#include "SpotLightComponent.hpp"
//
///******************************************************************************
//**	Class Definition
//******************************************************************************/
//namespace PantherEngine
//{
//	SpotLightComponent::SpotLightComponent()
//	{
//		m_eLight = PantherRenderer::ELight::Spot;
//	}
//
//	bool SpotLightComponent::Serialize(rapidjson::Writer<rapidjson::StringBuffer>& _kWriter)
//	{
//		return true;
//	}
//
//	bool SpotLightComponent::Deserialize(const rapidjson::Value& _kDocument)
//	{
//		return true;
//	}
//
//	void SpotLightComponent::Update()
//	{
//		SetRegisterable(true);
//	}
//
//	REGISTER_CLASS_SERIALIZABLE(PantherEngine::SpotLightComponent)
//}