#pragma once

#include "Utilities/Memory/FreeList.h"

struct Ind2 
{
	Ind2(int ind0, int ind1) { indices[0] = ind0; indices[1] = ind1; }
	// Operators.
	inline int* data() { return indices; }
	inline void operator+=(int val) { indices[0] += val; indices[1] += val; };
	inline void operator-=(int val) { indices[0] -= val; indices[1] -= val; };
	// Data.
	int indices[2] = { 0,1 };
};

struct UInd2 
{
	UInd2(unsigned ind0, unsigned ind1) { indices[0] = ind0; indices[1] = ind1; }
	// Operators.
	inline void operator+=(unsigned val) { indices[0] += val; indices[1] += val; };
	inline void operator-=(unsigned val) { indices[0] -= val; indices[1] -= val; };
	// Data.
	unsigned indices[2] = { 0,1 };
};

struct Ind3 
{
	Ind3(int ind0, int ind1, int ind2) { indices[0] = ind0; indices[1] = ind1; indices[2] = ind2; }
	// Operators.
	inline void operator+=(int val) { indices[0] += val; indices[1] += val; indices[2] += val; };
	inline void operator-=(int val) { indices[0] -= val; indices[1] -= val; indices[2] -= val; };
	// Data.
	int indices[3] = { 0,1,2 };
};

struct UInd3 
{
	UInd3(unsigned ind0, unsigned ind1, unsigned ind2) { indices[0] = ind0; indices[1] = ind1; indices[2] = ind2; }
	// Operators.
	inline void operator+=(unsigned val) { indices[0] += val; indices[1] += val; indices[2] += val; };
	inline void operator-=(unsigned val) { indices[0] -= val; indices[1] -= val; indices[2] -= val; };
	// Data.
	unsigned indices[3] = { 0,1,2 };
};


class IGraphicsPrimitivesBuffer
{
public:



private:
	inline IGraphicsPrimitivesBuffer() = default;
};

template <typename VertexType>
class GraphicsTrianglesBuffer : public IGraphicsPrimitivesBuffer
{
public:



private:
	FreeList<VertexType> m_vertexData;
	FreeList<UInd3> m_vertexData;
};
