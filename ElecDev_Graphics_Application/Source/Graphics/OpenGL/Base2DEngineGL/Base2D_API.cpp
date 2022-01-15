/*
Engine API.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Base2D_Engine.h"
#include "../CoreGL/Buffers/VertexArrayObjectGL.h"
#include "CoreGL/Entities/Vertex.h"
#include "../CoreGL/Entities/Text.h"
#include <iostream>
#include "Misc/ConsoleColor.h"
#include "CoreGL/FontsGL.h"

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

void Base2DEngineGL::drawLine(float position1[2], float position2[2], float color[4])
{
	std::vector<VertexData> vertices =
	{
		VertexData(position1[0], position1[1], 0.0f, color[0], color[1], color[2], color[3], 0),
		VertexData(position2[0], position2[1], 0.0f, color[0], color[1], color[2], color[3], 0)
	};
	std::vector<unsigned> indices({ 0,1 });
	//m_linesVAO->appendVertexData(vertices, indices);
}

void Base2DEngineGL::drawTriangleClear(float position1[2], float position2[2], float position3[2], float color[4])
{
	std::vector<VertexData> vertices =
	{
		VertexData(position1[0], position1[1], 0.0f, color[0], color[1], color[2], color[3], 0),
		VertexData(position2[0], position2[1], 0.0f, color[0], color[1], color[2], color[3], 0),
		VertexData(position3[0], position3[1], 0.0f, color[0], color[1], color[2], color[3], 0)
	};
	std::vector<unsigned> indices({ 0, 1, 1, 2, 2, 0 });
	//m_linesVAO->appendVertexData(vertices, indices);
}

void Base2DEngineGL::drawTriangleFilled(float position1[2], float position2[2], float position3[2], float color[4])
{
	std::vector<VertexData> vertices =
	{
		VertexData(position1[0], position1[1], 0.0f, color[0], color[1], color[2], color[3], 0),
		VertexData(position2[0], position2[1], 0.0f, color[0], color[1], color[2], color[3], 0),
		VertexData(position3[0], position3[1], 0.0f, color[0], color[1], color[2], color[3], 0)
	};
	std::vector<unsigned> indices({ 0,1,2 });
	//m_trianglesVAO->appendVertexData(vertices, indices);
}

// Draw a clear quad.
void Base2DEngineGL::drawQuadClear(float position[2], float width, float height, float color[4])
{
	width /= 2;	height /= 2;
	std::vector<VertexData> vertices =
	{
		VertexData(position[0] + width, position[1] + height, 0.0f, color[0], color[1], color[2], color[3], 0),
		VertexData(position[0] + width, position[1] - height, 0.0f, color[0], color[1], color[2], color[3], 0),
		VertexData(position[0] - width, position[1] - height, 0.0f, color[0], color[1], color[2], color[3], 0),
		VertexData(position[0] - width, position[1] + height, 0.0f, color[0], color[1], color[2], color[3], 0)
	};
	std::vector<unsigned> indices({ 0,1,1,2,2,3,3,0 });
	//m_linesVAO->appendVertexData(vertices, indices);
}

// Draw a filled quad.
void Base2DEngineGL::drawQuadFilled(float position[2], float width, float height, float color[4])
{
	width /= 2;	height /= 2;
	std::vector<VertexData> vertices =
	{
		VertexData(position[0] + width, position[1] + height, 0.0f, color[0], color[1], color[2], color[3], 0),
		VertexData(position[0] + width, position[1] - height, 0.0f, color[0], color[1], color[2], color[3], 0),
		VertexData(position[0] - width, position[1] - height, 0.0f, color[0], color[1], color[2], color[3], 0),
		VertexData(position[0] - width, position[1] + height, 0.0f, color[0], color[1], color[2], color[3], 0)
	};
	std::vector<unsigned> indices({ 0,1,2,2,3,0 });
	//m_trianglesVAO->appendVertexData(vertices, indices);
}

// Draws a clear circle.
void Base2DEngineGL::drawCircleClear(float coords[2], float radius, float color[4])
{
	glm::vec4 colorGLM(color[0], color[1], color[2], color[3]);
	glm::vec3 pos1(coords[0] - radius, coords[1] + radius, 0.f);
	glm::vec3 pos2(coords[0] + radius, coords[1] + radius, 0.f);
	glm::vec3 pos3(coords[0] + radius, coords[1] - radius, 0.f);
	glm::vec3 pos4(coords[0] - radius, coords[1] - radius, 0.f);
	glm::vec2 local1(-1.f, +1.f);
	glm::vec2 local2(+1.f, +1.f);
	glm::vec2 local3(+1.f, -1.f);
	glm::vec2 local4(-1.f, -1.f);
	std::vector<VertexDataCircle> vertices =
	{
		VertexDataCircle(pos1, local1, colorGLM, 0.1f, 0.005f, 0),
		VertexDataCircle(pos2, local2, colorGLM, 0.1f, 0.005f, 0),
		VertexDataCircle(pos3, local3, colorGLM, 0.1f, 0.005f, 0),
		VertexDataCircle(pos4, local4, colorGLM, 0.1f, 0.005f, 0)
	};
	std::vector<unsigned> indices({ 0,1,2,2,3,0 });
	//m_circlesVAO->appendVertexData(vertices, indices);
}

// Draws a filled circle.
void Base2DEngineGL::drawCircleFilled(float coords[2], float radius, float color[4])
{
	glm::vec4 colorGLM(color[0], color[1], color[2], color[3]);
	glm::vec3 pos1(coords[0] - radius, coords[1] + radius, 0.f);
	glm::vec3 pos2(coords[0] + radius, coords[1] + radius, 0.f);
	glm::vec3 pos3(coords[0] + radius, coords[1] - radius, 0.f);
	glm::vec3 pos4(coords[0] - radius, coords[1] - radius, 0.f);
	glm::vec2 local1(-1.f, +1.f);
	glm::vec2 local2(+1.f, +1.f);
	glm::vec2 local3(+1.f, -1.f);
	glm::vec2 local4(-1.f, -1.f);
	std::vector<VertexDataCircle> vertices
	{
		VertexDataCircle(pos1, local1, colorGLM, 1.f, 0.005f, 0),
		VertexDataCircle(pos2, local2, colorGLM, 1.f, 0.005f, 0),
		VertexDataCircle(pos3, local3, colorGLM, 1.f, 0.005f, 0),
		VertexDataCircle(pos4, local4, colorGLM, 1.f, 0.005f, 0)
	};
	std::vector<unsigned> indices({ 0,1,2,2,3,0 });
	//m_circlesVAO->appendVertexData(vertices, indices);
}

// Adds text to the VBO object.
void Base2DEngineGL::drawText(std::string text, float coords[2], float color[4], float scale, std::string align)
{
	//// Convert color to GLM.
	//glm::vec4 colorGLM(color[0], color[1], color[2], color[3]);
	//// Calculate the length & height of the string.
	//float length = 0;
	//float height = m_defaultFont->characterDictionary[text[0]].height;
	//for (char c : text) { length += m_defaultFont->characterDictionary[c].xAdvance; }
	//// Center the text.
	//if (align == "C" || align == "c")
	//{
	//	// Place the coords in the center of the text.
	//	coords[0] = coords[0] - (length * scale) / 2;
	//	// Place on top of coordinate.
	//	coords[1] = coords[1] + height * scale;
	//}
	//// Right allign the text.
	//else if (align == "R" || align == "r")
	//{
	//	// Align text to right.
	//	coords[0] = coords[0] - length * scale;
	//	// Place on top of coordinate.
	//	coords[1] = coords[1] + height * scale;
	//}
	//// Left allign the text.
	//else if (align == "L" || align == "l")
	//{
	//	// Place on top of the coordinate.
	//	coords[1] = coords[1] + height * scale;
	//}
	//// Display error.
	//else
	//{
	//	std::cout << "[INTERFACE] [ERROR]: '" << align << "' is not a valid alignment.\n\n";
	//	return;
	//}
	//// Write text to CPU side buffer.
	//glm::vec3 texPos(coords[0], coords[1], 0.0f);
	//m_textEnities =
	//{
	//	m_textEnities,
	//	Text<VertexDataTextured>(text, &texPos, &colorGLM, scale, 0, m_texturedTrianglesVAO.get(), m_defaultFont.get()
	//};
	//m_textEnities.insert(m_textEnities.end(), ));
	//m_texturedTrianglesVAO->appendVertexData(&m_textEnities.back().m_vertices);
}

void Base2DEngineGL::drawDemo(unsigned int loopCount)
{
	loopCount -= 1;
	for (unsigned int i = 0; i <= loopCount * 3; i += 3)
	{
		for (unsigned int k = 0; k <= loopCount * 3; k += 3)
		{
			float ftPos1[2] = { -1.0f + i, -1.0f + k };
			float ftPos2[2] = { -1.0f + i, -0.5f + k };
			float ftPos3[2] = { -1.5f + i, -1.0f + k };
			float ftColor[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
			drawTriangleFilled(ftPos1, ftPos2, ftPos3, ftColor);
			float cqCoords[2] = { 0.0f + i, 0.0f + k };
			float cqColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
			drawQuadClear(cqCoords, 2.0f, 2.0f, cqColor);
			float fqCoords[2] = { -0.5f + i, 0.5f + k };
			float fqColor[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
			drawQuadFilled(fqCoords, 0.25f, 0.3f, fqColor);
			float coords1[2] = { 0.0f + i, 0.0f + k };
			float color[4] = { 1.0f, 0.6f, 0.0f, 1.0f };
			drawCircleFilled(coords1, 0.2f, color);
			float coords2[2] = { (float)i, -0.75f + k };
			drawCircleClear(coords2, 0.2f, color);
			float ctPos1[2] = { 1.0f + i, -1.0f + k };
			float ctPos2[2] = { 1.5f + i, -1.0f + k };
			float ctPos3[2] = { 1.0f + i, -0.5f + k };
			float ctColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
			drawTriangleClear(ctPos1, ctPos2, ctPos3, ctColor);
			std::vector<VertexDataTextured> vertices = {
				VertexDataTextured(1.25f + i, 1.25f + k, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 2.0f, 0),
				VertexDataTextured(1.25f + i, 0.75f + k, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 2.0f, 0),
				VertexDataTextured(0.75f + i, 0.75f + k, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f, 0),
				VertexDataTextured(0.75f + i, 1.25f + k, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 2.0f, 0)
			};;
			std::vector<unsigned> indices({ 0,1,2,2,3,0 });
			//m_texturedTrianglesVAO->appendVertexData(vertices, indices);
			float pos[2] = { 0.5f + i, 0.5f + k };
			std::string text = "Testing font!";
			float colorText[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
			drawCircleFilled(pos, 0.01f, color);
			drawText(text, pos, colorText, 1.0f, "C");
		}
	}
}

//=============================================================================================================================================//
//  Camera.																																	   //
//=============================================================================================================================================//

// Automatically places the drawing in the center and scales it to fit into the window..
void Base2DEngineGL::autoCenter()
{
	//// First reset the matrices.
	//m_scalingMatrix = glm::mat4(1.0f);
	//m_translationMatrix = glm::mat4(1.0f);
	//m_rotationMatrix = glm::mat4(1.0f);
	//// And reset the base matrices.
	//m_scalingMatrixBase = glm::mat4(1.0f);
	//m_translationMatrixBase = glm::mat4(1.0f);
	//m_rotationMatrixBase = glm::mat4(1.0f);

	//// Dimensions of the drawing.
	//// These values are calculated in the following loops.
	//float max[2] = { 0, 0 };
	//float min[2] = { 0, 0 };

	//// ---------- //
	////  L I N E S //
	//// ---------- //

	//// If lines is not empty.
	//if (m_linesVAO->m_vertexDataCPU.size())
	//{
	//	// Init min/max values.
	//	max[0] = m_linesVAO->m_vertexDataCPU[0].position[0];
	//	max[1] = m_linesVAO->m_vertexDataCPU[0].position[1];
	//	min[0] = m_linesVAO->m_vertexDataCPU[0].position[0];
	//	min[1] = m_linesVAO->m_vertexDataCPU[0].position[1];

	//	// Find the maximum and minimum values for x and y.
	//	for (int i = 1; i < m_linesVAO->m_vertexDataCPU.size(); i++)
	//	{
	//		// Check max for x.
	//		if (m_linesVAO->m_vertexDataCPU[i].position[0] > max[0])
	//		{
	//			max[0] = m_linesVAO->m_vertexDataCPU[i].position[0];
	//		}
	//		// Check min for x.
	//		else if (m_linesVAO->m_vertexDataCPU[i].position[0] < min[0])
	//		{
	//			min[0] = m_linesVAO->m_vertexDataCPU[i].position[0];
	//		}
	//		// Check max for y.
	//		if (m_linesVAO->m_vertexDataCPU[i].position[1] > max[1])
	//		{
	//			max[1] = m_linesVAO->m_vertexDataCPU[i].position[1];
	//		}
	//		// Check min for y.
	//		else if (m_linesVAO->m_vertexDataCPU[i].position[1] < min[1])
	//		{
	//			min[1] = m_linesVAO->m_vertexDataCPU[i].position[1];
	//		}
	//	}
	//}
	//// If lines is empty init with triangles.
	//else if (m_trianglesVAO->m_vertexDataCPU.size())
	//{
	//	// Init min/max values.
	//	max[0] = m_trianglesVAO->m_vertexDataCPU[0].position[0];
	//	max[1] = m_trianglesVAO->m_vertexDataCPU[0].position[1];
	//	min[0] = m_trianglesVAO->m_vertexDataCPU[0].position[0];
	//	min[1] = m_trianglesVAO->m_vertexDataCPU[0].position[1];
	//}
	//// If triangels is empty init with textured triangles.
	//else if (m_texturedTrianglesVAO->m_vertexDataTexturedCPU.size())
	//{
	//	// Init min/max values.
	//	max[0] = m_texturedTrianglesVAO->m_vertexDataTexturedCPU[0].position[0];
	//	max[1] = m_texturedTrianglesVAO->m_vertexDataTexturedCPU[0].position[1];
	//	min[0] = m_texturedTrianglesVAO->m_vertexDataTexturedCPU[0].position[0];
	//	min[1] = m_texturedTrianglesVAO->m_vertexDataTexturedCPU[0].position[1];
	//}

	//// ------------------ //
	////  T R I A N G L E S //
	//// ------------------ //

	//if (m_trianglesVAO->m_vertexDataCPU.size())
	//{
	//	// Find the maximum and minimum values for x and y.
	//	for (int i = 0; i < m_trianglesVAO->m_vertexDataCPU.size(); i++)
	//	{
	//		// Check max for x.
	//		if (m_trianglesVAO->m_vertexDataCPU[i].position[0] > max[0])
	//		{
	//			max[0] = m_trianglesVAO->m_vertexDataCPU[i].position[0];
	//		}
	//		// Check min for x.
	//		else if (m_trianglesVAO->m_vertexDataCPU[i].position[0] < min[0])
	//		{
	//			min[0] = m_trianglesVAO->m_vertexDataCPU[i].position[0];
	//		}
	//		// Check max for y.
	//		if (m_trianglesVAO->m_vertexDataCPU[i].position[1] > max[1])
	//		{
	//			max[1] = m_trianglesVAO->m_vertexDataCPU[i].position[1];
	//		}
	//		// Check min for y.
	//		else if (m_trianglesVAO->m_vertexDataCPU[i].position[1] < min[1])
	//		{
	//			min[1] = m_trianglesVAO->m_vertexDataCPU[i].position[1];
	//		}
	//	}
	//}

	//// ------------------------------------- //
	////  T E X T U R E D   T R I A N G L E S  //
	//// ------------------------------------- //

	//if (m_texturedTrianglesVAO->m_vertexDataTexturedCPU.size())
	//{
	//	// Find the maximum and minimum values for x and y.
	//	for (int i = 0; i < m_texturedTrianglesVAO->m_vertexDataTexturedCPU.size(); i++)
	//	{
	//		// Check max for x.
	//		if (m_texturedTrianglesVAO->m_vertexDataTexturedCPU[i].position[0] > max[0])
	//		{
	//			max[0] = m_texturedTrianglesVAO->m_vertexDataTexturedCPU[i].position[0];
	//		}
	//		// Check min for x.
	//		else if (m_texturedTrianglesVAO->m_vertexDataTexturedCPU[i].position[0] < min[0])
	//		{
	//			min[0] = m_texturedTrianglesVAO->m_vertexDataTexturedCPU[i].position[0];
	//		}
	//		// Check max for y.
	//		if (m_texturedTrianglesVAO->m_vertexDataTexturedCPU[i].position[1] > max[1])
	//		{
	//			max[1] = m_texturedTrianglesVAO->m_vertexDataTexturedCPU[i].position[1];
	//		}
	//		// Check min for y.
	//		else if (m_texturedTrianglesVAO->m_vertexDataTexturedCPU[i].position[1] < min[1])
	//		{
	//			min[1] = m_texturedTrianglesVAO->m_vertexDataTexturedCPU[i].position[1];
	//		}
	//	}
	//}

	//// ------------- //
	////  C E N T E R  //
	//// ------------- //

	//// Calculate the values to translate.
	//float size[2] = { std::abs(max[0] - min[0]), std::abs(max[1] - min[1]) };
	//float translate[2];
	//translate[0] = -min[0] - (size[0] / 2);
	//translate[1] = -min[1] - (size[1] / 2);

	//// Now translate the camera accordingly.
	//m_translationMatrix = glm::translate(m_translationMatrix, glm::vec3(translate[0], translate[1], 0.0f));
	//// Add to base matrix.
	//m_translationMatrixBase = glm::translate(m_translationMatrixBase, glm::vec3(translate[0], translate[1], 0.0f));

	//// Scale the drawing according to the largest translation that took place (This gives us the std::make_unique< max value,
	//// centered around (0.0).
	//if (size[0] > size[1]) 
	//{
	//	float scale = size[0]/2;
	//	m_scalingMatrix = glm::scale(m_scalingMatrix, glm::vec3(1 / scale, 1 / scale, 1.0f));
	//	// Update base matrix.
	//	m_scalingMatrixBase = glm::scale(m_scalingMatrixBase, glm::vec3(1 / scale, 1 / scale, 1.0f));
	//}
	//else 
	//{
	//	float scale = size[1]/2;
	//	m_scalingMatrix = glm::scale(m_scalingMatrix, glm::vec3(1 / scale, 1 / scale, 1.0f));
	//	// Update base matrix.
	//	m_scalingMatrixBase = glm::scale(m_scalingMatrixBase, glm::vec3(1 / scale, 1 / scale, 1.0f));
	//}
	std::cout << red << "\n\n[OPENGL] [ERROR]: " << white << "Autocenter has been temporarily removed.\n";
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//