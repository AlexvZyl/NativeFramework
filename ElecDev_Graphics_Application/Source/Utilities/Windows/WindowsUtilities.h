#pragma once

/*
Helper functions related to windows, file handling etc.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <string>
#include <Windows.h>
#include <vector>
#include <filesystem>

//=============================================================================================================================================//
//  Module/Executable.																														   //
//=============================================================================================================================================//

// Returns the location of the executable as a string.
std::string getExecutableLocation(bool removeBackslash = false);

// Return the current module for resource loading (from the .exe).
HMODULE getCurrentModule();

//=============================================================================================================================================//
//  Files & Folders.																														   //
//=============================================================================================================================================//

// Open a file explorer and select a folder.
std::filesystem::path selectFolder(const std::string& root = "");

// Open a file explorer and select a single file.
std::filesystem::path selectFile(const std::string& windowTitle, const std::string& root, const std::string& defaultFileEntry, const std::string& buttonLabel);

// Open a file explorer and allow the user to select multiple files.
std::vector<std::filesystem::path> selectFileMultiple(const std::string& root);

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//