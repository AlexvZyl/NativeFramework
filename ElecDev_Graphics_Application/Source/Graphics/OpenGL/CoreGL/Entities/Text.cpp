//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "../VertexArrayObjectGL.h"
#include "Text.h"
#include "../FontLoader.h"
#include "CoreGL/Entities/Vertex.h"

//=============================================================================================================================================//
//  Constructor.																															   //
//=============================================================================================================================================//

// Writes the text to the buffer based on the font loaded in the constructor.
Text::Text(std::string text, glm::vec3* position, glm::vec4* color, float scale,
		   unsigned int eID, VertexArrayObject* vao, Font* font)
{
	// In the shader the function 'texture()' is used.  This assumes that the (0,0) point is in the top left
	// (standard for OpenGL).  However, BaseEngineGL is written where the (0,0) point is in the bottom left.
	// This has to be compensated for in the funciton.

	// Iterate through characters.
	float advance = 0;
	for (int i = 0; i < (int)text.length(); i++)
	{
		// Load character.
		Character c = font->characterDictionary[text[i]];

		// Create vertex data.
		glm::vec3 pos1(															// Top left.
			position->x + (advance + c.xOffset) * scale,						// x
			position->y - (c.yOffset) * scale,									// y
			position->z);														// z
		glm::vec2 texCoords1(c.x, 1.0f - c.y);									// Texture coordinates.
		VertexDataTextured v1(
			&pos1,																// Position.
			color,																// Color.
			&texCoords1,														// Texture coordinates.
			1.0f,																// Slot 1 is reserved for the text font atlas.
			eID);																// The entity ID.  Fow now empty.
																			

		glm::vec3 pos2(															// Top Right.
			position->x + (advance + c.xOffset + c.width) * scale,				// x
			position->y - (c.yOffset) * scale,									// y
			position->z);														// z
		glm::vec2 texCoords2(c.x + c.width, 1.0f - c.y);						// Texture coordinates.
		VertexDataTextured v2(																
			&pos2,																// Position.
			color,																// Color.
			&texCoords2,														// Texture coordinates.
			1.0f,																// Slot 1 is reserved for the text font atlas.
			eID);																// The entity ID.  Fow now empty.
		
		glm::vec3 pos3(															// Bottom Right.
			position->x + (advance + c.xOffset + c.width) * scale,				// x
			position->y + (-1 * c.yOffset - c.height) * scale,					// y	
			position->z);														// z
		glm::vec2 texCoords3(c.x + c.width, 1.0f - c.y - c.height);				// Texture coordinates.
		VertexDataTextured v3(																		
			&pos3,																// Position
			color,																// Color.
			&texCoords3,														// Texture coordinates.
			1.0f,																// Slot 1 is reserved for the text font atlas.
			eID);																// The entity ID.  Fow now empty.

		glm::vec3 pos4(															// Bottom Left.
			position->x + (advance + c.xOffset) * scale,						// x
			position->y + (-1 * c.yOffset - c.height) * scale,					// y
			position->z);														// z
		glm::vec2 texCoords4(c.x, 1.0f - c.y - c.height);						// Texture coordinates.
		VertexDataTextured v4(																	
			&pos4,																// z
			color,																// Color.
			&texCoords4,														// Texture coordinates.
			1.0f,																// Slot 1 is reserved for the text font atlas.
			eID);																// The entity ID.  Fow now empty.
		
		// Move the cursor right so that it can draw the next character.
		std::vector<VertexDataTextured> vertices = {v1,v2,v3,v3,v4,v1};
		m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
		advance += c.xAdvance;
	}
	// Write all of the vertices to the CPU side buffer.
	vao->appendDataCPU(this);
	vao->updateGPU();
}

Text::~Text() 
{
}

//=============================================================================================================================================//
//  EOF.																																   //
//=============================================================================================================================================//
