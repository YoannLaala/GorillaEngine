#ifndef _RENDERER_RENDER_PASS_HPP_
#define _RENDERER_RENDER_PASS_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/Common.hpp>

/******************************************************************************
**	Defines
******************************************************************************/
#define DECLARE_RENDER_PASS_VIRTUAL()																\
virtual uint32 GetType() const = 0;																	\
virtual const char* GetName() const = 0;															\

#define DECLARE_RENDER_PASS(_type_)																	\
class Class																							\
{																									\
public:																								\
	static inline uint32 GetType() { static uint32 uiId = Hash::Generate(#_type_); return uiId; }	\
	static inline const char* GetName() { return #_type_; }											\
};																									\
virtual uint32 GetType() const override { return Class::GetType(); }								\
virtual const char* GetName() const override { return Class::GetName(); }							\

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Renderer;
	class RenderContext;
	class Camera;
	class Octree;
	class RenderBuffer;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class RenderPass
	{
		friend class RenderContext;

		struct EFlag
		{
			enum Type : uint8
			{
				Activated = 1 << 0,
			};
		};

	protected:
		RenderPass();
		~RenderPass();
		DECLARE_RENDER_PASS_VIRTUAL()

		virtual void	Initialize		(RenderContext* _pRenderContext) = 0;
		virtual void	Release			(RenderContext* _pRenderContext) = 0;

		virtual void	Prepare			(Renderer* /*_pRenderer*/, RenderContext* /*_pContext*/, const Camera* /*_pCamera*/, const Octree* /*_pTree*/, RenderBuffer* /*_pBuffer*/) { };
		virtual void	Execute			(Renderer* _pRenderer, RenderContext* _pContext, RenderBuffer* _pBuffer) = 0;
		
	public:
		inline bool	IsActivated() const { return HasFlag(EFlag::Activated); }
		inline void Activate() { SetFlag(EFlag::Activated); }
		inline void Deactivate() { RemoveFlag(EFlag::Activated); }

	protected:
		inline bool		HasFlag		(EFlag::Type _eFlag) const { return (m_eFlag & _eFlag) != 0; }
		inline void		SetFlag		(EFlag::Type _eFlag) { m_eFlag |= _eFlag; }
		inline void		RemoveFlag	(EFlag::Type _eFlag) { m_eFlag &= ~_eFlag; }

	private:
		uint8			m_eFlag;
	};
}}

#endif