//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "vertexBuffer.h"

//----------------------------------------------------------------------------------------------------------------------
//  Vertex Buffer Class.
//----------------------------------------------------------------------------------------------------------------------

// Constructor.
VertexBuffer::VertexBuffer(const void* data, unsigned int size) 
{
	GLCall(glGenBuffers(1, &rendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

// Destructor.
VertexBuffer::~VertexBuffer() 
{
	GLCall(glDeleteBuffers(1, &rendererID));
}

// Bind the vertex bufer.
void VertexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
}

// Unbind the vertex buffer.
void VertexBuffer::unbind() const 
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------