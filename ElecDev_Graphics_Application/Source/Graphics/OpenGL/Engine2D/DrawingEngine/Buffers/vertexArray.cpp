//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "vertexArray.h"
#include <ErrorHandler/errorHandler.h>

//----------------------------------------------------------------------------------------------------------------------
//  Vertex Array Class.
//----------------------------------------------------------------------------------------------------------------------

// Constructor.
VertexArray::VertexArray() 
{
	GLCall(glGenVertexArrays(1, &rendererID));
}

// Desonstructor.
VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &rendererID));
}

// Add VBO to VAO.
void VertexArray::addBuffer(VertexBuffer& vB, const VertexBufferLayout& vBL) 
{
	// Bind Vertex Array.
	bind();
	// Bind Vertex Buffer.
	vB.bind();
	// Setup layout.
	const auto& elements = vBL.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) 
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized,
									 vBL.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}

}

void VertexArray::bind() const 
{
	GLCall(glBindVertexArray(rendererID));
}


void VertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------
