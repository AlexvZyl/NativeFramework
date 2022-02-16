//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <algorithm>
#include <functional>
#include <chrono>
#include <iostream>
#include "External/Misc/ConsoleColor.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Primitives/Vertex.h"
#include "OpenGL/Primitives/Primitive.h"
#include "OpenGL/Entities/Entity.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

template <typename VertexType>
VertexArrayObject<VertexType>::VertexArrayObject(GLenum type)
	: VertexArrayObjectPtr(type)
{
	// Generate the VAO.
	GLCall(glGenVertexArrays(1, &m_VAOID));
	GLCall(glBindVertexArray(m_VAOID));

	// Generate the IBO.
	GLCall(glGenBuffers(1, &m_IBOID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOID));

	// Generate a VBO for the VAO.
	GLCall(glGenBuffers(1, &m_VBOID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));

	// Enable vertex attributes.
	VertexType::initVertexAttributes(m_VAOID);
}

template <typename VertexType>
VertexArrayObject<VertexType>::~VertexArrayObject()
{
	// Clear GPU memory.
	GLCall(glDeleteBuffers(1, &m_VBOID));
	GLCall(glDeleteBuffers(1, &m_IBOID));
	GLCall(glDeleteVertexArrays(1, &m_VAOID));
}

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

template <typename VertexType>
void VertexArrayObject<VertexType>::render()
{
	// Checks before rendering.
	if		(!m_sized )		 resizeBuffer();
	else if (!m_synced)		 syncBuffer();  
	if      (!m_vertexCount) return;
	// Render.
	GLCall(glBindVertexArray(m_VAOID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOID));
	GLCall(glDrawElements(m_bufferType, m_indexCount, GL_UNSIGNED_INT, 0));
}

template <typename VertexType>
void VertexArrayObject<VertexType>::bind() const 
{ 
	GLCall(glBindVertexArray(m_VAOID)); 
}

template <typename VertexType>
void VertexArrayObject<VertexType>::unbind() const 
{ 
	GLCall(glBindVertexArray(0)); 
}

template <typename VertexType>
void VertexArrayObject<VertexType>::sync(PrimitivePtr* primitive)
{ 
	m_synced = false; 
	m_primitivesToSync.push_back(primitive);
}

template <typename VertexType>
void VertexArrayObject<VertexType>::resize() 
{ 
	m_sized = false; 
}

//=============================================================================================================================================//
//  Buffer data.					  																										   //
//=============================================================================================================================================//

template <typename VertexType>
void VertexArrayObject<VertexType>::appendVertexData(std::vector<std::unique_ptr<VertexType>>& vertices, std::vector<unsigned>& indices,
													 unsigned* vertexPos, unsigned* indexPos)
{
	if (!vertices.size())  return;  // Make sure we want this.
	if (vertexPos)		  *vertexPos = m_vertexCount;
	if (indexPos)		  *indexPos  = m_indexCount;

	// Add vertices.
	m_vertexCPU.reserve(m_vertexCount + vertices.size());
	for (std::unique_ptr<VertexType>& vertex : vertices) 
	{ 
		// Will insert be faster?
		m_vertexCPU.emplace_back(std::move(vertex)); 
	}  

	// Add indices.
	m_indexCPU.reserve(m_indexCount + indices.size());
	for (unsigned& ind : indices)
	{
		// First offset the index.
		ind += m_vertexCount;
		m_indexCPU.push_back(ind);
	}

	// Increment counts.
	m_vertexCount += vertices.size();
	m_indexCount += indices.size();
	// Set the VAO to be resized.
	resize();
}

template <typename VertexType>
void VertexArrayObject<VertexType>::deleteVertexData(unsigned vertexPos, unsigned vertexCount, unsigned indexPos, unsigned indexCount)
{
	// Ensure there are vertices to delete.
	if (!m_vertexCount || !m_indexCount)
	{
		// Log warning.
		return;
	}

	// Remove data from VAO.
	m_vertexCPU.erase(m_vertexCPU.begin() + vertexPos, m_vertexCPU.begin() + vertexPos + vertexCount);
	m_vertexCPU.shrink_to_fit();
	m_indexCPU.erase(m_indexCPU.begin() + indexPos, m_indexCPU.begin() + indexPos + indexCount);
	m_indexCPU.shrink_to_fit();

	// Decrement counts.
	m_vertexCount -= vertexCount;
	m_indexCount -= indexCount;

	// Offset indices placed after deleted indices.
	for (int i = indexPos; i < m_indexCount; i++) 
		m_indexCPU[i] -= vertexCount; 

	// Set the VAO to be resized.
	resize();
}

//=============================================================================================================================================//
//  Primitives buffer.					  																									   //
//=============================================================================================================================================//

template <typename VertexType>
void VertexArrayObject<VertexType>::pushPrimitive(PrimitivePtr* primitive)
{
	// Store primitive position.
	primitive->m_primitiveBufferPos = m_primitives.size();
	// Add primitive.
	m_primitives.push_back(primitive);
	// Reserve data for the sync vector.
	m_primitivesToSync.reserve(m_primitives.size());  // This is not the best solution.
}	

template <typename VertexType>
void VertexArrayObject<VertexType>::popPrimitive(int primitiveIndex, int vertexCount, int indexCount)
{
	// Remove primitive from the VAO vector.
	m_primitives.erase(m_primitives.begin() + primitiveIndex);
	m_primitives.shrink_to_fit();
	// Change metadata of primitives that sit after the popped primitive.
	for (int i = primitiveIndex; i < m_primitives.size(); i++)
	{
		PrimitivePtr* primitive = m_primitives[i];
		primitive->m_indexBufferPos -= indexCount;
		primitive->m_vertexBufferPos -= vertexCount;
		primitive->m_primitiveBufferPos -= 1;
	}
	// Reserve data for the sync vector.
	m_primitivesToSync.reserve(m_primitives.size());  // This is not the best solution.
}

//=============================================================================================================================================//
//  GPU management.																															   //
//=============================================================================================================================================//

template <typename VertexType>
void VertexArrayObject<VertexType>::resizeBuffer()
{
	// If no vertices, clear buffers.
	if (!m_vertexCount)
	{
		// Resize VBO.
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW));
		// Resize IBO and write data.
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW));
	}
	// Assign data with resize.
	else
	{
		// Resize VBO.
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, m_vertexCount * m_vertexCPU[0]->getTotalSize(), NULL, GL_DYNAMIC_DRAW));  // VS does not like this.
		// Write the data to the VBO.
		unsigned int index = 0;
		for (std::unique_ptr<VertexType>& vertex : m_vertexCPU)  
		{	
			// There has to be a way to change this to one draw call...
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex->getTotalSize(), vertex->getDataSize(), vertex->getData()));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex->getTotalSize() + vertex->getIDOffset(), vertex->getIDSize(), vertex->getID()));
			index += 1;
		}
		// Resize IBO and write data.
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(GLuint), m_indexCPU.data(), GL_DYNAMIC_DRAW));
	}
	// Update flags.
	m_synced = true;
	m_sized = true;
	// All primitives have been synced.
	m_primitivesToSync.clear();
}

// Could there be a situation where a removed primitive is set to be synced?
template <typename VertexType>
void VertexArrayObject<VertexType>::syncBuffer()
{
	// Update the primitives' vertex data.
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
	for (PrimitivePtr* primitive : m_primitivesToSync) 
	{
		for (int index = primitive->m_vertexBufferPos; index < primitive->m_vertexCount + primitive->m_vertexBufferPos; index++)
		{
			std::unique_ptr<VertexType>& vertex = m_vertexCPU[index];
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex->getTotalSize(), vertex->getDataSize(), vertex->getData()));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex->getTotalSize() + vertex->getIDOffset(), vertex->getIDSize(), vertex->getID()));
		}
	}
	// Update flags.
	m_synced = true;
	// Primitives have been synced.
	m_primitivesToSync.clear();
}

//=============================================================================================================================================//
//  Wiping.					  																										           //
//=============================================================================================================================================//

template <typename VertexType>
void VertexArrayObject<VertexType>::wipeCPU()
{
	// Clear vertex data.
	m_vertexCPU.clear();
	m_vertexCPU.shrink_to_fit();
	m_vertexCount = 0;
	// Clear index data.
	m_indexCPU.clear();
	m_indexCPU.shrink_to_fit();
	m_indexCount = 0;
	// Clear primitives.
	m_primitives.clear();
	m_primitives.shrink_to_fit();
	m_primitivesToSync.clear();
	m_primitivesToSync.shrink_to_fit();
}

template <typename VertexType>
void VertexArrayObject<VertexType>::wipeGPU()
{
	// Wipe VBO.
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW));
	// Wipe IBO.
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW));
}

template <typename VertexType>
void VertexArrayObject<VertexType>::wipe()
{
	wipeCPU();
	wipeGPU();
}

//=============================================================================================================================================//
//  Instantiations.																															   //
//=============================================================================================================================================//

template class VertexArrayObject<VertexData>;
template class VertexArrayObject<VertexDataTextured>;
template class VertexArrayObject<VertexDataCircle>;

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
