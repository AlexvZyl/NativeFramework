#pragma once

#include "glad/glad.h"
#include <stddef.h>

class BufferObject
{
public:

	// Constructor.
	inline BufferObject(int target) : m_target(target) { }
	// Destructor.
	inline ~BufferObject() { if (m_existsOnGPU) destroy(); }

	// OpenGL API.
	// All of the function descpritions can be found on 'docs.GL'.
	void bind();
	void unbind();
	void bufferData(int size, const void* data, int usage = GL_DYNAMIC_DRAW);
	void namedBufferData(int size, const void* data, int usage = GL_DYNAMIC_DRAW);
	void bufferSubData(int offset, int size, const void* data);
	void namedBufferSubData(int offset, int size, const void* data);
	void clearBufferData(int internalFormat, int format, int type, const void* data);
	void clearNamedBufferData(int internalFormat, int format, int type, const void* data);
	void clearBufferSubData(int internalFormat, int offset, int size, int format, int type, const void* data);
	void clearNamedBufferSubData(int internalFormat, int offset, int size, int format, int type, const void* data);

	// Extended OpenGL API.
	// These are custom functions that does not exist in the official API.
	void create();
	void destroy();
	void orphan();
	inline int getID()	   { return m_rendererID; }	
	inline int getTarget() { return m_target;     }	
	inline int capacity()  { return m_capacity;   }	

private:

	// Data.
	int m_target = NULL;
	unsigned m_rendererID = NULL;
	int m_capacity = NULL;
	bool m_existsOnGPU = false;
};

// Instantiations.

class VertexBufferObject : public BufferObject
{
public:
	VertexBufferObject() : BufferObject(GL_ARRAY_BUFFER) { }
};

class IndexBufferObject : public BufferObject
{
public:
	IndexBufferObject() : BufferObject(GL_ELEMENT_ARRAY_BUFFER) { }
};