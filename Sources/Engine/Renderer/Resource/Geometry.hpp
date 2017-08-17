#ifndef _RENDERER_GEOMETRY_HPP_
#define _RENDERER_GEOMETRY_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/Resource/Resource.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Renderer;
	class RenderContext;
	class Buffer;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Geometry : public Resource
	{
		friend class Renderer;
		friend class RenderContext;

		struct Group
		{
			uint32 IndexOffset;
			uint32 IndexCount;
		};

	public:
		DECLARE_RESOURCE(EResource::Geometry);

	private:
		Geometry();
		~Geometry();

		void Initialize	(Buffer* _pVertexBuffer, Buffer* _pIndexBuffer, const uint32* _pGroupBuffer, uint32 _uiGroupCount);
		virtual void Release () override;
	
	public:
		inline Buffer*	GetVertexBuffer() { return m_pVertexBuffer; }
		inline Buffer*	GetIndexBuffer() { return m_pIndexBuffer; }
		inline uint8	GetGroupCount() const { return (uint8)m_vGroup.GetSize(); }
		inline uint32	GetIndexOffset(uint8 _uiGroup = 0) const { return m_vGroup[_uiGroup].IndexOffset; }
		inline uint32	GetIndexCount(uint8 _uiGroup = 0) const { return m_vGroup[_uiGroup].IndexCount; }

	private:
		Buffer*			m_pVertexBuffer;
		Buffer*			m_pIndexBuffer;
		Vector<Group>	m_vGroup;
	};
}}

#endif