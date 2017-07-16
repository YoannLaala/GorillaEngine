#ifndef _ENGINE_MATERIAL_HPP_
#define _ENGINE_MATERIAL_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Asset/Effect/Shader.hpp>
#include <Engine/Asset/Texture/Texture.hpp>
#include <Engine/Asset/AssetHandle.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Material;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class Material : public Asset
	{
	public:
		Material();
		~Material();
		ASSET_DECLARE(Gorilla::Engine::Material);

	protected:
		virtual void Initialize	(StreamReader* _pStream) override;
		virtual void Release	() override;

	public:
		void SetShader(const AssetHandle<Shader>& _hShader);
		void SetNumber(uint32 _uiIndex, float32 _fValue);
		void SetNumber(uint32 _uiIndex, const Math::Vector3& _vValue);
		void SetTexture(uint32 _uiIndex, const AssetHandle<Texture>& _hTexture);

		inline Renderer::Material* GetData() const { return m_pMaterial; }

	private:
		Vector<AssetHandle<Texture>>	m_vTexture;
		AssetHandle<Shader>				m_hShader;
		Renderer::Material*				m_pMaterial;
	};
}}

#endif