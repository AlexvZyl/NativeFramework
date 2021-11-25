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
	unsigned int id = 0;				// Character code in ascii.
	float xTextureCoords[2] = { 0,0 };  // Position of the character in the texture that
	float yTextureCoords[2] = { 0,0 };	// is used by OpenGL.
	float xPlaneBounds[2] = { 0,0 };    // The local offsets of each character.
	float yPlaneBounds[2] = { 0,0 };	//
	float xAdvance = 0;					// Amount that the cursor should move to draw a new character.
	float width = 0;					// Character dimensions,
	float height = 0;					// given in size / pixel.
};

// Struct that contains the information regarding the font used in this renderer.
struct Font
{
	float lineSpace = 0;												// Size of line spacing (vertical).
	int textureSize[2] = { 0,0 };										// x, y.  Size of the .png file.
	float pixelsPerEM = 0;												// Pixels per EM.
	float sizeInEMs = 0;												// The size of the font given in EM's.
	std::string yOrigin;												// Where the 0 of the y axis is located.
	std::map<unsigned int, Character> characterDictionary;				// Dictionary containing the character information.
	std::map<std::pair<unsigned, unsigned>, float> kerningDictionary;	// Dictionary containing the kerning information.
	unsigned int textureID = NULL;										// Texture ID for the font atlas.
};

//=============================================================================================================================================//
//  Function decleration.																													   //
//=============================================================================================================================================//

// Load a font generated from MSDFGen & load the atlas to OpenGL.
Font msdfLoadFont(int fontID, int atlasID);
// Load without loading the texture to OpenGL.
Font msdfLoadFont(int fontID);

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//