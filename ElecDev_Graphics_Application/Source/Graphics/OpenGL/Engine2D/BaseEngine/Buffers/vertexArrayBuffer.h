#pragma once

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include <ErrorHandler/errorHandler.h>

//----------------------------------------------------------------------------------------------------------------------
//  Data Structures.
//----------------------------------------------------------------------------------------------------------------------

enum class ModelType
{
	LINES, TRIANGLES, QUAD 
};

// Struct that contains the layout of the line vertex data.
struct LineVertexData
{
	float position[3];
	float color[4];
};

// Struct that contains the layout of the triangle vertex data.
struct TriVertexData
{
	float position[3];
	float color[4];
	unsigned int indeces[3];
};

//----------------------------------------------------------------------------------------------------------------------
//  Vertex Array Class.
//----------------------------------------------------------------------------------------------------------------------

class VertexArray
{
private:
	
	// VAO ID.
	unsigned int m_vAID;;
	// VBO ID.
	unsigned int m_vBID;

public:
	
	// Constructor.
	VertexArray(ModelType type, unsigned int bufferCount);

	// Destructor.
	~VertexArray();

	// Function that can dynamically write to the vertex buffer binded to this VAO.
	void writeVBO(LineVertexData vertices[2]);

	// Binds the VAO.
	void bind() const;
	// Unbinds the VAO.
	void unbind() const;

};

// Function by "The Cherno".  Do not really see the need for it at this moment.  Will maybe go back to it later.
// Add VBO to VAO.
//void addBuffer(VertexBuffer& vB, const VertexBufferLayout& vBL);

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------
