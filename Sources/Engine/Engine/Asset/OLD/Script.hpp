//#ifndef _SCRIPT_HPP_
//#define _SCRIPT_HPP_
//
///******************************************************************************
//**	Includes
//******************************************************************************/
//#include "Asset.hpp"
//
///******************************************************************************
//**	Forward Declarations
//******************************************************************************/
//
///******************************************************************************
//**	Class Declaration
//******************************************************************************/
//namespace PantherRenderer
//{
//	class Script : public Asset
//	{
//	public:
//		CLASS_SERIALIZABLE(PantherRenderer::Script)
//		Script();
//		~Script();
//
//		void PreBuild	(const Common::FileSystem* _pFileSystem, Common::ArgumentList& _kArgumentList);
//		void PostBuild	();
//		void Release	();
//
//		uint32	AddInstance	();
//		void*	GetInstance	(uint32 _uiIndex) const { return m_vecInstance[_uiIndex]; };
//
//	private:
//		HMODULE				m_hModule;
//		fnCreateInstance	m_pCreateInstance;
//		Vector<void*>		m_vecInstance;
//	};
//}
//
//#endif