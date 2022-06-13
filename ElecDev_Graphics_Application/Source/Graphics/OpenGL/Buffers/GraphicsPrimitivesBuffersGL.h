#pragma once

#include "Utilities/Memory/FreeList.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Primitives/IPrimitive.h"
#include "Lumen/Lumen.h"
#include "imgui/imgui.h"

struct Int2 
{
	Int2(int int0, int int1) : int0(int0), int1(int1) { }
	inline int* data() { return &int0; }
	// Operators.
	inline void operator+=(int val) { int0 += val; int1 += val; }
	inline void operator-=(int val) { int0 -= val; int1 -= val; }
	// Data.
	int int0, int1;
	// Template.
	typedef int VarType;
	constexpr static inline int count() { return 2; }
};

struct UInt2 
{
	UInt2(unsigned int0, unsigned int1) : int0(int0), int1(int1) { }
	inline unsigned* data() { return &int0; }
	// Operators.
	inline void operator+=(unsigned val) { int0 += val; int1 += val; }
	inline void operator-=(unsigned val) { int0 -= val; int1 -= val; }
	// Data.
	unsigned int0, int1;
	// Template.
	typedef unsigned VarType;
	constexpr static inline int count() { return 2; }
};

struct Int3 
{
	Int3(int int0, int int1, int int2) : int0(int0), int1(int1), int2(int2) { }
	inline int* data() { return &int0; }
	// Operators.
	inline void operator+=(int val) { int0 += val; int1 += val; int2 += val; }
	inline void operator-=(int val) { int0 -= val; int1 -= val; int2 -= val; }
	// Data.
	int int0, int1, int2;
	// Template.
	typedef int VarType;
	constexpr static inline int count() { return 3; }
};

struct UInt3 
{
	UInt3(unsigned int0, unsigned int1, unsigned int2) : int0(int0), int1(int1), int2(int2) { }
	inline unsigned* data() { return &int0; }
	// Operators.
	inline void operator+=(unsigned val) { int0 += val; int1 += val; int2 += val; };
	inline void operator-=(unsigned val) { int0 -= val; int1 -= val; int2 -= val; };
	// Data.
	unsigned int0, int1, int2;
	// Template.
	typedef unsigned VarType;
	constexpr static inline int count() { return 3; }
};

class IGraphicsPrimitivesBuffer 
{
public:

	// Functions that the renderer has to be able to access universally.
	inline virtual bool onDrawCall() = 0;
	inline virtual VertexArrayObject& getVAO() = 0;
	inline virtual int getIndexCount() const = 0;
	inline virtual int getVertexCount() const = 0;

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
		m_vertexData.iterateMemory();
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
		// Ensure valid data.
		if (!size) return -1;

		// CPU.
		int index = m_vertexData.push(ptr, size);

		// GPU. If there is not resize the data should be loaded manually.
		if (!queryVerticesResize())
			getVAO().getVBO().namedBufferSubData(index * sizeof(VertexType), size * sizeof(VertexType), getVertex(index).data());

		return index;
	}

	// Push indices and return the index in memory.  Indices are copied.
	// The offset should be the starting vertex's position in memory that
	// was returned when pushing the vertices.
	inline int push(const IndexType* ptr, int size, int offset)
	{
		// Ensure valid data.
		if (!size) return -1;

		// Load data to CPU and offset indices.
		int index = m_indexData.push(ptr, size);
		if (offset) for (int i = index; i < index + size; i++) m_indexData[i] += offset;
		indicesChanged();
		return index;
	}

	// Push vertices and the corresponding indices.
	// The indices are offset automatically.
	// @returns { VerticesIndex, IndicesIndex }
	inline std::tuple<unsigned, unsigned> push(const VertexType* vertexPtr, int vertexCount, const IndexType* indexPtr, int indexCount)
	{
		int verticesIndex = push(vertexPtr, vertexCount);
		int indicesIndex = push(indexPtr, indexCount, verticesIndex);
		return { verticesIndex, indicesIndex};
	}

	// Erase the vertices given the position and size.
	inline void eraseVertices(int index, int size)
	{
		// Ensure valid data.
		if (!size || index == -1) return;

		// CPU.
		m_vertexData.erase(index, size);
	}

	// Erase the indices given the position and size.
	inline void eraseIndices(int index, int size)
	{
		// Ensure valid data.
		if (!size || index == -1) return;

		// CPU.
		m_indexData.erase(index, size);
		indicesChanged();
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
		if (m_indicesChanged)		   reloadIndices();
		if (m_primitivesToSync.size()) syncPrimitives();
		if (!m_indexData.count())      return false; 
		if (!m_vertexData.count())     return false;
		return true;
	}
	
	// Queue the primitive to be synced.
	inline void sync(IPrimitive* primitive) 
	{
		m_primitivesToSync.push_back(primitive);
		primitive->m_queuedForSync = true;
	}

	// Remove the primitive from the sync queue.
	inline void removeFromSync(IPrimitive* primitive)
	{
		// Ensure primitive is in queue.
		if (!primitive->m_queuedForSync) return;

		// Find primitive in queue.
		int index = 0;
		for (auto p : m_primitivesToSync)
		{
			if (primitive == p) break;
			index++;
		}

		// Remove from queue.
		m_primitivesToSync.erase(m_primitivesToSync.begin() + index);
		primitive->m_queuedForSync = false;
	}

	// Utilities.
	inline virtual VertexArrayObject& getVAO() override	{ return m_VAO; };
	inline VertexContainer<VertexType>& getVertexData() { return m_vertexData; }
	inline IndexContainer<IndexType>& getIndexData()	{ return m_indexData; }
	inline VertexType& getVertex(int index)				{ return m_vertexData[index]; }
	inline virtual int getIndexCount() const override	{ return m_indexData.count() * IndexType::count(); }
	inline virtual int getVertexCount() const override	{ return m_vertexData.count(); }
	inline float getResizeThreshold() const				{ return m_vertexData.getResizeThreshold(); }
	inline int getCapacityIncrements() const			{ return m_vertexData.getCapacityIncrements(); }
	inline void setResizeThreshold(float value)			{ m_vertexData.setResizeThreshold(value); m_indexData.setResizeThreshold(value); }
	inline void setCapacityIncrements(int value)		{ m_indexData.setCapacityIncrements(value); m_vertexData.setCapacityIncrements(value); }
	inline void indicesChanged()						{ m_indicesChanged = true; }

	// Template types.
	typedef VertexType t_vertexType;
	typedef IndexType t_indexType;
	typedef VertexContainer<VertexType> t_vertexContainer;
	typedef IndexContainer<IndexType> t_indexContainer;

	// Check if the vertices memory has to resize.
	inline bool queryVerticesResize() 
	{
		// Check if resize required.
		if (m_VAO.getVBO().capacity() != m_vertexData.allocated())
		{
			// Resize the VBO and reload the data.
			getVAO().getVBO().namedBufferData(m_vertexData.allocated(), NULL);
			reloadVertices();
			// If the VBO was resized, the vertices were reloaded and
			// there is no need to sync the remaining primitives.
			m_primitivesToSync.resize(0);
			return true;
		}
		// No resize ocurred.
		return false;
	}

	// Load all of the vertex CPU data to the GPU.
	inline void reloadVertices()
	{
		VertexBufferObject& vbo = getVAO().getVBO();
		vbo.bind();
		for (auto it = m_vertexData.begin(); it != m_vertexData.end(); ++it)
		{
			vbo.bufferSubData(it.m_index * sizeof(VertexType), it.m_elementsInMemoryRegion * sizeof(VertexType), (*it).data());
		}
	}

	// Load all of the index CPU data to the GPU.
	inline void reloadIndices()
	{
		IndexBufferObject& ibo = getVAO().getIBO();
		ibo.bind();

		// Orphan if no resize.
		if (m_indexData.size() == ibo.capacity()) ibo.orphan();
		// Resize.
		else ibo.bufferData(m_indexData.size(), NULL);

		// Reload index data.
		int offset = 0;
		for (auto it = m_indexData.begin(); it != m_indexData.end(); ++it)
		{
			ibo.bufferSubData(offset * sizeof(IndexType), it.m_elementsInMemoryRegion * sizeof(IndexType), (*it).data());
			offset += it.m_elementsInMemoryRegion;
		}

		// Index data is now updated.
		m_indicesChanged = false;
	}

	// Sync all of the primitives in the queue.
	inline void syncPrimitives() 
	{	
		VertexBufferObject& vbo = getVAO().getVBO();
		vbo.bind();
		// Update primitives data.
		for (auto primitive : m_primitivesToSync)
		{
			vbo.bufferSubData(primitive->m_vertexBufferPos * sizeof(VertexType), primitive->m_vertexCount * sizeof(VertexType), getVertex(primitive->m_vertexBufferPos).data());
			primitive->m_queuedForSync = false;
		}
		// All primitives have been synced.
		m_primitivesToSync.resize(0);
	}

	// Update indices for existing primitive with new indices.  
	// This is very useful for highly dynamic polygons where the vertex count is static.
	// Returns the new position in the freelist.
	inline int updateIndices(int oldIndexPosition, int oldIndexCount, int offset, IndexType* newIndices, int newIndexCount)
	{
		// Remove original data.
		eraseIndices(oldIndexPosition, oldIndexCount);
		// Push new data with the original offset.
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

	// All of the indices that are in the buffer will by drawn by the GPU, so there should be no
	// 'invalid' data.  The requires the indices to be able to be updated regardless of resizes.
	bool m_indicesChanged = false;
};

template <typename VertexType>
class GraphicsTrianglesBuffer : public GraphicsPrimitivesBuffer<VertexType, UInt3, FreeList, FreeList>
{ 
public:
	inline GraphicsTrianglesBuffer() : GraphicsPrimitivesBuffer<VertexType, UInt3, FreeList, FreeList>()
	{
		this->getVAO().setType(GL_TRIANGLES);
	}
};

template <typename VertexType>
class GraphicsLinesBuffer : public GraphicsPrimitivesBuffer<VertexType, UInt2, FreeList, SEFreeList>
{ 
public:
	inline GraphicsLinesBuffer() : GraphicsPrimitivesBuffer<VertexType, UInt2, FreeList, SEFreeList>()
	{
		this->getVAO().setType(GL_LINES);
	}
};