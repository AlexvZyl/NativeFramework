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
					   VertexArrayObject<VertexType>* vao, Font& font, ManagedEntity* parent, 
					   std::string horizontalAlignment, std::string verticalAlignment)
: Entity<VertexType>(parent)
{
	// Initialize variables.
	m_VAO = vao;
	m_trackedCenter = position;  //  This does not track the center, but rather the cursor position.

	// ------------------- //
	//  A L I G N M E N T  //
	// ------------------- //
	
	// Calculate the string length with kerning.
	float length = 0;
	for (int i = 0; i < (int)text.length(); i++)
	{ 
		// Retrieve kerning value from dictionary.
		float kerning = 0;
		if (i != 0)  // Kerning does not apply to the first character.
		{
			unsigned currCharacter = font.characterDictionary.at(text[i]).id;
			unsigned prevCharacter = font.characterDictionary.at(text[i - 1]).id;
			std::pair kerningPair = std::pair(prevCharacter, currCharacter);
			if (font.kerningDictionary.count(kerningPair))	// Check if kerning exists for current pair.
				kerning = font.kerningDictionary.at(kerningPair);
		}
		length += font.characterDictionary[text[i]].xAdvance + kerning; 
	}

	// Horizontal alignment.
	if		(horizontalAlignment == "C" || horizontalAlignment == "c")	{ position.x = position.x - (length * scale) / 2; }
	else if (horizontalAlignment == "R" || horizontalAlignment == "r")	{ position.x = position.x - (length * scale);	  }
	else if (horizontalAlignment == "L" || horizontalAlignment == "l")	{ /* Left is the default setting. */  }
	// Display error.
	else { std::cout << red << "\n[OPENGL] [ERROR]: " << white << "'" << horizontalAlignment << "' is not a valid horizontal alignment.\n"; return;	}

	// Vertical alignment.
	if		(verticalAlignment == "C" || verticalAlignment == "c") { position.y = position.y - ((font.ascender+font.descender) * scale) / 2;   }
	else if (verticalAlignment == "T" || verticalAlignment == "t") { position.y = position.y - ((font.ascender+font.descender) * scale);   	}
	else if (verticalAlignment == "B" || verticalAlignment == "b") { /* Bottom is the default setting. */ }
	// Display error.
	else { std::cout << red << "\n[OPENGL] [ERROR]: " << white << "'" << verticalAlignment << "' is not a valid vertical alignment.\n"; return; }

	// ----------------- //
	//  T E X T   B O X  //
	// ----------------- //

	// Vertices layout.
	//
	//   2 ------------------ 3
	//	 |   /\	   /\	 /\	  |
	//   |	/__\  /__\	/__\  |
	//   | /    \/    \/    \ |
	//   1 ------------------ 4
	// 
	// The box default is an invisible quad.
	// Will also be rendered just behind the text so
	// that it does not interfere with the text when 
	// made visible.
	glm::vec4 boxColour(0.f, 0.f, 0.f, 0.f);
	float boxZPos = position.z - 0.001;

	// -----------------------
	// Vertex 1.
	glm::vec3 pos1(position.x, position.y+font.descender*scale, boxZPos);
	glm::vec2 tex1(0.f, 0.f);
	VertexDataTextured v1(pos1, boxColour, tex1, 0, m_entityID);
	// -----------------------
	// Vertex2.
	glm::vec3 pos2(position.x, position.y + font.ascender*scale, boxZPos);
	glm::vec2 tex2(0.f, 1.f);
	VertexDataTextured v2(pos2, boxColour, tex2, 0, m_entityID);
	// -----------------------
	// Vertex 3.
	glm::vec3 pos3(position.x + length*scale, position.y + font.ascender*scale, boxZPos);
	glm::vec2 tex3(1.f, 1.f);
	VertexDataTextured v3(pos3, boxColour, tex3, 0, m_entityID);
	// -----------------------
	// Vertex 4.
	glm::vec3 pos4(position.x + length*scale, position.y + font.descender*scale, boxZPos);
	glm::vec2 tex4(0.f, 1.f);
	VertexDataTextured v4(pos4, boxColour, tex4, 0, m_entityID);
	// -----------------------
	
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
	// -----------------------

	// ----------------------------- //
	//  T E X T   R E N D E R I N G  //
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
	// with more than one font.  If this is required at a later stage we
	// can implement a system that binds the required font texture atlas
	// as needed before the draw calls.

	float totalAdvance = 0;
	// Generate a quad for each character.
	for (int i = 0; i < (int)text.length(); i++)
	{
		// -----------------------
		// Load character.
		Character c = font.characterDictionary.at(text[i]);
		// Retrieve kerning value from dictionary.
		float kerning = 0;
		if (i != 0)  // Kerning does not apply to the first character.
		{
			unsigned currCharacter = font.characterDictionary.at(text[i]).id;
			unsigned prevCharacter = font.characterDictionary.at(text[i - 1]).id;
			std::pair kerningPair = std::pair(prevCharacter, currCharacter);
			if (font.kerningDictionary.count(kerningPair))	// Check if kerning exists for current pair.
				kerning = font.kerningDictionary.at(kerningPair);
		}

		// -----------------------
		// Vertex 1.
		glm::vec3 pos1
		(
			position.x + ( totalAdvance + c.xPlaneBounds[0] + kerning	)	*	scale,
			position.y + ( c.yPlaneBounds[0]							)	*	scale,
			position.z
		);
		glm::vec2 tex1(c.xTextureCoords[0], c.yTextureCoords[0]);
		VertexDataTextured v1( pos1, color, tex1, 1, m_entityID );
		// -----------------------
		// Vertex2.
		glm::vec3 pos2
		( 
			position.x + ( totalAdvance + c.xPlaneBounds[0] + kerning	)	*	scale,
			position.y + ( c.yPlaneBounds[0] + c.height					)	*	scale,
			position.z
		);
		glm::vec2 tex2(c.xTextureCoords[0], c.yTextureCoords[1]);
		VertexDataTextured v2( pos2, color, tex2, 1, m_entityID );
		// -----------------------
		// Vertex 3.
		glm::vec3 pos3
		( 
			position.x + ( totalAdvance + c.xPlaneBounds[0] + c.width + kerning	)	*	scale,
			position.y + ( c.yPlaneBounds[0] + c.height							)	*	scale,
			position.z
		);
		glm::vec2 tex3(c.xTextureCoords[1], c.yTextureCoords[1]);
		VertexDataTextured v3( pos3, color, tex3, 1, m_entityID );
		// -----------------------
		// Vertex 4.
		glm::vec3 pos4
		( 
			position.x + ( totalAdvance + c.xPlaneBounds[0] + c.width + kerning	)	*	scale,
			position.y + ( c.yPlaneBounds[0]									)	*	scale,
			position.z
		);
		glm::vec2 tex4(c.xTextureCoords[1], c.yTextureCoords[0]);
		VertexDataTextured v4( pos4, color, tex4, 1, m_entityID );
		// -----------------------
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
		// -----------------------
	}
	// Write data to VAO.
	m_VAO->appendDataCPU(this);
}

template<typename VertexType>
Text<VertexType>::~Text(){}

//=============================================================================================================================================//
//  Text manipulation.																													       //
//=============================================================================================================================================//

template <typename VertexType>
void Text<VertexType>::setBoxColour(glm::vec4 colour) 
{ 
	for (int i = 0; i < 4; i++) { *m_vertices[i].color = colour; }
}

template <typename VertexType>
void Text<VertexType>::setColor(glm::vec4& color) 
{
	for (int i = 4; i < m_vertices.size(); i++) { *m_vertices[i].color = color; }
}

template <typename VertexType>
void Text<VertexType>::setLayer(float layer)
{
	for (int i = 0; i < 4; i++) { m_vertices[i].position->z = layer - 0.001; }
	for (int i = 4; i < m_vertices.size(); i++) { m_vertices[i].position->z = layer; }
}

//=============================================================================================================================================//
//  Instantiations.																															   //
//=============================================================================================================================================//

template class Text<>;
template class Text<VertexDataTextured>;

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
