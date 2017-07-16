/******************************************************************************
**	Includes
******************************************************************************/
#include "Component.hpp"
#include <Engine/Object/GameObject.hpp>
#include <Engine/World.hpp>
#include <Windows.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Engine
{
	//!	@brief		Constructor
	//!	@date		2015-11-01
	Component::Component()
		: m_pGameObject(NULL)
		, m_eFlag(0)
		, m_eState(EState::Start)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-11-01
	Component::~Component()
	{
		// Nothing to do
	}

	uint32 ExceptionFilter(uint32 _uiCode, EXCEPTION_POINTERS* /*_pException*/)   
	{  
		/*BOOL                result;
		HANDLE              process = GetCurrentProcess();
		HANDLE              thread = GetCurrentThread();
		CONTEXT             context;
		STACKFRAME64        stack;
		ULONG               frame;
		IMAGEHLP_SYMBOL64   symbol;
		DWORD64             displacement;
		char name[ 256 ];

		SymInitialize(process, NULL, TRUE);

		RtlCaptureContext( &context );
		memset( &stack, 0, sizeof( STACKFRAME64 ) );

		process                = GetCurrentProcess();
		thread                 = GetCurrentThread();
		displacement           = 0;
		stack.AddrPC.Offset    = context.Eip;
		stack.AddrPC.Mode      = AddrModeFlat;
		stack.AddrStack.Offset = context.Esp;
		stack.AddrStack.Mode   = AddrModeFlat;
		stack.AddrFrame.Offset = context.Ebp;
		stack.AddrFrame.Mode   = AddrModeFlat;

		for( frame = 0; ; frame++ )
		{
			result = StackWalk64
			(
				IMAGE_FILE_MACHINE_I386,
				process,
				thread,
				&stack,
				&context,
				NULL,
				SymFunctionTableAccess64,
				SymGetModuleBase64,
				NULL
			);

			symbol.SizeOfStruct  = sizeof( IMAGEHLP_SYMBOL64 );
			symbol.MaxNameLength = 255;

			SymGetSymFromAddr64( process, ( ULONG64 )stack.AddrPC.Offset, &displacement, &symbol );
			UnDecorateSymbolName( symbol.Name, ( PSTR )name, 256, UNDNAME_COMPLETE );

			printf
			(
				"Frame %lu:\n"
				"    Symbol name:    %s\n"
				"    PC address:     0x%08LX\n"
				"    Stack address:  0x%08LX\n"
				"    Frame address:  0x%08LX\n"
				"\n",
				frame,
				symbol.Name,
				( ULONG64 )stack.AddrPC.Offset,
				( ULONG64 )stack.AddrStack.Offset,
				( ULONG64 )stack.AddrFrame.Offset
			);

			if( !result )
			{
				break;
			}
		}*/

		

		//char name[ 256 ];
		//HANDLE hProcess = GetCurrentProcess();
		////for(;;) 
		////{
		//SymInitialize(hProcess, NULL, TRUE);

		//CONTEXT context;
		//RtlCaptureContext( &context );

		//STACKFRAME64 sFrame;
		//ZeroMemory(&sFrame, sizeof(sFrame));
		//sFrame.AddrPC.Offset       = context.Rip;
		//sFrame.AddrPC.Mode         = AddrModeFlat;
		//sFrame.AddrStack.Offset    = context.Rsp;
		//sFrame.AddrStack.Mode      = AddrModeFlat;
		//sFrame.AddrFrame.Offset    = context.Rbp;
		//sFrame.AddrFrame.Mode      = AddrModeFlat;

		//BOOL bResult = TRUE;
		//do 
		//{
		//	bResult = StackWalk64(IMAGE_FILE_MACHINE_I386, hProcess, GetCurrentThread(), &sFrame, &context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL);
		//	const int MaxNameLen = 255;
		//	IMAGEHLP_SYMBOL64* pSymbol = (IMAGEHLP_SYMBOL64*)malloc(sizeof(IMAGEHLP_SYMBOL64)+MaxNameLen*sizeof(TCHAR));
		//	memset(pSymbol, 0, sizeof( IMAGEHLP_SYMBOL64 ));
		//	pSymbol->SizeOfStruct  = sizeof( IMAGEHLP_SYMBOL64 );
		//	pSymbol->MaxNameLength = MaxNameLen;

		//	 DWORD64             displacement = 0;
		//	SymGetSymFromAddr64( hProcess, ( ULONG64 )sFrame.AddrPC.Offset, &displacement, pSymbol);
		//	SymGetSymFromAddr64( hProcess, ( ULONG64 )sFrame.AddrFrame.Offset, &displacement, pSymbol);
		//	SymGetSymFromAddr64( hProcess, ( ULONG64 )sFrame.AddrBStore.Offset, &displacement, pSymbol);
		//	SymGetSymFromAddr64( hProcess, ( ULONG64 )sFrame.AddrReturn.Offset, &displacement, pSymbol);
		//	SymGetSymFromAddr64( hProcess, ( ULONG64 )sFrame.AddrStack.Offset, &displacement, pSymbol);
		//	UnDecorateSymbolName( pSymbol->Name, ( PSTR )name, 256, UNDNAME_COMPLETE );
		//} while (bResult);
		//	
		////}

		//	
		//


		//ep;
		//DWORD dvalue = GetLastError();
		//LPSTR messageBuffer = nullptr;
		//FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
  //                               NULL, dvalue, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		//std::exception e;
		//e;
		return (_uiCode == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH);   // OK  
	}  

	//!	@brief		Execute
	//!	@date		2015-12-29
	void Component::Execute()
	{	
		bool bPaused = m_pGameObject->GetWorld()->HasFlag(World::EFlag::Paused);
		if(bPaused && !GetClass()->HasFlag(EFlag::Intrinsic)) return;

		// Allow user script to crash without crashing the engine
	#if defined(MASTER)
		__try 
	#endif
		{
			switch(m_eState)
			{
				case EState::Start:
				{
					SetState(EState::Update);
					Start();	
					break;
				}

				case EState::Update:
				{
					Update();			
					break;
				}

				case EState::Stop:
				{
					SetState(EState::Inactive);
					Stop();	
					break;
				}
			}
		}
	#if defined(MASTER)
		__except(ExceptionFilter(GetExceptionCode(), GetExceptionInformation())) 
		{		
			static const char* aState[EState::Count] = { "Inactive", "Start", "Update", "Stop" };
			LOG_ERROR("[%s::%s] An exception was caught in __except", GetGameObject()->GetName().GetBuffer(), aState[m_eState]);
		}		
	#endif
	}

	//!	@brief		Start
	//!	@date		2015-12-29
	void Component::Start()
	{
		// Nothing to do
	}

	//!	@brief		Update
	//!	@date		2015-12-29
	void Component::Update()
	{
		// Nothing to do
	}

	//!	@brief		Stop
	//!	@date		2015-12-29
	void Component::Stop()
	{
		// Nothing to do
	}

	////!	@brief		Deserialize
	////!	@date		2015-10-18
	//bool Component::Deserialize(FileReader& /*_kStream*/)
	//{
	//	return true;
	//}

	////!	@brief		Serialize
	////!	@date		2015-10-18
	//bool Component::Serialize(FileWriter& /*_kStream*/)
	//{
	//	return true;
	//}
}}