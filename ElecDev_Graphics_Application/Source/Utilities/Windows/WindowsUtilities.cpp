//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "WindowsUtilities.h"
#include <Windows.h>
#include <filesystem>
#include <shlobj_core.h>


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

std::string selectFolder(std::string root) 
{
    // Create the dialog.
    IFileDialog* dialog = NULL;
    CoCreateInstance( __uuidof(FileOpenDialog), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog) );
    wchar_t title[] = L"Lumen Directory Selector";
    dialog->SetTitle((LPCWSTR)title);
    dialog->SetOptions(FOS_PICKFOLDERS);

    // Set target folder at the .exe location.
    // This default location changes as the user uses the explorer (TBC).
    std::wstring exeLocationW = std::wstring(root.begin(), root.end());
    PIDLIST_ABSOLUTE targetFolderID;
    SHILCreateFromPath(exeLocationW.c_str(), &targetFolderID, NULL);
    IShellItem* defaultFolder;
    SHCreateShellItem(0,0, targetFolderID, &defaultFolder);
    dialog->SetDefaultFolder(defaultFolder);

    // Display.
    dialog->Show(GetConsoleWindow());

    // Get the selected folder.
    IShellItem* resultSI;
    dialog->GetResult(&resultSI);
    LPWSTR resultWindowsWS = nullptr;
    resultSI->GetDisplayName(SIGDN_NORMALDISPLAY, &resultWindowsWS);
    std::wstring resultW = resultWindowsWS;

    // Return the path as an std::string.
    return std::string(resultW.begin(), resultW.end());
}

std::string selectFile(std::string root) 
{
    return "Done";
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//