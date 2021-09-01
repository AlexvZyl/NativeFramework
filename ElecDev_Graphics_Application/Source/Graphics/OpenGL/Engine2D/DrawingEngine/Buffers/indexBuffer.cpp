//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "indexBuffer.h"

//----------------------------------------------------------------------------------------------------------------------
//  Vertex Buffer Class.
//----------------------------------------------------------------------------------------------------------------------

// Default constructor.
IndexBuffer::IndexBuffer() {}

// Constructor.
IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int totalIndeces)
	: count(totalIndeces)
{
	GLCall(glGenBuffers(1, &rendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW)); // Assuming sizeof(unsigned int) sizeof(GLuint)!
}

// Destructor.
IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &rendererID));
}

void IndexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
}

void IndexBuffer::unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------