#pragma once

/*
The parent class that contains all of the information that a vertex type can contain.
BufferLayout is used to setup the VAO.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "GLAD/glad.h"
#include <vector>
#include "GLM/glm.hpp"

//=============================================================================================================================================//
//  Struct layout.																															   //
//=============================================================================================================================================//

// Used to describe the layout of the VAO for the vertex type.
struct BufferLayout
{
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	const GLvoid* pointer;
};

// This is required to setup te VAO's.
//std::vector<BufferLayout> bufferLayout;

//=============================================================================================================================================//
//  Vertex Specification.																													   //
//=============================================================================================================================================//

struct Vertex
{
	// Common vertex attributes.
	glm::vec3 position = { 0.f,0.f,0.f };		// Position of the vertex in 3D space.
	glm::vec4 color = { 0.f,0.f,0.f,0.f };		// Color of the vertex.
	unsigned int entityID = 0;					// ID associated with the vertex.
	int totalSize = 0;							// All of the data.
	int dataSize = 0;							// All of the color texture data.
	int idOffset = 0;							// Offset to the entity ID.
	int idSize = sizeof(entityID);
	// Formats the data so that OpenGL can use it.
	virtual const void* dataGL() { return (const void*)NULL; }
	// Returns the ID in a format that OpenGL can use.
	const void* idGL() { return (const void*)&entityID; }
};

//=============================================================================================================================================//
//  Basic vertex. 																															   //
//=============================================================================================================================================//

struct VertexData : public Vertex
{
	float rawData[7] = { 0.f,0.f,0.f,0.f,0.f,0.f,0.f };

	// Constructor.
	VertexData() { setup(); }
	VertexData(glm::vec3 pos, glm::vec4 clr, unsigned int eID)
	{
		position = pos; color = clr; entityID = eID;
		setup();
	}
	VertexData(float pos0, float pos1, float pos2,
		float col0, float col1, float col2, float col3,
		unsigned int eID)
	{
		// Assign position.
		position.x = pos0; position.y = pos1; position.z = pos2;
		// Assign color.
		color.r = col0;
		color.g = col1;
		color.b = col2;
		color.a = col3;
		// Assign ID.
		entityID = eID;
		setup();
	}

	void setup() 
	{
		dataSize = idOffset = sizeof(position) + sizeof(color);
		totalSize = dataSize + sizeof(entityID);
	}

	virtual const void* dataGL() override
	{
		rawData[0] = position.x; rawData[1] = position.y; rawData[2] = position.z;
		rawData[3] = color.r; rawData[4] = color.g; rawData[5] = color.b; rawData[6] = color.a;
		return (const void*) rawData;
	}
};

//=============================================================================================================================================//
//  Textured basic vertex. 																													   //
//=============================================================================================================================================//

struct VertexDataTextured : public Vertex
{
	glm::vec2 textureCoords = { 0.f,0.f };
	float textureID = 0;
	float rawData[10] = { 0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f };

	// Constructor.
	VertexDataTextured() { setup(); }
	VertexDataTextured(glm::vec3 pos, glm::vec4 clr, glm::vec2 texCoords, float texID, unsigned int eID)
	{
		position = pos; color = clr; entityID = eID; textureCoords = texCoords; textureID = texID; entityID = eID;
		setup();
	}
	VertexDataTextured(float pos0, float pos1, float pos2,
		float col0, float col1, float col2, float col3,
		float texPos0, float texPos1, float texID,
		unsigned int eID)
	{
		// Assign position.
		position.x = pos0; position.y = pos1; position.z = pos2;
		// Assign color.
		color.r = col0; color.g = col1; color.b = col2; color.a = col3;
		// Assign entity ID.
		entityID = eID;
		// Assign texture ID position.
		textureCoords.x = texPos0; textureCoords.y = texPos1;
		// Assigne texture ID.
		textureID = (unsigned int)texID;
		setup();
	}

	void setup() 
	{
		dataSize = idOffset = sizeof(position) + sizeof(color) + sizeof(textureCoords) + sizeof(textureID);
		totalSize = dataSize + sizeof(entityID);
	}

	// Return the raw data for OpenGL to use.
	virtual const void* dataGL() override
	{
		rawData[0] = position.x; rawData[1] = position.y; rawData[2] = position.z;
		rawData[3] = color.r; rawData[4] = color.g; rawData[5] = color.b; rawData[6] = color.a;
		rawData[7] = textureCoords.x; rawData[8] = textureCoords.y; rawData[9] = textureID;
		return (const void*)rawData;
	}
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
