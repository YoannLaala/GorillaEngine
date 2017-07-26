#ifndef _EDITOR_COMPONENT_EDITOR_CONTROLLER_HPP_
#define _EDITOR_COMPONENT_EDITOR_CONTROLLER_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Engine/Component/Component.hpp>
#include <Core/Container/Dictionary.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Engine 
{
	class Asset;
	class View;
	class World;
}}

namespace Gorilla { namespace Renderer 
{
	class Viewport;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Editor 
{
	class EditorController : public Engine::Component
	{
		enum class Config : uint8
		{
			Editor = 0,
			Project,
			Count,
		};

	public:
		EditorController();
		~EditorController();
		COMPONENT_DECLARE(Gorilla::Editor::EditorController);

	protected:
		//virtual bool Serialize		(FileWriter& /*_kStream*/) override;
		//virtual bool Deserialize	(FileReader& /*_kStream*/) override;

		virtual void Start			() override;
		virtual void Update			() override;
		virtual void Stop			() override;

	public:
		inline Renderer::Viewport*	GetViewport(uint32 _uiIndex) { return _uiIndex < m_vViewport.GetSize() ? m_vViewport[_uiIndex] : nullptr; }
		uint32						CreateViewport(uint32 _uiLeft, uint32 _uiTop, uint32 _uiWidth, uint32 _uiHeight);

	private:
		void RefreshModule();
		bool LoadProject(const char* _szProjectPath = nullptr);
		void LoadWorld(const char* _szWorldPath = nullptr);
		void SaveWorld(const char* _szWorldPath);
		void SelectGameObject(Engine::GameObject* _pGameObject);
		inline Dictionary& GetConfig(Config eConfig) { return m_aConfig[static_cast<uint8>(eConfig)]; }

	private:
		SIGNAL_RECEIVER(EditorController, LogChanged, ELog::Type /*_eLog*/, const char* /*_szMessage*/);
		void OnLogChanged(ELog::Type _eLog, const char* _szMessage);

		SIGNAL_RECEIVER(EditorController, AssetChanged, uint8 /*_eEvent*/, Engine::Asset* /*_pAsset*/, bool /*_bSucceeded*/);
		void OnAssetChanged(uint8 _eEvent, Engine::Asset* _pAsset, bool _bSucceeded);

		SIGNAL_RECEIVER(EditorController, FileChanged, uint8 /*_eType*/, const char* /*_szDirectoryPath*/, const char* /*_szRelativePath*/);
		void OnFileChanged(uint8 _eType, const char* _szDirectoryPath, const char* _szRelativePath);

	public:
		Engine::View*				View;

	private:
		Vector<Renderer::Viewport*> m_vViewport;
		Dictionary					m_aConfig[static_cast<uint8>(Config::Count)];
		String						m_sScript;
		Engine::GameObject*			m_pSelection;
		Engine::World*				m_pWorld;

	};
}}

#endif