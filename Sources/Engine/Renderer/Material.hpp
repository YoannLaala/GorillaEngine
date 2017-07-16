#ifndef _RENDERER_MATERIAL_HPP_
#define _RENDERER_MATERIAL_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Renderer;
	class RenderContext;
	class Shader;
	class Buffer;
	class Texture2D;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Material
	{
		friend class Renderer;

		struct EFlag
		{
			enum Type
			{
				NumberChanged = 1 << 0,
			};
		};

	private:
		Material();
		~Material();

		void Initialize(Shader* _pPixelShader, float32* _pNumber, uint32 _uiNumberCount);
		void Release();

		void Synchronize(RenderContext* _pContext);

	public:
		inline Shader* GetShader() { return m_pShader; }
		inline Buffer* GetNumberBuffer() { return m_pNumberBuffer; }
		inline const Vector<Texture2D*>& GetVecTexture() { return m_vTexture; }

		inline void SetNumber (uint32 _uiIndex, float32 _fValue) { m_vNumber.ExpandTo(_uiIndex); m_vNumber[_uiIndex] = _fValue; SetFlag(EFlag::NumberChanged); }
		inline void SetNumber (uint32 _uiIndex, const Math::Vector3& _vValue) { m_vNumber.ExpandTo(_uiIndex + 2); memcpy_s(&m_vNumber[_uiIndex], sizeof(Math::Vector3), &_vValue, sizeof(Math::Vector3)); SetFlag(EFlag::NumberChanged); }
		inline void SetTexture(uint32 _uiIndex, Texture2D* _pTexture) { m_vTexture.ExpandTo(_uiIndex); m_vTexture[_uiIndex] = _pTexture;  }

		inline bool HasChanged() const { return m_eFlag != 0; }

	private:
		inline bool	HasFlag		(EFlag::Type _eFlag) const { return (m_eFlag & _eFlag) != 0; }
		inline void	SetFlag		(EFlag::Type _eFlag) { m_eFlag |= _eFlag; }
		inline void	RemoveFlag	(EFlag::Type _eFlag) { m_eFlag &= ~_eFlag; }

	private:
		Vector<Texture2D*>	m_vTexture;
		Vector<float32>		m_vNumber;
		Shader*				m_pShader;
		Buffer*				m_pNumberBuffer;
		uint8				m_eFlag;
	};
}}

#endif