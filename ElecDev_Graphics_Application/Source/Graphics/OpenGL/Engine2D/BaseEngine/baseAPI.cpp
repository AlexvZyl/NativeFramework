/*
Engine API.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "Engine2D/BaseEngine/baseEngineCore.h"

//----------------------------------------------------------------------------------------------------------------------
//  Rendering.
//----------------------------------------------------------------------------------------------------------------------

// Draws a line.
void BaseEngineGL::drawLine(float position1[2], float position2[2], float color[4])
{
	// Define position data.
	VertexData v1(position1[0], position1[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v2(position2[0], position2[1], 0.0f, color[0], color[1], color[2], color[3]);
	std::vector<VertexData> vertices = {v1,v2};
	// Write to OpenGL buffer.
	m_linesVAO->writeData(vertices);
	// Write to CPU side buffer.
	m_verticesCPU.push_back(v1);
	m_verticesCPU.push_back(v2);
}

// Draw clear triangle.
void BaseEngineGL::drawTriangleClear(float position1[2], float position2[2], float position3[2], float color[4])
{
	// Define position data.
	VertexData v1(position1[0], position1[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v2(position2[0], position2[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v3(position3[0], position3[1], 0.0f, color[0], color[1], color[2], color[3]);
	std::vector<VertexData> vertices = { v1, v2, v2, v3, v3, v1 };
	// Write to OpenGL buffer.
	m_linesVAO->writeData(vertices);
	// Write to CPU side buffer.
	m_verticesCPU.push_back(v1);
	m_verticesCPU.push_back(v2);
	m_verticesCPU.push_back(v3);
}

// Draw filled triangle.
void BaseEngineGL::drawTriangleFilled(float position1[2], float position2[2], float position3[2], float color[4]) 
{
	// Define position data.
	VertexData v1(position1[0], position1[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v2(position2[0], position2[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v3(position3[0], position3[1], 0.0f, color[0], color[1], color[2], color[3]);
	std::vector<VertexData> vertices = { v1, v2, v3 };
	// Write to OpenGL buffer.
	m_trianglesVAO->writeData(vertices);
	// Write to CPU side buffer.
	m_verticesCPU.push_back(v1);
	m_verticesCPU.push_back(v2);
	m_verticesCPU.push_back(v3);
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
	// Write to OpenGL buffer.
	m_linesVAO->writeData(vertices);
	// Write to CPU side buffer.
	m_verticesCPU.push_back(v1);
	m_verticesCPU.push_back(v2);
	m_verticesCPU.push_back(v3);
	m_verticesCPU.push_back(v4);
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
	// Write to OpenGL buffer.
	m_trianglesVAO->writeData(vertices);
	// Write to CPU side buffer.
	m_verticesCPU.push_back(v1);
	m_verticesCPU.push_back(v2);
	m_verticesCPU.push_back(v3);
	m_verticesCPU.push_back(v4);
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
		// Write to OpenGL buffer.
		m_linesVAO->writeData(vertices);
		// Write to CPU side buffer.
		m_verticesCPU.push_back(v1);
		m_verticesCPU.push_back(v2);
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
		// Write to OpenGL buffer.
		m_trianglesVAO->writeData(vertices);
		// Write to CPU side buffer.
		m_verticesCPU.push_back(v1);
		m_verticesCPU.push_back(v2);
		m_verticesCPU.push_back(v3);
	}
}

// Adds text to the VBO object.
void BaseEngineGL::drawText(std::string text, float coords[2], float color[4], float scale)
{
	// Test the text rendering.
	m_textRenderer->writeText(text, coords, m_textureTrianglesVAO, 1, color, scale);
	// Write to CPU side buffer.
	VertexData v1{coords[0], coords[1], 0.0f, color[0], color[1], color[2], color[3]};
	m_verticesCPU.push_back(v1);
}

// Draws the demo drawing.
void BaseEngineGL::drawDemo(unsigned int loopCount)
{
	for (int i = 0; i <= loopCount * 3; i += 3)
	{
		for (int k = 0; k <= loopCount * 3; k += 3)
		{
			// Draw filled triangle example.
			float ftPos1[2] = { -1.0f + i, -1.0f + k };
			float ftPos2[2] = { -1.0f + i, -0.5 + k };
			float ftPos3[2] = { -1.5f + i, -1.0f + k };
			float ftColor[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
			drawTriangleFilled(ftPos1, ftPos2, ftPos3, ftColor);

			// Draw clear quad.
			float cqCoords[2] = { 0.0f + i, 0.0f + k };
			float cqColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
			drawQuadClear(cqCoords, 2, 2, cqColor);

			// Draw filled quad.
			float fqCoords[2] = { -0.5f + i, 0.5f + k };
			float fqColor[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
			drawQuadFilled(fqCoords, 0.25, 0.3, fqColor);

			// Draw filed ciricle.
			float coords1[2] = { 0.0f + i, 0.0f + k };
			float color[4] = { 1.0f, 0.6f, 0.0f, 1.0f };
			drawCircleFilled(coords1, 0.2, color);
			// Draw clear ciricle.
			float coords2[2] = { i, -0.75f + k };
			drawCircleClear(coords2, 0.2, color);

			// Draw clear triangle example.
			float ctPos1[2] = { 1.0f + i, -1.0f + k };
			float ctPos2[2] = { 1.5f + i, -1.0f + k };
			float ctPos3[2] = { 1.0f + i, -0.5f + k };
			float ctColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
			drawTriangleClear(ctPos1, ctPos2, ctPos3, ctColor);

			// Test textures.
			TexturedVertexData v1(1.25f + i, 1.25f + k, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 2.0f);
			TexturedVertexData v2(1.25f + i, 0.75f + k, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 2.0f);
			TexturedVertexData v3(0.75f + i, 0.75f + k, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f);
			TexturedVertexData v4(0.75f + i, 1.25f + k, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 2.0f);
			std::vector<TexturedVertexData> verticesTex = { v1, v2, v3, v3, v4, v1 };
			m_textureTrianglesVAO->writeData(verticesTex);

			// Test the text rendering.
			float pos[2] = { 0.5f + i, 0.5f + k };
			std::string text = "Testing-Font and Different_characters. [!&>*?\\] ";
			float colorText[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
			drawText(text, pos, colorText, 1);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  Camera manipulation.
//----------------------------------------------------------------------------------------------------------------------

// Automatically places the drawing in the center and scales it to fit into the window..
void BaseEngineGL::autoCenter() 
{	
	// Store min and max values.  Assign first values as default.
	float max[2] = { m_verticesCPU[0].position[0], m_verticesCPU[0].position[1] };
	float min[2] = { m_verticesCPU[0].position[0], m_verticesCPU[0].position[1] };

	// Find the maximum and minimum values for x and y.
	for (int i=1; i<m_verticesCPU.size(); i++) 
	{	
		// Check max for x.
		if (m_verticesCPU[i].position[0] > max[0])
		{
			max[0] = m_verticesCPU[i].position[0]; 
		}
		// Check min for x.
		else if (m_verticesCPU[i].position[0] < min[0])
		{
			min[0] = m_verticesCPU[i].position[0];
		}
		// Check max for y.
		if (m_verticesCPU[i].position[1] > max[1])
		{
			max[1] = m_verticesCPU[i].position[1];
		}
		// Check min for y.
		else if (m_verticesCPU[i].position[1] < min[1])
		{
			min[1] = m_verticesCPU[i].position[1];
		}
	}

	// Calculate the values to translate.
	float translate[2]{};
	translate[0] = -(max[0] - min[0]) / 2;
	translate[1] = -(max[1] - min[1]) / 2;

	// Now translate the camera accordingly.
	m_translationMatrix = glm::translate(m_translationMatrix, glm::vec3(translate[0], translate[1], 0.0f));
	// Scale the drawing according to the largest translation that took place (This gives us the new max value,
	// centered around (0.0).
	if (translate[0] > translate[1]) 
	{
		float scale = std::abs(translate[0] * (1.1f));
		m_scalingMatrix = glm::scale(m_scalingMatrix, glm::vec3(1 / scale, 1 / scale, 1.0f));
	}
	else 
	{
		float scale = std::abs(translate[1] * (1.1f));
		m_scalingMatrix = glm::scale(m_scalingMatrix, glm::vec3(1 / scale, 1 / scale, 1.0f));
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------
