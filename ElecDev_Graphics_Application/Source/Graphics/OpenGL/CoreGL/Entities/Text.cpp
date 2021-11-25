//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "CoreGL/VertexArrayObjectGL.h"
#include "CoreGL/Entities/Vertex.h"
#include "EntityManager.h"
# include<iostream>
#include "CoreGL/Entities/Text.h"
#include "CoreGL/FontLoader.h"
#include "External/Misc/ConsoleColor.h"

//=============================================================================================================================================//
//  Constructor.																															   //
//=============================================================================================================================================//

// Writes the text to the buffer based on the font loaded in the constructor.
template<typename VertexType>
Text<VertexType>::Text(std::string text, glm::vec3& position, glm::vec4& color, float scale, 
					   VertexArrayObject<VertexType>* vao, Font& font, ManagedEntity* parent, std::string align)
: Entity<VertexType>(parent)
{
	// In the shader the function 'texture()' is used.  This assumes that the (0,0) point is in the top left
	// (standard for OpenGL).  However, BaseEngineGL is written where the (0,0) point is in the bottom left.
	// This has to be compensated for in the funciton.
	// Iterate through characters.

	// Initialize variables.
	m_trackedCenter = position;
	m_VAO = vao;
	/*m_entityID = EntityManager::generateEID();*/
	m_entityID = 10;

	// ------------------- //
	//  A L I G N M E N T  //
	// ------------------- //
	// 
	// Calculate the length of the string.
	float length = 0;
	for (char c : text) { length += font.characterDictionary[c].xAdvance; }
	// Center.
	if (align == "C" || align == "c") {position.x = position.x - (length * scale) / 2; }
	// Right.
	else if (align == "R" || align == "r") { position.x = position.x - length; }
	// Left alignment is the default.
	else if (align == "L" || align == "l") { }
	// Display error.
	else { std::cout << red << "\n[OPENGL] [ERROR]: " << white << "'" << align << "' is not a valid alignment.\n"; return;	}

	// ----------------------------- //
	//  Q U A D   R E N D E R I N G  //
	// ----------------------------- //

	// Vertices layout.
	//
	//   2 ------ 3
	//	 |   /\   |
	//   |	/__\  |
	//   | /    \ |
	//   1 ------ 4
	//
	// The texture ID '1' is reserved for font atlasses.
	// Our engine does not need the ability to be able to render text
	// with more than one font.

	float totalAdvance = 0;
	// Generate a quad for each character.
	for (int i = 0; i < (int)text.length(); i++)
	{
		// Load character.
		Character c = font.characterDictionary.at(text[i]);

		// Retrieve kerning value from dictionary.
		float kerning = 0;
		// Never apply kerning to first character.
		if (i != 0) 
		{
			unsigned currCharacter = font.characterDictionary.at(text[i]).id;
			unsigned prevCharacter = font.characterDictionary.at(text[i - 1]).id;
			std::pair kerningPair(prevCharacter, currCharacter);
			// Check if kerning exists for current pair.
			if (font.kerningDictionary.count(kerningPair))
			{
				kerning = font.kerningDictionary.at(kerningPair);
			}
		}

		// Vertex 1.
		glm::vec3 pos1
		(
			position.x + ( totalAdvance + c.xPlaneBounds[0] + kerning	)	*	scale,
			position.y + ( c.yPlaneBounds[0]							)	*	scale,
			position.z
		);
		glm::vec2 tex1(c.xTextureCoords[0], c.yTextureCoords[0]);
		VertexDataTextured v1(
			pos1,
			color,
			tex1,
			1,
			m_entityID
		);

		// Vertex 2.
		glm::vec3 pos2
		( 
			position.x + ( totalAdvance + c.xPlaneBounds[0] + kerning	)	*	scale,
			position.y + ( c.yPlaneBounds[0] + c.height					)	*	scale,
			position.z
		);
		glm::vec2 tex2(c.xTextureCoords[0], c.yTextureCoords[1]);
		VertexDataTextured v2(
			pos2,
			color,
			tex2,
			1,
			m_entityID
		);

		// Vertex 3.
		glm::vec3 pos3
		( 
			position.x + ( totalAdvance + c.xPlaneBounds[0] + c.width + kerning	)	*	scale,
			position.y + ( c.yPlaneBounds[0] + c.height							)	*	scale,
			position.z
		);
		glm::vec2 tex3(c.xTextureCoords[1], c.yTextureCoords[1]);
		VertexDataTextured v3(
			pos3,
			color,
			tex3,
			1,
			m_entityID
		);
		
		// Vertex 4.
		glm::vec3 pos4
		( 
			position.x + ( totalAdvance + c.xPlaneBounds[0] + c.width + kerning	)	*	scale,
			position.y + ( c.yPlaneBounds[0]									)	*	scale,
			position.z
		);
		glm::vec2 tex4(c.xTextureCoords[1], c.yTextureCoords[0]);
		VertexDataTextured v4(
			pos4,
			color,
			tex4,
			1,
			m_entityID
		);

		// Insert vertices.
		m_vertices.insert(m_vertices.end(), { v1,v2,v3,v4 });
		// Insert indices.
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
		// Move cursor for next character.
		totalAdvance += c.xAdvance + kerning;
	}
	// Write data to VAO.
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
