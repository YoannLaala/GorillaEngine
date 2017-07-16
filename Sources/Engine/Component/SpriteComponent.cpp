///******************************************************************************
//**	Includes
//******************************************************************************/
//#include "../Common.hpp"
//#include "SpriteComponent.hpp"
//
//// Asset
//#include <Renderer/Asset/Texture.hpp>
//
///******************************************************************************
//**	Class Definition
//******************************************************************************/
//namespace PantherEngine
//{
//	SpriteComponent::SpriteComponent()
//	{
//		m_pTexture = NULL;
//	}
//
//	void SpriteComponent::SetTexture(const char* _szRelativeFilePath)
//	{
//		m_pTexture = GetAssetManager()->GetAsset<PantherRenderer::Texture>(_szRelativeFilePath);
//	}
//
//	void SpriteComponent::Release()
//	{
//		m_pTexture = NULL;
//	}
//
//	SpriteComponent::~SpriteComponent()
//	{
//		Release();
//	}
//}