#ifndef _ENGINE_MESH_HPP_
#define _ENGINE_MESH_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Asset/Asset.hpp>
#include <Renderer/Geometry/BoundingBox.hpp>

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
		inline Renderer::Geometry*			GetGeometry() { return m_pGeometry; }
		inline const Renderer::BoundingBox&	GetBoundingBox() const { return m_kBoundingBox; }

	private:
		Renderer::BoundingBox	m_kBoundingBox;
		Renderer::Geometry*		m_pGeometry;
	};
}}

#endif