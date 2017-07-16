#ifndef _RENDERER_SAMPLER_STATE_HPP_
#define _RENDERER_SAMPLER_STATE_HPP_

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
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class SamplerState : public Resource
	{
		friend class Renderer;
		friend class RenderContext;

	private:
		SamplerState();
		~SamplerState();
		DECLARE_RESOURCE(EResource::SamplerState);

		void Initialize	(void* _pHandle);
		void Release	();
		
		inline void* GetHandle() const { return m_pHandle; }

	private:
		void* m_pHandle;
	};
}}

#endif