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
/* Resource IDs.                                                                                                                            */
/*=======================================================================================================================================*/

#define TEXTFILE 255
#define PNG 256
#define BASIC_SHADER 257
#define BACKGROUND_SHADER 258
#define TEXTURE_SHADER 259
#define STATIC_TEXTURE_SHADER 260
#define CIRCLE_SHADER 261
#define OUTLINE_SHADER_POSTPROC 265
#define COMPONENT_PNG 268
#define DRAW_CIRCUIT_BUCKETS_PNG 269
#define DRAW_MCC_PNG 270
#define CIRCUIT_TREE_PNG 271
#define ARIAL_NORMAL_PNG 272
#define ARIAL_NORMAL_JSON 273
#define ARIAL_NORMAL_TTF 274
#define ROBOTO_REGULAR_TTF 275
#define ARIAL_BOLD_MSDF_PNG 276
#define ARIAL_BOLD_MSDF_JSON 277
#define IDI_ICON1 278
#define ICON_PNG 279		
#define CIRCUIT_CIRCLE_JPEG 280
#define FILE_ICON 281
#define FOLDER_ICON 282
#define CIRCUIT_FILE_ICON 283
#define LEFT_ARROW_ICON 284
#define UP_ARROW_ICON 285
#define COMPONENT_FILE_ICON 286
#define RELOAD_ICON 287
#define LOAD_FILE_ICON 289
#define SETTINGS_ICON 290
#define USER_ICON 291
#define CABLE_ICON 292
#define DRAW_POLY_CLEAR_ICON 293
#define DRAW_POLY_FILLED_ICON 294
#define DRAW_CIRCLE_CLEAR_ICON 295
#define DRAW_CIRCLE_FILLED_ICON 296
#define DRAW_TEXT_ICON 297
#define DRAW_LINE_ICON 298
#define DELETE_ICON 299
#define COLOUR_PALETTE_ICON 300
#define PORT_ICON 301
#define DROPDOWN_ICON 302
#define PENCIL_ICON 303
#define MSAA_SHADER 303

/*=======================================================================================================================================*/
/* Functions.                                                                                                                            */
/*=======================================================================================================================================*/

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