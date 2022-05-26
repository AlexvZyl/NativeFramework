#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "OpenGL/ErrorHandlerGL.h"
#include "OpenGL/Buffers/IndexBufferObject.h"
#include "OpenGL/Buffers/VertexBufferObject.h"
#include <glad/glad.h>
#include <vector>
#include "glm/glm.hpp"
#include <memory>
#include "Utilities/Memory/FreeList.h"

//=============================================================================================================================================//
//  Index Data.																																   //
//=============================================================================================================================================//

// Putting indices in a struct allows them to be used with FreeLists.
// Currently IndexData2 cannot be put in a freelist, it is too small.

struct IndexData2
{
	inline IndexData2(int index0, int index1)
	{
		indices[0] = index0;
		indices[1] = index1;
	}
	inline IndexData2() = default;
	int indices[2] = { 0, 0 };
	inline int* data() { return &indices[0]; }
	static inline int size() { return 2; }
};

struct IndexData3
{
	inline IndexData3(int index0, int index1, int index2) 
	{
		indices[0] = index0;
		indices[1] = index1;
		indices[2] = index2;
	}
	inline IndexData3() = default;
	int indices[3] = { 0, 0, 0 };
	inline int* data() { return &indices[0]; }
	static inline int size() { return 3; }
};

struct IndexData4
{
	inline IndexData4(int index0, int index1, int index2, int index3)
	{
		indices[0] = index0;
		indices[1] = index1;
		indices[2] = index2;
		indices[3] = index3;
	}
	inline IndexData4() = default;
	int indices[4] = { 0, 0, 0, 0 };
	inline int* data() { return &indices[0]; }
	static inline int size() { return 4; }
};

//=============================================================================================================================================//
//  VAO Interface.																															   //
//=============================================================================================================================================//

class IVertexArrayObject 
{ 
public:

	// Create the VAO.
	inline void createArray() 
	{
		assert(!existsOnGPU(), "VAO already exists on the GPU.");
		GLCall(glGenVertexArrays(1, &m_VAOID));
		m_existsOnGPU = true;
	}

	// Delete the VAO.
	inline void deleteArray() 
	{
		assert(existsOnGPU(), "VAO does not exist on the GPU.");
		GLCall(glDeleteVertexArrays(1, &m_VAOID));
		m_VBO.deleteBuffer();
		m_IBO.deleteBuffer();
		m_existsOnGPU = false;
	}

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

	// Does the VAO exists on the GPU?
	// (Has it been created?)
	inline bool existsOnGPU() 
	{
		return m_existsOnGPU;
	}
	
protected:

	// Constructor.
	IVertexArrayObject(GLenum type) : m_bufferType(type) {}

	// Methods.
	inline virtual bool onDrawCall() = 0;

	// Data.
	VertexBufferObject m_VBO;			// The buffer containing the vertices.
	IndexBufferObject m_IBO;			// The buffer containing the indices.
	GLenum m_bufferType = 0;			// Data type used in this VAO.	
	unsigned m_VAOID = 0;				// Vertex Array Object ID.
	bool m_vertexBufferSynced = true;	// Keep track of if the CPU and GPU data is synced for the vertices.
	bool m_indexBufferSynced = true;	// Keep track of if the CPU and GPU data is synced for the iondices.
	bool m_existsOnGPU = false;			// Has the VAO been created?
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
		// VAO setup.
		createArray();
		bind();

		// VBO setup.
		m_VBO.createBuffer();
		m_VBO.bind();

		// IBO setup.
		m_IBO.createBuffer();
		m_IBO.bind();

		// Enable vertex attributes.
		VertexType::initVertexAttributes(m_VAOID);

		// Default value.
		setCapacityIncrements(BUFFER_INCREMENTS);
	}

	// Destructor.
	inline ~VertexArrayObject() 
	{
		deleteArray();
	}

	// Push vertices and return their index in memory.
	inline int push(const std::vector<VertexType>& vertices) 
	{
		return push(vertices.data(), vertices.size());
	}

	// Push vertices and return their index in memory.
	inline int push(VertexType* vertexPtr, int size)
	{
		verticesChanged();
		return m_vertexData.push(vertexPtr, size);
	}

	// Erase vertices.
	inline void eraseVertices(int index, int size) 
	{
		verticesChanged();
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
		indicesChanged();
		return m_indexData.push(indices, size);
	}

	// Erase indices.
	inline void eraseIndices(int index, int size)
	{
		indicesChanged();
		m_indexData.erase(index, size);
	}

	// Utilities.
	inline void setCapacityIncrements(int size)		{ m_vertexData.setCapacityIncrements(size); m_indexData.setCapacityIncrements(size); };
	inline int getCapacityIncrements()				{ return m_vertexData.getCapacityIncrements(); };	
	inline void setResizeThreshold(float threshold) { m_vertexData.setResizeThreshold(threshold); m_indexData.setResizeThreshold(threshold); }
	inline float getResizeThreshold()				{ return m_vertexData.getResizeThreshold(); }
	inline int getVertexCount()					    { return m_vertexData.size(); }
	inline int getIndexCount()						{ return m_indexData.size() * IndexType::size(); }
	inline int getVertexCapacityCPU()				{ return m_vertexData.capacity(); }
	inline int getVertexCapacityGPU()				{ return m_VBO.capacity(); }
	inline int getIndexCapacityCPU()				{ return m_indexData.capacity() * IndexType::size(); }  // Given in indices, not IndexData count.
	inline int getIndexCapacityGPU()				{ return m_IBO.capacity(); }						    // Given in indices, not IndexData count.
	inline void indicesChanged()					{ m_indexBufferSynced = false; }
	inline void verticesChanged()					{ m_vertexBufferSynced = false; }

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
