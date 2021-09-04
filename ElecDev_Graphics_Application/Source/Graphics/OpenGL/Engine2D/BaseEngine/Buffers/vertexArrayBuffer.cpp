//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "vertexArrayBuffer.h"

//----------------------------------------------------------------------------------------------------------------------
//  Vertex Array Class.
//----------------------------------------------------------------------------------------------------------------------

// Constructor.
VertexArray::VertexArray(ModelType type, unsigned int bufferCount)
{
	// Generate the VAO.
	GLCall(glGenVertexArrays(1, &m_vAID));

	// Generate and bind a VBO for the VAO.
	GLCall(glGenBuffers(1, &m_vBID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vBID));
	// Allocate memory for the buffer based on the data type.
	if (type == ModelType::LINES) { GLCall(glBufferData(GL_ARRAY_BUFFER, bufferCount * sizeof(LineVertexData), NULL, GL_DYNAMIC_DRAW)); }
	if (type == ModelType::TRIANGLES) { GLCall(glBufferData(GL_ARRAY_BUFFER, bufferCount * sizeof(TriVertexData), NULL, GL_DYNAMIC_DRAW)); }
	
	// Bind attributes to the data.




}

// Desonstructor.
VertexArray::~VertexArray()
{
	// Delete VBO.
	GLCall( glDeleteBuffers(1, &m_vBID) )
	// Delete VAO.
	GLCall( glDeleteVertexArrays(1, &m_vAID) );
}

// Bind the VAO.
void VertexArray::bind() const 
{
	GLCall(glBindVertexArray(m_vAID));
}

// Unbind the VAO.
void VertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
}

// Function by "The Cherno".  Do not really see the need for it at this moment.  Will maybe go back to it later.
//// Add VBO to VAO.
//void VertexArray::addBuffer(VertexBuffer& vB, const VertexBufferLayout& vBL) 
//{
//	// Bind Vertex Array.
//	bind();
//	// Bind Vertex Buffer.
//	vB.bind();
//	// Setup layout.
//	const auto& elements = vBL.getElements();
//	unsigned int offset = 0;
//	for (unsigned int i = 0; i < elements.size(); i++) 
//	{
//		const auto& element = elements[i];
//		GLCall(glEnableVertexAttribArray(i));
//		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized,
//									 vBL.GetStride(), (const void*)offset));
//		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
//	}
//
//}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------
