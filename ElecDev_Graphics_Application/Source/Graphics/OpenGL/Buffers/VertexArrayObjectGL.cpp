#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "glad/glad.h"
#include <assert.h>

VertexArrayObject::~VertexArrayObject()
{
	if(m_existsOnGPU) destroy();
}

void VertexArrayObject::create()
{
	assert(!m_existsOnGPU); // VAO already exists on the GPU.
	GLCall(glGenVertexArrays(1, &m_rendererID));
	m_existsOnGPU = true;

	// Create the default buffers.
	m_VBO.create();
	m_IBO.create();
}

void VertexArrayObject::destroy()
{
	assert(m_existsOnGPU); //  VAO does not exist on the GPU.
	m_VBO.destroy();
	m_IBO.destroy();
	GLCall(glDeleteVertexArrays(1, &m_rendererID));
	m_existsOnGPU = false;
}

void VertexArrayObject::bind() 
{
	assert(m_existsOnGPU);  // VAO does not exist on the GPU.
	GLCall(glBindVertexArray(m_rendererID));
}

void VertexArrayObject::unbind() 
{
	GLCall(glBindVertexArray(0));
}