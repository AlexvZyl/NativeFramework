//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

// Class include.
#include "textRenderer.h"

//----------------------------------------------------------------------------------------------------------------------
//  TextRenderer Methods.
//----------------------------------------------------------------------------------------------------------------------

// Constructor.
TextRenderer::TextRenderer(const char* fontPath, unsigned int fontSize) 
{
	//---------------------------------------------------------------------------------------
	// Setup FreeType (Used to load fonts.).
	//---------------------------------------------------------------------------------------

	// Check if library created correctly.
	if (FT_Init_FreeType(&m_library))
	{
		std::cout << "[FREETYPE][ERROR] Could not init FreeType Library." << std::endl << std::endl;
		// Destroy library.
		clearFT();
	}
	// Check if font loaded correctly.
	if (FT_New_Face(m_library, fontPath, 0, &m_font))
	{
		std::cout << "[FREETYPE][ERROR]: Failed to load font from '" << fontPath << "'." << std::endl << std::endl;
		clearFT();
	}
	// Set font size.
	FT_Set_Pixel_Sizes(m_font, 0, fontSize);

	loadChar(100);
}

// Destructor.
TextRenderer::~TextRenderer() 
{
	clearFT();
}

// Load character.
void TextRenderer::loadChar(unsigned long charCode) 
{
	// Load character.
	if (FT_Load_Char(m_font, charCode, FT_LOAD_RENDER)) 
	{
		std::cout << "[FREETYPE][ERROR]: Failed to load character." << std::endl << std::endl;
		clearFT();
		return;
	}

	// Load character details.
	int glyphWidth = m_font->glyph->bitmap.width;
	int glyphHeight = m_font->glyph->bitmap.rows;
	unsigned char* bitmapBuffer = m_font->glyph->bitmap.buffer;
	int i = 0;
	


}

// Clear memory used.
void TextRenderer::clearFT()
{
	// Destroy font.
	FT_Done_Face(m_font);
	// Destroy library.
	FT_Done_FreeType(m_library);
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------