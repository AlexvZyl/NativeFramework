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
    // -------------
    // If the directory returned gives an unexpected value, try some of 
    // these parameters with GetDisplayName().
    // SIGDN_NORMALDISPLAY
    // SIGDN_FILESYSPATH
    // SIGDN_DESKTOPABSOLUTEPARSING
    // -------------
    
    // Stores the results of the windows calls.
    HRESULT hResult;

    // Create the dialog.
    IFileDialog* dialog = NULL;
    hResult = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    hResult = CoCreateInstance( __uuidof(FileOpenDialog), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog) );
    wchar_t title[] = L"Lumen Directory Selector";
    hResult = dialog->SetTitle((LPCWSTR)title);
    hResult = dialog->SetOptions(FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST);

    // Set target folder at the .exe location.
    // This default location changes as the user uses the explorer (TBC).
    std::wstring exeLocationW = std::wstring(root.begin(), root.end());
    PIDLIST_ABSOLUTE targetFolderID;
    hResult = SHILCreateFromPath(exeLocationW.c_str(), &targetFolderID, NULL);
    IShellItem* defaultFolder;
    hResult = SHCreateShellItem(0,0, targetFolderID, &defaultFolder);
    hResult = dialog->SetDefaultFolder(defaultFolder);
    hResult = defaultFolder->Release();

    // Display.
    hResult = dialog->Show(GetConsoleWindow());
    // Return if operation cancelled.
    if (hResult == 0x800704c7) { return "OPERATION_CANCELLED"; } // This number might change.
   
    // Get the selected folder.
    IShellItem* resultSI;
    hResult = dialog->GetResult(&resultSI);
    LPWSTR resultWindowsWS;
    hResult = resultSI->GetDisplayName(SIGDN_FILESYSPATH, &resultWindowsWS);
    // If no file was loaded, return.
    if (!resultWindowsWS) { return "FOLDER_EMPTY"; }
    std::wstring resultW = resultWindowsWS;
    hResult = resultSI->Release();

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