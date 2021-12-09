//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "WindowsUtilities.h"
#include <Windows.h>
#include <filesystem>

//=============================================================================================================================================//
//  Executable.																																   //
//=============================================================================================================================================//

std::string getExecutableLocation()
{
    char pathWindows[100];
    GetModuleFileNameA(getCurrentModule(), pathWindows, 100);
    std::string path = pathWindows;
    path.erase(path.length()-4, 4);                     // Remove .exe
    while (path.back() != '\\') { path.pop_back(); }    // Remove exe name.
    return path;
}

HMODULE getCurrentModule()
{
    HMODULE hModule = NULL;
    GetModuleHandleEx(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
        (LPCTSTR)getCurrentModule,
        &hModule);
    return hModule;
}

//=============================================================================================================================================//
//  Files & Folders.																														   //
//=============================================================================================================================================//



//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//