//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "CoreGL/VertexArrayObjectGL.h"
#include "CoreGL/Entities/Vertex.h"
#include "EntityManager.h"
# include<iostream>
#include "CoreGL/Entities/Text.h"
#include "CoreGL/FontLoader.h"

//=============================================================================================================================================//
//  Constructor.																															   //
//=============================================================================================================================================//

// Writes the text to the buffer based on the font loaded in the constructor.
template<typename VertexType>
Text<VertexType>::Text(std::string text, glm::vec3& position, glm::vec4& color, float scale,
				       VertexArrayObject<VertexType>* vao, Font& font, std::string align)
{
	// In the shader the function 'texture()' is used.  This assumes that the (0,0) point is in the top left
	// (standard for OpenGL).  However, BaseEngineGL is written where the (0,0) point is in the bottom left.
	// This has to be compensated for in the funciton.
	// Iterate through characters.
	m_trackedCenter = position;
	m_VAO = vao;
	m_entityID = EntityManager::generateEID();
	float advance = 0;
	m_vertexCount = 0;
	m_indexCount = 0;

	//Correct allignment
	// Calculate the length & height of the string.
	float length = 0;
	float height = font.characterDictionary[text[0]].height;
	for (char c : text) { length += font.characterDictionary[c].xAdvance; }
	// Center the text.
	if (align == "C" || align == "c")
	{
		// Place the coords in the center of the text.
		position.x = position.x - (length * scale) / 2;
		// Place on top of coordinate.
		position.y = position.y + height * scale;
	}
	// Right allign the text.
	else if (align == "R" || align == "r")
	{
		// Align text to right.
		position.x = position.x - length * scale;
		// Place on top of coordinate.
		position.y = position.y + height * scale;
	}
	// Left allign the text.
	else if (align == "L" || align == "l")
	{
		// Place on top of the coordinate.
		position.y = position.y + height * scale;
	}
	// Display error.
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << align << "' is not a valid alignment.\n\n";
		return;
	}

	for (int i = 0; i < (int)text.length(); i++)
	{
		// Load character.
		Character c = font.characterDictionary.at(text[i]);

		// Create vertex data.
		glm::vec3 pos1(															// Top left.
			position.x + (advance + c.xOffset) * scale,							// x
			position.y - (c.yOffset) * scale,									// y
			position.z);														// z
		glm::vec2 texCoords1(c.x, 1.0f - c.y);									// Texture coordinates.
		VertexDataTextured v1(
			pos1,																// Position.
			color,																// Color.
			texCoords1,															// Texture coordinates.
			1.0f,																// Slot 1 is reserved for the text font atlas.
			m_entityID);														// The entity ID.  Fow now empty.														

		glm::vec3 pos2(															// Top Right.
			position.x + (advance + c.xOffset + c.width) * scale,				// x
			position.y - (c.yOffset) * scale,									// y
			position.z);														// z
		glm::vec2 texCoords2(c.x + c.width, 1.0f - c.y);						// Texture coordinates.
		VertexDataTextured v2(
			pos2,																// Position.
			color,																// Color.
			texCoords2,															// Texture coordinates.
			1.0f,																// Slot 1 is reserved for the text font atlas.
			m_entityID);														// The entity ID.  Fow now empty.
		
		glm::vec3 pos3(															// Bottom Right.
			position.x + (advance + c.xOffset + c.width) * scale,				// x
			position.y + (-1 * c.yOffset - c.height) * scale,					// y	
			position.z);														// z
		glm::vec2 texCoords3(c.x + c.width, 1.0f - c.y - c.height);				// Texture coordinates.
		VertexDataTextured v3(
			pos3,																// Position
			color,																// Color.
			texCoords3,															// Texture coordinates.
			1.0f,																// Slot 1 is reserved for the text font atlas.
			m_entityID);														// The entity ID.  Fow now empty.

		glm::vec3 pos4(															// Bottom Left.
			position.x + (advance + c.xOffset) * scale,							// x
			position.y + (-1 * c.yOffset - c.height) * scale,					// y
			position.z);														// z
		glm::vec2 texCoords4(c.x, 1.0f - c.y - c.height);						// Texture coordinates.
		VertexDataTextured v4(
			pos4,																// z
			color,																// Color.
			texCoords4,															// Texture coordinates.
			1.0f,																// Slot 1 is reserved for the text font atlas.
			m_entityID);														// The entity ID.  Fow now empty.
		
		// Create vertices.
		m_vertices.insert(m_vertices.end(), { v1,v2,v3,v4 });
		m_indices.insert(m_indices.end(), 
			{
				0 + m_vertexCount,
				1 + m_vertexCount,
				2 + m_vertexCount,
				2 + m_vertexCount,
				3 + m_vertexCount,
				0 + m_vertexCount
			});
		m_vertexCount += 4;
		m_indexCount += 6;
		// Move the cursor right so that it can draw the next character.
		advance += c.xAdvance;
	}
	// Write all of the vertices to the CPU side buffer.
	m_VAO->appendDataCPU(this);
}

template<typename VertexType>
Text<VertexType>::~Text(){}

//=============================================================================================================================================//
//  Instantiations.																															   //
//=============================================================================================================================================//

template class Text<>;
template class Text<VertexDataTextured>;

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
