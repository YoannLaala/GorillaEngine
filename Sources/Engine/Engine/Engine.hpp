#ifndef _ENGINE_HPP_
#define _ENGINE_HPP_

/******************************************************************************
**	Include
******************************************************************************/
#include <Core/Core.hpp>
#include <Core/Singleton.hpp>
#include <Core/Class/ClassManager.hpp>
#include <Core/Container/Dictionary.hpp>
#include <Engine/Asset/System/Module.hpp>

/******************************************************************************
**	Defines
******************************************************************************/
#if !defined(DEBUG)
	#define LOG_ENGINE_ENABLED
#endif

#ifdef LOG_ENGINE_ENABLED
	#define PRINT(_format_, ...)			Gorilla::Engine::Engine::GetInstance()->GetLogManager()->Print(_format_, __VA_ARGS__);
	#define LOG_PRINT(_format_, ...)		Gorilla::Engine::Engine::GetInstance()->GetLogManager()->Print(Gorilla::ELog::Normal, _format_, __VA_ARGS__);
	#define LOG_WARNING(_format_, ...)		Gorilla::Engine::Engine::GetInstance()->GetLogManager()->Print(Gorilla::ELog::Warning, _format_, __VA_ARGS__);
	#define LOG_ERROR(_format_, ...)		Gorilla::Engine::Engine::GetInstance()->GetLogManager()->Print(Gorilla::ELog::Error, _format_, __VA_ARGS__);
#else
	#define PRINT(_format_, ...) UNUSED(__VA_ARGS__)
	#define LOG_PRINT(_format_, ...) UNUSED(__VA_ARGS__)
	#define LOG_WARNING(_format_, ...) UNUSED(__VA_ARGS__)
	#define LOG_ERROR(_format_, ...) UNUSED(__VA_ARGS__)
#endif

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla 
{
	class Time;
	class Input;
	class FileManager;
}

namespace Gorilla { namespace Engine
{
	class AssetManager;
	class World;
}}

namespace Gorilla { namespace Renderer
{
	class Renderer;
	class RenderContext;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class Engine : public Singleton<Engine>
	{
	public:
		Engine();
		~Engine();

		bool						Initialize			();
		void						Release				();

		void						Update				();
		void						OnResize			();
		
		World*						CreateWorld			();
		void						DestroyWorld		(World* _pWorld);

		void						AddWorld			(const AssetHandle<World>& hWorld);
		void						RemoveWorld			(const AssetHandle<World>& hWorld);

		uint32						GetLayer			(const char* _szName) const;
		uint32						AddLayer			(const char* _szName);

		void						LoadModule			();
		void						LoadDescriptor		();
		inline Node					GetComponentDescriptor(const char* _szName) const { return m_dComponentDescriptor.Get(_szName); }
	#if defined(GORILLA_EDITOR)
		inline Dictionary&			GetAllComponentDescriptor() { return m_dComponentDescriptor; }
	#endif

		// Access
		inline LogManager*			GetLogManager		() const { return m_pLogManager; }
		inline Renderer::Renderer*	GetRenderer			() const { return m_pRenderer; }
		inline FileManager*			GetFileManager		() const { return m_pFileManager; }
		inline const Time*			GetTime				() const { return m_pTime; }
		inline const Input*			GetInput			() const { return m_pInput; }
		inline AssetManager*		GetAssetManager		() const { return m_pAssetManager; }

	private:
		SIGNAL_RECEIVER(Engine, RenderContextCreated, Renderer::RenderContext*, uint8);
		void OnRenderContextCreated(Renderer::RenderContext* _pContext, uint8 _eFilter);
	
	private:
		LogManager*						m_pLogManager;
		FileManager*					m_pFileManager;
		Renderer::Renderer*				m_pRenderer;
		Time*							m_pTime;
		Input*							m_pInput;
		AssetManager*					m_pAssetManager;

		Dictionary						m_dComponentDescriptor;
		Vector<AssetHandle<Module>>		m_vModule;
		Vector<World*>					m_vWorld;
		HashMap<const char*, uint32>	m_mLayer;
	};
}}

/**************************************************************************
**	Direct Access
**************************************************************************/
namespace Gorilla
{	
	inline Engine::Engine*			GetEngine				() { return Engine::Engine::GetInstance(); }
	inline LogManager*				GetLogManager			() { return GetEngine()->GetLogManager(); }
	inline FileManager*				GetFileManager			() { return GetEngine()->GetFileManager(); }
	inline const Time*				GetTime					() { return GetEngine()->GetTime(); }
	inline const Input*				GetInput				() { return GetEngine()->GetInput(); }
	inline Renderer::Renderer*		GetRenderer				() { return GetEngine()->GetRenderer(); }
	inline Engine::AssetManager*	GetAssetManager			() { return GetEngine()->GetAssetManager(); }
}

#endif