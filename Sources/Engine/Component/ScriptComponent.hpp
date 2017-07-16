//#ifndef _SCRIPT_COMPONENT_HPP_
//#define _SCRIPT_COMPONENT_HPP_
//
///******************************************************************************
//**	Includes
//******************************************************************************/
//#include "Component.hpp"
//
///******************************************************************************
//**	Forward Declarations
//******************************************************************************/
//namespace PantherRenderer
//{
//	class Script;
//}
//
//namespace PantherEngine
//{
//	class ScriptObject;
//	class Engine;
//}
//
///******************************************************************************
//**	Class Declaration
//******************************************************************************/
//namespace PantherEngine
//{
//	class ScriptInfo
//	{
//	public:
//		ScriptInfo();
//		~ScriptInfo();
//
//		void Init(const PantherRenderer::Script* _pScript, uint32 _uiIndex);
//
//		ScriptObject* GetInstance();
//
//	private:
//		const PantherRenderer::Script*	m_pScript;
//		uint32							m_uiIndex;
//	};
//
//	class ScriptComponent : public Component
//	{
//	public:
//		ScriptComponent();
//
//		void			Update				(Engine* _pEngine);
//		void			Release				();
//
//		void			AddScript			(const char* _szFilePath);
//
//	private:
//		Vector<ScriptInfo>	m_vecScript;
//	};
//}
//
//#endif