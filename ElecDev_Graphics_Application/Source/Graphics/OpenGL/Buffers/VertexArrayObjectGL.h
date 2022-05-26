#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "OpenGL/ErrorHandlerGL.h"
#include <glad/glad.h>
#include <vector>
#include "glm/glm.hpp"
#include <memory>
#include "Utilities/Memory/FreeList.h"

//=============================================================================================================================================//
//  Index Data.																																   //
//=============================================================================================================================================//

// Putting indices in a struct allows them to be used with FreeLists.

struct IndexData2
{
	// Constructors.
	inline IndexData2(int index0, int index1)
	{
		indices[0] = index0;
		indices[1] = index1;
	}
	inline IndexData2() = default;

	// Data.
	int indices[2] = { 0, 0 };
	inline int size() { return 2; }
};

struct IndexData3
{
	// Constructors.
	inline IndexData3(int index0, int index1, int index2) 
	{
		indices[0] = index0;
		indices[1] = index1;
		indices[2] = index2;
	}
	inline IndexData3() = default;

	// Data.
	int indices[3] = { 0, 0, 0 };
	inline int size() { return 3; }
};

struct IndexData4
{
	// Constructors.
	inline IndexData4(int index0, int index1, int index2, int index3)
	{
		indices[0] = index0;
		indices[1] = index1;
		indices[2] = index2;
		indices[3] = index3;
	}
	inline IndexData4() = default;

	// Data.
	int indices[4] = { 0, 0, 0, 0 };
	inline int size() { return 4; }
};


//=============================================================================================================================================//
//  VAO Interface.																															   //
//=============================================================================================================================================//

// In some aspects the VAO acts as a GPU version of a FreeList.
class IVertexArrayObject 
{ 
public:

	// Bind the VAO.
	inline void bind() const			
	{ 
		GLCall(glBindVertexArray(m_VAOID)); 
	}
	// Unbind the VAO.
	inline void unbind() const			
	{ 
		GLCall(glBindVertexArray(0)); 
	}
	// Get the type of the VAO.
	inline GLenum getBufferType() const 
	{ 
		return m_bufferType; 
	}
	
protected:

	// Constructor.
	IVertexArrayObject(GLenum type) : m_bufferType(type) {}

	// Methods.
	inline virtual bool onDrawCall() = 0;

	// Data.
	GLenum m_bufferType = 0;				// Data type used in this VAO.	
	unsigned m_VAOID = 0;					// Vertex Array Object ID.
	unsigned m_VBOID = 0;					// Vertex Buffer Objext ID.	
	unsigned m_IBOID = 0;					// Index Buffer Object ID.
	unsigned m_vertexCount = 0;				// The amount of vertices in this VAO.
	unsigned m_indexCount = 0;				// The amount of indices in this VAO.
	unsigned m_vertexBufferCapacity = 0;	// Capacity of the vertices buffer.
	unsigned m_indexBufferCapacity = 0;		// Capacity of the indices buffer.
	bool m_vertexBufferSynced = true;		// Keep track of if the CPU and GPU data is synced for the vertices.
	bool m_indexBufferSynced = true;		// Keep track of if the CPU and GPU data is synced for the iondices.
};

//=============================================================================================================================================//
//  VAO Templated Class.																													   //
//=============================================================================================================================================//

template <typename VertexType, typename IndexType>
class VertexArrayObject : public IVertexArrayObject
{	
public:

	// Constructor.
	inline VertexArrayObject(GLenum type) 
		: IVertexArrayObject(type)
	{
		// Generate the VAO.
		GLCall(glGenVertexArrays(1, &m_VAOID));
		GLCall(glBindVertexArray(m_VAOID));

		// Generate the IBO.
		GLCall(glGenBuffers(1, &m_IBOID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOID));

		// Generate the VBO.
		GLCall(glGenBuffers(1, &m_VBOID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));

		// Enable vertex attributes.
		VertexType::initVertexAttributes(m_VAOID);

		// Default value.
		setBufferIncrementSize(VERTEX_BUFFER_INCREMENTS);
	}

	// Destructor.
	inline ~VertexArrayObject()
	{
		// Clear GPU memory.
		GLCall(glDeleteBuffers(1, &m_VBOID));
		GLCall(glDeleteBuffers(1, &m_IBOID));
		GLCall(glDeleteVertexArrays(1, &m_VAOID));
	}

	// Push vertices and return their index in memory.
	inline int push(const std::vector<VertexType>& vertices) 
	{
		return push(vertices.data(), vertices.size());
	}

	// Push vertices and return their index in memory.
	inline int push(VertexType* vertexPtr, int size)
	{
		return m_vertexData.push(vertexPtr, size);
	}

	// Erase vertices.
	inline void erase(int index, int size) 
	{
		m_vertexData.erase(index, size);
	}

	// Push indices and return their index in memory.
	inline int push(const std::vector<IndexType>& indices) 
	{
		return push(indices.data(), indices.size());
	}

	// Push indices and return their index in memory.
	inline int push(IndexType* indices, int size)
	{
		return m_indexData.push(indices, size);
	}

	// Utilities.
	inline void setCapacityIncrements(int size)		{ m_vertexData.setCapacityIncrements(size); m_indexData.setCapacityIncrements(size); };
	inline int getCapacityIncrements()				{ return m_vertexData.getCapacityIncrements(); };	
	inline void setResizeThreshold(float threshold) { m_vertexData.setResizeThreshold(threshold); m_indexData.setResizeThreshold(threshold); }
	inline float getResizeThreshold()				{ return m_vertexData.getResizeThreshold(); }

	// Data.
	FreeList<VertexType> m_vertexData;
	FreeList<IndexType> m_indexData;

private:

	friend class Scene;
	friend class FrameBufferObject;
	friend class RendererStats;
	friend class Renderer;
	friend class BackgroundColorEditor;
	friend class Grid;

	// ----------------------------------- //
	//  M E M O R Y   M A N A G E M E N T  //
	// ----------------------------------- //

	// Resizes the buffers on the GPU.
	bool queryBufferResize();
	// Updates the data on the GPU.
	void syncPrimitives();
	// Update the index buffer data.
	void syncIndexBuffer();
	// Update the index buffer data.
	void syncVertexBuffer();
	// This function deletes the data on the CPU side and keeps the GPU side data.
	// This is useful when no more changes are going to be made and RAM should be reduced.
	void wipeCPU();
	// Wipes all of the data GPU side but keeps the CPU data.
	void wipeGPU();
	// Wipes all of the data (CPU and GPU).
	void wipeAll();
	// Checks if buffer should resize, sync and render.
	virtual bool onDrawCall() override;
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
