#pragma once

/*
The parent class that contains all of the information that a vertex type can contain.
BufferLayout is used to setup the VAO.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <vector>
#include "GLM/glm.hpp"
#include "External/GLAD/Includes/glad/glad.h"
#include "OpenGL/ErrorHandlerGL.h"

//=============================================================================================================================================//
//  Vertex Specification.																													   //
//=============================================================================================================================================//

class IVertexData
{
public:
	// Returns a pointer to that data contained in the vertex.
	inline constexpr const void* data() const { return (const void*)this; }

protected:
	inline IVertexData() = default;
};

//=============================================================================================================================================//
//  Basic vertex. 																															   //
//=============================================================================================================================================//

class VertexData : public IVertexData
{
public:

	// Constructor.
	inline VertexData(const glm::vec3& position, const glm::vec4& color, unsigned entityID = 0, float outline = 0.f)
		: position(position), color(color), entityID(entityID), outline(outline)
	{ }

	// Set the VAO's vertex layout.
	inline static void setVertexLayout(unsigned VAO)
	{
		constexpr static int vertexSize = sizeof(VertexData);
		int offset = 0;

		// Position.
		GLCall(glEnableVertexArrayAttrib(VAO, 0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(glm::vec3);
		// Color.
		GLCall(glEnableVertexArrayAttrib(VAO, 1));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(glm::vec4);
		// Outline.
		GLCall(glEnableVertexArrayAttrib(VAO, 2));
		GLCall(glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(float);
		// Entity ID.
		GLCall(glEnableVertexArrayAttrib(VAO, 3));
		GLCall(glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, vertexSize, (const void*)offset));
	}

	// Data.
	glm::vec3 position;		
	glm::vec4 color;		
	float outline;
	unsigned entityID;
};

//=============================================================================================================================================//
//  Textured basic vertex. 																													   //
//=============================================================================================================================================//

class VertexDataTextured : public IVertexData
{
public:

	// Constructor.
	inline VertexDataTextured(const glm::vec3& position, const glm::vec4& color, const glm::vec2& textureCoords, float textureID, unsigned int entityID = 0, float outline = 0.f)
		: position(position), color(color), textureCoords(textureCoords), textureID(textureID), entityID(entityID), outline(outline)
	{ }

	// Set the VAO's vertex layout.
	inline static void setVertexLayout(unsigned VAO) 
	{
		constexpr static int vertexSize = sizeof(VertexDataTextured);
		int offset = 0;

		// Position.
		GLCall(glEnableVertexArrayAttrib(VAO, 0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(glm::vec3);
		// Color.
		GLCall(glEnableVertexArrayAttrib(VAO, 1));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(glm::vec4);
		// Texture coords.
		GLCall(glEnableVertexArrayAttrib(VAO, 2));
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(glm::vec2);
		// Outline value.
		GLCall(glEnableVertexArrayAttrib(VAO, 3));
		GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(float);
		// Texture ID.
		GLCall(glEnableVertexArrayAttrib(VAO, 4));
		GLCall(glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(float);
		// Entity ID.
		GLCall(glEnableVertexArrayAttrib(VAO, 5));
		GLCall(glVertexAttribIPointer(5, 1, GL_UNSIGNED_INT, vertexSize, (const void*)offset));
	}

	// Data.
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 textureCoords;
	float outline;
	float textureID;
	unsigned entityID;
};

//=============================================================================================================================================//
//  Circle Vertex Data																														   //
//=============================================================================================================================================//

class VertexDataCircle : public IVertexData
{
public:

	// Constructor.
	inline VertexDataCircle(const glm::vec3& position, const glm::vec2& localCoords, float radius, const glm::vec4& color, float thickness, float fade, unsigned int eID = 0, float outline = 0.f) 
		: position(position), color(color), radius(radius), localCoords(localCoords), outline(outline), thickness(thickness), fade(fade), entityID(entityID)
	{ }

	// Set the VAO's vertex layout.
	inline static void setVertexLayout(unsigned VAO)
	{
		static int vertexSize = sizeof(VertexDataCircle);
		int offset = 0;

		// Position.
		GLCall(glEnableVertexArrayAttrib(VAO, 0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(glm::vec3);
		// Color.
		GLCall(glEnableVertexArrayAttrib(VAO, 1));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(glm::vec4);
		// Radius.
		GLCall(glEnableVertexArrayAttrib(VAO, 2));
		GLCall(glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(float);
		// Local coordinates.
		GLCall(glEnableVertexArrayAttrib(VAO, 3));
		GLCall(glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(glm::vec2);
		// Outline value.
		GLCall(glEnableVertexArrayAttrib(VAO, 4));
		GLCall(glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(float);
		// Circle thickness.
		GLCall(glEnableVertexArrayAttrib(VAO, 5));
		GLCall(glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(float);
		// Circle fade.
		GLCall(glEnableVertexArrayAttrib(VAO, 6));
		GLCall(glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(float);
		// Entity ID.
		GLCall(glEnableVertexArrayAttrib(VAO, 7));
		GLCall(glVertexAttribIPointer(7, 1, GL_UNSIGNED_INT, vertexSize, (const void*)offset));
	}

	// Data.
	glm::vec3 position;
	glm::vec4 color;
	float radius;
	glm::vec2 localCoords;
	float outline;
	float thickness;
	float fade;
	unsigned entityID;
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
