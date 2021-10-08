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
#define STATIC_SHADER 252
#define TEXTURE_SHADER 251
//-------------------------------------------------------------------------------------------------
// ImGui images.
#define COMPONENT_PNG 250				
#define DRAW_CIRCUIT_BUCKETS_PNG 249
#define DRAW_MCC_PNG 248				
//-------------------------------------------------------------------------------------------------
// OpenGL Textures.
#define CIRCUIT_TREE_PNG 247
//-------------------------------------------------------------------------------------------------
// OpenGL Fonts. 
#define ARIAL_SDF_FNT 246		
#define ARIAL_SDF_PNG 245		
#define ARIAL_SDF_MIN_FNT 243
#define ARIAL_SDF_MIN_PNG 242	
//-------------------------------------------------------------------------------------------------
// Application icon.
#define IDI_ICON1 241			// Exe icon.
#define ICON_PNG 240			// GLFW icon.
#define PANDA 239
//-------------------------------------------------------------------------------------------------

/*=======================================================================================================================================*/
/* Includes.                                                                                                                             */
/*=======================================================================================================================================*/

// Windows api for the exe.
#include <Windows.h>
#include <string>

/*=======================================================================================================================================*/
/* Functions.                                                                                                                            */
/*=======================================================================================================================================*/

// Load the current module.
HMODULE getCurrentModule();

// Load shader from resource.
std::string loadShaderFromResource(int shaderID);

// Loading images.
BITMAP loadImageFromResource(int resourceID);
unsigned int loadBitmapToGL(BITMAP bitmap);

/*=======================================================================================================================================*/
/* EOF.                                                                                                                                  */
/*=======================================================================================================================================*/