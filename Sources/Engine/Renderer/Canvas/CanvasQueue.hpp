#ifndef _RENDER_CANVAS_QUEUE_HPP_
#define _RENDER_CANVAS_QUEUE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Thread/Mutex.hpp>
#include <Core/Thread/ConditionVariable.hpp>
#include <Renderer/Common.hpp>
#include <Renderer/Canvas/CanvasCommand.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class CanvasRenderer;
	class CanvasSurface;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class CanvasQueue
	{
	public:
		CanvasQueue		();
		~CanvasQueue	();

		void			Initialize();
		void			Release();

		inline bool		HasCommand() const { return m_vCommand[m_aBufferIndex[EContext::Prepare]].GetSize() > 0; }
		void			SwitchBuffer();
		void			Execute(CanvasRenderer* _pRenderer);

		template <ECanvasCommand::Type T>
		CanvasCommandParameter<T>*		PushCommand(CanvasSurface* _pSurface, uint8 _uiDepth);

	private:
		struct EContext
		{
			enum Type
			{
				Prepare = 0,
				Execute,

				Count
			};
		};

	private:
		Vector<CanvasCommand>	m_vCommand[EContext::Count];
		Vector<uint8>			m_kParameter[EContext::Count];
		uint32					m_aBufferIndex[EContext::Count];

		Mutex					m_aMutex[EContext::Count];
		ConditionVariable		m_aCondition[EContext::Count];
	};

	//!	@brief		PushCommand
	//!	@date		2016-04-23
	template <ECanvasCommand::Type T>
	CanvasCommandParameter<T>* CanvasQueue::PushCommand(CanvasSurface* _pSurface, uint8 _uiDepth)
	{
		uint32 uiIndex = m_aBufferIndex[EContext::Prepare];
		Vector<CanvasCommand>& vCommand = m_vCommand[uiIndex];
		Vector<uint8>& kParameter = m_kParameter[uiIndex];

		// Allocatate parameter
		uint32 uiOffset = kParameter.GetSize();
		kParameter.Expand(sizeof(CanvasCommandParameter<T>));

		// Allocate Command if needed		
		CanvasCommand& kCommand = vCommand.Add();
		kCommand.Initialize(T, _pSurface, _uiDepth, uiOffset);

		return reinterpret_cast<CanvasCommandParameter<T>*>(&kParameter[uiOffset]);
	}
}}

#endif