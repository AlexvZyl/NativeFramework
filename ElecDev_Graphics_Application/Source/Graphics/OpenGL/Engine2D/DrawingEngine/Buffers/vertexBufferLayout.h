#pragma once

/* The header file for the implementation of the vertex buffer layouts.  This is a header only implementation.
   Part of abstracting the buffer functionality of OpenGL. */

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include <vector>

// Include OpenGL.
#include <glad/glad.h>

#include <ErrorHandler/errorHandler.h>

//----------------------------------------------------------------------------------------------------------------------
//  Structs.
//----------------------------------------------------------------------------------------------------------------------

struct VertexBufferElement 
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type) 
	{
		switch (type) 
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}
		ASSERT(false);
		return 0;
	}
};

//----------------------------------------------------------------------------------------------------------------------
//  Vertex Buffer Layout Class.
//----------------------------------------------------------------------------------------------------------------------

class VertexBufferLayout 
{
private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride;

public:

	// Construtor.
	VertexBufferLayout()
		: stride(0) {}
	
	template<typename T>
	void push(unsigned int count) 
	{
		static_assert(false);
	}

	template<>
	void push<float>(unsigned int count)
	{
		elements.push_back({ GL_FLOAT, count, GL_FALSE });
		stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void push<unsigned char>(unsigned int count)
	{
		elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement> getElements() const { return elements; }
	inline unsigned int GetStride() const { return stride; }

};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------