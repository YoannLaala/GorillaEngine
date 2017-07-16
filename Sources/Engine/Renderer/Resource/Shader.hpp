#ifndef _RENDERER_SHADER_HPP_
#define _RENDERER_SHADER_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/Resource/Resource.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class Shader : public Resource
	{
		friend class Renderer;
		friend class RenderContext;

	public:
		struct Parameter
		{
			char			Name[20];
			uint8			Index;
			EFormat::Type	Format;
			bool			Instancing;
		};

	private:
		Shader();
		~Shader();
		DECLARE_RESOURCE(EResource::Shader);

		void Initialize(void* _pHandle, void* _pInputLayout = NULL);
		virtual void Release () override;

	public:
		inline void* GetInputLayout() { return m_pInputLayout; }
		inline void* GetHandle() { return m_pHandle; }

	private:
		void*	m_pInputLayout;
		void*	m_pHandle;		
	};
}}

#endif