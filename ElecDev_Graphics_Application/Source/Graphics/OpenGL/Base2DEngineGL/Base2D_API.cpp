/*
Engine API.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Base2D_Engine.h"

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

// Draws a line.
void Base2DEngineGL::drawLine(float position1[2], float position2[2], float color[4])
{
	// Define position data.
	VertexData v1(position1[0], position1[1], 0.0f, color[0], color[1], color[2], color[3], 0);
	VertexData v2(position2[0], position2[1], 0.0f, color[0], color[1], color[2], color[3], 0);
	std::vector<VertexData> vertices = {v1,v2};
	// Write to CPU side buffer.
	m_linesVAO->writeData(vertices);
}

// Draw clear triangle.
void Base2DEngineGL::drawTriangleClear(float position1[2], float position2[2], float position3[2], float color[4])
{
	// Define position data.
	VertexData v1(position1[0], position1[1], 0.0f, color[0], color[1], color[2], color[3], 0);
	VertexData v2(position2[0], position2[1], 0.0f, color[0], color[1], color[2], color[3], 0);
	VertexData v3(position3[0], position3[1], 0.0f, color[0], color[1], color[2], color[3], 0);
	std::vector<VertexData> vertices = { v1, v2, v2, v3, v3, v1 };
	// Write to CPU side buffer.
	m_linesVAO->writeData(vertices);
}

// Draw filled triangle.
void Base2DEngineGL::drawTriangleFilled(float position1[2], float position2[2], float position3[2], float color[4]) 
{
	// Define position data.
	VertexData v1(position1[0], position1[1], 0.0f, color[0], color[1], color[2], color[3], 0);
	VertexData v2(position2[0], position2[1], 0.0f, color[0], color[1], color[2], color[3], 0);
	VertexData v3(position3[0], position3[1], 0.0f, color[0], color[1], color[2], color[3], 0);
	std::vector<VertexData> vertices = { v1, v2, v3 };
	// Write to CPU side buffer.
	m_trianglesVAO->writeData(vertices);
}

// Draw a clear quad.
void Base2DEngineGL::drawQuadClear(float position[2], float width, float height, float color[4]) 
{
	width /= 2;
	height /= 2;
	VertexData v1(position[0]+width, position[1]+height, 0.0f, color[0], color[1], color[2], color[3], 0);
	VertexData v2(position[0]+width, position[1]-height, 0.0f, color[0], color[1], color[2], color[3], 0);
	VertexData v3(position[0]-width, position[1]-height, 0.0f, color[0], color[1], color[2], color[3], 0);
	VertexData v4(position[0]-width, position[1]+height, 0.0f, color[0], color[1], color[2], color[3], 0);
	std::vector<VertexData> vertices = { v1, v2, v2, v3, v3, v4, v4, v1 };
	// Write to CPU side buffer.
	m_linesVAO->writeData(vertices);
}

// Draw a filled quad.
void Base2DEngineGL::drawQuadFilled(float position[2], float width, float height, float color[4]) 
{
	width /= 2;
	height /= 2;
	VertexData v1(position[0] + width, position[1] + height, 0.0f, color[0], color[1], color[2], color[3], 0);
	VertexData v2(position[0] + width, position[1] - height, 0.0f, color[0], color[1], color[2], color[3], 0);
	VertexData v3(position[0] - width, position[1] - height, 0.0f, color[0], color[1], color[2], color[3], 0);
	VertexData v4(position[0] - width, position[1] + height, 0.0f, color[0], color[1], color[2], color[3], 0);
	std::vector<VertexData> vertices = { v1, v2, v3, v3, v4, v1 };
	// Write to CPU side buffer.
	m_trianglesVAO->writeData(vertices);
}

// Draws a clear circle.
void Base2DEngineGL::drawCircleClear(float coords[2], float radius, float color[4])
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
		VertexData v1(x1, y1, 0.0f, color[0], color[1], color[2], color[3], 0);
		VertexData v2(x2, y2, 0.0f, color[0], color[1], color[2], color[3], 0);
		std::vector<VertexData> verticesTemp = { v1, v2 };
		// Add line to total lines.
		vertices.insert(vertices.end(), verticesTemp.begin(), verticesTemp.end());
	}
	// Write to CPU side buffer.
	m_linesVAO->writeData(vertices);
}

// Draws a filled circle.
void Base2DEngineGL::drawCircleFilled(float coords[2], float radius, float color[4])
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
		VertexData v1(coords[0], coords[1], 0.0f, color[0], color[1], color[2], color[3], 0);
		VertexData v2(x1, y1, 0.0f, color[0], color[1], color[2], color[3], 0);
		VertexData v3(x2, y2, 0.0f, color[0], color[1], color[2], color[3], 0);
		std::vector<VertexData> verticesTemp = { v1, v2, v3 };
		// Add triangle to total triangles.
		vertices.insert(vertices.end(), verticesTemp.begin(), verticesTemp.end());
	}
	// Write to CPU side buffer.
	m_trianglesVAO->writeData(vertices);
}

// Adds text to the VBO object.
void Base2DEngineGL::drawText(std::string text, float coords[2], float color[4], float scale, std::string align)
{
	// Calculate the length & height of the string.
	float length = 0;
	float height = m_textRenderer->m_characterDictionary[text[0]].height;
	for (char c : text)
	{
		length += m_textRenderer->m_characterDictionary[c].xAdvance;
	}
	// Center the text.
	if (align == "C" || align == "c")
	{
		// Place the coords in the center of the text.
		coords[0] = coords[0] - (length * scale) / 2;
		// Place on top of coordinate.
		coords[1] = coords[1] + height * scale;
		// Write text to CPU side buffer.
		m_textRenderer->writeText(&m_texturedTrianglesVAO->m_VertexDataTexturedCPU, text, coords, color, scale);
	}
	// Right allign the text.
	else if (align == "R" || align == "r") 
	{
		// Align text to right.
		coords[0] = coords[0] - length * scale;
		// Place on top of coordinate.
		coords[1] = coords[1] + height * scale;
		// Write text to CPU side buffer.
		m_textRenderer->writeText(&m_texturedTrianglesVAO->m_VertexDataTexturedCPU, text, coords, color, scale);
	}
	// Left allign the text.
	else if (align == "L" || align == "l") 
	{
		// Place on top of the coordinate.
		coords[1] = coords[1] + height * scale;
		// Write text to CPU side buffer.
		m_textRenderer->writeText(&m_texturedTrianglesVAO->m_VertexDataTexturedCPU, text, coords, color, scale);
	}
	// Display error.
	else 
	{
		std::cout << "[INTERFACE][ERROR] '" << align << "' is not a valid alignment.\n\n";
	}	
}

// Draws the demo drawing.
void Base2DEngineGL::drawDemo(unsigned int loopCount)
{
	loopCount -= 1;
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
			VertexDataTextured v1(1.25f + i, 1.25f + k, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 2.0f, 0);
			VertexDataTextured v2(1.25f + i, 0.75f + k, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 2.0f, 0);
			VertexDataTextured v3(0.75f + i, 0.75f + k, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f, 0);
			VertexDataTextured v4(0.75f + i, 1.25f + k, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 2.0f, 0);
			std::vector<VertexDataTextured> verticesTex = { v1, v2, v3, v3, v4, v1 };
			m_texturedTrianglesVAO->writeData(verticesTex);

			// Test the text rendering.
			float pos[2] = { 0.5f + i, 0.5f + k };
			std::string text = "Testing font!";
			float colorText[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
			drawCircleFilled(pos, 0.01f, color);
			drawText(text, pos, colorText, 1.0f, "C");
		}
	}
	// Load the CPU buffers into the GPU.
	updateGPU();
}

//=============================================================================================================================================//
//  Buffer.																																	   //
//=============================================================================================================================================//

// Writes the CPU side buffers to the GPU.
void Base2DEngineGL::updateGPU()
{
	m_linesVAO->updateGPU();
	m_trianglesVAO->updateGPU();
	m_texturedTrianglesVAO->updateGPU();
}

//=============================================================================================================================================//
//  Camera.																																	   //
//=============================================================================================================================================//

// Automatically places the drawing in the center and scales it to fit into the window..
void Base2DEngineGL::autoCenter() 
{	
	// First reset the matrices.
	m_scalingMatrix = glm::mat4(1.0f);
	m_translationMatrix = glm::mat4(1.0f);
	m_rotationMatrix = glm::mat4(1.0f);
	// And reset the base matrices.
	m_scalingMatrixBase = glm::mat4(1.0f);
	m_translationMatrixBase = glm::mat4(1.0f);
	m_rotationMatrixBase = glm::mat4(1.0f);

	// Dimensions of the drawing.
	// These values are calculated in the following loops.
	float max[2] = { 0, 0 };
	float min[2] = { 0, 0 };

	// ---------- //
	//  L I N E S //
	// ---------- //

	// If lines is not empty.
	if (m_linesVAO->m_vertexDataCPU.size())
	{
		// Init min/max values.
		max[0] = m_linesVAO->m_vertexDataCPU[0].position[0];
		max[1] = m_linesVAO->m_vertexDataCPU[0].position[1];
		min[0] = m_linesVAO->m_vertexDataCPU[0].position[0];
		min[1] = m_linesVAO->m_vertexDataCPU[0].position[1];

		// Find the maximum and minimum values for x and y.
		for (int i = 1; i < m_linesVAO->m_vertexDataCPU.size(); i++)
		{
			// Check max for x.
			if (m_linesVAO->m_vertexDataCPU[i].position[0] > max[0])
			{
				max[0] = m_linesVAO->m_vertexDataCPU[i].position[0];
			}
			// Check min for x.
			else if (m_linesVAO->m_vertexDataCPU[i].position[0] < min[0])
			{
				min[0] = m_linesVAO->m_vertexDataCPU[i].position[0];
			}
			// Check max for y.
			if (m_linesVAO->m_vertexDataCPU[i].position[1] > max[1])
			{
				max[1] = m_linesVAO->m_vertexDataCPU[i].position[1];
			}
			// Check min for y.
			else if (m_linesVAO->m_vertexDataCPU[i].position[1] < min[1])
			{
				min[1] = m_linesVAO->m_vertexDataCPU[i].position[1];
			}
		}
	}
	// If lines is empty init with triangles.
	else if (m_trianglesVAO->m_vertexDataCPU.size())
	{
		// Init min/max values.
		max[0] = m_trianglesVAO->m_vertexDataCPU[0].position[0];
		max[1] = m_trianglesVAO->m_vertexDataCPU[0].position[1];
		min[0] = m_trianglesVAO->m_vertexDataCPU[0].position[0];
		min[1] = m_trianglesVAO->m_vertexDataCPU[0].position[1];
	}
	// If triangels is empty init with textured triangles.
	else if (m_texturedTrianglesVAO->m_VertexDataTexturedCPU.size())
	{
		// Init min/max values.
		max[0] = m_texturedTrianglesVAO->m_VertexDataTexturedCPU[0].position[0];
		max[1] = m_texturedTrianglesVAO->m_VertexDataTexturedCPU[0].position[1];
		min[0] = m_texturedTrianglesVAO->m_VertexDataTexturedCPU[0].position[0];
		min[1] = m_texturedTrianglesVAO->m_VertexDataTexturedCPU[0].position[1];
	}

	// ------------------ //
	//  T R I A N G L E S //
	// ------------------ //

	if (m_trianglesVAO->m_vertexDataCPU.size())
	{
		// Find the maximum and minimum values for x and y.
		for (int i = 0; i < m_trianglesVAO->m_vertexDataCPU.size(); i++)
		{
			// Check max for x.
			if (m_trianglesVAO->m_vertexDataCPU[i].position[0] > max[0])
			{
				max[0] = m_trianglesVAO->m_vertexDataCPU[i].position[0];
			}
			// Check min for x.
			else if (m_trianglesVAO->m_vertexDataCPU[i].position[0] < min[0])
			{
				min[0] = m_trianglesVAO->m_vertexDataCPU[i].position[0];
			}
			// Check max for y.
			if (m_trianglesVAO->m_vertexDataCPU[i].position[1] > max[1])
			{
				max[1] = m_trianglesVAO->m_vertexDataCPU[i].position[1];
			}
			// Check min for y.
			else if (m_trianglesVAO->m_vertexDataCPU[i].position[1] < min[1])
			{
				min[1] = m_trianglesVAO->m_vertexDataCPU[i].position[1];
			}
		}
	}

	// ------------------------------------- //
	//  T E X T U R E D   T R I A N G L E S  //
	// ------------------------------------- //

	if (m_texturedTrianglesVAO->m_VertexDataTexturedCPU.size())
	{
		// Find the maximum and minimum values for x and y.
		for (int i = 0; i < m_texturedTrianglesVAO->m_VertexDataTexturedCPU.size(); i++)
		{
			// Check max for x.
			if (m_texturedTrianglesVAO->m_VertexDataTexturedCPU[i].position[0] > max[0])
			{
				max[0] = m_texturedTrianglesVAO->m_VertexDataTexturedCPU[i].position[0];
			}
			// Check min for x.
			else if (m_texturedTrianglesVAO->m_VertexDataTexturedCPU[i].position[0] < min[0])
			{
				min[0] = m_texturedTrianglesVAO->m_VertexDataTexturedCPU[i].position[0];
			}
			// Check max for y.
			if (m_texturedTrianglesVAO->m_VertexDataTexturedCPU[i].position[1] > max[1])
			{
				max[1] = m_texturedTrianglesVAO->m_VertexDataTexturedCPU[i].position[1];
			}
			// Check min for y.
			else if (m_texturedTrianglesVAO->m_VertexDataTexturedCPU[i].position[1] < min[1])
			{
				min[1] = m_texturedTrianglesVAO->m_VertexDataTexturedCPU[i].position[1];
			}
		}
	}

	// ------------- //
	//  C E N T E R  //
	// ------------- //

	// Calculate the values to translate.
	float size[2] = { std::abs(max[0] - min[0]), std::abs(max[1] - min[1]) };
	float translate[2];
	translate[0] = -min[0] - (size[0] / 2);
	translate[1] = -min[1] - (size[1] / 2);

	// Now translate the camera accordingly.
	m_translationMatrix = glm::translate(m_translationMatrix, glm::vec3(translate[0], translate[1], 0.0f));
	// Add to base matrix.
	m_translationMatrixBase = glm::translate(m_translationMatrixBase, glm::vec3(translate[0], translate[1], 0.0f));

	// Scale the drawing according to the largest translation that took place (This gives us the new max value,
	// centered around (0.0).
	if (size[0] > size[1]) 
	{
		float scale = size[0]/2;
		m_scalingMatrix = glm::scale(m_scalingMatrix, glm::vec3(1 / scale, 1 / scale, 1.0f));
		// Update base matrix.
		m_scalingMatrixBase = glm::scale(m_scalingMatrixBase, glm::vec3(1 / scale, 1 / scale, 1.0f));
	}
	else 
	{
		float scale = size[1]/2;
		m_scalingMatrix = glm::scale(m_scalingMatrix, glm::vec3(1 / scale, 1 / scale, 1.0f));
		// Update base matrix.
		m_scalingMatrixBase = glm::scale(m_scalingMatrixBase, glm::vec3(1 / scale, 1 / scale, 1.0f));
	}
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//