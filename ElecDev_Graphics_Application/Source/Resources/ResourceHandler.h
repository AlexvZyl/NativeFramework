#pragma once

/*=======================================================================================================================================*/
/* Includes.                                                                                                                             */
/*=======================================================================================================================================*/

// Windows api for the exe.
#include <Windows.h>
#include <string>

/*=======================================================================================================================================*/
/* Setup by Visual Studio.                                                                                                               */
/*=======================================================================================================================================*/

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        116
#define _APS_NEXT_COMMAND_VALUE         40003
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif

/*=======================================================================================================================================*/
/* Resources.                                                                                                                            */
/*=======================================================================================================================================*/

//-------------------------------------------------------------------------------------------------
// Resource Types
#define TEXTFILE 255
#define PNG 254
//-------------------------------------------------------------------------------------------------
// Shaders
#define BASIC_SHADER 253
#define BACKGROUND_SHADER_2D 252
#define BACKGROUND_SHADER_3D 251
#define TEXTURE_SHADER 250
#define STATIC_TEXTURE_SHADER 235
#define CIRCLE_SHADER 237
//-------------------------------------------------------------------------------------------------
// ImGui images.
#define COMPONENT_PNG 249				
#define DRAW_CIRCUIT_BUCKETS_PNG 248
#define DRAW_MCC_PNG 247				
//-------------------------------------------------------------------------------------------------
// OpenGL Textures.
#define CIRCUIT_TREE_PNG 246
//-------------------------------------------------------------------------------------------------
// OpenGL Fonts. 
#define ROBOTO_MEDIUM_MSDF_PNG 245
#define ROBOTO_MEDIUM_MSDF_JSON 244
#define ROBOTO_REGULAR_TTF 243
#define ARIAL_BOLD_MSDF_PNG 237
#define ARIAL_BOLD_MSDF_JSON 236
//-------------------------------------------------------------------------------------------------
// Application icon.
#define IDI_ICON1 240			// Exe icon.
#define ICON_PNG 239			// GLFW icon.
#define PANDA 238
//-------------------------------------------------------------------------------------------------

/*=======================================================================================================================================*/
/* Functions.                                                                                                                            */
/*=======================================================================================================================================*/

// Return the current module for resource loading (from the .exe).
HMODULE getCurrentModule();

// Loads the text file from the executable and returns it as a string.
std::string loadTextFromResource(int textID);

// Loading images.
BITMAP loadImageFromResource(int resourceID);
// Loads a windows bitmap into OpenGL and returns the texture ID.
unsigned int loadBitmapToGL(BITMAP bitmap);

// Returns the location of a resource in memory.
void* getFontResourceMemoryLocation(int fontID);
// Returns the size of the font resource in bytes.
unsigned getFontResourceSize(int fontID);

/*=======================================================================================================================================*/
/* EOF.                                                                                                                                  */
/*=======================================================================================================================================*/