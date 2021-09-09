#pragma once

/*
Class that handles the text rendring in the OpenGL engines.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

// General.
#include <iostream>

// Freetype.
#include <ft2build.h>
#include FT_FREETYPE_H  

//----------------------------------------------------------------------------------------------------------------------
//  Class.
//----------------------------------------------------------------------------------------------------------------------

class TextRenderer 
{

private:

	// The FreeType Library.
	FT_Library m_library;
	// Font used for this renderer.
	FT_Face m_font;

public:

	// Constructor.
	TextRenderer(const char* fontPath, unsigned int fontSize);
	// Destructor.
	~TextRenderer();

	// Load the character.
	void loadChar(unsigned long charCode);
	// Free up the memory.
	void clearFT();

};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------