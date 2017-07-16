#ifndef _RENDERER_RESOURCE_HPP_
#define _RENDERER_RESOURCE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/Common.hpp>

/******************************************************************************
**	Defines
******************************************************************************/
#define DECLARE_RESOURCE_VIRTUAL()												\
virtual uint8 GetType() const = 0;												\

#define DECLARE_RESOURCE(_type_)												\
class Class																		\
{																				\
	friend class RenderContext;													\
	static inline uint8 GetType() { return _type_; }							\
};																				\
virtual uint8 GetType() const override { return _type_; }						\

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class RenderContext;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Resource
	{
		friend class Renderer;
		friend class RenderContext;

	public:
		Resource();		
		~Resource();
		DECLARE_RESOURCE_VIRTUAL()

		virtual void Release() = 0;

	protected:
		inline uint32	GetId				() const { return m_uiId; }
		inline void		SetId				(uint32 _uiId) { m_uiId = _uiId; }

		inline void		AddReference		() { ++m_uiReferenceCount; }
		inline void		RemoveReference		() { --m_uiReferenceCount; }
		inline bool		HasReference		() const { return m_uiReferenceCount != 0; }

	private:
		uint32 m_uiId;
		uint32 m_uiReferenceCount;
	};
}}

#endif