/******************************************************************************
**	Includes
******************************************************************************/
#include "Material.hpp"

#include <Engine/Engine.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/Material.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	ASSET_REGISTER(Gorilla::Engine::Material);

	//!	@brief		Contructor
	//!	@date		2015-10-18
	Material::Material()
		: m_pMaterial(NULL)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-10-18
	Material::~Material()
	{
		// Nothing to do
	}

	//!	@brief		Initialize
	//!	@date		2015-10-18
	void Material::Initialize(StreamReader* /*_pStream*/)
	{
		// Read dependency
		/*ReadDependency(_kStream, m_hPixelShader);

		// Read header
		uint32 uiNumberCount;
		_kStream.Read(uiNumberCount);

		float32 aNumber[256];
		_kStream.Read(aNumber, uiNumberCount * sizeof(float32));

		// Create the Material
		m_pMaterial = GetRenderer()->CreateMaterial(m_hPixelShader->GetData(), aNumber, uiNumberCount);*/
	}

	//!	@brief		Release
	//!	@date		2015-10-18
	void Material::Release()
	{

	}

	//!	@brief		SetShader
	//!	@date		2015-10-18
	void Material::SetShader(const AssetHandle<Shader>& _hShader)
	{
		m_hShader = _hShader;
		m_pMaterial = GetRenderer()->CreateMaterial(m_hShader->GetData());
	}

	//!	@brief		SetNumber
	//!	@date		2015-10-18
	void Material::SetNumber(uint32 _uiIndex, float32 _fValue)
	{
		m_pMaterial->SetNumber(_uiIndex, _fValue);
	}

	//!	@brief		SetNumber
	//!	@date		2015-10-18
	void Material::SetNumber(uint32 _uiIndex, const Math::Vector3& _vValue)
	{
		m_pMaterial->SetNumber(_uiIndex, _vValue);
	}

	//!	@brief		SetTexture
	//!	@date		2015-10-18
	void Material::SetTexture(uint32 _uiIndex, const AssetHandle<Texture>& _hTexture)
	{
		m_vTexture.ExpandTo(_uiIndex);
		m_vTexture[_uiIndex] = _hTexture;
		m_pMaterial->SetTexture(_uiIndex, _hTexture->GetData());
	}
}}