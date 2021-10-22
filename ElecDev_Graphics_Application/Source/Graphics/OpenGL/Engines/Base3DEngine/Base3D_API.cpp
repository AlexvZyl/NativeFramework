//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Base3D_Engine.h"


//=============================================================================================================================================//
//  Basic primitives.																																   //
//=============================================================================================================================================//

void Base3DEngineGL::drawQuadFilled3D(float position1[3], float position2[3], float position3[3], float position4[3], float color[4]) 
{
	// Create the vertices.
	VertexData v1(
		position1[0], position1[1], position1[2],
		color[0], color[1], color[2], color[3]
	);
	// Create the vertices.
	VertexData v2(
		position2[0], position2[1], position2[2],
		color[0], color[1], color[2], color[3]
	);
	// Create the vertices.
	VertexData v3(
		position3[0], position3[1], position3[2],
		color[0], color[1], color[2], color[3]
	);
	// Create the vertices.
	VertexData v4(
		position4[0], position4[1], position4[2],
		color[0], color[1], color[2], color[3]
	);
	std::vector<VertexData> vertices = { v1,v2,v3,v3,v4,v1 };
	m_trianglesVAO->writeData(vertices);
}

void Base3DEngineGL::drawCuboidFilled(float position1[3], float position2[3], float position3[3], float position4[3], float depth, float color[4]) 
{
	// Create the vertices.
	VertexData v1(
		position1[0], position1[1], position1[2],
		color[0], color[1], color[2], color[3]
	);
	// Create the vertices.
	VertexData v2(
		position2[0], position2[1], position2[2],
		color[0], color[1], color[2], color[3]
	);
	// Create the vertices.
	VertexData v3(
		position3[0], position3[1], position3[2],
		color[0], color[1], color[2], color[3]
	);
	// Create the vertices.
	VertexData v4(
		position4[0], position4[1], position4[2],
		color[0], color[1], color[2], color[3]
	);

	// Calculate the depth vector.
	glm::vec3 vector1 = glm::vec3(
		position1[0] - position2[0],
		position1[1] - position2[1],
		position1[2] - position2[2]);
	glm::vec3 vector2 = glm::vec3(
		position3[0] - position2[0],
		position3[1] - position2[1],
		position3[2] - position2[2]);
	glm::vec3 depthVector = depth * glm::normalize(glm::cross(vector1, vector2));

	// Create depth vertices.
	VertexData v5(
		position1[0] + depthVector[0], position1[1] + depthVector[1], position1[2] + depthVector[2],
		color[0], color[1], color[2], color[3]
	);
	// Create the vertices.
	VertexData v6(
		position2[0] + depthVector[0], position2[1] + depthVector[1], position2[2] + depthVector[2],
		color[0], color[1], color[2], color[3]
	);
	// Create the vertices.
	VertexData v7(
		position3[0] + depthVector[0], position3[1] + depthVector[1], position3[2] + depthVector[2],
		color[0], color[1], color[2], color[3]
	);
	// Create the vertices.
	VertexData v8(
		position4[0] + depthVector[0], position4[1] + depthVector[1], position4[2] + depthVector[2],
		color[0], color[1], color[2], color[3]
	);

	std::vector<VertexData> vertices = {
		v1, v2, v3, v3, v4, v1,
		v5, v6, v7, v7, v8, v5,
		v1, v2, v5, v5, v6, v2,
		v2, v3, v7, v7, v6, v2,
		v3, v7, v8, v8, v4, v3,
		v4,v8, v5, v5, v1, v4
	};
	m_trianglesVAO->writeData(vertices);
}


//=============================================================================================================================================//
//  Testing.																																   //
//=============================================================================================================================================//

void Base3DEngineGL::drawDemo(unsigned int loopCount) 
{
	loopCount -= 1;
	for (int i = 0; i <= loopCount; i++)
	{
		for (int j = 0; j <= loopCount; j++) 
		{
			for (int k = 0; k <= loopCount; k++)
			{

				float size = 0.25f;
				// 3D Cube bottom vertices.
				VertexData vb1(			// Top left.
					i - size, j + size, k - size,
					1.0f, 0.0f, 0.0f, 1.0f
				);
				VertexData vb2(			// Top right.
					i + size, j + size, k - size,
					0.0f, 1.0f, 0.0f, 1.0f
				);
				VertexData vb3(			// Bottom right.
					i + size, j - size, k - size,
					0.0f, 0.0f, 1.0f, 1.0f
				);
				VertexData vb4(			// Bottom left.
					i - size, j - size, k - size,
					1.0f, 1.0f, 0.0f, 1.0f
				);
				// 3D Cube top vertices.
				VertexData vt1(			// Top left.
					i - size, j + size, k + size,
					1.0f, 0.0f, 1.0f, 1.0f
				);
				VertexData vt2(			// Top right.
					i + size, j + size, k + size,
					0.0f, 1.0f, 1.0f, 1.0f
				);
				VertexData vt3(			// Bottom right.
					i + size, j - size, k + size,
					1.0f, 0.0f, 0.0f, 1.0f
				);
				VertexData vt4(			// Bottom left.
					i - size, j - size, k + size,
					0.0f, 1.0f, 0.0f, 1.0f
				);
				// Write the data.
				std::vector<VertexData> vertices = { vb1, vb2, vb3, vb3, vb4, vb1,
													vb1, vb2, vt2, vt2, vt1, vb1,
													vb3, vb2, vt2, vt2, vt3, vb3,
													vb4, vb3, vt3, vt3, vt4, vb4,
													vb4, vt4, vt1, vt1, vb1, vb4 };
				m_trianglesVAO->writeData(vertices);

				// 3D Cube bottom vertices.
				VertexData vb1_l(			// Top left.
					i - size, j + size, k - size,
					0.0f, 0.0f, 0.0f, 1.0f
				);
				VertexData vb2_l(			// Top right.
					i + size, j + size, k - size,
					0.0f, 0.0f, 0.0f, 1.0f
				);
				VertexData vb3_l(			// Bottom right.
					i + size, j - size, k - size,
					0.0f, 0.0f, 0.0f, 1.0f
				);
				VertexData vb4_l(			// Bottom left.
					i - size, j - size, k - size,
					0.0f, 0.0f, 0.0f, 1.0f
				);
				// 3D Cube top vertices.
				VertexData vt1_l(			// Top left.
					i - size, j + size, k + size,
					0.0f, 0.0f, 0.0f, 1.0f
				);
				VertexData vt2_l(			// Top right.
					i + size, j + size, k + size,
					0.0f, 0.0f, 0.0f, 1.0f
				);
				VertexData vt3_l(			// Bottom right.
					i + size, j - size, k + size,
					0.0f, 0.0f, 0.0f, 1.0f
				);
				VertexData vt4_l(			// Bottom left.
					i - size, j - size, k + size,
					0.0f, 0.0f, 0.0f, 1.0f
				);
				// Write the data.
				std::vector<VertexData> vertices_l = { vb1_l, vb2_l, vb2_l, vb3_l, vb3_l, vb4_l, vb4_l, vb1_l, vb1_l, vb3_l,
														vt1_l, vt2_l, vt2_l, vt3_l, vt3_l, vt4_l, vt4_l, vt1_l,
														vb4_l, vt4_l, vb3_l, vt3_l, vb2_l, vt2_l, vb1_l, vt1_l };
				m_linesVAO->writeData(vertices_l);


				// Render a texture to the cubes.
				// 3D Cube bottom vertices.
				TexturedVertexData tv1(			// Top left.
					i - size, j + size, k + size,
					1.0f, 0.0f, 0.0f, 1.0f,
					0.0f, 1.0f,
					0.0f
				);
				TexturedVertexData tv2(			// Top right.
					i + size, j + size, k + size,
					0.0f, 1.0f, 0.0f, 1.0f,
					1.0f, 1.0f,
					0.0f
				);
				TexturedVertexData tv3(			// Bottom right.
					i + size, j - size, k + size,
					0.0f, 0.0f, 1.0f, 1.0f,
					1.0f, 0.0f,
					0.0f
				);
				TexturedVertexData tv4(			// Bottom left.
					i - size, j - size, k + size,
					1.0f, 0.0f, 1.0f, 1.0f,
					0.0f, 0.0f,
					0.0f
				);	
				std::vector<TexturedVertexData> tVertices = {tv1, tv2, tv3, tv3, tv4, tv1};
				m_textureTrianglesVAO->writeData(tVertices);
			}
		}
	}

	updateBuffers();
}
 
//=============================================================================================================================================//
// EOF.																																		   //
//=============================================================================================================================================//