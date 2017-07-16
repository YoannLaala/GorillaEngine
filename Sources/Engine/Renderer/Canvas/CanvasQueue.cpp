/******************************************************************************
**	Includes
******************************************************************************/
#include "CanvasQueue.hpp"
#include <Core/Time/Profiler.hpp>
#include <Renderer/Canvas/CanvasRenderer.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	//!	@brief		Constructor 
	//!	@date		2015-11-11
	CanvasQueue::CanvasQueue()
	{
		m_aBufferIndex[EContext::Prepare] = EContext::Prepare;
		m_aBufferIndex[EContext::Execute] = EContext::Execute;
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-11
	CanvasQueue::~CanvasQueue()
	{
		// Nothing to do
	}

	//!	@brief		SwitchBuffer 
	//!	@date		2016-03-26
	void CanvasQueue::SwitchBuffer()
	{
		uint32 uiIndexPrepare = m_aBufferIndex[EContext::Prepare];
		uint32 uiIndexExecute = m_aBufferIndex[EContext::Execute];

		// Wait frame to finish
		if(m_vCommand[uiIndexExecute].GetSize())
		{
			m_aCondition[EContext::Prepare].Wait(m_aMutex[EContext::Prepare]);
		}
		m_aCondition[EContext::Execute].Signal();

		// Swicth Buffer
		m_aBufferIndex[EContext::Prepare] = uiIndexExecute;
		m_aBufferIndex[EContext::Execute] = uiIndexPrepare;
	}

	//!	@brief		Execute 
	//!	@date		2016-03-26
	void CanvasQueue::Execute(CanvasRenderer* _pRenderer)
	{
		Vector<CanvasCommand>* pVecCommand = NULL;
		Vector<uint8>* pParameter = NULL;
		do 
		{
			uint32 uiIndex = m_aBufferIndex[EContext::Execute];
			pVecCommand = &m_vCommand[uiIndex];
			pParameter = &m_kParameter[uiIndex];
			if(pVecCommand->GetSize() == 0)
			{
				pVecCommand = NULL;
				pParameter = NULL;
				m_aCondition[EContext::Execute].Wait(m_aMutex[EContext::Execute]);
			}
		} while(!pVecCommand && !pParameter);

		//PROFILE_FUNCTION();
		
		// Retrieve proper command list and parameters
		
		// Sort command before
		//vCommand.Sort();

		// Execute all command
		CanvasSurface* pLastSurface = NULL;
		const uint32 uiCommandCount = pVecCommand->GetSize();
		for(uint32 uiCommand = 0; uiCommand < uiCommandCount; ++uiCommand)
		{
			CanvasCommand* pCommand = &(*pVecCommand)[uiCommand];

			// Retrieve information
			ECanvasCommand::Type eType = pCommand->GetType();
			CanvasSurface* pSurface = pCommand->GetSurface();
			uint32 uiOffset = pCommand->GetOffset();

			// Handle switch surface (begin/end draw)
			if(pSurface != pLastSurface)
			{
				if(pLastSurface)
				{
					_pRenderer->DrawEnd(pLastSurface);
				}

				_pRenderer->DrawBegin(pSurface);
				pLastSurface = pSurface;
			}

			// Execute command
			ECanvasCommand::Execute[eType](_pRenderer, pSurface, &(*pParameter)[uiOffset]);
		}

		// Finalize last surface
		_pRenderer->DrawEnd(pLastSurface);

		// Clean vector and signal the frame is finished
		pVecCommand->Clear();
		pParameter->Clear();

		m_aCondition[EContext::Prepare].Signal();
	}
}}