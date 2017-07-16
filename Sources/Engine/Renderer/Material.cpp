/******************************************************************************
**	Includes
******************************************************************************/
#include "Material.hpp"
#include <Renderer/Renderer.hpp>
#include <Renderer/RenderContext.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	Material::Material()
		: m_pShader(NULL)
		, m_pNumberBuffer(NULL)
		, m_eFlag(0)
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	Material::~Material()
	{
		// Nothing to do
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	void Material::Initialize(Shader* _pShader, float32* _pNumber, uint32 _uiNumberCount)
	{
		m_pShader = _pShader;
		
		// Copy numbers
		if(_pNumber)
		{
			uint32 uiByte = _uiNumberCount * sizeof(float32);
			m_vNumber.Resize(_uiNumberCount);
			memcpy_s(&m_vNumber[0], uiByte,  _pNumber,uiByte );
			SetFlag(EFlag::NumberChanged);
		}		
	}

	//!	@brief		Release 
	//!	@date		2015-11-11
	void Material::Release()
	{
		// Nothing to do
	}

	//!	@brief		Synchronize 
	//!	@date		2015-11-11
	void Material::Synchronize(RenderContext* _pContext)
	{
		Renderer* pRenderer = _pContext->GetRenderer();
		if(HasFlag(EFlag::NumberChanged))
		{
			// Check if the size is large enough
			const uint32 uiNumberCount = m_vNumber.GetSize();
			if(m_pNumberBuffer && m_pNumberBuffer->GetElementCount() < uiNumberCount)
			{
				pRenderer->DestroyResource(m_pNumberBuffer);
			}

			// Create buffer if needed
			if(!m_pNumberBuffer)
			{
				m_pNumberBuffer = pRenderer->CreateShaderResourceBuffer(sizeof(float32), uiNumberCount, EBind::ShaderResource, EUsage::Dynamic, NULL);
			}

			// Copy the current data
			uint32 uiByteCount = uiNumberCount * sizeof(float32);
			float32* pBuffer = reinterpret_cast<float32*>(pRenderer->Map(_pContext, m_pNumberBuffer));
			memcpy_s(pBuffer, uiByteCount, &m_vNumber[0], uiByteCount);
			pRenderer->Unmap(_pContext, m_pNumberBuffer);

			RemoveFlag(EFlag::NumberChanged);
		}
	}
}}