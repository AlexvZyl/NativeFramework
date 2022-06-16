#pragma once

#include "BufferObjectsGL.h"

class VertexArrayObject
{
public:

	// Constructors.
	inline VertexArrayObject() = default;
	inline VertexArrayObject(int type) : m_type(type) { }
	// Destructor.
	inline ~VertexArrayObject() { if (m_existsOnGPU) destroy(); };

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
		VertexType::setVertexLayout(m_rendererID);
	}

	// Delete the array along with its buffers.
	void destroy();

	// Bind the array.
	void bind();

	// Unbind the array.
	void unbind();

	// Utilities.
	inline bool existsOnGPU() const		{ return m_existsOnGPU; }
	inline VertexBufferObject& getVBO() { return m_VBO; };
	inline IndexBufferObject& getIBO()	{ return m_IBO; }
	inline int getID() const			{ return m_rendererID; }
	inline int getType() const			{ return m_type; }	
	inline void setType(int type)		{ m_type = type; }

private:

	// Data.
	unsigned m_rendererID = NULL;
	int m_type = NULL;
	bool m_existsOnGPU = false;

	// For now each VAO can only have one of each.
	// Later support adding various buffers given layouts.
	VertexBufferObject m_VBO;
	IndexBufferObject m_IBO;
};