//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "indexBuffer.h"

//----------------------------------------------------------------------------------------------------------------------
//  Vertex Buffer Class.
//----------------------------------------------------------------------------------------------------------------------

// Constructor.
IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
{
	GLCall(glGenBuffers(1, &rendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

// Destructor.
IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &rendererID));
}

void IndexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
}

void IndexBuffer::unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------