#ifndef _COMPONENT_MESH_HPP_
#define _COMPONENT_MESH_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Component/RenderableNode.hpp>
#include <Engine/Asset/AssetHandle.hpp>
#include <Engine/Asset/Mesh/Mesh.hpp>
#include <Engine/Asset/Material/Material.hpp>
//#include <Asset/Effect/Effect.hpp>
//#include <Asset/Texture/Texture.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class RenderContext;
}}

/*#define DECLARE_ASSET_PROPERTY(_type_, _name_) 																							\
public:																																	 \
	inline Engine::AssetHandle<_type_>& Get##_name_() { return _name_; }																 \
	inline void Set##_name_(const char* _szRelativePath, const char* _szParam = NULL) { SetAsset(_name_, _szRelativePath, _szParam); }	 \
protected:																																 \
	Engine::AssetHandle<_type_> _name_;																									 \

#define DECLARE_ASSET_PROPERTY_ARRAY(_type_, _name_) 																					\
public:																																	 \
	inline Engine::AssetHandle<_type_>& Get##_name_(uint32 _uiIndex,) { return _name_[_uiIndex]; }																 \
	inline void Set##_name_(uint32 _uiIndex, const char* _szRelativePath, const char* _szParam = NULL) { SetAsset(_name_[_uiIndex], _szRelativePath, _szParam); }	 \
protected:																																 \
	Vector<Engine::AssetHandle<_type_>> _name_;		*/																					 \

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Component
{
	class Mesh : public RenderableNode // Rendering simple geometry
	{
	public:
		Mesh();
		~Mesh();
		COMPONENT_DECLARE(Gorilla::Component::Mesh);

	protected:
		virtual void Start			() override;
		virtual void Update			() override;
		virtual void Stop			() override;

	protected:
		//virtual bool Serialize		(FileWriter& /*_kStream*/) override { return true; }
		//virtual bool Deserialize	(FileReader& /*_kStream*/) override { return true; }

		virtual void PushCommand(Renderer::RenderBuffer* _pBuffer) override;

		/*template <class T>
		void SetAsset(Engine::AssetHandle<T>& hAsset, const char* _szRelativePath, const char* _szParam)
		{
			hAsset = GetAssetManager()->Get<T>(_szRelativePath, _szParam);
		}*/

	public:
		Engine::AssetHandle<Engine::Mesh> Asset;
		Vector<Engine::AssetHandle<Engine::Material>> Materials;
	};
}}

#endif