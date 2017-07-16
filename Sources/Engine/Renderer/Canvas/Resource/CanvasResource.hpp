#ifndef _RENDERER_CANVAS_RESOURCE_HPP_
#define _RENDERER_CANVAS_RESOURCE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class CanvasContext;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class CanvasResource
	{
		friend class CanvasContext;

	public:
		CanvasResource();		
		~CanvasResource();
	
		virtual void Release() = 0;

	protected:
		inline uint32	GetId				() const { return m_uiId; }
		inline void		SetId				(uint32 _uiId) { m_uiId = _uiId; }

		inline void		AddReference		() { ++m_uiReferenceCount; }
		inline void		RemoveReference		() { --m_uiReferenceCount; }
		inline bool		IsReferenced		() const { return m_uiReferenceCount != 0; }

	private:
		uint32 m_uiType;
		uint32 m_uiId;
		uint32 m_uiReferenceCount;
	};
}}

#endif