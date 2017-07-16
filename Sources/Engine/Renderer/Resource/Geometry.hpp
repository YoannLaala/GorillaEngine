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

	public:
		DECLARE_RESOURCE(EResource::Geometry);

	private:
		Geometry();
		~Geometry();

		void Initialize	(Buffer* _pVertexBuffer, Buffer* _pIndexBuffer, const uint32* _pSectionBuffer, uint32 _uiSectionCount);
		virtual void Release () override;
	
	public:
		inline Buffer*	GetVertexBuffer() { return m_pVertexBuffer; }
		inline Buffer*	GetIndexBuffer() { return m_pIndexBuffer; }
		inline uint8	GetSectionCount() const { return (uint8)m_vSection.GetSize(); }
		inline uint32	GetIndexOffset(uint8 _uiSection = 0) const { return _uiSection ? m_vSection[_uiSection-1] : 0; }
		inline uint32	GetIndexCount(uint8 _uiSection = 0) const { return m_vSection[_uiSection]; }

	private:
		Buffer* m_pVertexBuffer;
		Buffer* m_pIndexBuffer;
		Vector<uint32> m_vSection;
	};
}}

#endif