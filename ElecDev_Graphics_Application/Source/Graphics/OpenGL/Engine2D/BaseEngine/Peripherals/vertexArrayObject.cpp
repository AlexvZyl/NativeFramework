//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "vertexArrayObject.h"

//----------------------------------------------------------------------------------------------------------------------
//  Vertex Array Class.
//----------------------------------------------------------------------------------------------------------------------

// Constructor.
VertexArrayObject::VertexArrayObject(GLenum type, unsigned int maxVertices, bool textured)
	: m_bufferType(type), m_maxVertices(maxVertices)
{
	// Generate the VAO.
	GLCall(glCreateVertexArrays(1, &m_vAID));
	GLCall(glBindVertexArray(m_vAID));

	// Create untextured array.
	if (!textured) 
	{
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
	// Create textured array.
	else 
	{
		// Generate and bind a VBO for the VAO.
		GLCall(glCreateBuffers(1, &m_vBID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vBID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, m_maxVertices * sizeof(TexturedVertexData), NULL, GL_DYNAMIC_DRAW));

		// Bind Vertex position attribute.
		GLCall(glEnableVertexArrayAttrib(m_vBID, 0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertexData), (const void*)offsetof(TexturedVertexData, position)));
		// Bind Vertex color attribute.
		GLCall(glEnableVertexArrayAttrib(m_vBID, 1));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(TexturedVertexData), (const void*)offsetof(TexturedVertexData, color)));
		// Bind texture position attribute
		GLCall(glEnableVertexArrayAttrib(m_vBID, 2));
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertexData), (const void*)offsetof(TexturedVertexData, texturePosition)));
		// Bind texture ID attribute
		GLCall(glEnableVertexArrayAttrib(m_vBID, 3));
		GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(TexturedVertexData), (const void*)offsetof(TexturedVertexData, textureID)));
	}
}

// Destructor.
VertexArrayObject::~VertexArrayObject()
{
	// Delete VBO.
	GLCall( glDeleteBuffers(1, &m_vBID) )
	// Delete VAO.
	GLCall( glDeleteVertexArrays(1, &m_vAID) );
}

// Write untextured data.
void VertexArrayObject::writeData(std::vector<VertexData> vertices)
{
	for (VertexData& vertex : vertices) {
		// Write the data to the buffer.
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vBID));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, m_bufferPtr * sizeof(vertex), sizeof(vertex), vertex.rawData()));
		m_bufferPtr += 1;
	}
}
// Write textured data.
void VertexArrayObject::writeData(std::vector<TexturedVertexData> vertices)
{
	for (TexturedVertexData& vertex : vertices) {
		// Write the data to the buffer.
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vBID));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, m_bufferPtr * sizeof(vertex), sizeof(vertex), vertex.rawData()));
		m_bufferPtr += 1;
	}
}

// Function that draws the data in the VAO based on the model type selected in the constructor.
void VertexArrayObject::render()
{
	// Bind the VAO.
	bind();
	// Draw call.
	GLCall(glDrawArrays(m_bufferType, 0, m_bufferPtr));
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
