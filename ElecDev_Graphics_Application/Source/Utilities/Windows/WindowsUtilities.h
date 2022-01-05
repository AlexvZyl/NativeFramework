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

//=============================================================================================================================================//
//  Module/Executable.																														   //
//=============================================================================================================================================//

// Returns the location of the executable as a string.
std::string getExecutableLocation();

// Return the current module for resource loading (from the .exe).
HMODULE getCurrentModule();

//=============================================================================================================================================//
//  Files & Folders.																														   //
//=============================================================================================================================================//

// Open a file explorer and select a folder.
std::string selectFolder(std::string root = "");
// Open a file explorer and select a single file.
std::string selectFile(std::string title, std::string root, std::string defaultFile, std::string buttonLabel);
// Open a file explorer and allow the user to select multiple files.
std::vector<std::string> selectFileMultiple(std::string root);

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//