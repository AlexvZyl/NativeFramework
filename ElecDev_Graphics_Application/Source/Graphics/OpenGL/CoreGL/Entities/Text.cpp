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
Text::Text(std::string text, glm::vec3 position, glm::vec4 color, float scale,
		   unsigned int eID, std::shared_ptr<VertexArrayObject> vao, Font* font)
{
	// In the shader the function 'texture()' is used.  This assumes that the (0,0) point is in the top left
	// (standard for OpenGL).  However, BaseEngineGL is written where the (0,0) point is in the bottom left.
	// This has to be compensated for in the funciton.

	// Iterate through characters.
	float advance = 0;
	for (int i = 0; i < (int)text.length(); i++)
	{
		Character c = font->characterDictionary[text[i]];

		std::shared_ptr<VertexDataTextured> v1 = std::make_shared<VertexDataTextured>(	// Top left.
			position.x + (advance + c.xOffset) * scale,									// x
			position.y - (c.yOffset) * scale,											// y
			position.z,																	// z
			color[0], color[1], color[2], color[3],										// Color.
			c.x, 1.0f - c.y,															// Texture coordinates.
			1.0f,																			// Slot 1 is reserved for the text font atlas.
			eID																			// The entity ID.  Fow now empty.
		);
		std::shared_ptr<VertexDataTextured> v2 = std::make_shared<VertexDataTextured>(	// Top Right.
			position.x + (advance + c.xOffset + c.width) * scale,						// x
			position.y - (c.yOffset) * scale,											// y
			position.z,																	// z
			color[0], color[1], color[2], color[3],										// Color.
			c.x + c.width, 1.0f - c.y,													// Texture coordinates.
			1.0f,																			// Slot 1 is reserved for the text font atlas.
			eID																			// The entity ID.  Fow now empty.
		);
		std::shared_ptr<VertexDataTextured> v3 = std::make_shared<VertexDataTextured>(	// Bottom right.
			position.x + (advance + c.xOffset + c.width) * scale,						// x
			position.y + (-1 * c.yOffset - c.height) * scale,							// y	
			position.z,																	// z
			color[0], color[1], color[2], color[3],										// Color.
			c.x + c.width, 1.0f - c.y - c.height,										// Texture coordinates.
			1.0f,																			// Slot 1 is reserved for the text font atlas.
			eID																			// The entity ID.  Fow now empty.
		);
		std::shared_ptr<VertexDataTextured> v4 = std::make_shared<VertexDataTextured>(	// Bottom left.
			position.x + (advance + c.xOffset) * scale,									// x
			position.y + (-1 * c.yOffset - c.height) * scale,							// y
			position.z,																	// z
			color[0], color[1], color[2], color[3],										// Color.
			c.x, 1.0f - c.y - c.height,													// Texture coordinates.
			1.0f,																			// Slot 1 is reserved for the text font atlas.
			eID																			// The entity ID.  Fow now empty.
		);
		// Move the cursor right so that it can draw the next character.
		std::vector<std::shared_ptr<VertexDataTextured>> vertices = {v1,v2,v3,v3,v4,v1};
		m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
		advance += c.xAdvance;
	}
	// Write all of the vertices to the CPU side buffer.
	vao->appendDataCPU(this);
	//vao->appendDataCPU(m_vertices);
}

Text::~Text() 
{
}

//=============================================================================================================================================//
//  EOF.																																   //
//=============================================================================================================================================//
