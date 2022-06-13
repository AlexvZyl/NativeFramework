#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "Utilities/Assert/Assert.h"
#include "glad/glad.h"

void VertexArrayObject::create()
{
	LUMEN_DEBUG_ASSERT(!m_existsOnGPU, "VAO already exists on the GPU.");

	GLCall(glGenVertexArrays(1, &m_rendererID));
	m_existsOnGPU = true;
	bind();

	// Create the default buffers for now.
	m_VBO.create();
	m_IBO.create();
}

void VertexArrayObject::destroy()
{
	LUMEN_DEBUG_ASSERT(m_existsOnGPU, "VAO does not exist on the GPU.");

	// Destroy default buffers for now.
	m_VBO.destroy();
	m_IBO.destroy();
	GLCall(glDeleteVertexArrays(1, &m_rendererID));
	m_existsOnGPU = false;
}

void VertexArrayObject::bind() 
{
	LUMEN_DEBUG_ASSERT(m_existsOnGPU, "VAO does not exist on the GPU.");

	GLCall(glBindVertexArray(m_rendererID));
}

void VertexArrayObject::unbind() 
{
	LUMEN_DEBUG_ASSERT(m_existsOnGPU, "VAO does not exist on the GPU.");

	GLCall(glBindVertexArray(0));
}