#include "Utilities/Memory/FreeList.h"
#include "OpenGL/Buffers/IndexBufferObject.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "glad/glad.h"
#include <assert.h>

IndexBufferObject::~IndexBufferObject()
{
	if (existsOnGPU())
		deleteBuffer();
}

void IndexBufferObject::createBuffer()
{
	assert(!existsOnGPU(), "IBO already exists on the GPU.");
	GLCall(glGenBuffers(1, &m_rendererID));
	m_existsOnGPU = true;
}

void IndexBufferObject::deleteBuffer()
{
	assert(existsOnGPU(), "IBO does not exist on the GPU.");
	GLCall(glDeleteBuffers(1, &m_rendererID));
	m_existsOnGPU = false;
}

bool IndexBufferObject::existsOnGPU()
{
	return m_existsOnGPU;
}

int IndexBufferObject::ID()
{
	return m_rendererID;
}

void IndexBufferObject::bind()
{
	assert(existsOnGPU(), "IBO does not exist on the GPU.");
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
}

void IndexBufferObject::unbind()
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBufferObject::bufferData(int size, const void* data, int usage)
{
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage));
	m_capacity = size;
}

void IndexBufferObject::namedBufferData(int size, const void* data, int usage)
{
	GLCall(glNamedBufferData(m_rendererID, size, data, usage));
	m_capacity = size;
}

void IndexBufferObject::bufferSubData(int offset, int size, const void* data)
{
	assert(offset + size < m_capacity, "Setting data out of range.");
	GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data));
}

void IndexBufferObject::namedBufferSubData(int offset, int size, const void* data)
{
	assert(offset + size < m_capacity, "Setting data out of range.");
	GLCall(glNamedBufferSubData(m_rendererID, offset, size, data));
}

int IndexBufferObject::capacity()
{
	return m_capacity;
}