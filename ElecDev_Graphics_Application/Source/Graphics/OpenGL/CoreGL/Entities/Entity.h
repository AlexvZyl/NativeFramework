#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <vector>
#include <glm.hpp>
#include "External/GLM/glm.hpp"
#include "External/Misc/ConsoleColor.h"

//=============================================================================================================================================//
//  Forward declerations.																													   //
//=============================================================================================================================================//

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

	// ------------------- //
	//  F U N C T I O N S  //
	// ------------------- //

	void functionNotImplemented(std::string functionName) 
	{ std::cout << red << "\n[OPENGL] [ERROR] : " << white << "'" << functionName << "' is not implemented for the entity type.\n"; }

	virtual void translate(glm::vec3 translation) 
	{
		for (VertexData& vertex : m_vertexData) 
		{
		}
	}
	virtual void rotate(glm::vec3 rotation) { functionNotImplemented(__FUNCTION__); };
	virtual void scale(glm::vec3 scaling) { functionNotImplemented(__FUNCTION__); };
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
