//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "VertexArrayObjectGL.h"
#include "Entities/Entity.h"
#include "External/Misc/ConsoleColor.h"
#include "CoreGL/Entities/Vertex.h"
#include <algorithm> // for std::copy
#include "ErrorHandlerGL.h"

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

	// ----------------- //
	//  V E R T I C E S  //
	// ----------------- //

	if (typeid(VertexType)==typeid(VertexData))
	{
		int vertexSize = sizeof(VertexData::position) + sizeof(VertexData::color) + sizeof(VertexData::entityID);
		int colOffset = sizeof(VertexData::position);
		int idOffset = sizeof(VertexData::position) + sizeof(VertexData::color);
		// Generate a VBO for the VAO.
		GLCall(glGenBuffers(1, &m_VBOID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
		// Potition.
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
		// Generate a VBO for the VAO.
		GLCall(glGenBuffers(1, &m_VBOID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
		// Bind Vertex position attribute.
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(VertexDataTextured::position);
		// Bind Vertex color attribute.
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 1));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(VertexDataTextured::color);
		// Bind texture position attribute
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 2));
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(VertexDataTextured::textureCoords);
		// Bind texture ID attribute
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
		// Generate a VBO for the VAO.
		GLCall(glGenBuffers(1, &m_VBOID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
		// Bind Vertex position attribute.
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(VertexDataCircle::position);
		// Bind Vertex color attribute.
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 1));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(VertexDataCircle::color);
		// Local coords attribute.
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 1));
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(VertexDataCircle::localCoords);
		// Circle thickness attribute.
		GLCall(glEnableVertexArrayAttrib(m_VAOID, 2));
		GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
		offset += sizeof(VertexDataCircle::thickness);
		// Circle fade attribute.
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
	// Clear memory on CPU.
	wipeCPU();
	// Delete VBO.
	GLCall(glDeleteBuffers(1, &m_VBOID))
	// Delete VAO.
	GLCall(glDeleteVertexArrays(1, &m_VAOID));
}

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

template <typename VertexType>
void VertexArrayObject<VertexType>::render()
{
	// Bind the VAO.
	GLCall(glBindVertexArray(m_VAOID));
	// Draw call.
	GLCall(glDrawArrays(m_bufferType, 0, m_bufferIndex));
}

template <typename VertexType>
void VertexArrayObject<VertexType>::bind() const { GLCall(glBindVertexArray(m_VAOID)); }

template <typename VertexType>
void VertexArrayObject<VertexType>::unbind() const { GLCall(glBindVertexArray(0)); }

//=============================================================================================================================================//
//  Vertex.					  																												   //
//=============================================================================================================================================//

template <typename VertexType>
void VertexArrayObject<VertexType>::appendDataCPU(std::vector<VertexType>* vertices)
{
	m_vertexCPU.insert(m_vertexCPU.begin(), vertices->begin(), vertices->end());
	m_bufferIndex += vertices->size();
	m_isUpdated = false;
}
template <typename VertexType>
void VertexArrayObject<VertexType>::assignDataCPU(std::vector<VertexType>* vertices, unsigned int index)
{
	// Create the VAO if it is empty.
	if (!m_bufferIndex) { appendDataCPU(vertices); }
	// Otherwise replace current data.
	else { m_vertexCPU.insert(m_vertexCPU.begin()+index, vertices->begin(), vertices->end()); }
}
template <typename VertexType>
void VertexArrayObject<VertexType>::assignDataGPU(std::vector<VertexType>* vertices, unsigned int index)
{
	// Bind VBO.
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
	// Populate with vertex data.
	for (Vertex& vertex : *vertices)
	{
		// Write the data to the buffer.
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex.getTotalSize(), vertex.getDataSize(), vertex.dataGL()));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex.getIDOffset(), vertex.getIDSize(), vertex.idGL()));
		index += 1;
	}
}

//=============================================================================================================================================//
//  Entity.																																	   //
//=============================================================================================================================================//

template <typename VertexType>
void VertexArrayObject<VertexType>::appendDataCPU(Entity<VertexType>* entity)
{
	// Add the polygon to the vector.
	m_entityCPU.push_back(entity);
	entity->m_bufferStartIndex = m_bufferIndex;
	m_bufferIndex += entity->m_vertices.size();
	m_isUpdated = false;
}

template <typename VertexType>
void VertexArrayObject<VertexType>::assignDataGPU(Entity<VertexType>* entity)
{
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
		// Change buffer index to the end of the untouched entity.
		if (index != 0) { m_bufferIndex = m_entityCPU[index - 1]->m_bufferStartIndex + m_entityCPU[index - 1]->m_vertices.size(); }
		else { m_bufferIndex = 0; }
		// Update the buffer indeces of the entities.
		for (int i = index; i < m_entityCPU.size(); i++)
		{
			m_entityCPU[i]->m_bufferStartIndex = m_bufferIndex;
			m_bufferIndex += m_entityCPU[i]->m_vertices.size();
		}
		m_bufferIndex += m_vertexCPU.size();
		m_isUpdated = false;
	}
	// Entity was not found.
	else { std::cout << red << "\n[OPENGL] [ERROR]: " << white << "Tried to delete entity, but it is not in the list."; }
}

//=============================================================================================================================================//
//  Memory management.																														   //
//=============================================================================================================================================//

template <typename VertexType>
void VertexArrayObject<VertexType>::updateGPU()
{
	// --------------------------------------- //
	//  V E R T I C E S   &   E N T I T I E S  //
	// --------------------------------------- //
	if (m_vertexCPU.size() && m_entityCPU.size())
	{
		// Calculate the size of the new VBO.
		unsigned int vertexCount = 0;
		for (Entity<VertexType>* entity : m_entityCPU)
		{ vertexCount += entity->m_vertices.size(); }		// Add entities vertices.
		vertexCount += m_vertexCPU.size();					// Add normal vertices.
		// Reset the buffer pointer.
		unsigned int index = 0;
		// Bind VBO.
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
		// Define buffer size.
		GLCall(glBufferData(GL_ARRAY_BUFFER, vertexCount * m_vertexCPU[0].getTotalSize(), NULL, GL_DYNAMIC_DRAW))
		// Populate with entity vertex data.
		for (Entity<VertexType>* entity : m_entityCPU)
		{
			for (VertexType& vertex : entity->m_vertices)
			{
				// Write the data to the buffer.
				GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex.getTotalSize(), vertex.getDataSize(), vertex.dataGL()));
				GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex.getTotalSize() + vertex.getIDOffset(), vertex.getIDSize(), vertex.idGL()));
				index += 1;
			}
		}
		// Populate with normal vertex data.
		for (VertexType& vertex : m_vertexCPU)
		{
			// Write the data to the buffer.
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex.getTotalSize(), vertex.getDataSize(), vertex.dataGL()));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex.getTotalSize() + vertex.getIDOffset(), vertex.getIDSize(), vertex.idGL()));
			index += 1;
		}
	}
	// ----------------- //
	//  V E R T I C E S  //
	// ----------------- //
	else if (m_vertexCPU.size())
	{
		// Reset the buffer pointer.
		unsigned int index = 0;
		// Bind VBO.
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
		// Define buffer size.
		GLCall(glBufferData(GL_ARRAY_BUFFER, (m_bufferIndex) * m_vertexCPU[0].getTotalSize(), NULL, GL_DYNAMIC_DRAW));
		// Populate with vertex data.
		for (VertexType& vertex : m_vertexCPU)
		{
			// Write the data to the buffer.
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex.getTotalSize(), vertex.getDataSize(), vertex.dataGL()));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex.getTotalSize() + vertex.getIDOffset(), vertex.getIDSize(), vertex.idGL()));
			index += 1;
		}
	}
	// ----------------- //
	//  E N T I T I E S  //
	// ----------------- //
	else if (m_entityCPU.size())
	{
		// Calculate the size of the new VBO.
		unsigned int vertexCount = 0;
		for (Entity<VertexType>* entity : m_entityCPU) { vertexCount += entity->m_vertices.size(); }
		// Reset the buffer pointer.
		unsigned int index = 0;
		// Bind VBO.
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
		// Define buffer size.
		GLCall(glBufferData(GL_ARRAY_BUFFER, vertexCount * m_entityCPU[0]->m_vertices[0].getTotalSize(), NULL, GL_DYNAMIC_DRAW));
		// Populate with vertex data.
		for (Entity<VertexType>* entity : m_entityCPU)
		{
			for (VertexType& vertex : entity->m_vertices)
			{
				// Write the data to the buffer.
				GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex.getTotalSize(), vertex.getDataSize(), vertex.dataGL()));
				GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex.getTotalSize() + vertex.getIDOffset(), vertex.getIDSize(), vertex.idGL()));
				index += 1;
			}
		}
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