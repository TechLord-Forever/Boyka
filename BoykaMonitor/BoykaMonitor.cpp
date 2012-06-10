/////////////////////////////////////////////////////////////////////////////////////
// BoykaMonitor.cpp
//
// SERVER SIDE Fault Monitor.
// It coordinates the attack with the hijacked client.
/////////////////////////////////////////////////////////////////////////////////////

#include "Boyka.h"
#include <Windows.h>


int
main(int argc, char *argv[])
{


	/////////////////////////////////////////////////////////////////////////////////////
	// Attach to the process
	/////////////////////////////////////////////////////////////////////////////////////
	BOOL bAttach = DebugActiveProcess(pe32.th32ProcessID);
	if(bAttach == 0) {
		printf("[Debug] Couldn't attach to %s. ErrCode: %u\n", pe32.szExeFile, GetLastError());
		ExitProcess(1);
	} else {
		printf("[Debug] Attached to %s!\n", pe32.szExeFile);
	}


	/////////////////////////////////////////////////////////////////////////////////////
	// Here starts the action :)
	/////////////////////////////////////////////////////////////////////////////////////
	BoykaDebugLoop();


	return 0;
}




void
BoykaDebugLoop()
{
	DEBUG_EVENT de;
	DWORD dwContinueStatus = 0;

	while(1)
	{
		WaitForDebugEvent(&de, INFINITE);

		switch (de.dwDebugEventCode)
		{
		case EXCEPTION_DEBUG_EVENT:
			switch(de.u.Exception.ExceptionRecord.ExceptionCode)
			{
			case EXCEPTION_ACCESS_VIOLATION:
				// TODO: Maybe consolidate all this logging callbacks using OOP:
				//		 inherit from Exception Logging object or something like that
				unsigned int lav = LogExceptionAccessViolation();
				CommunicateToConsole(lav);

				dwContinueStatus = DBG_CONTINUE;
				break;

			case EXCEPTION_STACK_OVERFLOW:
				unsigned int lso = LogExceptionStackOverflow();
				CommunicateToConsole(lso);

				dwContinueStatus = DBG_CONTINUE;
				break;

			default:	/* unhandled exception */
				dwContinueStatus = DBG_EXCEPTION_NOT_HANDLED;
				break;
			}
			break;

		// We are only interested in exceptions. The rest aren't processed (for now)
		default:
			dwContinueStatus = DBG_EXCEPTION_NOT_HANDLED;
			break;
		}

		ContinueDebugEvent(de.dwProcessId, de.dwThreadId, dwContinueStatus);
	}
}


unsigned int
LogExceptionAccessViolation()
{
	// TODO: Maybe return type could be something more complex
	//		 than int. Some kind of structure? 

	return 0;
}


unsigned int
LogExceptionStackOverflow()
{

	return 0;
}