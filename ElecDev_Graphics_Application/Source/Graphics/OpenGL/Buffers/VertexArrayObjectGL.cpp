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
#include "Graphics/Entities/Entity.h"
#include "Lumen.h"

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

	// Default value.
	setBufferIncrementSize(VERTEX_BUFFER_INCREMENTS);
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
	if (!m_vertexBufferSynced)	syncVertexBuffer();
	if (!m_indexBufferSynced)	syncIndexBuffer();
	if (!m_primitivesSynced)	syncPrimitives();
	if (!m_vertexCount)			return;
	if (!m_indexCount)			return;
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

//=============================================================================================================================================//
//  Buffer data.					  																										   //
//=============================================================================================================================================//

template <typename VertexType>
void VertexArrayObject<VertexType>::setBufferIncrementSize(unsigned size) 
{
	m_bufferIncrementSize = size;
}

template <typename VertexType>
void VertexArrayObject<VertexType>::syncIndexBuffer() 
{
	// Reload all of the IBO data.
	if (m_indexCount)
	{
		GLCall(glNamedBufferData(m_IBOID, m_indexBufferSize * sizeof(GLuint), m_indexCPU.data(), GL_DYNAMIC_DRAW));
	}
	// Wipe IBO data.
	else
	{
		GLCall(glNamedBufferData(m_IBOID, 0, NULL, GL_DYNAMIC_DRAW));
		m_indexBufferSize = 0;
	}
	// Update.
	m_indexBufferSynced = true;
}

template <typename VertexType>
void VertexArrayObject<VertexType>::syncVertexBuffer()
{
	// Reload all of the VBO data.
	if (m_vertexCount)
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
		unsigned index = 0;
		for (auto& vertex : m_vertexCPU)
		{
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex.getTotalSize(), vertex.getDataSize(), vertex.getData()));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex.getTotalSize() + vertex.getIDOffset(), vertex.getIDSize(), vertex.getID()));
			index++;
		}
		// Updating all the data syncs the primitives.
		for (auto& primitive : m_primitivesToSync) primitive->m_queuedForSync = false;
		m_primitivesToSync.clear();
		m_primitivesSynced = true;
	}
	// Wipe VBO data.
	else
	{
		GLCall(glNamedBufferData(m_VBOID, 0, NULL, GL_DYNAMIC_DRAW));
		m_vertexBufferSize = 0;
	}
	// Update.
	m_vertexBufferSynced = true;
}

//=============================================================================================================================================//
//  Primitives buffer.					  																									   //
//=============================================================================================================================================//

template <typename VertexType>
void VertexArrayObject<VertexType>::pushPrimitive(PrimitivePtr* primitive, const std::vector<VertexType>& vertices, const std::vector<unsigned>& indices)
{
	// Store primitive metadata.
	primitive->m_primitiveBufferPos = m_primitives.size();
	primitive->m_vertexBufferPos = m_vertexCount;
	primitive->m_indexBufferPos = m_indexCount;
	primitive->m_queuedForSync = false;

	// Increment counts.
	m_vertexCount += vertices.size();
	m_indexCount += indices.size();
	// Check for resize before loading data.
	queryBufferResize();

	// Store the index & vertex data on the CPU.
	m_vertexCPU.insert(m_vertexCPU.end(), vertices.begin(), vertices.end());
	unsigned vertexCountIn = vertices.size();
	std::for_each(indices.begin(), indices.end(), [&](const unsigned& val)
		{
			m_indexCPU.emplace_back(val + m_vertexCount - vertexCountIn);
		});

	// Add primitive to VAO.
	m_primitives.push_back(primitive);
	// Reserve data for the sync vector.
	m_primitivesToSync.reserve(m_primitives.size());  // FIX.
	// Syn CPU data with GPU.
	syncPrimitiveData(primitive);
}	

template <typename VertexType>
void VertexArrayObject<VertexType>::popPrimitive(PrimitivePtr* primitive)
{
	// Primitive data.
	// Gotta keep dat cache HOT
	unsigned vertexCount = primitive->m_vertexCount;
	unsigned vertexPos = primitive->m_vertexBufferPos;
	unsigned indexCount = primitive->m_indexCount;
	unsigned indexPos = primitive->m_indexBufferPos;
	unsigned primitiveIndex = primitive->m_primitiveBufferPos;

	// Remove vertices.
	if(vertexCount)
		m_vertexCPU.erase(m_vertexCPU.begin() + vertexPos, m_vertexCPU.begin() + vertexPos + vertexCount);
	
	// Remove indices.
	if(indexCount)
		m_indexCPU.erase(m_indexCPU.begin() + indexPos, m_indexCPU.begin() + indexPos + indexCount);

	// Decrement counts.
	m_vertexCount -= vertexCount;
	m_indexCount -= indexCount;

	// Offset indices placed after deleted indices.
	for (int i = indexPos; i < m_indexCount; i++)
		m_indexCPU[i] -= vertexCount;

	// Remove primitive.
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

	// Remove from primitives to sync.
	if (primitive->m_queuedForSync) 
	{
		for (int i = 0; i < m_primitivesToSync.size(); i++)
		{
			if (m_primitivesToSync[i] == primitive)
			{
				m_primitivesToSync.erase(m_primitivesToSync.begin() + i);
				break;
			}
		}
	}

	// Reserve data for the sync vector.
	m_primitivesToSync.reserve(m_primitives.size());  // This is not the best solution.

	// Check for resize.
	// TODO: We do not need to reload all of the data.
	if (!queryBufferResize()) 
	{
		m_indexBufferSynced = false;
		m_vertexBufferSynced = false;
	}
}

template <typename VertexType>
void VertexArrayObject<VertexType>::syncPrimitiveData(PrimitivePtr* primitive)
{
	syncPrimitiveVertexData(primitive);
	syncPrimitiveIndexData(primitive);
}

template <typename VertexType>
void VertexArrayObject<VertexType>::syncPrimitiveVertexData(PrimitivePtr* primitive)
{
	if (primitive->m_queuedForSync) return;
	primitive->m_queuedForSync = true;
	m_primitivesSynced = false;
	m_primitivesToSync.push_back(primitive);
}

template <typename VertexType>
void VertexArrayObject<VertexType>::syncPrimitiveIndexData(PrimitivePtr* primitive)
{
	GLCall(glNamedBufferSubData(m_IBOID, primitive->m_indexBufferPos * sizeof(GLuint), primitive->m_indexCount * sizeof(GLuint), m_indexCPU.data()+primitive->m_indexBufferPos));
}

//=============================================================================================================================================//
//  GPU management.																															   //
//=============================================================================================================================================//

template <typename VertexType>
bool VertexArrayObject<VertexType>::queryBufferResize()
{	
	// NOTE: Counts have to be updated before calling this function.

	// ----------------- //
	//  C A P A C I T Y  //
	// ----------------- //

	// Increase capacity.
	if (m_vertexCount > m_vertexBufferSize)
	{
		// GPU.
		while (m_vertexCount > m_vertexBufferSize)
			m_vertexBufferSize += m_bufferIncrementSize;
		// Calculate IBO size based on index/vertex ratio.
		m_indexBufferSize = (int)((float)m_vertexBufferSize * ((float)m_indexCount / (float)m_vertexCount));

		// CPU.
		m_vertexCPU.reserve(m_vertexBufferSize);
		m_indexCPU.reserve(m_indexBufferSize);
	}

	// Reduce capacity.
	else if (m_vertexCount < (m_vertexBufferSize - (VERTEX_BUFFER_REDUCTION_SCALE * m_bufferIncrementSize)))
	{
		while (m_vertexCount < (m_vertexBufferSize - (VERTEX_BUFFER_REDUCTION_SCALE * m_bufferIncrementSize)))
			m_vertexBufferSize -= m_bufferIncrementSize;
		// Calculate IBO size based on index/vertex ratio.
		m_indexBufferSize = (int)((float)m_vertexBufferSize * ((float)m_indexCount / (float)m_vertexCount));

		// CPU.
		m_vertexCPU.shrink_to_fit();
		m_vertexCPU.reserve(m_vertexBufferSize);
		m_indexCPU.shrink_to_fit();
		m_indexCPU.reserve(m_indexBufferSize);
		// Find a better way to do this.
		// Should be fixed when we implement our own freelist...
	}

	// No resize occured.
	else return false;

	// --------------------------- //
	//  V E R T E X   B U F F E R  //
	// --------------------------- //

	// Resize buffers.
	if (m_vertexCount)
	{
		// VBO GPU.
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, m_vertexBufferSize * VertexType::getTotalSize(), NULL, GL_DYNAMIC_DRAW));
		unsigned index = 0;
		// Reload current vertices.
		for (auto& vertex : m_vertexCPU)
		{
			// There has to be a way to change this to one draw call...
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex.getTotalSize(), vertex.getDataSize(), vertex.getData()));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex.getTotalSize() + vertex.getIDOffset(), vertex.getIDSize(), vertex.getID()));
			index++;
		}
	}
	// Clear buffer.
	else
	{
		m_vertexCPU.clear();
		m_vertexCPU.shrink_to_fit();
		m_vertexBufferSize = 0;
		GLCall(glNamedBufferData(m_VBOID, 0, NULL, GL_DYNAMIC_DRAW));
	}

	// ------------------------- //
	//  I N D E X   B U F F E R  //
	// ------------------------- //

	// Resize buffers.
	if (m_indexCount)
	{
		GLCall(glNamedBufferData(m_IBOID, m_indexBufferSize * sizeof(GLuint), m_indexCPU.data(), GL_DYNAMIC_DRAW));
	}
	// Clear buffer.
	else
	{
		GLCall(glNamedBufferData(m_IBOID, 0, NULL, GL_DYNAMIC_DRAW));
	}

	// Update flags.
	m_indexBufferSynced = true;
	m_vertexBufferSynced = true;

	// Resize occured.
	return true;
}

template <typename VertexType>
void VertexArrayObject<VertexType>::syncPrimitives()
{
	// Update the primitives' vertex data.
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
	for (PrimitivePtr* primitive : m_primitivesToSync)
	{
		for (int index = primitive->m_vertexBufferPos; index < primitive->m_vertexCount + primitive->m_vertexBufferPos; index++)
		{
			auto& vertex = m_vertexCPU[index];
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex.getTotalSize(), vertex.getDataSize(), vertex.getData()));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex.getTotalSize() + vertex.getIDOffset(), vertex.getIDSize(), vertex.getID()));
		}
		// Primitive removed from queue.
		primitive->m_queuedForSync = false;
	}
	
	// Update flags.
	m_primitivesSynced = true;
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
	GLCall(glNamedBufferData(m_VBOID, 0, NULL, GL_DYNAMIC_DRAW));
	GLCall(glNamedBufferData(m_IBOID, 0, NULL, GL_DYNAMIC_DRAW));
}

template <typename VertexType>
void VertexArrayObject<VertexType>::wipeAll()
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
