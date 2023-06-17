//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "WindowsUtilities.h"
#include <Windows.h>
#include <filesystem>
#include <shlobj_core.h>

//=============================================================================================================================================//
//  Data.																																       //
//=============================================================================================================================================//

// Files supported by Lumen.
const COMDLG_FILTERSPEC fileSpec[] =
{
    { L"Lumen File", L"*.lmct; *.lmcp; *.lmcb;" }
};

//=============================================================================================================================================//
//  Executable.																																   //
//=============================================================================================================================================//

std::string getExecutableLocation(bool removeBackslash)
{
    char pathWindows[100];
    GetModuleFileNameA(getCurrentModule(), pathWindows, 100);
    std::string path = pathWindows;
    path.erase(path.length()-4, 4);                     // Remove '.exe'.
    while (path.back() != '\\') { path.pop_back(); }    // Remove exe name.
    if (removeBackslash) path.pop_back();
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

// -------------
// If the directory returned gives an unexpected value, try some of 
// these parameters with GetDisplayName().
// SIGDN_NORMALDISPLAY
// SIGDN_FILESYSPATH
// SIGDN_DESKTOPABSOLUTEPARSING
// -------------

std::filesystem::path selectFolder(const std::string& root)
{
    // Default location.
    std::string usingRoot = root;
    if (!usingRoot.length()) { usingRoot = getExecutableLocation(); }

    // Stores the results of the windows calls.
    HRESULT hResult;

    // Create the dialog.
    IFileDialog* dialog = NULL;
    hResult = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    hResult = CoCreateInstance(__uuidof(FileOpenDialog), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog));
    wchar_t title[] = L"Lumen Directory Selector";
    hResult = dialog->SetTitle((LPCWSTR)title);
    hResult = dialog->SetOptions(FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST);

    // Set target folder at the .exe location.
    // This default location changes as the user uses the explorer (TBC).
    std::wstring exeLocationW = std::wstring(usingRoot.begin(), usingRoot.end());
    PIDLIST_ABSOLUTE targetFolderID;
    hResult = SHILCreateFromPath(exeLocationW.c_str(), &targetFolderID, NULL);
    IShellItem* defaultFolder;
    hResult = SHCreateShellItem(0,0, targetFolderID, &defaultFolder);
    hResult = dialog->SetDefaultFolder(defaultFolder);
    hResult = defaultFolder->Release();

    // Display.
    hResult = dialog->Show(GetConsoleWindow());
    // Return if operation cancelled.
    if (hResult == 0x800704c7) { return ""; } // This number might change.
   
    // Get the selected folder.
    IShellItem* resultSI;
    hResult = dialog->GetResult(&resultSI);
    LPWSTR resultWindowsWS;
    hResult = resultSI->GetDisplayName(SIGDN_FILESYSPATH, &resultWindowsWS);
    // If no file was loaded, return.
    if (!resultWindowsWS) { return ""; }
    std::wstring resultW = resultWindowsWS;
    hResult = resultSI->Release();

    // Return the path as an std::string.
    return std::filesystem::path(std::string(resultW.begin(), resultW.end()));
}

std::filesystem::path selectFile(const std::string& windowTitle, const std::string& root, const std::string& defaultFileEntry, const std::string& buttonLabel)
{
    // Stores the results of the windows calls.
    HRESULT hResult;

    // Create the dialog.
    IFileDialog* dialog = NULL;
    hResult = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    hResult = CoCreateInstance(__uuidof(FileOpenDialog), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog));
    std::wstring titleW;
    if (!windowTitle.size()) { titleW = L"Lumen File Selector"; }
    else                     { titleW = std::wstring(windowTitle.begin(), windowTitle.end()); }
    
    hResult = dialog->SetTitle((LPCWSTR)titleW.c_str());

 

    hResult = dialog->SetFileTypes(1, fileSpec);
    hResult = dialog->SetOptions(FOS_STRICTFILETYPES);

    // Default root is at the exe location.
    std::string usingRoot = root;
    if (!usingRoot.length()) { usingRoot = getExecutableLocation(); }
    // User specified root.
    else
    {
        std::wstring exeLocationW = std::wstring(usingRoot.begin(), usingRoot.end());
        PIDLIST_ABSOLUTE targetFolderID;
        hResult = SHILCreateFromPath(exeLocationW.c_str(), &targetFolderID, NULL);
        IShellItem* defaultFolder;
        hResult = SHCreateShellItem(0, 0, targetFolderID, &defaultFolder);
        hResult = dialog->SetDefaultFolder(defaultFolder);
        hResult = defaultFolder->Release();
    }

    // Set the default file.
    if (defaultFileEntry.size())
    {
        std::wstring defaultFileW = std::wstring(defaultFileEntry.begin(), defaultFileEntry.end());
        dialog->SetFileName((LPCWSTR)defaultFileW.c_str());
    }

    // Set the button label.
    if (buttonLabel.size())
    {
        std::wstring buttonLabelW = std::wstring(buttonLabel.begin(), buttonLabel.end());
        hResult = dialog->SetOkButtonLabel((LPCWSTR)buttonLabelW.c_str());
    }

    // Display.
    hResult = dialog->Show(GetConsoleWindow());
    // Return if operation cancelled.
    if (hResult == 0x800704c7) { return ""; } // This number might change.

    // Get the selected folder.
    IShellItem* resultSI;
    hResult = dialog->GetResult(&resultSI);
    LPWSTR resultWindowsWS;
    hResult = resultSI->GetDisplayName(SIGDN_FILESYSPATH, &resultWindowsWS);
    // If no file was loaded, return.
    if (!resultWindowsWS) { return ""; }
    std::wstring resultW = resultWindowsWS;
    hResult = resultSI->Release();

    // Return the path as an std::string.
    return std::filesystem::path(std::string(resultW.begin(), resultW.end()));
}

std::vector<std::filesystem::path> selectFileMultiple(std::string root)
{
    std::vector<std::filesystem::path> filePaths;
    return filePaths;
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//