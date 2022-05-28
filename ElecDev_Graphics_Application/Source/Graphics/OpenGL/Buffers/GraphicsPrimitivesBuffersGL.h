#pragma once

#include "Utilities/Memory/FreeList.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"

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
	virtual VertexArrayObject& getVAO() { return m_VAO; };

private:

	// Constructor.
	inline GraphicsPrimitivesBuffer() 
	{
		m_VAO.createArray();
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
	
	}


private:
	// CPU data.
	FreeList<VertexType> m_vertexData;
	FreeList<UInd3> m_vertexData;
};
