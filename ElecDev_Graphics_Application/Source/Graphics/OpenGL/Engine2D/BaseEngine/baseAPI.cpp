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

	// Write to CPU side buffer.
	m_linesCPU.insert(m_linesCPU.end(), vertices.begin(), vertices.end());
}

// Draw clear triangle.
void BaseEngineGL::drawTriangleClear(float position1[2], float position2[2], float position3[2], float color[4])
{
	// Define position data.
	VertexData v1(position1[0], position1[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v2(position2[0], position2[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v3(position3[0], position3[1], 0.0f, color[0], color[1], color[2], color[3]);
	std::vector<VertexData> vertices = { v1, v2, v2, v3, v3, v1 };

	// Write to CPU side buffer.
	m_linesCPU.insert(m_linesCPU.end(), vertices.begin(), vertices.end());
}

// Draw filled triangle.
void BaseEngineGL::drawTriangleFilled(float position1[2], float position2[2], float position3[2], float color[4]) 
{
	// Define position data.
	VertexData v1(position1[0], position1[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v2(position2[0], position2[1], 0.0f, color[0], color[1], color[2], color[3]);
	VertexData v3(position3[0], position3[1], 0.0f, color[0], color[1], color[2], color[3]);
	std::vector<VertexData> vertices = { v1, v2, v3 };

	// Write to CPU side buffer.
	m_trianglesCPU.insert(m_trianglesCPU.end(), vertices.begin(), vertices.end());
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

	// Write to CPU side buffer.
	m_linesCPU.insert(m_linesCPU.end(), vertices.begin(), vertices.end());
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

	// Write to CPU side buffer.
	m_trianglesCPU.insert(m_trianglesCPU.end(), vertices.begin(), vertices.end());
}

// Draws a clear circle.
void BaseEngineGL::drawCircleClear(float coords[2], float radius, float color[4])
{
	// Lines used to draw the circle.
	std::vector<VertexData> vertices;

	// Create lines requried to draw a circle.
	for (int i = 0; i <= m_circleResolution; i++)
	{
		// Create one of the lines that make up the circle.
		float x1 = coords[0] + radius * std::cos((i - 1) * PI * 2 / m_circleResolution);
		float y1 = coords[1] + radius * std::sin((i - 1) * PI * 2 / m_circleResolution);
		float x2 = coords[0] + radius * std::cos(i * PI * 2 / m_circleResolution);
		float y2 = coords[1] + radius * std::sin(i * PI * 2 / m_circleResolution);
		VertexData v1(x1, y1, 0.0f, color[0], color[1], color[2], color[3]);
		VertexData v2(x2, y2, 0.0f, color[0], color[1], color[2], color[3]);
		std::vector<VertexData> verticesTemp = { v1, v2 };
		// Add line to total lines.
		vertices.insert(vertices.end(), verticesTemp.begin(), verticesTemp.end());
	}

	// Write to CPU side buffer.
	m_linesCPU.insert(m_linesCPU.end(), vertices.begin(), vertices.end());
}

// Draws a filled circle.
void BaseEngineGL::drawCircleFilled(float coords[2], float radius, float color[4])
{
	// Lines used to draw the circle.
	std::vector<VertexData> vertices;

	// Create triangles requried to draw a circle.
	for (int i = 0; i <= m_circleResolution; i++)
	{
		float x1 = coords[0] + radius * std::cos((i - 1) * PI * 2 / m_circleResolution);
		float y1 = coords[1] + radius * std::sin((i - 1) * PI * 2 / m_circleResolution);
		float x2 = coords[0] + radius * std::cos(i * PI * 2 / m_circleResolution);
		float y2 = coords[1] + radius * std::sin(i * PI * 2 / m_circleResolution);
		VertexData v1(coords[0], coords[1], 0.0f, color[0], color[1], color[2], color[3]);
		VertexData v2(x1, y1, 0.0f, color[0], color[1], color[2], color[3]);
		VertexData v3(x2, y2, 0.0f, color[0], color[1], color[2], color[3]);
		std::vector<VertexData> verticesTemp = { v1, v2, v3 };
		// Add triangle to total triangles.
		vertices.insert(vertices.end(), verticesTemp.begin(), verticesTemp.end());
	}
	// Write to CPU side buffer.
	m_trianglesCPU.insert(m_trianglesCPU.end(), vertices.begin(), vertices.end());
}

// Adds text to the VBO object.
void BaseEngineGL::drawText(std::string text, float coords[2], float color[4], float scale, char align)
{
	// Calculate the length & height of the string.
	float length = 0;
	float height = m_textRenderer->m_characterDictionary[text[0]].height;
	for (char c : text) 
	{
		length += m_textRenderer->m_characterDictionary[c].xAdvance;
	}

	// Center the text.
	if (align == 'C' || align == 'c')
	{
		// Place the coords in the center of the text.
		coords[0] = coords[0] - (length * scale) / 2;
		// Place on top of coordinate.
		coords[1] = coords[1] + height * scale;
		// Write text to CPU side buffer.
		m_textRenderer->writeText(&m_texturedTrianglesCPU, text, coords, color, scale);
	}
	// Right allign the text.
	else if (align == 'R' || align == 'r') 
	{
		// Align text to right.
		coords[0] = coords[0] - length * scale;
		// Place on top of coordinate.
		coords[1] = coords[1] + height * scale;
		// Write text to CPU side buffer.
		m_textRenderer->writeText(&m_texturedTrianglesCPU, text, coords, color, scale);
	}
	// Left allign the text.
	else if (align == 'L' || align == 'l') 
	{
		// Place on top of the coordinate.
		coords[1] = coords[1] + height * scale;
		// Write text to CPU side buffer.
		m_textRenderer->writeText(&m_texturedTrianglesCPU, text, coords, color, scale);
	}
	// Display error.
	else 
	{
		std::cout << "[INTERFACE][ERROR] '" << align << "' is not a valid alignment.\n\n";
	}	
}

// Draws the demo drawing.
void BaseEngineGL::drawDemo(unsigned int loopCount)
{
	for (unsigned int i = 0; i <= loopCount * 3; i += 3)
	{
		for (unsigned int k = 0; k <= loopCount * 3; k += 3)
		{
			// Draw filled triangle example.
			float ftPos1[2] = { -1.0f + i, -1.0f + k };
			float ftPos2[2] = { -1.0f + i, -0.5f + k };
			float ftPos3[2] = { -1.5f + i, -1.0f + k };
			float ftColor[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
			drawTriangleFilled(ftPos1, ftPos2, ftPos3, ftColor);

			// Draw clear quad.
			float cqCoords[2] = { 0.0f + i, 0.0f + k };
			float cqColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
			drawQuadClear(cqCoords, 2.0f, 2.0f, cqColor);

			// Draw filled quad.
			float fqCoords[2] = { -0.5f + i, 0.5f + k };
			float fqColor[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
			drawQuadFilled(fqCoords, 0.25f, 0.3f, fqColor);

			// Draw filed ciricle.
			float coords1[2] = { 0.0f + i, 0.0f + k };
			float color[4] = { 1.0f, 0.6f, 0.0f, 1.0f };
			drawCircleFilled(coords1, 0.2f, color);
			// Draw clear ciricle.
			float coords2[2] = { (float)i, -0.75f + k };
			drawCircleClear(coords2, 0.2f, color);

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
			m_texturedTrianglesCPU.insert(m_texturedTrianglesCPU.end(), verticesTex.begin(), verticesTex.end());

			// Test the text rendering.
			float pos[2] = { 0.5f + i, 0.5f + k };
			std::string text = "Testing font!";
			float colorText[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
			drawCircleFilled(pos, 0.01f, color);
			drawText(text, pos, colorText, 1.0f, 'C');
		}
	}
	// Load the CPU buffers into the GPU.
	updateBuffers();
}

//----------------------------------------------------------------------------------------------------------------------
//  Buffers.
//----------------------------------------------------------------------------------------------------------------------

// Writes the CPU side buffers to the GPU.
void BaseEngineGL::updateBuffers() 
{
	m_linesVAO->writeData(m_linesCPU);
	m_trianglesVAO->writeData(m_trianglesCPU);
	m_textureTrianglesVAO->writeData(m_texturedTrianglesCPU);
}

//----------------------------------------------------------------------------------------------------------------------
//  Camera manipulation.
//----------------------------------------------------------------------------------------------------------------------

// Automatically places the drawing in the center and scales it to fit into the window..
void BaseEngineGL::autoCenter() 
{	
	// First reset the matrices.
	m_scalingMatrix = glm::mat4(1.0f);
	m_translationMatrix = glm::mat4(1.0f);
	m_rotationMatrix = glm::mat4(1.0f);
	// And reset the base matrices.
	m_scalingMatrixBase = glm::mat4(1.0f);
	m_translationMatrixBase = glm::mat4(1.0f);
	m_rotationMatrixBase = glm::mat4(1.0f);

	// Init min/max values.
	float max[2] = { m_linesCPU[0].position[0], m_linesCPU[0].position[1] };
	float min[2] = { m_linesCPU[0].position[0], m_linesCPU[0].position[1] };

	//----------------------------------------------------------------------
	// Lines.
	//----------------------------------------------------------------------

	// Find the maximum and minimum values for x and y.
	for (int i=1; i< m_linesCPU.size(); i++)
	{	
		// Check max for x.
		if (m_linesCPU[i].position[0] > max[0])
		{
			max[0] = m_linesCPU[i].position[0];
		}
		// Check min for x.
		else if (m_linesCPU[i].position[0] < min[0])
		{
			min[0] = m_linesCPU[i].position[0];
		}
		// Check max for y.
		if (m_linesCPU[i].position[1] > max[1])
		{
			max[1] = m_linesCPU[i].position[1];
		}
		// Check min for y.
		else if (m_linesCPU[i].position[1] < min[1])
		{
			min[1] = m_linesCPU[i].position[1];
		}
	}

	//----------------------------------------------------------------------
	// Triangles.
	//----------------------------------------------------------------------

	// Find the maximum and minimum values for x and y.
	for (int i = 0; i < m_trianglesCPU.size(); i++)
	{
		// Check max for x.
		if (m_trianglesCPU[i].position[0] > max[0])
		{
			max[0] = m_trianglesCPU[i].position[0];
		}
		// Check min for x.
		else if (m_trianglesCPU[i].position[0] < min[0])
		{
			min[0] = m_trianglesCPU[i].position[0];
		}
		// Check max for y.
		if (m_trianglesCPU[i].position[1] > max[1])
		{
			max[1] = m_trianglesCPU[i].position[1];
		}
		// Check min for y.
		else if (m_trianglesCPU[i].position[1] < min[1])
		{
			min[1] = m_trianglesCPU[i].position[1];
		}
	}

	//----------------------------------------------------------------------
	// Textured triangles.
	//----------------------------------------------------------------------

	// Find the maximum and minimum values for x and y.
	for (int i = 0; i < m_texturedTrianglesCPU.size(); i++)
	{
		// Check max for x.
		if (m_texturedTrianglesCPU[i].position[0] > max[0])
		{
			max[0] = m_texturedTrianglesCPU[i].position[0];
		}
		// Check min for x.
		else if (m_texturedTrianglesCPU[i].position[0] < min[0])
		{
			min[0] = m_texturedTrianglesCPU[i].position[0];
		}
		// Check max for y.
		if (m_texturedTrianglesCPU[i].position[1] > max[1])
		{
			max[1] = m_texturedTrianglesCPU[i].position[1];
		}
		// Check min for y.
		else if (m_texturedTrianglesCPU[i].position[1] < min[1])
		{
			min[1] = m_texturedTrianglesCPU[i].position[1];
		}
	}

	//----------------------------------------------------------------------

	// Calculate the values to translate.
	float translate[2]{};
	translate[0] = -(max[0] + min[0]) / 2;
	translate[1] = -(max[1] + min[1]) / 2;

	// Now translate the camera accordingly.
	m_translationMatrix = glm::translate(m_translationMatrix, glm::vec3(translate[0], translate[1], 0.0f));
	// Add to base matrix.
	m_translationMatrixBase = glm::translate(m_translationMatrixBase, glm::vec3(translate[0], translate[1], 0.0f));

	// Scale the drawing according to the largest translation that took place (This gives us the new max value,
	// centered around (0.0).
	if (translate[0] > translate[1]) 
	{
		float scale = std::abs(translate[0] * 1.1f);
		m_scalingMatrix = glm::scale(m_scalingMatrix, glm::vec3(1 / scale, 1 / scale, 1.0f));
		// Update base matrix.
		m_scalingMatrixBase = glm::scale(m_scalingMatrixBase, glm::vec3(1 / scale, 1 / scale, 1.0f));
	}
	else 
	{
		float scale = std::abs(translate[1] * 1.1f);
		m_scalingMatrix = glm::scale(m_scalingMatrix, glm::vec3(1 / scale, 1 / scale, 1.0f));
		// Update base matrix.
		m_scalingMatrixBase = glm::scale(m_scalingMatrixBase, glm::vec3(1 / scale, 1 / scale, 1.0f));
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------