#include "Utilities/Memory/FreeList.h"
#include "OpenGL/Buffers/VertexBufferObject.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "glad/glad.h"
#include <assert.h>

VertexBufferObject::~VertexBufferObject()
{
	if (existsOnGPU())
		deleteBuffer();
}

void VertexBufferObject::createBuffer()
{
	assert(!existsOnGPU(), "VBO already exists on the GPU.");
	GLCall(glGenBuffers(1, &m_rendererID));
	m_existsOnGPU = true;
}

void VertexBufferObject::deleteBuffer()
{
	assert(existsOnGPU(), "VBO does not exist on the GPU.");
	GLCall(glDeleteBuffers(1, &m_rendererID));
	m_existsOnGPU = false;
}

bool VertexBufferObject::existsOnGPU()
{
	return m_existsOnGPU;
}

int VertexBufferObject::ID()
{
	return m_rendererID;
}

void VertexBufferObject::bind()
{
	assert(existsOnGPU(), "VBO does not exist on the GPU.");
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
}

void VertexBufferObject::unbind()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBufferObject::bufferData(int size, const void* data, int usage)
{
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
	m_capacity = size;
}

void VertexBufferObject::namedBufferData(int size, const void* data, int usage)
{
	GLCall(glNamedBufferData(m_rendererID, size, data, usage));
	m_capacity = size;
}

void VertexBufferObject::bufferSubData(int offset, int size, const void* data)
{
	assert(offset + size < m_capacity, "Setting data out of range.");
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}

void VertexBufferObject::namedBufferSubData(int offset, int size, const void* data)
{
	assert(offset + size < m_capacity, "Setting data out of range.");
	GLCall(glNamedBufferSubData(m_rendererID, offset, size, data));
}

int VertexBufferObject::capacity()
{
	return m_capacity;
}