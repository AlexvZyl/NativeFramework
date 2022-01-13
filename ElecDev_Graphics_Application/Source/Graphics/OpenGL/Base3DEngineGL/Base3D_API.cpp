//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "CoreGL/Entities/Vertex.h"
#include "Base3D_Engine.h"
#include "../CoreGL/Buffers/VertexArrayObjectGL.h"
#include "Misc/ConsoleColor.h"
#include <iostream>

//=============================================================================================================================================//
//  Basic primitives.																														   //
//=============================================================================================================================================//

void Base3DEngineGL::drawQuadFilled3D(float position1[3], float position2[3], float position3[3], float position4[3], float color[4]) 
{
	//// Create the vertices.
	//Vertex* v1 = new VertexData(
	//	position1[0], position1[1], position1[2],
	//	color[0], color[1], color[2], color[3],
	//	0
	//);
	//// Create the vertices.
	//Vertex* v2 = new VertexData(
	//	position2[0], position2[1], position2[2],
	//	color[0], color[1], color[2], color[3],
	//	0
	//);
	//// Create the vertices.
	//Vertex* v3 = new VertexData(
	//	position3[0], position3[1], position3[2],
	//	color[0], color[1], color[2], color[3],
	//	0
	//);
	//// Create the vertices.
	//Vertex* v4 = new VertexData(
	//	position4[0], position4[1], position4[2],
	//	color[0], color[1], color[2], color[3],
	//	0
	//);
	//std::vector<Vertex*> vertices = {v1,v2,v3,v3,v4,v1};
	//m_trianglesVAO->appendVertexData(&vertices);
	//vertices.clear(), vertices.shrink_to_fit();
}

void Base3DEngineGL::drawCuboidFilled(float position1[3], float position2[3], float position3[3], float position4[3], float depth, float color[4]) 
{
	//// Create the vertices.
	//Vertex* v1 = new VertexData(
	//	position1[0], position1[1], position1[2],
	//	color[0], color[1], color[2], color[3],
	//	0
	//);
	//// Create the vertices.
	//Vertex* v2 = new VertexData(
	//	position2[0], position2[1], position2[2],
	//	color[0], color[1], color[2], color[3],
	//	0
	//);
	//// Create the vertices.
	//Vertex* v3 = new VertexData(
	//	position3[0], position3[1], position3[2],
	//	color[0], color[1], color[2], color[3],
	//	0
	//);
	//// Create the vertices.
	//Vertex* v4 = new VertexData(
	//	position4[0], position4[1], position4[2],
	//	color[0], color[1], color[2], color[3],
	//	0
	//);

	//// Calculate the depth vector.
	//glm::vec3 vector1 = glm::vec3(
	//	position1[0] - position2[0],
	//	position1[1] - position2[1],
	//	position1[2] - position2[2]);
	//glm::vec3 vector2 = glm::vec3(
	//	position3[0] - position2[0],
	//	position3[1] - position2[1],
	//	position3[2] - position2[2]);
	//glm::vec3 depthVector = depth * glm::normalize(glm::cross(vector1, vector2));

	//// Create depth vertices.
	//Vertex* v5 = new VertexData(
	//	position1[0] + depthVector[0], position1[1] + depthVector[1], position1[2] + depthVector[2],
	//	color[0], color[1], color[2], color[3],
	//	0
	//);
	//// Create the vertices.
	//Vertex* v6 = new VertexData(
	//	position2[0] + depthVector[0], position2[1] + depthVector[1], position2[2] + depthVector[2],
	//	color[0], color[1], color[2], color[3],
	//	0
	//);
	//// Create the vertices.
	//Vertex* v7 = new VertexData(
	//	position3[0] + depthVector[0], position3[1] + depthVector[1], position3[2] + depthVector[2],
	//	color[0], color[1], color[2], color[3],
	//	0
	//);
	//// Create the vertices.
	//Vertex* v8 = new VertexData(
	//	position4[0] + depthVector[0], position4[1] + depthVector[1], position4[2] + depthVector[2],
	//	color[0], color[1], color[2], color[3],
	//	0
	//);

	//// Push to vertex data.
	//std::vector<Vertex*> vertices = 
	//{
	//	v1, v2, v3, v3, v4, v1,
	//	v5, v6, v7, v7, v8, v5,
	//	v1, v2, v5, v5, v6, v2,
	//	v2, v3, v7, v7, v6, v2,
	//	v3, v7, v8, v8, v4, v3,
	//	v4, v8, v5, v5, v1, v4
	//};
	//m_trianglesVAO->appendVertexData(&vertices);
	//vertices.clear(); vertices.shrink_to_fit();
}

//=============================================================================================================================================//
//  Camera manipulation.																													   //
//=============================================================================================================================================//

// Automatically places the drawing in the center and scales it to fit into the window..
void Base3DEngineGL::autoCenter()
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

	////----------------------------------------------------------------------
	//// Lines.
	////----------------------------------------------------------------------

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

	////----------------------------------------------------------------------
	//// Triangles.
	////----------------------------------------------------------------------

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

	////----------------------------------------------------------------------
	//// Textured triangles.
	////----------------------------------------------------------------------

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

	////----------------------------------------------------------------------

	//// Calculate the values to translate.
	//float size[2] = { std::abs(max[0] - min[0]), std::abs(max[1] - min[1]) };
	//float translate[2];
	//translate[0] = -min[0] - (size[0] / 2);
	//translate[1] = -min[1] - (size[1] / 2);

	//// Now translate the camera accordingly.
	//m_translationMatrix = glm::translate(m_translationMatrix, glm::vec3(translate[0], translate[1], 0.0f));
	//// Add to base matrix.
	//m_translationMatrixBase = glm::translate(m_translationMatrixBase, glm::vec3(translate[0], translate[1], 0.0f));

	std::cout << red << "\n\n[OPENGL] [ERROR]: " << white << "Autocenter has been temporarily removed.\n";

}


//=============================================================================================================================================//
//  Testing.																																   //
//=============================================================================================================================================//

void Base3DEngineGL::drawDemo(unsigned int loopCount) 
{
	//loopCount -= 1;
	//for (int i = 0; i <= loopCount; i++)
	//{
	//	for (int j = 0; j <= loopCount; j++) 
	//	{
	//		for (int k = 0; k <= loopCount; k++)
	//		{

	//			float size = 0.25f;
	//			// 3D Cube bottom vertices.
	//			VertexData vb1(			// Top left.
	//				i - size, j + size, k - size,
	//				1.0f, 0.0f, 0.0f, 1.0f,
	//				i+j+k
	//			);
	//			VertexData vb2(			// Top right.
	//				i + size, j + size, k - size,
	//				0.0f, 1.0f, 0.0f, 1.0f,
	//				i + j + k
	//			);
	//			VertexData vb3(			// Bottom right.
	//				i + size, j - size, k - size,
	//				0.0f, 0.0f, 1.0f, 1.0f,
	//				i + j + k
	//			);
	//			VertexData vb4(			// Bottom left.
	//				i - size, j - size, k - size,
	//				1.0f, 1.0f, 0.0f, 1.0f,
	//				i + j + k
	//			);
	//			// 3D Cube top vertices.
	//			VertexData vt1(			// Top left.
	//				i - size, j + size, k + size,
	//				1.0f, 0.0f, 1.0f, 1.0f,
	//				i + j + k
	//			);
	//			VertexData vt2(			// Top right.
	//				i + size, j + size, k + size,
	//				0.0f, 1.0f, 1.0f, 1.0f,
	//				i + j + k
	//			);
	//			VertexData vt3(			// Bottom right.
	//				i + size, j - size, k + size,
	//				1.0f, 0.0f, 0.0f, 1.0f,
	//				i + j + k
	//			);
	//			VertexData vt4(			// Bottom left.
	//				i - size, j - size, k + size,
	//				0.0f, 1.0f, 0.0f, 1.0f,
	//				i + j + k
	//			);
	//			// Write the data.
	//			std::vector<VertexData> vertices = { vb1, vb2, vb3, vb3, vb4, vb1,
	//												vb1, vb2, vt2, vt2, vt1, vb1,
	//												vb3, vb2, vt2, vt2, vt3, vb3,
	//												vb4, vb3, vt3, vt3, vt4, vb4,
	//												vb4, vt4, vt1, vt1, vb1, vb4 };
	//			m_trianglesVAO->appendVertexData(vertices);

	//			// 3D Cube bottom vertices.
	//			VertexData vb1_l(			// Top left.
	//				i - size, j + size, k - size,
	//				0.0f, 0.0f, 0.0f, 1.0f,
	//				i + j + k
	//			);
	//			VertexData vb2_l(			// Top right.
	//				i + size, j + size, k - size,
	//				0.0f, 0.0f, 0.0f, 1.0f,
	//				i + j + k
	//			);
	//			VertexData vb3_l(			// Bottom right.
	//				i + size, j - size, k - size,
	//				0.0f, 0.0f, 0.0f, 1.0f,
	//				i + j + k
	//			);
	//			VertexData vb4_l(			// Bottom left.
	//				i - size, j - size, k - size,
	//				0.0f, 0.0f, 0.0f, 1.0f,
	//				i + j + k
	//			);
	//			// 3D Cube top vertices.
	//			VertexData vt1_l(			// Top left.
	//				i - size, j + size, k + size,
	//				0.0f, 0.0f, 0.0f, 1.0f,
	//				i + j + k
	//			);
	//			VertexData vt2_l(			// Top right.
	//				i + size, j + size, k + size,
	//				0.0f, 0.0f, 0.0f, 1.0f,
	//				i + j + k
	//			);
	//			VertexData vt3_l(			// Bottom right.
	//				i + size, j - size, k + size,
	//				0.0f, 0.0f, 0.0f, 1.0f,
	//				i + j + k
	//			);
	//			VertexData vt4_l(			// Bottom left.
	//				i - size, j - size, k + size,
	//				0.0f, 0.0f, 0.0f, 1.0f,
	//				i + j + k
	//			);
	//			// Write the data.
	//			std::vector<VertexData> vertices_l = { vb1_l, vb2_l, vb2_l, vb3_l, vb3_l, vb4_l, vb4_l, vb1_l, vb1_l, vb3_l,
	//													vt1_l, vt2_l, vt2_l, vt3_l, vt3_l, vt4_l, vt4_l, vt1_l,
	//													vb4_l, vt4_l, vb3_l, vt3_l, vb2_l, vt2_l, vb1_l, vt1_l };
	//			m_linesVAO->appendVertexData(vertices_l);


	//			// Render a texture to the cubes.
	//			// 3D Cube bottom vertices.
	//			VertexDataTextured tv1(			// Top left.
	//				i - size, j + size, k + size,
	//				1.0f, 0.0f, 0.0f, 1.0f,
	//				0.0f, 1.0f,
	//				0.0f,
	//				i + j + k
	//			);
	//			VertexDataTextured tv2(			// Top right.
	//				i + size, j + size, k + size,
	//				0.0f, 1.0f, 0.0f, 1.0f,
	//				1.0f, 1.0f,
	//				0.0f,
	//				i + j + k
	//			);
	//			VertexDataTextured tv3(			// Bottom right.
	//				i + size, j - size, k + size,
	//				0.0f, 0.0f, 1.0f, 1.0f,
	//				1.0f, 0.0f,
	//				0.0f,
	//				i + j + k
	//			);
	//			VertexDataTextured tv4(			// Bottom left.
	//				i - size, j - size, k + size,
	//				1.0f, 0.0f, 1.0f, 1.0f,
	//				0.0f, 0.0f,
	//				0.0f,
	//				i + j + k
	//			);	
	//			std::vector<VertexDataTextured> tVertices = {tv1, tv2, tv3, tv3, tv4, tv1};
	//			m_texturedTrianglesVAO->appendVertexData(tVertices);
	//		}
	//	}
	//}

	//updateGPU();
}
 
//=============================================================================================================================================//
// EOF.																																		   //
//=============================================================================================================================================//