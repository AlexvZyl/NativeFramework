//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Base3D_Engine.h"

//=============================================================================================================================================//
//  Functions.																																   //
//=============================================================================================================================================//

void Base3DEngineGL::drawDemo(unsigned int loopCount) 
{
	for (int i = 0; i <= loopCount; i++)
	{
		for (int j = 0; j <= loopCount; j++) 
		{
			for (int k = 0; k <= loopCount; k++)
			{

				float size = 0.25f;
				// 3D Cube bottom vertices.
				VertexData vb1(			// Top left.
					i-size, j+size, k-size,
					1.0f, 0.0f, 0.0f, 1.0f
				);
				VertexData vb2(			// Top right.
					i+size, j+size, k-size,
					0.0f, 1.0f, 0.0f, 1.0f
				);
				VertexData vb3(			// Bottom right.
					i+size, j-size, k-size,
					0.0f, 0.0f, 1.0f, 1.0f
				);
				VertexData vb4(			// Bottom left.
					i-size, j-size, k-size,
					1.0f, 1.0f, 0.0f, 1.0f
				);
				// 3D Cube top vertices.
				VertexData vt1(			// Top left.
					i-size, j+size, k+size,
					1.0f, 0.0f, 1.0f, 1.0f
				);
				VertexData vt2(			// Top right.
					i+size, j+size, k+size,
					0.0f, 1.0f, 1.0f, 1.0f
				);
				VertexData vt3(			// Bottom right.
					i+size, j-size, k+size,
					1.0f, 0.0f, 0.0f, 1.0f
				);
				VertexData vt4(			// Bottom left.
					i-size, j-size, k+size,
					0.0f, 1.0f, 0.0f, 1.0f
				);
				// Write the data.
				std::vector<VertexData> vertices = { vb1, vb2, vb3, vb3, vb4, vb1,
													vb1, vb2, vt2, vt2, vt1, vb1,
													vt1, vt2, vt3, vt3, vt4, vt1,
													vb3, vb2, vt2, vt2, vt3, vb3,
													vb4, vb3, vt3, vt3, vt4, vb4,
													vb4, vt4, vt1, vt1, vb1, vb4 };
				m_trianglesCPU.insert(m_trianglesCPU.end(), vertices.begin(), vertices.end());

				// 3D Cube bottom vertices.
				VertexData vb1_l(			// Top left.
					i-size, j+size, k-size,
					0.0f, 0.0f, 0.0f, 1.0f
				);
				VertexData vb2_l(			// Top right.
					i+size, j+size, k-size,
					0.0f, 0.0f, 0.0f, 1.0f
				);
				VertexData vb3_l(			// Bottom right.
					i+size, j-size, k-size,
					0.0f, 0.0f, 0.0f, 1.0f
				);
				VertexData vb4_l(			// Bottom left.
					i-size, j-size, k-size,
					0.0f, 0.0f, 0.0f, 1.0f
				);
				// 3D Cube top vertices.
				VertexData vt1_l(			// Top left.
					i-size, j+size, k+size,
					0.0f, 0.0f, 0.0f, 1.0f
				);
				VertexData vt2_l(			// Top right.
					i+size, j+size, k+size,
					0.0f, 0.0f, 0.0f, 1.0f
				);
				VertexData vt3_l(			// Bottom right.
					i+size, j-size, k+size,
					0.0f, 0.0f, 0.0f, 1.0f
				);
				VertexData vt4_l(			// Bottom left.
					i-size, j-size, k+size,
					0.0f, 0.0f, 0.0f, 1.0f
				);
				// Write the data.
				std::vector<VertexData> vertices_l = { vb1_l, vb2_l, vb2_l, vb3_l, vb3_l, vb4_l, vb4_l, vb1_l, vb1_l, vb3_l,
														vt1_l, vt2_l, vt2_l, vt3_l, vt3_l, vt4_l, vt4_l, vt1_l, vt1_l, vt3_l,
														vb4_l, vt4_l, vb3_l, vt3_l, vb2_l, vt2_l, vb1_l, vt1_l };
				//m_linesCPU.insert(m_linesCPU.end(), vertices_l.begin(), vertices_l.end());
			}
		}
	}
	updateBuffers();
}
 
//=============================================================================================================================================//
// EOF.																																		   //
//=============================================================================================================================================//