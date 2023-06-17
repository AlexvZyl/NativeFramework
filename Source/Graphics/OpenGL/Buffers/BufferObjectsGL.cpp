#include "OpenGL/Buffers/BufferObjectsGL.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "Utilities/Assert/Assert.h"

void BufferObject::create()
{
	LUMEN_DEBUG_ASSERT(!m_existsOnGPU, "Buffer already exists on the GPU.");

	GLCall(glGenBuffers(1, &m_rendererID));
	m_existsOnGPU = true;
	bind();
}

void BufferObject::orphan() 
{
	LUMEN_DEBUG_ASSERT(m_existsOnGPU, "Buffer does not exist on the GPU.");

	GLCall(glInvalidateBufferData(getID()));
}

void BufferObject::destroy()
{
	LUMEN_DEBUG_ASSERT(m_existsOnGPU, "Buffer does not exist on the GPU.");

	GLCall(glDeleteBuffers(1, &m_rendererID));
	m_existsOnGPU = false;
}

void BufferObject::bind()
{
	LUMEN_DEBUG_ASSERT(m_existsOnGPU, "Buffer does not exist on the GPU.");

	GLCall(glBindBuffer(getTarget(), m_rendererID));
}

void BufferObject::unbind()
{
	LUMEN_DEBUG_ASSERT(m_existsOnGPU, "Buffer does not exist on the GPU.");

	GLCall(glBindBuffer(getTarget(), 0));
}

void BufferObject::bufferData(int size, const void* data, int usage)
{
	LUMEN_DEBUG_ASSERT(m_existsOnGPU, "Buffer does not exist on the GPU.");

	GLCall(glBufferData(getTarget(), size, data, usage));
	m_capacity = size;
}

void BufferObject::namedBufferData(int size, const void* data, int usage)
{
	LUMEN_DEBUG_ASSERT(m_existsOnGPU, "Buffer does not exist on the GPU.");

	GLCall(glNamedBufferData(getID(), size, data, usage));
	m_capacity = size;
}

void BufferObject::bufferSubData(int offset, int size, const void* data)
{
	LUMEN_DEBUG_ASSERT(m_existsOnGPU, "Buffer does not exist on the GPU.");
	LUMEN_DEBUG_ASSERT(offset >= 0 && offset + size <= m_capacity, "Setting data out of range.");

	GLCall(glBufferSubData(getTarget(), offset, size, data));
}

void BufferObject::namedBufferSubData(int offset, int size, const void* data)
{
	LUMEN_DEBUG_ASSERT(m_existsOnGPU, "Buffer does not exist on the GPU.");
	LUMEN_DEBUG_ASSERT(offset >= 0 && offset + size <= m_capacity, "Setting data out of range.");

	GLCall(glNamedBufferSubData(getID(), offset, size, data));
}

void BufferObject::clearBufferData(int internalFormat, int format, int type, const void* data)
{
	LUMEN_DEBUG_ASSERT(m_existsOnGPU, "Buffer does not exist on the GPU.");

	GLCall(glClearBufferData(getTarget(), internalFormat, format, type, data));
}

void BufferObject::clearNamedBufferData(int internalFormat, int format, int type, const void* data)
{
	LUMEN_DEBUG_ASSERT(m_existsOnGPU, "Buffer does not exist on the GPU.");

	GLCall(glClearNamedBufferData(getID(), internalFormat, format, type, data));
}

void BufferObject::clearBufferSubData(int internalFormat, int offset, int size, int format, int type, const void* data)
{
	LUMEN_DEBUG_ASSERT(m_existsOnGPU, "Buffer does not exist on the GPU.");
	LUMEN_DEBUG_ASSERT(offset >= 0 && offset + size <= m_capacity, "Setting data out of range.");

	GLCall(glClearBufferSubData(getTarget(), internalFormat, offset, size, format, type, data));
}

void BufferObject::clearNamedBufferSubData(int internalFormat, int offset, int size, int format, int type, const void* data)
{
	LUMEN_DEBUG_ASSERT(m_existsOnGPU, "Buffer does not exist on the GPU.");
	LUMEN_DEBUG_ASSERT(offset >= 0 && offset + size <= m_capacity, "Setting data out of range.");

	GLCall(glClearNamedBufferSubData(getID(), internalFormat, offset, size, format, type, data));
}