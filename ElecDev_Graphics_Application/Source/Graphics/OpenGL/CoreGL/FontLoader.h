#pragma once

/*
Font loading utility.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <map>
#include <string>

//=============================================================================================================================================//
//  Font structures.																														   //
//=============================================================================================================================================//

// Stores the information regarding the characters.
struct Character
{
	unsigned int id = 0;		// Character code in ascii.
	float x = 0;				// Top left coordinate of character.
	float y = 0;				// Top left coordinate of character
	float width = 0;			// Width of character in pixels.
	float height = 0;			// Height of the charcter in pixels
	float xOffset = 0;			// Offset from the cursor psition.
	float yOffset = 0;			// Offset from the cursor psition.
	float xAdvance = 0;			// Amount that the cursor should move to draw a new character.
};

// Struct that contains the information regarding the font used in this renderer.
struct Font
{
	std::string name;											// Name of the font used.
	int padding = 0;											// Padding around each character.
	int textureSize[2] = { 0,0 };								// x, y.  Size of the .png file.
	std::map<unsigned int, Character> characterDictionary;		// Dictionary containing the character information.
	unsigned int textureID;										// Texture ID for the font atlas.
};

//=============================================================================================================================================//
//  Function decleration.																													   //
//=============================================================================================================================================//

// Loads the font from resource and stores the information in 
// the Font struct.
Font loadFont(int fontID, int atlasID);

// Loads a font using the MSDFGen library.
void msdfgenLoadFont();

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//