#ifndef _ENGINE_CUBE_MAP_HPP_
#define _ENGINE_CUBE_MAP_HPP_

/******************************************************************************
**	Include
******************************************************************************/
#include <Engine/Asset/Asset.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Texture2D;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class CubeMap : public Asset
	{
	public:
		CubeMap();
		~CubeMap();
		ASSET_DECLARE(Gorilla::Engine::CubeMap);

	protected:
		virtual void Initialize	(StreamReader* _pStream) override;
		virtual void Release	() override;

	public:
		inline Renderer::Texture2D* GetData() { return m_pTexture; };

	private:
		Renderer::Texture2D* m_pTexture;
	};
}}	// Gorilla::Asset

#endif