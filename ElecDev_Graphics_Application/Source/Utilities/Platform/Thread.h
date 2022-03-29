#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <thread>

//==============================================================================================================================================//
//  Windows.																																	//
//==============================================================================================================================================//

#if defined(_WIN32)|| defined (_WIN64) || defined(__CYGWIN__)

#include <Windows.h>

void lumenTerminateThread(std::thread& thread) 
{
	std::thread::native_handle_type threadHandle = thread.native_handle();
	DWORD exitCode = 0;
	thread.detach();
	TerminateThread(threadHandle, exitCode);
}

#endif

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//