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
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;

public:

	// Construtor.
	VertexBufferLayout()
		: m_stride(0) {}
	
	template<typename T>
	void push(unsigned int count) 
	{
		static_assert(false);
	}

	template<>
	void push<float>(unsigned int count)
	{
		m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void push<unsigned char>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement> getElements() const { return m_elements; }
	inline unsigned int GetStride() const { return m_stride; }

};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------