//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Primitives/Vertex.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/SceneGL.h"
#include "Utilities/Logger/Logger.h"
#include "Graphics/Entities/EntityManager.h"
#include "Graphics/Fonts/FontLoader.h"
#include <iostream>
#include "OpenGL/Primitives/Text.h"
#include "External/Misc/ConsoleColor.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

//=============================================================================================================================================//
//  Constructor.																															   //
//=============================================================================================================================================//

// Writes the text to the buffer based on the font loaded in the constructor.
Text::Text(const std::string& text, const glm::vec3& position, const glm::vec4& color, float scale, 
		   GraphicsTrianglesBuffer<VertexDataTextured>* gtb, Font* font, Entity* parent,
		   const std::string& horizontalAlignment, const std::string& verticalAlignment)
	: Primitive<GraphicsTrianglesBuffer<VertexDataTextured>>(parent)
{
	// ---------- //
	// S E T U P  //
	// ---------- //

	setGraphicsBuffer(gtb);
	m_trackedCenter = position;  // This does not track the center, but rather the initial cursor position.
								 // This does not affect functionality but the name does not make sense.
	m_textScale = scale;
	m_font = font;
	m_verticalAlign = verticalAlignment;
	m_horizontalAlign = horizontalAlignment;
	m_colour = color;
	m_string = text;

	// --------------------- //
	//  T E X T   Q U A D S  //
	// --------------------- //

	generateText(text);
}

void Text::generateText(const std::string& text)
{
	// Init.
	glm::vec3 cursorStart = m_trackedCenter;

	// If the text is empty send a box so that if still exists.
	// Is this the best thing to do?...
	if (!text.size()) 
	{ 
		UInt3 indices[2] = { {0, 1, 2}, {2, 3, 0} };
		VertexDataTextured zero({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f, 0.f }, {0.f, 0.f}, 0.f, -1.f);
		VertexDataTextured vertices[4] = { zero, zero, zero, zero };
		auto [m_vertexBufferPos, m_indexBufferPos] = getGraphicsBuffer().push(vertices, 4, indices, 2);
		return; 
	}

	// Create variables and reserve memory.
	std::vector<VertexDataTextured> vertices;
	std::vector<UInt3> indices;
	int charCount = text.length();
	indices.reserve((charCount + 1) * 2);	// Add one to the char count for
	vertices.reserve((charCount + 1) * 4);	// the text box.

	// Calculate the string length with kerning. 
	// (Kerning does not apply to the first character)
	m_textLength = 0;
	m_textLength += m_font->characterDictionary[text[0]].xAdvance;  
	for (int i = 1; i < charCount; i++)
	{
		// Retrieve kerning value from dictionary.
		float kerning = 0;
		// Create kerning pair.
		std::pair kerningPair = std::pair(m_font->characterDictionary.at(text[i]).id,
										  m_font->characterDictionary.at(text[i - 1]).id);

		// Check if kerning exists for current pair.  OPTIMIZE!
		if (m_font->kerningDictionary.count(kerningPair))	
			kerning = m_font->kerningDictionary.at(kerningPair);

		// Add character length to total.
		m_textLength += m_font->characterDictionary[text[i]].xAdvance + kerning;
	}

	// ------------------- //
	//  A L I G N M E N T  //
	// ------------------- //

	// Horizontal alignment.
	if (m_horizontalAlign == "C" || m_horizontalAlign == "c") 
	{ 
		cursorStart.x = cursorStart.x - (m_textLength / 2) * m_textScale; 
	}
	else if (m_horizontalAlign == "R" || m_horizontalAlign == "r") 
	{ 
		const Character& endChar = m_font->characterDictionary.at(text[charCount-1]);
		float offset = endChar.xAdvance - endChar.xPlaneBounds[1];
		cursorStart.x = cursorStart.x - (m_textLength - offset) * m_textScale;
	}
	else if (m_horizontalAlign == "L" || m_horizontalAlign == "l") 
	{ 
		const Character& initialChar = m_font->characterDictionary.at(text[0]);
		cursorStart.x = cursorStart.x - (initialChar.xPlaneBounds[0]) * m_textScale;
	}
	// Invalid alignment.
	else	
	{ 
		std::string message = "'" + m_horizontalAlign + "' is not a valid horizontal alignment.\n";
		LUMEN_LOG_ERROR(message, "Text");
		return;
	}

	// Vertical alignment.
	if (m_verticalAlign == "C" || m_verticalAlign == "c") 
	{
		cursorStart.y = cursorStart.y - ((m_font->ascender + m_font->descender) * m_textScale) / 2; 
	}
	else if (m_verticalAlign == "T" || m_verticalAlign == "t") 
	{
		cursorStart.y = cursorStart.y - ((m_font->ascender + m_font->descender) * m_textScale); 
	}
	else if (m_verticalAlign == "U" || m_verticalAlign == "u") 
	{
		cursorStart.y = cursorStart.y - ((m_font->descender) * m_textScale); 
	}
	else if (m_verticalAlign == "B" || m_verticalAlign == "b") 
	{
		// Bottom is the default setting.
	}
	// Invalid alignment.
	else
	{
		std::string message = "'" + m_verticalAlign + "' is not a valid vertical alignment.\n";
		LUMEN_LOG_ERROR(message, "Text");
		return;
	}

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
	float boxZPos = cursorStart.z - 0.001;

	// -----------------------
	// Vertex 1.
	vertices.emplace_back(VertexDataTextured
	(
		glm::vec3(cursorStart.x, cursorStart.y + m_font->descender * m_textScale, boxZPos),
		m_boxColor, glm::vec2(0.f, 0.f), 0, m_entityID)
	);
	// -----------------------
	// Vertex2.
	vertices.emplace_back(VertexDataTextured
	(
		glm::vec3(cursorStart.x, cursorStart.y + m_font->ascender * m_textScale, boxZPos),
		m_boxColor, glm::vec2(0.f, 1.f), 0, m_entityID)
	);
	// -----------------------
	// Vertex 3.
	vertices.emplace_back(VertexDataTextured
	(
		glm::vec3(cursorStart.x + m_textLength * m_textScale, cursorStart.y + m_font->ascender * m_textScale, boxZPos),
		m_boxColor, glm::vec2((1.f, 1.f)), 0, m_entityID)
	);
	// -----------------------
	// Vertex 4.
	vertices.emplace_back(VertexDataTextured
	(
		glm::vec3(cursorStart.x + m_textLength * m_textScale, cursorStart.y + m_font->descender * m_textScale, boxZPos),
		m_boxColor, glm::vec2((0.f, 1.f)), 0, m_entityID)
	);
	// -----------------------
	// Indices.
	indices.insert(indices.end(), {
				{ 0 + m_vertexCount, 1 + m_vertexCount, 2 + m_vertexCount },
				{ 2 + m_vertexCount, 3 + m_vertexCount, 0 + m_vertexCount }
			});
	// Increment counts.
	m_vertexCount += 4;
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

	// Generate a quad for each character.
	float totalAdvance = 0;
	for (int i = 0; i < charCount; i++)
	{
		// -----------------------
		// Load character.
		const Character& c = m_font->characterDictionary.at(text[i]);
		// Retrieve kerning value from dictionary.
		float kerning = 0;
		if (i)  // Kerning does not apply to the first character.
		{
			// Create kerning pair.
			std::pair kerningPair = std::pair(m_font->characterDictionary.at(text[i]).id,
											  m_font->characterDictionary.at(text[i - 1]).id);

			// Check if kerning exists for current pair.
			if (m_font->kerningDictionary.count(kerningPair))	
				kerning = m_font->kerningDictionary.at(kerningPair);
		}

		// -----------------------
		// Vertex 1.
		vertices.emplace_back(VertexDataTextured
		(
			glm::vec3(cursorStart.x + (totalAdvance + c.xPlaneBounds[0] + kerning) * m_textScale,
					  cursorStart.y + (c.yPlaneBounds[0]) * m_textScale,
					  cursorStart.z),
			m_colour, glm::vec2(c.xTextureCoords[0], c.yTextureCoords[0]), 1, m_entityID)
		);
		// -----------------------
		// Vertex2.
		vertices.emplace_back(VertexDataTextured
		(
			glm::vec3(cursorStart.x + (totalAdvance + c.xPlaneBounds[0] + kerning) * m_textScale,
					  cursorStart.y + (c.yPlaneBounds[0] + c.height) * m_textScale,
					  cursorStart.z),
			m_colour, glm::vec2(c.xTextureCoords[0], c.yTextureCoords[1]), 1, m_entityID)
		);
		// -----------------------
		// Vertex 3.
		vertices.emplace_back(VertexDataTextured
		(
			glm::vec3(cursorStart.x + (totalAdvance + c.xPlaneBounds[0] + c.width + kerning) * m_textScale,
					  cursorStart.y + (c.yPlaneBounds[0] + c.height) * m_textScale,
					  cursorStart.z),
			m_colour, glm::vec2(c.xTextureCoords[1], c.yTextureCoords[1]), 1, m_entityID)
		);
		// -----------------------
		// Vertex 4.
		vertices.emplace_back(VertexDataTextured
		(
			glm::vec3(cursorStart.x + (totalAdvance + c.xPlaneBounds[0] + c.width + kerning) * m_textScale,
					  cursorStart.y + (c.yPlaneBounds[0]) * m_textScale,
					  cursorStart.z),
			m_colour, glm::vec2(c.xTextureCoords[1], c.yTextureCoords[0]), 1, m_entityID)
		);
		// -----------------------
		// Indices.
		indices.insert(indices.end(), {
				{ 0 + m_vertexCount, 1 + m_vertexCount, 2 + m_vertexCount },
				{ 2 + m_vertexCount, 3 + m_vertexCount, 0 + m_vertexCount }
			});
		// Move cursor for next character.
		totalAdvance += c.xAdvance + kerning;
		// Increment counts.
		m_vertexCount += 4;
		// -----------------------
	}

	// Push data to the buffer.
	pushToGraphicsBuffer(vertices.data(), vertices.size(), indices.data(), indices.size());
}

void Text::setScale(float scale)
{
	m_textScale = scale;
	updateText(m_string);
}

//=============================================================================================================================================//
//  Text manipulation.																													       //
//=============================================================================================================================================//

bool Text::updateText(const std::string& text) 
{
	if (m_string == text) return false;

	m_string = text;
	removeFromGraphicsBuffer();
	generateText(m_string);

	// A bit hacky...
	if (m_outlineEnabled)
	{
		m_outlineEnabled = false;
		enableOutline();
	}

	return true;
}

bool Text::updateAlignment(const std::string& horizontalAlignment, const std::string& verticalAlignment)
{
	if (horizontalAlignment == m_horizontalAlign && verticalAlignment == m_verticalAlign) return false;

	removeFromGraphicsBuffer();
	m_horizontalAlign = horizontalAlignment;
	m_verticalAlign = verticalAlignment;
	generateText(m_string);

	// A bit hacky...
	if (m_outlineEnabled)
	{
		m_outlineEnabled = false;
		enableOutline();
	}

	return true;
}

void Text::setBoxColour(const glm::vec4& colour) 
{ 
	m_boxColor = colour;
	for (int i = 0; i < 4; i++)
		getVertex(i).color = colour;

	syncWithGPU();
}

void Text::setColor(const glm::vec4& color) 
{
	m_colour = color;
	for (int i = 4; i < m_vertexCount; i++)
		getVertex(i).color = color;

	syncWithGPU();
}

void Text::setLayer(float layer)
{
	// Text box.
	for (int i = 0; i < 4; i++)
		getVertex(i).position.z = layer - 0.001;

	// Text.
	for (int i = 4; i < m_vertexCount; i++)
		getVertex(i).position.z = layer;

	m_trackedCenter.z = layer;
	syncWithGPU();
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//