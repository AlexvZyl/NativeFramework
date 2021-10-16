//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "base3DEngine.h"

//=============================================================================================================================================//
//  Functions.																																   //
//=============================================================================================================================================//

void Base3DEngineGL::drawDemo(unsigned int loopCount) 
{
	float size = 0.25f;
	// 3D Cube bottom vertices.
	TexturedVertexData vb1(			// Top left.
		-size, size, -size,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f,
		2.0f
	);
	TexturedVertexData vb2(			// Top right.
		size, size, -size,
		0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f,
		2.0f
	);
	TexturedVertexData vb3(			// Bottom right.
		size, -size, -size,
		0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f,
		2.0f
	);
	TexturedVertexData vb4(			// Bottom left.
		-size, -size, -size,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f,
		2.0f
	);
	// 3D Cube top vertices.
	TexturedVertexData vt1(			// Top left.
		-size, size, size,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f,
		2.0f
	);
	TexturedVertexData vt2(			// Top right.
		size, size, size,
		0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f,
		2.0f
	);
	TexturedVertexData vt3(			// Bottom right.
		size, -size, size,
		0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f,
		2.0f
	);
	TexturedVertexData vt4(			// Bottom left.
		-size, -size, size,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f,
		2.0f
	);

	// Write the data.
	std::vector<TexturedVertexData> vertices = {vb1, vb2, vb3, vb3, vb4, vb1, 
												vb1, vb2, vt2, vt2, vt1, vb1,
												vt1, vt2, vt3, vt3, vt4, vt1,
												vb3, vb2, vt2, vt2, vt3, vb3,
												vb4, vb3, vt3, vt3, vt4, vb4,
												vb4, vt4, vt1, vt1, vb1, vb4};
	m_textureTrianglesVAO->writeData(vertices);
}
 
//=============================================================================================================================================//
// EOF.																																		   //
//=============================================================================================================================================//