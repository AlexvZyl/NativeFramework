/*
Engine API.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "Engine2D/BaseEngine/core.h"

//----------------------------------------------------------------------------------------------------------------------
//  Functions.
//----------------------------------------------------------------------------------------------------------------------

// Draws a line.
void BaseEngineGL::drawLine(float position1[2], float position2[2], float color[4])
{
	// Define position data.
	VertexData v1(position1[0], position1[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v2(position2[0], position2[1], 0.0f, color[0], color[1], color[2], color[3]);
	std::vector<VertexData> vertices = {v1,v2};
	// Write to bbuffer.
	m_linesVAO->writeData(vertices);
}

// Draw clear triangle.
void BaseEngineGL::drawTriangleClear(float position1[2], float position2[2], float position3[2], float color[4])
{
	// Define position data.
	VertexData v1(position1[0], position1[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v2(position2[0], position2[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v3(position3[0], position3[1], 0.0f, color[0], color[1], color[2], color[3]);
	std::vector<VertexData> vertices = { v1, v2, v2, v3, v3, v1 };
	// Write to buffer.
	m_linesVAO->writeData(vertices);
}

// Draw filled triangle.
void BaseEngineGL::drawTriangleFilled(float position1[2], float position2[2], float position3[2], float color[4]) 
{
	// Define position data.
	VertexData v1(position1[0], position1[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v2(position2[0], position2[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v3(position3[0], position3[1], 0.0f, color[0], color[1], color[2], color[3]);
	std::vector<VertexData> vertices = { v1, v2, v3,};
	// Write to buffer.
	m_trianglesVAO->writeData(vertices);
}

// Draw a clear quad.
void BaseEngineGL::drawQuadClear(float position[2], float width, float height, float color[4]) 
{
	width /= 2;
	height /= 2;
	VertexData v1(position[0]+width, position[1]+height, 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v2(position[0]+width, position[1]-height, 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v3(position[0]-width, position[1]-height, 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v4(position[0]-width, position[1]+height, 0.0f, color[0], color[1], color[2], color[3]);
	std::vector<VertexData> vertices = { v1, v2, v2, v3, v3, v4, v4, v1 };
	m_linesVAO->writeData(vertices);
}

// Draw a filled quad.
void BaseEngineGL::drawQuadFilled(float position[2], float width, float height, float color[4]) 
{
	width /= 2;
	height /= 2;
	VertexData v1(position[0] + width, position[1] + height, 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v2(position[0] + width, position[1] - height, 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v3(position[0] - width, position[1] - height, 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v4(position[0] - width, position[1] + height, 0.0f, color[0], color[1], color[2], color[3]);
	std::vector<VertexData> vertices = { v1, v2, v3, v3, v4, v1 };
	m_trianglesVAO->writeData(vertices);
}

// Draws a clear circle.
void BaseEngineGL::drawCircleClear(float coords[2], float radius, float color[4])
{
	for (int i = 0; i <= m_circleResolution; i++)
	{
		float x1 = coords[0] + radius * std::cos((i - 1) * PI * 2 / m_circleResolution);
		float y1 = coords[1] + radius * std::sin((i - 1) * PI * 2 / m_circleResolution);
		float x2 = coords[0] + radius * std::cos(i * PI * 2 / m_circleResolution);
		float y2 = coords[1] + radius * std::sin(i * PI * 2 / m_circleResolution);
		VertexData v1(x1, y1, 0.0f, color[0], color[1], color[2], color[3]);
		VertexData v2(x2, y2, 0.0f, color[0], color[1], color[2], color[3]);
		std::vector<VertexData> vertices = { v1, v2 };
		m_linesVAO->writeData(vertices);
	}
}

// Draws a filled circle.
void BaseEngineGL::drawCircleFilled(float coords[2], float radius, float color[4])
{
	for (int i = 0; i <= m_circleResolution; i++)
	{
		float x1 = coords[0] + radius * std::cos((i - 1) * PI * 2 / m_circleResolution);
		float y1 = coords[1] + radius * std::sin((i - 1) * PI * 2 / m_circleResolution);
		float x2 = coords[0] + radius * std::cos(i * PI * 2 / m_circleResolution);
		float y2 = coords[1] + radius * std::sin(i * PI * 2 / m_circleResolution);
		VertexData v1(coords[0], coords[1], 0.0f, color[0], color[1], color[2], color[3]);
		VertexData v2(x1, y1, 0.0f, color[0], color[1], color[2], color[3]);
		VertexData v3(x2, y2, 0.0f, color[0], color[1], color[2], color[3]);
		std::vector<VertexData> vertices = { v1, v2, v3 };
		m_trianglesVAO->writeData(vertices);
	}
}

// Adds text to the VBO object.
void BaseEngineGL::drawText()
{
	return;
}

// Displays the new drawing to the screen.
// Required after each new element has been added.
void BaseEngineGL::display()
{

}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------
