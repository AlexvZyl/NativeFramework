//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

// Class include.
#include "textRenderer.h"

// File reading.
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>

// Resource loading.
#include "../Resources/resource.h"

//----------------------------------------------------------------------------------------------------------------------
//  TextRenderer Methods.
//----------------------------------------------------------------------------------------------------------------------

// Constructor.
// Parses the .fnt file so that the .png can be used to render text,
TextRenderer::TextRenderer()
{
	// Font information.
	float textureDimensions[2] = { 512, 512 };	// Dimensions of the .png file.
	float padding = 3;							// Amount of padding around each character.

	// Load font text file from executable.
	HRSRC resource = FindResource(getCurrentModule(), MAKEINTRESOURCE(ARIAL_SDF_FNT), MAKEINTRESOURCE(TEXTFILE));
	HGLOBAL data = LoadResource(getCurrentModule(), resource);
	DWORD size = SizeofResource(getCurrentModule(), resource);
	char* finalData = (char*)LockResource(data);
	std::string fontData;
	fontData.assign(finalData, size);
	// Convert to stream.
	std::istringstream source(fontData);

	// Read the lines.
	int lineCount=0;
	for (std::string line; getline(source, line); )
	{
		// Read the character data.
		if (lineCount >= 4 && lineCount <= 97-1+4) 
		{
			// Create character struct.
			Character c;

			//-------------------------------------
			// Read the character ID.
			//-------------------------------------
			int cursor = 8;
			std::string id;
			while (!isspace(line[cursor]))
			{
				id.push_back(line[cursor]);
				cursor++;
			}
			c.id = ::atof(id.c_str());

			//-------------------------------------
			// Read the character x.
			//-------------------------------------
			cursor = 18;
			std::string x;
			while (!isspace(line[cursor]))
			{
				x.push_back(line[cursor]);
				cursor++;
			}
			c.x = (::atof(x.c_str()) ) / textureDimensions[0];
			
			//-------------------------------------
			// Read the character y.
			//-------------------------------------
			cursor = 25;
			std::string y;
			while (!isspace(line[cursor]))
			{
				y.push_back(line[cursor]);
				cursor++;
			}			
			c.y = (::atof(y.c_str())) / textureDimensions[1];

			//-------------------------------------
			// Read the character width.
			//-------------------------------------
			cursor = 36;
			std::string width;
			while (!isspace(line[cursor]))
			{
				width.push_back(line[cursor]);
				cursor++;
			}
			c.width = (::atof(width.c_str())) / textureDimensions[0];

			//-------------------------------------
			// Read the character height.
			//-------------------------------------
			cursor = 48;
			std::string height;
			while (!isspace(line[cursor]))
			{
				height.push_back(line[cursor]);
				cursor++;
			}
			c.height = (::atof(height.c_str())) / textureDimensions[1];

			//-------------------------------------
			// Read the character x offset.
			//-------------------------------------
			cursor = 61;
			std::string xOffset;
			while (!isspace(line[cursor]))
			{
				xOffset.push_back(line[cursor]);
				cursor++;
			}
			c.xOffset = ::atof(xOffset.c_str()) / textureDimensions[0];

			//-------------------------------------
			// Read the character y offset.
			//-------------------------------------
			cursor = 74;
			std::string yOffset;
			while (!isspace(line[cursor]))
			{
				yOffset.push_back(line[cursor]);
				cursor++;
			}
			c.yOffset = ::atof(yOffset.c_str()) / textureDimensions[1];

			//-------------------------------------
			// Read the character x advance.
			//-------------------------------------
			cursor = 88;
			std::string xAdvance;
			while (!isspace(line[cursor]))
			{
				xAdvance.push_back(line[cursor]);
				cursor++;
			}
			c.xAdvance = ::atof(xAdvance.c_str()) / textureDimensions[0];

			// Add cahracter data to dictionary.
			m_characterDictionary.insert({c.id, c});

			// Print the data.
			//std::cout << line << "\n";
			//std::cout << c.id << " , " << c.x * 512 << " , " << c.y * 512 << " , " << c.width * 512 << " , " << c.height * 512 << " , " << c.xOffset * 512 << " , " << c.yOffset * 512 << " , " << c.xAdvance * 512 << "\n\n";
		}
		// Increment line counter.
		lineCount++;
	}
}

// Destructor.
TextRenderer::~TextRenderer() 
{

}

// Writes the text to the buffer based on the font loaded in the constructor.
void TextRenderer::writeText(std::string text, float coords[2], VertexArrayObject* vao, float textureID, float color[4], float scale)
{	
	// In the shader the function 'texture()' is used.  This assumes that the (0,0) point is in the top left
	// (standard for OpenGL).  However, BaseEngineGL is currently written where the (0,0) point is in the bottom left.
	// This has to be compensated for in the funciton.

	// Iterate through characters.
	int textLength = text.length();
	float advance = 0;
	for (int i = 0; i < textLength; i++)
	{
		Character c = m_characterDictionary[(int)text[i]];
		// Write character data to the VAO.
		TexturedVertexData v1(										// Top left.
			coords[0] + (advance + c.xOffset) * scale,				// x
			coords[1] - (c.yOffset) * scale,						// y
			0.0f,													// z
			color[0], color[1], color[2], color[3],					// Color.
			c.x, 1.0f-c.y,											// Texture coordinates.
			textureID												// Which texture to draw.
		);
		TexturedVertexData v2(										// Top right.
			coords[0] + (advance + c.xOffset + c.width) * scale,	// x
			coords[1] - (c.yOffset) * scale,						// y
			0.0f,													// z
			color[0], color[1], color[2], color[3],					// Color.
			c.x+c.width, 1.0f-c.y,									// Texture coordinates.
			textureID												// Which texture to draw.
		);
		TexturedVertexData v3(										// Bottom right.
			coords[0] + (advance + c.xOffset + c.width) * scale,	// x
			coords[1] + (-1*c.yOffset - c.height) * scale,			// y	
			0.0f,													// z
			color[0], color[1], color[2], color[3],					// Color.
			c.x+c.width, 1.0f-c.y - c.height,						// Texture coordinates.
			textureID												// Which texture to draw.
		);
		TexturedVertexData v4(										// Bottom left.
			coords[0] + (advance + c.xOffset) * scale,				// x
			coords[1] + (-1*c.yOffset - c.height) * scale,			// y
			0.0f,													// z
			color[0], color[1], color[2], color[3],					// Color.
			c.x, 1.0f - c.y - c.height,								// Texture coordinates.
			textureID												// Which texture to draw.
		);
		std::vector<TexturedVertexData> vertices = { v1,v2,v3,v3,v4,v1 };
		vao->writeData(vertices);
		// Move the cursor right so that it can draw the next character.
		advance += c.xAdvance;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------