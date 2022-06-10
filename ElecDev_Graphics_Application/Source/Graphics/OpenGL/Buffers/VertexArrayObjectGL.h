#pragma once

#include "BufferObjectsGL.h"

class VertexArrayObject
{
public:

	// Constructor.
	inline VertexArrayObject(int type) : m_type(type) { }
	inline VertexArrayObject() = default;

	// Destructor.
	~VertexArrayObject();

	// Create the array.
	// (For now this creates the default buffers with it).
	void create();

	// Create the array based on the vertex type.
	// (For now this creates the default buffers with it).
	template <typename VertexType>
	void setLayout()
	{
		// Bind the buffers.
		bind();
		m_VBO.bind();

		// Setup the vertex layout for the buffers.
		VertexType::initVertexAttributes(m_rendererID);
	}

	// Delete the array along with its buffers.
	void destroy();

	// Bind the array.
	void bind();

	// Unbind the array.
	void unbind();

	// Checks if the VAO exists on the GPU.
	inline bool existsOnGPU() { return m_existsOnGPU; }

	// Get the VBO.
	inline VertexBufferObject& getVBO() { return m_VBO; };

	// Get the IBO.
	inline IndexBufferObject& getIBO() { return m_IBO; }

	// Get the VAO type.
	inline int getType() { return m_type; }

	// Set the type of the VAO.
	inline void setType(int type) { m_type = type; }

private:

	// Data.
	unsigned m_rendererID = NULL;
	bool m_existsOnGPU = false;
	int m_type = NULL;

	// For now each VAO can only have one of each.
	// Layer support adding various buffers given layouts.
	VertexBufferObject m_VBO;
	IndexBufferObject m_IBO;
};