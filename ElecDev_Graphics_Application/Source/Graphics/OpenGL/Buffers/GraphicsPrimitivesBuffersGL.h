#pragma once

#include "Utilities/Memory/FreeList.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "Lumen/Lumen.h"

struct Ind2 
{
	Ind2(int ind0, int ind1) { _data[0] = ind0; _data[1] = ind1; }
	inline int* data() { return _data; }
	// Operators.
	inline void operator+=(int val) { _data[0] += val; _data[1] += val; };
	inline void operator-=(int val) { _data[0] -= val; _data[1] -= val; };
	// Data.
	union {
		int _data[2] = { 0,1 };
		int ind0, ind1;
	};
};

struct UInd2 
{
	UInd2(unsigned ind0, unsigned ind1) { _data[0] = ind0; _data[1] = ind1; }
	inline unsigned* data() { return _data; }
	// Operators.
	inline void operator+=(unsigned val) { _data[0] += val; _data[1] += val; };
	inline void operator-=(unsigned val) { _data[0] -= val; _data[1] -= val; };
	// Data.
	union {
		unsigned _data[2] = { 0,1 };
		unsigned ind0, ind1;
	};
};

struct Ind3 
{
	Ind3(int ind0, int ind1, int ind2) { _data[0] = ind0; _data[1] = ind1; _data[2] = ind2; }
	inline int* data() { return _data; }
	// Operators.
	inline void operator+=(int val) { _data[0] += val; _data[1] += val; _data[2] += val; };
	inline void operator-=(int val) { _data[0] -= val; _data[1] -= val; _data[2] -= val; };
	// Data.
	union {
		int _data[3] = { 0,1,2 };
		int ind0, ind1, ind2;
	};
};

struct UInd3 
{
	UInd3(unsigned ind0, unsigned ind1, unsigned ind2) { _data[0] = ind0; _data[1] = ind1; _data[2] = ind2; }
	inline unsigned* data() { return _data; }
	// Operators.
	inline void operator+=(unsigned val) { _data[0] += val; _data[1] += val; _data[2] += val; };
	inline void operator-=(unsigned val) { _data[0] -= val; _data[1] -= val; _data[2] -= val; };
	// Data.
	union {
		unsigned ind0, ind1, ind2;
		unsigned _data[3] = { 0,1,2 };
	};
};

class GraphicsPrimitivesBuffer
{
public:

	// Get the VAO that is used to render this primitive.
	inline virtual VertexArrayObject& getVAO() { return m_VAO; };

	// Notify the buffer of a draw call so that updates can be made.
	inline virtual bool onDrawCall() = 0;

protected:

	// Constructor.
	inline GraphicsPrimitivesBuffer() 
	{
		m_VAO.create();
	};

	// GPU data.
	VertexArrayObject m_VAO;
};

template <typename VertexType>
class GraphicsTrianglesBuffer : public GraphicsPrimitivesBuffer
{
public:

	// Constructor.
	inline GraphicsTrianglesBuffer() 
	{
		// Setup CPU data.
		m_vertexData.iterateElements();
		m_indexData.iterateMemory();
		m_vertexData.setCapacityIncrements(BUFFER_INCREMENTS);
		m_indexData.setCapacityIncrements(BUFFER_INCREMENTS);
		m_vertexData.setResizeThreshold(VERTEX_BUFFER_REDUCTION_SCALE);
		m_indexData.setResizeThreshold(VERTEX_BUFFER_REDUCTION_SCALE);

		// Setup GPU data.
		getVAO().setLayout<VertexType>();
	}

	// Push vertices and return the index in memory.
	// Vertices are copied.
	inline int push(const VertexType* ptr, int size = 1) 
	{
		return m_vertexData.push(ptr, size);
	}

	// Push indices and return the index in memory.
	// indices are copied.
	// The offset should be the starting vertex's position in memory.
	inline int push(UInd3* ptr, int size = 1, int offset = 0)
	{
		int index = m_indexData.push(ptr, size);
		if (!offset) return index;
		// Offset the incides.
		for (int i = index; i < size; i++) m_indexData[i] += offset;
		return index;
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
	inline virtual bool onDrawCall() 
	{
		
	}

	// Utilities.
	inline float getResizeThreshold()				{ return m_vertexData.getResizeThreshold();	   }
	inline int getCapacityIncrements()				{ return m_vertexData.getCapacityIncrements(); }	
	inline void setResizeThreshold(float value)		{ m_vertexData.setResizeThreshold(value);
													  m_indexData.setResizeThreshold(value);	   }
	inline void setCapacityIncrements(int value)	{ m_indexData.setCapacityIncrements(value);			
													  m_vertexData.setCapacityIncrements(value);   }	

private:

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
			ibo.bufferSubData(it.m_index * sizeof(unsigned), it.m_elementsInMemoryRegion * sizeof(unsigned), (*it).data());
		}
	}

	// CPU data.
	FreeList<VertexType> m_vertexData;
	FreeList<UInd3> m_indexData;
};
