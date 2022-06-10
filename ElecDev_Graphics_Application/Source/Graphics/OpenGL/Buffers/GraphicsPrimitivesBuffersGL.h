#pragma once

#include "Utilities/Memory/FreeList.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "Lumen/Lumen.h"

struct Int2 
{
	Int2(int ind0, int ind1) { _data[0] = ind0; _data[1] = ind1; }
	inline int* data() { return _data; }
	// Operators.
	inline void operator+=(int val) { _data[0] += val; _data[1] += val; };
	inline void operator-=(int val) { _data[0] -= val; _data[1] -= val; };
	// Data.
	union {
		int _data[2] = { 0,1 };
		int ind0, ind1;
	};
	typedef int VarType;
};

struct UInt2 
{
	UInt2(unsigned ind0, unsigned ind1) { _data[0] = ind0; _data[1] = ind1; }
	inline unsigned* data() { return _data; }
	// Operators.
	inline void operator+=(unsigned val) { _data[0] += val; _data[1] += val; };
	inline void operator-=(unsigned val) { _data[0] -= val; _data[1] -= val; };
	// Data.
	union {
		unsigned _data[2] = { 0,1 };
		unsigned ind0, ind1;
	};
	typedef unsigned VarType;
};

struct Int3 
{
	Int3(int ind0, int ind1, int ind2) { _data[0] = ind0; _data[1] = ind1; _data[2] = ind2; }
	inline int* data() { return _data; }
	// Operators.
	inline void operator+=(int val) { _data[0] += val; _data[1] += val; _data[2] += val; };
	inline void operator-=(int val) { _data[0] -= val; _data[1] -= val; _data[2] -= val; };
	// Data.
	union {
		int _data[3] = { 0,1,2 };
		int ind0, ind1, ind2;
	};
	typedef int VarType;
};

struct UInt3 
{
	UInt3(unsigned ind0, unsigned ind1, unsigned ind2) { _data[0] = ind0; _data[1] = ind1; _data[2] = ind2; }
	inline unsigned* data() { return _data; }
	// Operators.
	inline void operator+=(unsigned val) { _data[0] += val; _data[1] += val; _data[2] += val; };
	inline void operator-=(unsigned val) { _data[0] -= val; _data[1] -= val; _data[2] -= val; };
	// Data.
	union {
		unsigned ind0, ind1, ind2;
		unsigned _data[3] = { 0,1,2 };
	};
	typedef unsigned VarType;
};

class IGraphicsPrimitivesBuffer 
{
public:

	// Functions that the renderer has to be able to access universally.
	inline virtual bool onDrawCall() = 0;
	inline virtual VertexArrayObject& getVAO() = 0;
	inline virtual int getIndexCount() = 0;
	inline virtual int getVertexCount() = 0;

protected:

	// Constructor.
	inline IGraphicsPrimitivesBuffer() = default;
	// Destructor.
	inline virtual ~IGraphicsPrimitivesBuffer() = default;
};

template <typename VertexType, 
		  typename IndexType, 
	      template <typename> class VertexContainer, 
	      template <typename> class IndexContainer>
class GraphicsPrimitivesBuffer : public IGraphicsPrimitivesBuffer
{

protected:

	// Constructor.
	inline GraphicsPrimitivesBuffer() 
	{
		// Setup GPU data.
		m_VAO.create();
		m_VAO.setLayout<VertexType>();

		// Setup CPU data.
		m_vertexData.iterateElements();
		m_indexData.iterateMemory();
		m_vertexData.setCapacityIncrements(BUFFER_INCREMENTS);
		m_indexData.setCapacityIncrements(BUFFER_INCREMENTS);
		m_vertexData.setResizeThreshold(VERTEX_BUFFER_REDUCTION_SCALE);
		m_indexData.setResizeThreshold(VERTEX_BUFFER_REDUCTION_SCALE);
	};

	// Destructor.
	inline ~GraphicsPrimitivesBuffer() = default;

public:

	// Push vertices and return the index in memory.  Vertices are copied.
	inline int push(const VertexType* ptr, int size = 1)
	{
		return m_vertexData.push(ptr, size);
	}

	// Push indices and return the index in memory.  Indices are copied.
	// The offset should be the starting vertex's position in memory.
	inline int push(IndexType* ptr, int offset, int size = 1)
	{
		int index = m_indexData.push(ptr, size);
		if (!offset) return index;
		// Offset the incides.
		for (int i = index; i < size; i++) m_indexData[i] += offset;
		return index;
	}

	// Push vertices and the corresponding indices.
	// This function handles the offsetting of indices.
	// returns { VerticesIndex, IndicesIndex }
	inline std::tuple<unsigned, unsigned> push(VertexType* vertexPtr, int vertexCount, IndexType* indexPtr, int indexCount)
	{
		int verticesIndex = push(vertexPtr, vertexCount);
		int indicesIndex = push(indexPtr, indexCount, verticesIndex);
		return { verticesIndex, indicesIndex};
	}

	// Erase the vertices given the position and size.
	inline void eraseVertices(int index, int size)
	{
		m_vertexData.erase(index, size);
	}

	// Erase the indices given the position and size.
	inline void eraseIndices(int index, int size)
	{
		m_indexData.erase(index, size);
	}

	// Notify of draw call.
	inline virtual bool onDrawCall() override
	{
		if (!m_indexData.count()) return false; 
		if (!m_vertexData.count()) return false;
		checkResize();
		return true;
	}

	// Utilities.
	inline VertexType& getVertex(int index)				{ return m_vertexData[index]; }
	inline float getResizeThreshold()					{ return m_vertexData.getResizeThreshold(); }
	inline int getCapacityIncrements()					{ return m_vertexData.getCapacityIncrements(); }
	inline virtual VertexArrayObject& getVAO() override	{ return m_VAO; };
	inline void setResizeThreshold(float value)			{ m_vertexData.setResizeThreshold(value); m_indexData.setResizeThreshold(value); }
	inline void setCapacityIncrements(int value)		{ m_indexData.setCapacityIncrements(value); m_vertexData.setCapacityIncrements(value); }
	inline virtual int getIndexCount() override			{ return m_indexData.count(); }
	inline virtual int getVertexCount() override		{ return m_vertexData.count(); }
	inline VertexContainer<VertexType>& getVertexData() { return m_vertexData; }
	inline IndexContainer<IndexType>& getIndexData()	{ return m_indexData; }

	// Template types.
	typedef VertexType t_vertexType;
	typedef IndexType t_indexType;
	typedef VertexContainer<VertexType> t_vertexContainer;
	typedef IndexContainer<IndexType> t_indexContainer;

	// Check if the buffers have to resize.
	inline void checkResize()
	{
		if (m_VAO.getIBO().capacity() != m_indexData.capacity())  resizeIBO();
		if (m_VAO.getVBO().capacity() != m_vertexData.capacity()) resizeVBO();
	}

	// Resize the GPU vertex buffer.
	inline void resizeVBO() 
	{
		getVAO().getVBO().bufferData(m_vertexData.capacity() * VertexType::getTotalSize(), NULL);
		reloadAllVertices();
	}

	// Resize the GPU index buffer.
	inline void resizeIBO()
	{
		getVAO().getIBO().bufferData(m_indexData.capacity() * sizeof(unsigned), NULL);
		reloadAllIndices();
	}

	// Load all of the vertex CPU data to the GPU.
	inline void reloadAllVertices()
	{
		VertexBufferObject& vbo = getVAO().getVBO();
		vbo.bind();
		for (auto it = m_vertexData.begin(); it != m_vertexData.end(); ++it)
		{
			vbo.bufferSubData(it.m_index * VertexType::getTotalSize(), VertexType::getDataSize(), (*it).getData());
			vbo.bufferSubData(it.m_index * VertexType::getTotalSize() + VertexType::getIDOffset(), VertexType::getDataSize(), (*it).getData());
		}
	}

	// Load all of the index CPU data to the GPU.
	inline void reloadAllIndices()
	{
		IndexBufferObject& ibo = getVAO().getIBO();
		ibo.bind();
		for (auto it = m_indexData.begin(); it != m_indexData.end(); ++it)
		{
			ibo.bufferSubData(it.m_index * sizeof(IndexType), it.m_elementsInMemoryRegion * sizeof(IndexType), (*it).data());
		}
	}

private:

	// GPU data.
	VertexArrayObject m_VAO;
	// CPU data.
	VertexContainer<VertexType> m_vertexData;
	IndexContainer<IndexType> m_indexData;
};

template <typename VertexType>
class GraphicsTrianglesBuffer : public GraphicsPrimitivesBuffer<VertexType, UInt3, FreeList, FreeList>
{ 
public:

	// Constructor.
	inline GraphicsTrianglesBuffer() : GraphicsPrimitivesBuffer<VertexType, UInt3, FreeList, FreeList>()
	{
		this->getVAO().setType(GL_TRIANGLES);
	}
};

template <typename VertexType>
class GraphicsLinesBuffer : public GraphicsPrimitivesBuffer<VertexType, UInt2, FreeList, SEFreeList>
{ 
public:

	// Constructor.
	inline GraphicsLinesBuffer() : GraphicsPrimitivesBuffer<VertexType, UInt2, FreeList, SEFreeList>()
	{
		this->getVAO().setType(GL_LINES);
	}
};
