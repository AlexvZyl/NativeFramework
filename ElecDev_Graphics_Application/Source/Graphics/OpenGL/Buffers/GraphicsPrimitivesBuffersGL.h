#pragma once

#include "Utilities/Memory/FreeList.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Primitives/IPrimitive.h"
#include "Lumen/Lumen.h"
#include "imgui/imgui.h"

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
		// CPU.
		int index = m_vertexData.push(ptr, size);
		if (queryVerticesResize()) return index;

		// If no resize, manually load data.
		// TODO: This can be reduced to one call.
		VertexBufferObject& vbo = getVAO().getVBO();
		for (int i = index; i < index + size; i++)
		{
			vbo.bufferSubData(i * VertexType::getTotalSize(), VertexType::getDataSize(), getVertex(i).getData());
			vbo.bufferSubData(i * VertexType::getTotalSize() + VertexType::getIDOffset(), VertexType::getDataSize(), getVertex(i).getID());
		}
		return index;
	}

	// Push indices and return the index in memory.  Indices are copied.
	// The offset should be the starting vertex's position in memory.
	inline int push(const IndexType* ptr, int size, int offset)
	{
		// CPU.
		int index = m_indexData.push(ptr, size);
		// Offset the incides.
		if (offset) for (int i = index; i < index + size; i++) m_indexData[i] += offset;
		if (queryIndicesResize()) return index;
		
		// If no resize, manually load data.
		getVAO().getIBO().namedBufferSubData(index * sizeof(IndexType), size * sizeof(IndexType), &m_indexData[index]);
		return index;
	}

	// Push vertices and the corresponding indices.
	// This function handles the offsetting of indices.
	// returns { VerticesIndex, IndicesIndex }
	inline std::tuple<unsigned, unsigned> push(const VertexType* vertexPtr, int vertexCount, const IndexType* indexPtr, int indexCount)
	{
		int verticesIndex = push(vertexPtr, vertexCount);
		int indicesIndex = push(indexPtr, indexCount, verticesIndex);
		return { verticesIndex, indicesIndex};
	}

	// Erase the vertices given the position and size.
	inline void eraseVertices(int index, int size)
	{
		// CPU.
		m_vertexData.erase(index, size);

		// GPU.
		getVAO().getVBO().namedBufferSubData(index * sizeof(VertexType), size * sizeof(VertexType), NULL);
	}

	// Erase the indices given the position and size.
	inline void eraseIndices(int index, int size)
	{
		// CPU.
		m_indexData.erase(index, size);

		// GPU.
		getVAO().getIBO().namedBufferSubData(index * sizeof(IndexType), size * sizeof(IndexType), NULL);
	}

	// Erase vertices and indices.
	inline void erase(int verticesIndex, int verticesCount, int indicesIndex, int indicesCount) 
	{
		eraseIndices(indicesIndex, indicesCount);
		eraseVertices(verticesIndex, verticesCount);
	}

	// Do necessary updates and return if should be drawn.
	inline virtual bool onDrawCall() override
	{
		if (!m_indexData.count())      return false; 
		if (!m_vertexData.count())     return false;
		if (m_primitivesToSync.size()) syncPrimitives();
		return true;
	}

	// Queue the primitive to be synced.
	inline void sync(IPrimitive* primitive) 
	{
		m_primitivesToSync.push_back(primitive);
	}

	// Utilities.
	inline virtual VertexArrayObject& getVAO() override	{ return m_VAO; };
	inline VertexContainer<VertexType>& getVertexData() { return m_vertexData; }
	inline IndexContainer<IndexType>& getIndexData()	{ return m_indexData; }
	inline VertexType& getVertex(int index)				{ return m_vertexData[index]; }
	inline virtual int getIndexCount() override			{ return m_indexData.count(); }
	inline virtual int getVertexCount() override		{ return m_vertexData.count(); }
	inline float getResizeThreshold()					{ return m_vertexData.getResizeThreshold(); }
	inline int getCapacityIncrements()					{ return m_vertexData.getCapacityIncrements(); }
	inline void setResizeThreshold(float value)			{ m_vertexData.setResizeThreshold(value); m_indexData.setResizeThreshold(value); }
	inline void setCapacityIncrements(int value)		{ m_indexData.setCapacityIncrements(value); m_vertexData.setCapacityIncrements(value); }

	// Template types.
	typedef VertexType t_vertexType;
	typedef IndexType t_indexType;
	typedef VertexContainer<VertexType> t_vertexContainer;
	typedef IndexContainer<IndexType> t_indexContainer;

	// Check if the buffers have to resize.
	inline bool queryResize()
	{
		return queryVerticesResize() || queryIndicesResize();
	}

	// Check if the indices memory has to resize.
	inline bool queryIndicesResize() 
	{
		if (m_VAO.getIBO().capacity() != m_indexData.allocated())
		{
			resizeIBO();
			return true;
		}
		return false;
	}

	// Check if the vertices memory has to resize.
	inline bool queryVerticesResize() 
	{
		if (m_VAO.getVBO().capacity() != m_vertexData.allocated())
		{
			resizeVBO();
			// If the VBO was resized, the vertices were reloaded and
			// there is no need to sync the remaining primitives.
			m_primitivesToSync.resize(0);
			return true;
		}
		return false;
	}

	// Resize the GPU vertex buffer.
	inline void resizeVBO() 
	{
		getVAO().getVBO().namedBufferData(m_vertexData.allocated(), NULL);
		reloadAllVertices();
	}

	// Resize the GPU index buffer.
	inline void resizeIBO()
	{
		getVAO().getIBO().namedBufferData(m_indexData.allocated(), NULL);
		reloadAllIndices();
	}

	// Load all of the vertex CPU data to the GPU.
	inline void reloadAllVertices()
	{
		VertexBufferObject& vbo = getVAO().getVBO();
		vbo.bind();
		for (auto it = m_vertexData.begin(); it != m_vertexData.end(); ++it)
		{
			// TODO: This can be reduced to one call.
			vbo.bufferSubData(it.m_index * VertexType::getTotalSize(), VertexType::getDataSize(), (*it).getData());
			vbo.bufferSubData(it.m_index * VertexType::getTotalSize() + VertexType::getIDOffset(), VertexType::getIDSize(), (*it).getID());
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

	// Sync all of the primitives in the queue.
	inline void syncPrimitives() 
	{	
		VertexBufferObject& vbo = getVAO().getVBO();
		vbo.bind();
		// Iterate primitives.
		for (auto primitive : m_primitivesToSync)
		{
			// Iterate vertices contained by the primtive.
			// TODO: This can be reduced to one call.
			for (int i = primitive->m_vertexBufferPos; i < primitive->m_vertexBufferPos + primitive->m_vertexCount; i++)
			{
				vbo.bufferSubData(i * VertexType::getTotalSize(), VertexType::getDataSize(), getVertex(i).getData());
				vbo.bufferSubData(i * VertexType::getTotalSize() + VertexType::getIDOffset(), VertexType::getDataSize(), getVertex(i).getID());
			}
			// The primitive is no longer in the queue.
			primitive->m_queuedForSync = false;
		}
		// All primitives have been synced.
		m_primitivesToSync.resize(0);
	}

	// Update the indices with the new vector.
	// Returns the new position in the freelist.
	inline int updateIndices(int indexPosition, int indexCount, int offset, IndexType* newIndices, int newIndexCount)
	{
		// Remove original data.
		eraseIndices(indexPosition, indexCount);
		// Push new data.
		return push(newIndices, newIndexCount, offset);
	}

private:

	// GPU data.
	VertexArrayObject m_VAO;
	// CPU data.
	VertexContainer<VertexType> m_vertexData;
	IndexContainer<IndexType> m_indexData;
	// The primitives that have been queued for sync.
	// TODO: Need a vector that does not resize and shrink every time.
	// For now ImVector will work fine.
	ImVector<IPrimitive*> m_primitivesToSync;
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