#ifndef _COMPONENT_MATERIAL_HPP_
#define _COMPONENT_MATERIAL_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Component/Component.hpp>
#include <Engine/Asset/AssetHandle.hpp>
#include <Engine/Asset/Effect/Shader.hpp>
#include <Engine/Asset/Texture/Texture.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Component
{
	class Material : public Engine::Component
	{
	public:
		Material();
		~Material();
		COMPONENT_DECLARE(Gorilla::Component::Material);

		//virtual bool Deserialize	(FileReader& /*_kStream*/) override { return true; }


	private:
		
		//DECLARE_ATTRIBUTE_VECTOR(Engine::Texture, Textures)
		//DECLARE_ATTRIBUTE_ASSET_VECTOR(Engine::Texture, Textures)

		/*class Instance
		{
			
		public:
			inline void SetNumber(uint32 _uiIndex, float32 _fValue)
			{
				m_vNumber.ExpandTo(_uiIndex);
				m_vNumber[_uiIndex] = _fValue;
			}

		public:
			Engine::AssetHandle<Engine::Shader> Shader;

		private:
			Vector<float32> m_vNumber;
		};*/

		class Parameter
		{
		public:

			Parameter()
			{
				/*REGISTER_ATTRIBUTE_ARRAY("Parameters", Parameters);
				REGISTER_ATTRIBUTE("Shader", "Shader used for the material rendering", Shader);*/
			}

			~Parameter()
			{
				// Nothing to do
			}

			enum Type : uint8
			{
				Shader = 0,
				Float32,
				Texture,
				Count
			};

			inline Type GetType() const { return m_eType; }

			inline void Get(Engine::AssetHandle<Engine::Shader>& _hValue) const { _hValue = *reinterpret_cast<Engine::AssetHandle<Engine::Shader>*>(m_kValue.Pointer); }
			inline void Get(float32& _fValue) const { _fValue = m_kValue.Float32; }
			inline void Get(Engine::AssetHandle<Engine::Texture>& _hValue) const { _hValue = *reinterpret_cast<Engine::AssetHandle<Engine::Texture>*>(m_kValue.Pointer); }

			void Set(const Engine::AssetHandle<Engine::Shader>& _hShader) 
			{ 
				SetType(Type::Shader);
				m_kValue.Pointer = new Engine::AssetHandle<Engine::Shader>(_hShader);
			}

			void Set(float32 _fValue) 
			{ 
				SetType(Type::Float32);
				m_kValue.Float32 = _fValue;
			}

			void Set(const Engine::AssetHandle<Engine::Texture>& _hTexture) 
			{ 
				SetType(Type::Texture);
				m_kValue.Pointer = new Engine::AssetHandle<Engine::Texture>(_hTexture);
			}

		private:
			void SetType(Type _eType)
			{
				switch(m_eType)
				{
					case Type::Shader:
					case Type::Texture:
					{
						SAFE_DELETE(m_kValue.Pointer);
						break;
					}
				}

				m_eType = _eType;
			}

		private:
			struct Value
			{
				uint64 UInt64;
				union
				{
					float32 Float32;
					void* Pointer;
				};
			};

			Value			m_kValue;
			Type			m_eType;

			/*String			m_sName;
			Math::Vector2	m_vPosition;
			bool			m_bIsExposed;*/
		};


		//class testSerialization
		//{
		//	virtual bool Serialize	(JsonWriter& /*_kStream*/)  { return true; }
		//};

		//#define DECLARE_ATTRIBUTE_ASSET(_type_, _name_) Engine::AssetHandle<_type_> _name_;

		struct MaterialInstance
		{			
			Vector<Parameter> Parameters;				// All parameters available for this Material
			Engine::AssetHandle<Engine::Shader> Shader;	// Shader used for the rendering
		};


	public:

		void SetShader(uint32 _uiMaterialIndex, const Engine::AssetHandle<Engine::Shader>& _hShader)
		{
			m_vInstance[_uiMaterialIndex].Shader = _hShader;
		}

		void SetParameter(uint32 _uiMaterialIndex, uint32 _uiParameterIndex, float32 _fValue)
		{
			Parameter& kParameter = m_vInstance[_uiMaterialIndex].Parameters[_uiParameterIndex];
			kParameter.Set(_fValue);
		}

		void SetParameter(uint32 _uiMaterialIndex, uint32 _uiParameterIndex, const Engine::AssetHandle<Engine::Texture>& _hValue)
		{
			Parameter& kParameter = m_vInstance[_uiMaterialIndex].Parameters[_uiParameterIndex];
			kParameter.Set(_hValue);
		}

	private:
		Vector<MaterialInstance> m_vInstance;
	};
}}	// Gorilla::Asset
#endif