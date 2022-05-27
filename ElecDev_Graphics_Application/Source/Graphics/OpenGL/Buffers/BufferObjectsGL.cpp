
#include "OpenGL/Buffers/BufferObjectsGL.h"
#include "OpenGL/ErrorHandlerGL.h"
#include <assert.h>

BufferObject::BufferObject(int target)
{
	m_target = target;
}

BufferObject::~BufferObject()
{
	if (m_existsOnGPU) deleteBuffer();
}

void BufferObject::createBuffer()
{
	assert(!m_existsOnGPU); // Buffer already exists on the GPU.
	GLCall(glGenBuffers(1, &m_rendererID));
	m_existsOnGPU = true;
}

void BufferObject::deleteBuffer()
{
	assert(m_existsOnGPU); // Buffer does not exist on the GPU.
	GLCall(glDeleteBuffers(1, &m_rendererID));
	m_existsOnGPU = false;
}

int BufferObject::ID()
{
	return m_rendererID;
}

void BufferObject::bind()
{
	assert(m_existsOnGPU); // Buffer does not exist on the GPU.
	GLCall(glBindBuffer(m_target, m_rendererID));
}

void BufferObject::unbind()
{
	GLCall(glBindBuffer(m_target, 0));
}

void BufferObject::bufferData(int size, const void* data, int usage)
{
	GLCall(glBufferData(m_target, size, data, usage));
	m_capacity = size;
}

void BufferObject::namedBufferData(int size, const void* data, int usage)
{
	GLCall(glNamedBufferData(m_rendererID, size, data, usage));
	m_capacity = size;
}

void BufferObject::bufferSubData(int offset, int size, const void* data)
{
	assert(offset + size < m_capacity); // Setting data out of range.
	GLCall(glBufferSubData(m_target, offset, size, data));
}

void BufferObject::namedBufferSubData(int offset, int size, const void* data)
{
	assert(offset + size < m_capacity); // Setting data out of range.
	GLCall(glNamedBufferSubData(m_rendererID, offset, size, data));
}

int BufferObject::capacity()
{
	return m_capacity;
}