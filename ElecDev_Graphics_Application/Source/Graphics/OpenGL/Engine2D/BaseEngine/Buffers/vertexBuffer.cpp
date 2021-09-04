//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "vertexBuffer.h"

//----------------------------------------------------------------------------------------------------------------------
//  Vertex Buffer Class.
//----------------------------------------------------------------------------------------------------------------------

// Constructor.
VertexBuffer::VertexBuffer(unsigned int size) 
{
	GLCall(glGenBuffers(1, &m_rendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW));
}

// Destructor.
VertexBuffer::~VertexBuffer() 
{
	GLCall(glDeleteBuffers(1, &m_rendererID));
}

// Bind the vertex bufer.
void VertexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
}

// Unbind the vertex buffer.
void VertexBuffer::unbind() const 
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------