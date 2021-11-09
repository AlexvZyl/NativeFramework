//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Text.h"
#include "../FontLoader.h"
#include "../VertexArrayObjectGL.h"

//=============================================================================================================================================//
//  Constructor.																																//
//=============================================================================================================================================//

// Writes the text to the buffer based on the font loaded in the constructor.
Text::Text(std::string text, glm::vec3 position, glm::vec4 color, float scale, 
		   unsigned int eID, VertexArrayObject* vao, Font* font) 
{
	// In the shader the function 'texture()' is used.  This assumes that the (0,0) point is in the top left
	// (standard for OpenGL).  However, BaseEngineGL is written where the (0,0) point is in the bottom left.
	// This has to be compensated for in the funciton.

	// Buffer that contains total information.
	std::vector<VertexDataTextured> vertices;

	// Iterate through characters.
	float advance = 0;
	for (int i = 0; i < (int)text.length(); i++)
	{
		Character c = font->characterDictionary[text[i]];
		// Write character data to the VAO.
		VertexDataTextured v1(										// Top left.
			position.x + (advance + c.xOffset) * scale,				// x
			position.y - (c.yOffset) * scale,						// y
			position.z,												// z
			color[0], color[1], color[2], color[3],					// Color.
			c.x, 1.0f - c.y,										// Texture coordinates.
			1,														// Slot 1 is reserved for the text font atlas.
			eID														// The entity ID.  Fow now empty.
		);
		VertexDataTextured v2(										// Top right.
			position.x + (advance + c.xOffset + c.width) * scale,	// x
			position.y - (c.yOffset) * scale,						// y
			position.z,												// z
			color[0], color[1], color[2], color[3],					// Color.
			c.x + c.width, 1.0f - c.y,								// Texture coordinates.
			1,														// Slot 1 is reserved for the text font atlas.
			eID														// The entity ID.  Fow now empty.
		);
		VertexDataTextured v3(										// Bottom right.
			position.x + (advance + c.xOffset + c.width) * scale,	// x
			position.y + (-1 * c.yOffset - c.height) * scale,		// y	
			position.z,												// z
			color[0], color[1], color[2], color[3],					// Color.
			c.x + c.width, 1.0f - c.y - c.height,					// Texture coordinates.
			1,														// Slot 1 is reserved for the text font atlas.
			eID														// The entity ID.  Fow now empty.
		);
		VertexDataTextured v4(										// Bottom left.
			position.x + (advance + c.xOffset) * scale,				// x
			position.y + (-1 * c.yOffset - c.height) * scale,		// y
			position.z,												// z
			color[0], color[1], color[2], color[3],					// Color.
			c.x, 1.0f - c.y - c.height,								// Texture coordinates.
			1,														// Slot 1 is reserved for the text font atlas.
			eID														// The entity ID.  Fow now empty.
		);
		// Create temp buffer.
		std::vector<VertexDataTextured> verticesTemp = { v1,v2,v3,v3,v4,v1 };
		// Add to total vertices.
		vertices.insert(vertices.end(), verticesTemp.begin(), verticesTemp.end());
		// Move the cursor right so that it can draw the next character.
		advance += c.xAdvance;
	}
	// Write all of the vertices to the CPU side buffer.
	vao->appendDataCPU(vertices);
}

//=============================================================================================================================================//
//  EOF.																																   //
//=============================================================================================================================================//
