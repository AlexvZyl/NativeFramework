//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "vertexArrayObject.h"

//----------------------------------------------------------------------------------------------------------------------
//  Vertex Array Class.
//----------------------------------------------------------------------------------------------------------------------

// Constructor.
VertexArrayObject::VertexArrayObject(DrawType type, unsigned int maxVertices)
	: m_drawType(type), m_maxVertices(maxVertices)
{
	// Generate the VAO.
	GLCall(glCreateVertexArrays(1, &m_vAID));
	GLCall(glBindVertexArray(m_vAID));

	// Generate and bind a VBO for the VAO.
	GLCall(glCreateBuffers(1, &m_vBID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vBID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, m_maxVertices * sizeof(VertexData), NULL, GL_DYNAMIC_DRAW));

	// Bind Vertex position attribute.
	GLCall(glEnableVertexArrayAttrib(m_vBID, 0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void*)offsetof(VertexData, position)));
	// Bind Vertex color attribute.
	GLCall(glEnableVertexArrayAttrib(m_vBID, 1));
	GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void*)offsetof(VertexData, color)));
}

// Destructor.
VertexArrayObject::~VertexArrayObject()
{
	// Delete VBO.
	GLCall( glDeleteBuffers(1, &m_vBID) )
	// Delete VAO.
	GLCall( glDeleteVertexArrays(1, &m_vAID) );
}

// Write 1 vertex data to the VBO.
void VertexArrayObject::writeData(VertexData v)
{
	// Write the data to the buffer.
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vBID));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, m_bufferPtr * sizeof(VertexData), sizeof(VertexData), v.rawData()));
	m_bufferPtr += 1;
}


// Write 2 vertices data to the VBO.
void VertexArrayObject::writeData(VertexData v1, VertexData v2)
{
	// Write the data to the buffer.
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vBID));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, m_bufferPtr*sizeof(VertexData), sizeof(VertexData), v1.rawData()));
	m_bufferPtr += 1;
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, m_bufferPtr*sizeof(VertexData), sizeof(VertexData), v2.rawData()));
	m_bufferPtr += 1;
}

// Write 3 vertices data to the VBO.
void VertexArrayObject::writeData(VertexData v1, VertexData v2, VertexData v3)
{
	// Write the data to the buffer.
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vBID));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, m_bufferPtr * sizeof(VertexData), sizeof(VertexData), v1.rawData()));
	m_bufferPtr += 1;
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, m_bufferPtr * sizeof(VertexData), sizeof(VertexData), v2.rawData()));
	m_bufferPtr += 1;
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, m_bufferPtr * sizeof(VertexData), sizeof(VertexData), v3.rawData()));
	m_bufferPtr += 1;
}

// Write 4 vertices data to the VBO.
void VertexArrayObject::writeData(VertexData v1, VertexData v2, VertexData v3, VertexData v4)
{
	// Write the data to the buffer.
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vBID));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, m_bufferPtr * sizeof(VertexData), sizeof(VertexData), v1.rawData()));
	m_bufferPtr += 1;
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, m_bufferPtr * sizeof(VertexData), sizeof(VertexData), v2.rawData()));
	m_bufferPtr += 1;
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, m_bufferPtr * sizeof(VertexData), sizeof(VertexData), v3.rawData()));
	m_bufferPtr += 1;
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, m_bufferPtr * sizeof(VertexData), sizeof(VertexData), v4.rawData()));
	m_bufferPtr += 1;
}

// Function that draws the data in the VAO based on the model type selected in the constructor.
void VertexArrayObject::render()
{
	// Bind the VAO.
	bind();
	// Draw call.
	GLCall(glDrawArrays((GLenum)m_drawType, 0, m_bufferPtr));
}

// Bind the VAO.
void VertexArrayObject::bind() const
{
	GLCall(glBindVertexArray(m_vAID));
}

// Unbind the VAO.
void VertexArrayObject::unbind() const
{
	GLCall(glBindVertexArray(0));
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------
