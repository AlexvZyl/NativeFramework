#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "OpenGL/ErrorHandlerGL.h"
#include "OpenGL/Buffers/BufferObjects.h"
#include "Lumen/Lumen.h"
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

// Data used to issue a reload command.
struct ReloadCommand 
{
	ReloadCommand(int index, int size) 
		: index(index), size(size) 
	{ }

	int index = 0;
	int size = 0;
	int null = NULL; // So that it can be used in the freelist.
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

	// Utilities.
	virtual inline void setCapacityIncrements(int size) = 0;
	virtual inline int getCapacityIncrements() = 0;
	virtual inline void setResizeThreshold(float threshold) = 0;
	virtual inline float getResizeThreshold() = 0;
	virtual inline int getVertexCount() = 0;
	virtual inline int getIndexCount() = 0;
	virtual inline int getVertexCapacityCPU() = 0;
	virtual inline int getVertexCapacityGPU() = 0;
	virtual inline int getIndexCapacityCPU() = 0;
	virtual inline int getIndexCapacityGPU() = 0;
	
protected:

	friend class Renderer;

	// Constructor.
	IVertexArrayObject(GLenum type) 
		: m_bufferType(type) 
	{ }

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

template <typename VertexType, typename IndexType = IndexData3>
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
	virtual inline void setCapacityIncrements(int size)	override	 { m_vertexData.setCapacityIncrements(size); m_indexData.setCapacityIncrements(size); };
	virtual inline int getCapacityIncrements() override				 { return m_vertexData.getCapacityIncrements(); };
	virtual inline void setResizeThreshold(float threshold) override { m_vertexData.setResizeThreshold(threshold); m_indexData.setResizeThreshold(threshold); }
	virtual inline float getResizeThreshold() override				 { return m_vertexData.getResizeThreshold(); }
	virtual inline int getVertexCount() override					 { return m_vertexData.size(); }
	virtual inline int getIndexCount() override						 { return m_indexData.size() * IndexType::size(); }
	virtual inline int getVertexCapacityCPU() override				 { return m_vertexData.capacity(); }
	virtual inline int getVertexCapacityGPU() override				 { return m_VBO.capacity(); }
	virtual inline int getIndexCapacityCPU() override				 { return m_indexData.capacity() * IndexType::size(); }  // Given in indices, not IndexData count.
	virtual inline int getIndexCapacityGPU() override				 { return m_IBO.capacity(); }						     // Given in indices, not IndexData count.
	
	// Data.
	FreeList<VertexType> m_vertexData;
	FreeList<IndexType> m_indexData;

private:

	// Friends.
	friend class Scene;
	friend class FrameBufferObject;
	friend class RendererStats;
	friend class Renderer;
	friend class BackgroundColorEditor;
	friend class Grid;

	// Utilities.
	inline void indicesChanged()  { m_indexBufferSynced = false;  }
	inline void indicesSynced()   { m_indexBufferSynced = true;   }
	inline void verticesChanged() { m_vertexBufferSynced = false; }
	inline void verticesSynced()  { m_vertexBufferSynced = true;  }

	// ----------------------------------- //
	//  M E M O R Y   M A N A G E M E N T  //
	// ----------------------------------- //

	// Sync the entire VBO the the GPU.
	inline void syncEntireVBOtoGPU() 
	{
		// Get vertex info.
		int totalSize = VertexType::getTotalSize();
		int dataSize = VertexType::getDataSize();
		int idOffset = VertexType::getIDOffset();
		int idSize = VertexType::getIDSize();

		// Resize.
		m_VBO.bind();
		int newCapacity = m_vertexData.size();
		if(m_VBO.capacity() != newCapacity) m_VBO.resize(newCapacity * totalSize);

		// Load the data.
		int index = 0;
		for (auto& vertex : m_vertexData)
		{
			m_VBO.bufferSubData(index * totalSize, dataSize, vertex.getData());
			m_VBO.bufferSubData(index * totalSize + idOffset, idSize, vertex.getID());
			index++;
		}

		// Done.
		verticesSynced();
	}

	// Sync the entire IBO the the GPU.
	inline void syncEntireIBOtoGPU()
	{
		const static sizeOfUnsigned = sizeof(unsigned);

		// Resize.
		m_IBO.bind();
		int newCapacity = m_indexData.size();
		if (m_IBO.capacity() != newCapacity) m_IBO.resize(newCapacity * sizeOfUnsigned);

		// Load the data.
		int index = 0;
		for (auto& indexdata : m_indexData)
		{
			m_IBO.bufferSubData(index * sizeOfUnsigned * IndexType::size(), sizeOfUnsigned * IndexType::size(), indexdata.data());
			index++:
		}

		indicesSynced();
	}

	// Sync the GPU vertex data with the CPU data.
	inline void syncVerticesToGPU(int index, int size)
	{

	}

	// Sync the GPU index data with the CPU data.
	inline void syncIndicesToGPU(int index, int size)
	{

	}

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Updates and checks if has to render.
	inline virtual bool onDrawCall() override 
	{
		if (!m_vertexBufferSynced) syncEntireVBOtoGPU();
		if (!m_indexBufferSynced)  syncEntireIBOtoGPU();
		if (!getVertexCount())	   return false;
		if (!getIndexCount())	   return false;
		return true;
	}
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//