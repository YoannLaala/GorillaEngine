#ifndef _RENDER_CANVAS_COMMAND_HPP_
#define _RENDER_CANVAS_COMMAND_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Renderer/Common.hpp>
#include <Renderer/Canvas/CanvasCommandEnum.hpp>

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
	class CanvasCommand
	{
	public:
		CanvasCommand();
		~CanvasCommand();
		
		void Initialize(ECanvasCommand::Type _eType, CanvasSurface* _pSurface, uint8 _uiDepth, uint32 _uiOffset);
		void Execute(CanvasRenderer* _pRenderer, uint8* _pParameter);

		inline CanvasSurface*		GetSurface() { return m_pSurface; }
		inline ECanvasCommand::Type	GetType() { return m_eType; }
		inline uint32				GetOffset() { return m_uiOffset; }

		inline bool operator < (const CanvasCommand& _kCommand) const { return (m_uiId < _kCommand.m_uiId); }
		inline bool operator > (const CanvasCommand& _kCommand) const { return (m_uiId > _kCommand.m_uiId); }
		inline bool operator == (const CanvasCommand& _kCommand) const { return (m_uiId == _kCommand.m_uiId); }
		inline bool operator != (const CanvasCommand& _kCommand) const { return (m_uiId != _kCommand.m_uiId); }

	private:
		struct CanvasCommandKey
		{		
			uint32 SurfaceId:16;
			uint32 Depth:8;
			uint32 Priority:8;
		};
		
		CanvasSurface*			m_pSurface;
		ECanvasCommand::Type	m_eType;
		uint32					m_uiOffset;
		union
		{
			uint32 m_uiId;
			CanvasCommandKey m_kKey;
		};
	};

}}

#endif