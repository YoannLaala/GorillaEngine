#ifndef _ENGINE_MESH_HPP_
#define _ENGINE_MESH_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Asset/Asset.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Geometry;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class Mesh : public Asset
	{
	public:
		Mesh();
		~Mesh();
		ASSET_DECLARE(Gorilla::Engine::Mesh);

	protected:
		virtual void Initialize	(StreamReader* _pStream) override;
		virtual void Release	() override;
		
	public:
		inline Renderer::Geometry* GetData() { return m_pGeometry; }

	private:
		Renderer::Geometry* m_pGeometry;
	};
}}

#endif