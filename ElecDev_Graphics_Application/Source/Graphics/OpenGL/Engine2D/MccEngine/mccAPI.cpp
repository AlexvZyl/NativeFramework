/*
Engine API.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "mccEngineCore.h"

//----------------------------------------------------------------------------------------------------------------------
//  Functions.
//----------------------------------------------------------------------------------------------------------------------

// Draws a line.
void MccEngineGL::drawLine(std::string mccName, float position1[2], float position2[2], float color[4])
{
	// Define position data.
	VertexData v1(position1[0], position1[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v2(position2[0], position2[1], 0.0f, color[0], color[1], color[2], color[3]);
	std::vector<VertexData> vertices = { v1,v2 };
	// Write to bbuffer.
	 m_mccDictionary[mccName]->m_linesVAO->writeData(vertices);
}

// Draw clear triangle.
void MccEngineGL::drawTriangleClear(std::string mccName, float position1[2], float position2[2], float position3[2], float color[4])
{
	// Define position data.
	VertexData v1(position1[0], position1[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v2(position2[0], position2[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v3(position3[0], position3[1], 0.0f, color[0], color[1], color[2], color[3]);
	std::vector<VertexData> vertices = { v1, v2, v2, v3, v3, v1 };
	// Write to buffer.
	m_mccDictionary[mccName]->m_linesVAO->writeData(vertices);
}

// Draw filled triangle.
void MccEngineGL::drawTriangleFilled(std::string mccName, float position1[2], float position2[2], float position3[2], float color[4])
{
	// Define position data.
	VertexData v1(position1[0], position1[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v2(position2[0], position2[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v3(position3[0], position3[1], 0.0f, color[0], color[1], color[2], color[3]);
	std::vector<VertexData> vertices = { v1, v2, v3 };
	// Write to buffer.
	m_mccDictionary[mccName]->m_trianglesVAO->writeData(vertices);
}

// Draw a clear quad.
void MccEngineGL::drawQuadClear(std::string mccName, float position[2], float width, float height, float color[4])
{
	width /= 2;
	height /= 2;
	VertexData v1(position[0] + width, position[1] + height, 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v2(position[0] + width, position[1] - height, 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v3(position[0] - width, position[1] - height, 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v4(position[0] - width, position[1] + height, 0.0f, color[0], color[1], color[2], color[3]);
	std::vector<VertexData> vertices = { v1, v2, v2, v3, v3, v4, v4, v1 };
	m_mccDictionary[mccName]->m_linesVAO->writeData(vertices);
}

// Draw a filled quad.
void MccEngineGL::drawQuadFilled(std::string mccName, float position[2], float width, float height, float color[4])
{
	width /= 2;
	height /= 2;
	VertexData v1(position[0] + width, position[1] + height, 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v2(position[0] + width, position[1] - height, 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v3(position[0] - width, position[1] - height, 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v4(position[0] - width, position[1] + height, 0.0f, color[0], color[1], color[2], color[3]);
	std::vector<VertexData> vertices = { v1, v2, v3, v3, v4, v1 };
	m_mccDictionary[mccName]->m_trianglesVAO->writeData(vertices);
}

// Draws a clear circle.
void MccEngineGL::drawCircleClear(std::string mccName, float coords[2], float radius, float color[4])
{
	for (int i = 0; i <= m_mccDictionary[mccName]->m_circleResolution; i++)
	{
		float x1 = coords[0] + radius * std::cos((i - 1) * PI * 2 / m_mccDictionary[mccName]->m_circleResolution);
		float y1 = coords[1] + radius * std::sin((i - 1) * PI * 2 / m_mccDictionary[mccName]->m_circleResolution);
		float x2 = coords[0] + radius * std::cos(i * PI * 2 / m_mccDictionary[mccName]->m_circleResolution);
		float y2 = coords[1] + radius * std::sin(i * PI * 2 / m_mccDictionary[mccName]->m_circleResolution);
		VertexData v1(x1, y1, 0.0f, color[0], color[1], color[2], color[3]);
		VertexData v2(x2, y2, 0.0f, color[0], color[1], color[2], color[3]);
		std::vector<VertexData> vertices = { v1, v2 };
		m_mccDictionary[mccName]->m_linesVAO->writeData(vertices);
	}
}

// Draws a filled circle.
void MccEngineGL::drawCircleFilled(std::string mccName, float coords[2], float radius, float color[4])
{
	for (int i = 0; i <= m_mccDictionary[mccName]->m_circleResolution; i++)
	{
		float x1 = coords[0] + radius * std::cos((i - 1) * PI * 2 / m_mccDictionary[mccName]->m_circleResolution);
		float y1 = coords[1] + radius * std::sin((i - 1) * PI * 2 / m_mccDictionary[mccName]->m_circleResolution);
		float x2 = coords[0] + radius * std::cos(i * PI * 2 / m_mccDictionary[mccName]->m_circleResolution);
		float y2 = coords[1] + radius * std::sin(i * PI * 2 / m_mccDictionary[mccName]->m_circleResolution);
		VertexData v1(coords[0], coords[1], 0.0f, color[0], color[1], color[2], color[3]);
		VertexData v2(x1, y1, 0.0f, color[0], color[1], color[2], color[3]);
		VertexData v3(x2, y2, 0.0f, color[0], color[1], color[2], color[3]);
		std::vector<VertexData> vertices = { v1, v2, v3 };
		m_mccDictionary[mccName]->m_trianglesVAO->writeData(vertices);
	}
}

// Adds text to the VBO object.
void MccEngineGL::drawText(std::string mccName, std::string text, float coords[2], float color[4], float scale)
{
	// Test the text rendering.
	m_mccDictionary[mccName]->m_textRenderer->writeText(text, coords, m_mccDictionary[mccName]->m_textureTrianglesVAO, 1, color, scale);
}

// Displays the new drawing to the screen.
// Required after each new element has been added.
void MccEngineGL::display()
{

}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------
