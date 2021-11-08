#pragma once

/*
All entities have to be a child of this class.  The data contained in this class is all that is required 
for a VAO to be able to render the entity to the screen.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <vector>
#include <glm.hpp>

//=============================================================================================================================================//
//  Forward declerations.																													   //
//=============================================================================================================================================//

class VertexArrayObject;
struct VertexDataTextured;
struct VertexData;

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

class Entity 
{
public:

	// ------------------- //
	//  V A R I A B L E S  //
	// ------------------- //

	std::vector<VertexData> m_vertexData;					// The untextured vertex data used for the entity.
	std::vector<VertexDataTextured> m_vertexDataTextured;	// The textured vertex data used for the entity.
	unsigned int m_bufferStartIndex;						// The start position of the entity in the VAO.
	VertexArrayObject* m_VAO = NULL;						// Pointer to the VAO that the entity is drawn to.
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
