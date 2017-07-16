//#ifndef _EDITOR_VIEW_HPP_
//#define _EDITOR_VIEW_HPP_
//
///******************************************************************************
//**	Include
//******************************************************************************/
//#include <Engine/View.hpp>
//
///******************************************************************************
//**	Forward Declaration
//******************************************************************************/
//namespace Gorilla { namespace Editor
//{
//	class EditorPass;
//}}
//
///******************************************************************************
//**	Class Declaration
//******************************************************************************/
//namespace Gorilla { namespace Editor
//{
//	class EditorView : public Engine::View
//	{
//	private:
//		struct EViewport
//		{
//			enum Type
//			{
//				Window = 0,
//				First,
//				Second,
//				Third,
//				Fourth,
//
//				Count,
//			};
//		};
//
//	public:
//		EditorView();
//		~EditorView();
//
//		virtual void Initialize() override;
//		virtual void Release() override;
//
//	public:
//		virtual const char* GetName() { return "Gorilla Editor"; }
//		virtual void CreateViewport(uint32 _uiLeft, uint32 _uiTop, uint32 _uiWidth, uint32 _uiHeight) override;
//
//	private:
//		void RefreshEditorPass();
//		virtual void PostInitialize() = 0;
//
//	private:
//		EditorPass* m_pEditorPass;
//	};
//}}
//
//#endif