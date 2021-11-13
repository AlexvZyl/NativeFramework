//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "VertexArrayObjectGL.h"
#include "Entities/Entity.h"
#include "External/Misc/ConsoleColor.h"
#include "CoreGL/Entities/Vertex.h"
#include <algorithm> // for std::copy
#include "ErrorHandlerGL.h"
#include <algorithm>
#include <functional>

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

template <typename VertexType>
VertexArrayObject<VertexType>::VertexArrayObject(){}

template <typename VertexType>
VertexArrayObject<VertexType>::VertexArrayObject(GLenum type)
	: m_bufferType(type)
{
	// Generate the VAO.
	GLCall(glGenVertexArrays(1, &m_VAOID));
	GLCall(glBindVertexArray(m_VAOID));

	// Generate the IBO.
	GLCall(glGenBuffers(1, &m_IBOID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOID));

	// ----------------- //
	//  V E R T I C E S  //
	// ----------------- //

	if (typeid(VertexType)==typeid(VertexData))
	{
		int vertexSize = sizeof(VertexData::position) + sizeof(VertexData::color) + sizeof(VertexData::entityID);
		int colOffset = sizeof(VertexData::position);
		int idOffset = sizeof(VertexData::position) + sizeof(VertexData::color);
		// Generate.
		GLCall(glGenBuffers(1, &m_VBOID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
		// Position.
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (const void*)0));
		// Color.
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 1));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize, (const void*)colOffset));
		// Entity ID.
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 2));
		GLCall(glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, vertexSize, (const void*)idOffset));
	}
	
	// ----------------------------------- //
	//  V E R T I C E S   T E X T U R E D  //
	// ----------------------------------- //

	else if (typeid(VertexType)==typeid(VertexDataTextured))
	{
		int vertexSize = sizeof(VertexDataTextured::position) + sizeof(VertexDataTextured::color) + sizeof(VertexDataTextured::textureCoords) +
						 sizeof(VertexDataTextured::textureID) + sizeof(VertexDataTextured::entityID);
		int offset = 0;
		// Generate.
		GLCall(glGenBuffers(1, &m_VBOID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
		// Position.
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(VertexDataTextured::position);
		// Colour.
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 1));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(VertexDataTextured::color);
		// Texture position.
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 2));
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(VertexDataTextured::textureCoords);
		// Texture ID.
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 3));
		GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(VertexDataTextured::textureID);
		// Entity ID.
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 4));
		GLCall(glVertexAttribIPointer(4, 1, GL_UNSIGNED_INT, vertexSize, (const void*)offset));
	}

	// --------------------------------- //
	//  V E R T I C E S   C I R C L E S  //
	// --------------------------------- //

	else if (typeid(VertexType)== typeid(VertexDataCircle))
	{
		int vertexSize = sizeof(VertexDataCircle::position) + sizeof(VertexDataCircle::color) + sizeof(VertexDataCircle::fade) +
						 sizeof(VertexDataCircle::thickness) + sizeof(VertexDataCircle::entityID) + sizeof(VertexDataCircle::localCoords);
		int offset = 0;
		// Generate.
		GLCall(glGenBuffers(1, &m_VBOID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
		// Position.
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(VertexDataCircle::position);
		// Colour.
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 1));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(VertexDataCircle::color);
		// Local coordinates.
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 1));
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(VertexDataCircle::localCoords);
		// Circle thickness.
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 2));
		GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(VertexDataCircle::thickness);
		// Circle fade.
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 3));
		GLCall(glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(VertexDataCircle::fade);
		// Entity ID.
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 4));
		GLCall(glVertexAttribIPointer(5, 1, GL_UNSIGNED_INT, vertexSize, (const void*)offset));
	}
}

template <typename VertexType>
VertexArrayObject<VertexType>::~VertexArrayObject()
{
	wipeCPU();
	GLCall(glDeleteBuffers(1, &m_VBOID))
	GLCall(glDeleteVertexArrays(1, &m_VAOID));
}

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

template <typename VertexType>
void VertexArrayObject<VertexType>::render()
{
	GLCall(glBindVertexArray(m_VAOID));
	GLCall(glDrawElements(m_bufferType, m_indexCount, GL_UNSIGNED_INT, 0));
}

template <typename VertexType>
void VertexArrayObject<VertexType>::bind() const { GLCall(glBindVertexArray(m_VAOID)); }

template <typename VertexType>
void VertexArrayObject<VertexType>::unbind() const { GLCall(glBindVertexArray(0)); }

//=============================================================================================================================================//
//  Vertex.					  																												   //
//=============================================================================================================================================//

template <typename VertexType>
void VertexArrayObject<VertexType>::appendDataCPU(std::vector<VertexType>& vertices, std::vector<unsigned> indices)
{
	m_vertexCPU.insert(m_vertexCPU.end(), vertices.begin(), vertices.end());
	for (unsigned& ind : indices) { ind += m_vertexCount; }
	m_indexCPU.insert(m_indexCPU.end(), indices.begin(), indices.end());
	m_vertexCount += vertices.size();
	m_indexCount += indices.size();
	m_isUpdated = false;
}

//=============================================================================================================================================//
//  Entity.																																	   //
//=============================================================================================================================================//

template <typename VertexType>
void VertexArrayObject<VertexType>::appendDataCPU(Entity<VertexType>* entity)
{
	m_entityCPU.push_back(entity);
	entity->m_bufferStartIndex = m_vertexCount;
	entity->m_indecesStartIndex = m_indexCount;
	entity->offsetIndices(m_vertexCount);
	m_vertexCount += entity->m_vertexCount;
	m_indexCount += entity->m_indexCount;
	m_isUpdated = false;
}

template <typename VertexType>
void VertexArrayObject<VertexType>::assignDataGPU(Entity<VertexType>* entity)
{
	// Make sure all of the data is on the GPU side.
	if (m_isUpdated)
	{
		unsigned index = entity->m_bufferStartIndex;
		// Bind VBO.
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
		// Populate with vertex data.
		for (VertexType& vertex : entity->m_vertices)
		{
			// Write the data to the buffer.
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex.getTotalSize(), vertex.getDataSize(), vertex.dataGL()));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex.getTotalSize() + vertex.getIDOffset(), vertex.getIDSize(), vertex.idGL()));
			index += 1;
		}
	}
	else { updateGPU(); }
}

template <typename VertexType>
void VertexArrayObject<VertexType>::deleteDataCPU(Entity<VertexType>* entity)
{
	// Find entity that has to be deleted.
	auto iterator = std::find(m_entityCPU.begin(), m_entityCPU.end(), entity);
	// Check if the entity was found.
	if (iterator != m_entityCPU.end())
	{
		// Find index based on the iterator.
		int index = std::distance(m_entityCPU.begin(), iterator);
		// Delete entity entry.
		m_entityCPU.erase(m_entityCPU.begin() + index);
		// Update the buffer indeces of the entities.
		for (int i = index; i < m_entityCPU.size(); i++)
		{
			m_entityCPU[i]->m_bufferStartIndex -= entity->m_vertexCount;
			m_entityCPU[i]->m_indecesStartIndex -= entity->m_indexCount;
			m_entityCPU[i]->offsetIndices(-1*(entity->m_vertexCount));
		}
		m_vertexCount -= entity->m_vertexCount;
		m_indexCount -= entity->m_indexCount;
		m_isUpdated = false;
	}
	// Entity was not found.
	else { std::cout << yellow << "\n[OPENGL] [WARNING]: " << white << "Tried to delete entity, but it is not in the list."; }
}

//=============================================================================================================================================//
//  Memory management.																														   //
//=============================================================================================================================================//

template <typename VertexType>
void VertexArrayObject<VertexType>::updateGPU()
{
	// ----------------- //
	//  E N T I T I E S  //
	// ----------------- //
	if (m_entityCPU.size())
	{
		// Resize VBO.
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, m_vertexCount * m_entityCPU[0]->m_vertices[0].getTotalSize(), NULL, GL_DYNAMIC_DRAW));
		// Resize IBO.
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(GLuint), NULL, GL_DYNAMIC_DRAW));
		// Reset the buffer pointerss
		unsigned int verticesIndex = 0;
		unsigned int indicesIndex = 0;
		// Populate with vertex data.
		for (Entity<VertexType>* entity : m_entityCPU)
		{
			// Write to the IBO
			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOID));
			GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indicesIndex * sizeof(GLuint), entity->m_indexCount * sizeof(GLuint), static_cast<const void*>(&(entity->m_indices[0]))));
			indicesIndex += entity->m_indexCount;
			// Write to VBO.
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
			for (VertexType& vertex : entity->m_vertices)
			{
				GLCall(glBufferSubData(GL_ARRAY_BUFFER, verticesIndex * vertex.getTotalSize(), vertex.getDataSize(), vertex.dataGL()));
				GLCall(glBufferSubData(GL_ARRAY_BUFFER, verticesIndex * vertex.getTotalSize() + vertex.getIDOffset(), vertex.getIDSize(), vertex.idGL()));
				verticesIndex += 1;
			}
		}
	}
	// ----------------- //
	//  V E R T I C E S  //
	// ----------------- //
	else if (m_vertexCPU.size())
	{
		// Reset the buffer pointer.
		unsigned int index = 0;
		// Resize VBO.
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, (m_vertexCount) * m_vertexCPU[0].getTotalSize(), NULL, GL_DYNAMIC_DRAW));
		// Write the data to the buffers.
		for (VertexType& vertex : m_vertexCPU)
		{
			// VBO.
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex.getTotalSize(), vertex.getDataSize(), vertex.dataGL()));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex.getTotalSize() + vertex.getIDOffset(), vertex.getIDSize(), vertex.idGL()));
			index += 1;
		}
		// Resize IBO and write data.
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount*sizeof(GLuint), static_cast<const void*>(&m_indexCPU[0]), GL_DYNAMIC_DRAW));
	}
	m_isUpdated = true;
}

template <typename VertexType>
void VertexArrayObject<VertexType>::wipeCPU()
{
	if (m_vertexCPU.size()) { m_vertexCPU.clear(); m_vertexCPU.shrink_to_fit(); }
	if (m_entityCPU.size()) { m_entityCPU.clear(); m_entityCPU.shrink_to_fit(); }
	m_isUpdated = false;
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