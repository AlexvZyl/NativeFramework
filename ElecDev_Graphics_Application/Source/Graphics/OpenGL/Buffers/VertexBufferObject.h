#include "Utilities/Memory/FreeList.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "glad/glad.h"
#include <assert.h>

class VertexBufferObject 
{
	// Constructor should do nothing.
	inline VertexBufferObject() = default;

	// Destructor.
	~VertexBufferObject()
	{
		if(existsOnGPU())
			deleteBuffer();
	}

	// Create the buffer.
	inline void createBuffer()
	{ 
		assert(!existsOnGPU(), "VBO already exists on the GPU.");
		GLCall(glGenBuffers(1, &m_rendererID)); 
		m_existsOnGPU = true;
	}

	// Delete the buffer.
	inline void deleteBuffer() 
	{
		assert(existsOnGPU(), "VBO does not exist on the GPU.");
		GLCall(glDeleteBuffers(1, &m_rendererID));
		m_existsOnGPU = false;
	}

	// Check if the buffer exists on the GPU.
	inline bool existsOnGPU() 
	{
		return m_existsOnGPU;
	}

	// Get the VBO ID.
	inline int ID()
	{ 
		return m_rendererID; 
	}

	// Bind the VBO.
	inline void bind()
	{ 
		assert(existsOnGPU(), "VBO does not exist on the GPU.");
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
	}

	// Unbind the VBO.
	inline void unbind()
	{ 
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	// Set the data.
	inline void bufferData(int size, const void* data, int usage) 
	{
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
		m_capacity = size;
	}

	// Set the data by name.
	inline void namedBufferData(int size, const void* data, int usage)
	{
		GLCall(glNamedBufferData(m_rendererID, size, data, usage));
		m_capacity = size;
	}

	// Set the buffer sub data.
	inline void bufferSubData(int offset, int size, const void* data) 
	{
		assert(offset + size < m_capacity, "Setting data out of range.");
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
	}

	// Set the buffer sub data by name.
	inline void namedBufferSubData(int offset, int size, const void* data)
	{
		assert(offset + size < m_capacity, "Setting data out of range.");
		GLCall(glNamedBufferSubData(m_rendererID, offset, size, data));
	}

	// Get the capacity.
	inline int capacity() 
	{
		return m_capacity;
	}

private:

	unsigned m_rendererID = NULL;
	int m_capacity = NULL;
	bool m_existsOnGPU = false;
};