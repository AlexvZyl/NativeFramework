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
	// Write to bbuffer.
	m_linesVAO->writeData(v1,v2);
}

// Draw clear triangle.
void BaseEngineGL::drawTriangleClear(float position1[2], float position2[2], float position3[2], float color[4]) 
{
	// Define position data.
	VertexData v1(position1[0], position1[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v2(position2[0], position2[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v3(position3[0], position3[1], 0.0f, color[0], color[1], color[2], color[3]);
	// Write to buffer.
	m_trianglesClearVAO->writeData(v1,v2);
	m_trianglesClearVAO->writeData(v2,v3);
	m_trianglesClearVAO->writeData(v3,v1);
}

// Draw filled triangle.
void BaseEngineGL::drawTriangleFilled(float position1[2], float position2[2], float position3[2], float color[4]) 
{
	// Define position data.
	VertexData v1(position1[0], position1[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v2(position2[0], position2[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v3(position3[0], position3[1], 0.0f, color[0], color[1], color[2], color[3]);
	// Write to buffer.
	m_trianglesFilledVAO->writeData(v1, v2, v3);
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
	m_quadsClearVAO->writeData(v1, v2, v2, v3);
	m_quadsClearVAO->writeData(v3, v4, v4, v1);
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
	m_quadsFilledVAO->writeData(v1, v2, v3);
	m_quadsFilledVAO->writeData(v3, v4, v1);
}

// Draws a filled circle.
void BaseEngineGL::drawCircleFilled(float coords[2], float radius, float color[4])
{
	for (int i = 0; i <= m_circleResolution; i++)
	{
		float x1 = coords[0] + radius * std::cos((i-1) * PI * 2 / m_circleResolution);
		float y1 = coords[1] + radius * std::sin((i-1) * PI * 2 / m_circleResolution);
		float x2 = coords[0] + radius * std::cos(i * PI * 2 / m_circleResolution);
		float y2 = coords[1] + radius * std::sin(i * PI * 2 / m_circleResolution);
		VertexData v1(coords[0], coords[1], 0.0f, color[0], color[1], color[2], color[3]);
		VertexData v2(x1, y1, 0.0f, color[0], color[1], color[2], color[3]);
		VertexData v3(x2, y2, 0.0f, color[0], color[1], color[2], color[3]);
		m_circlesFilledVAO->writeData(v1);
		m_circlesFilledVAO->writeData(v2);
		m_circlesFilledVAO->writeData(v3);
	}
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
		m_circlesClearVAO->writeData(v1);
		m_circlesClearVAO->writeData(v2);
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
