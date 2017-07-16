#ifndef _ENGINE_ENGINE_DESCRIPTOR_HPP_
#define _ENGINE_ENGINE_DESCRIPTOR_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>
#include <Core/Container/Dictionary.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class AssetProcess;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Engine
{
	class AssetDescriptor
	{
	public:
		AssetDescriptor();
		~AssetDescriptor();

	public:
		inline const Vector<String>& GetVecInput	() const { return m_vInput; }
		
		inline AssetProcess*	GetProcess			() const { return m_pProcess; }
		inline uint64			GetProcessTime		() const { return m_uiProcessTime; }

		inline bool				IsExternal			() const { return m_bIsExternal; }

		bool Deserialize		(Node _kNode);
		bool Serialize			(Node _kNode);		

	private:
		Vector<String>			m_vInput;
		AssetProcess*			m_pProcess;
		uint64					m_uiProcessTime;
		bool					m_bIsExternal;
	};
}}	// Gorilla::Engine

#endif