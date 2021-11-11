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

VertexArrayObject::VertexArrayObject(GLenum type, bool textured, bool circle)
	: m_bufferType(type)
{
	// Generate the VAO.
	GLCall(glGenVertexArrays(1, &m_VAOID));
	GLCall(glBindVertexArray(m_VAOID));

	// Create untextured array.
	if (!textured)
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
	// Create textured array.
	else
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
}

VertexArrayObject::~VertexArrayObject()
{
	// Delete VBO.
	GLCall(glDeleteBuffers(1, &m_VBOID))
	// Delete VAO.
	GLCall(glDeleteVertexArrays(1, &m_VAOID));
}

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

void VertexArrayObject::render()
{
	// Bind the VAO.
	GLCall(glBindVertexArray(m_VAOID));
	// Draw call.
	GLCall(glDrawArrays(m_bufferType, 0, m_bufferIndex));
}

void VertexArrayObject::bind() const { GLCall(glBindVertexArray(m_VAOID)); }

void VertexArrayObject::unbind() const { GLCall(glBindVertexArray(0)); }

//=============================================================================================================================================//
//  Vertex.					  																												   //
//=============================================================================================================================================//

void VertexArrayObject::appendDataCPU(std::vector<Vertex*>* vertices)
{
	m_vertexCPU.insert(m_vertexCPU.begin(), vertices->begin(), vertices->end());
	m_bufferIndex += vertices->size();
}
void VertexArrayObject::assignDataCPU(std::vector<Vertex*>* vertices, unsigned int index)
{
	// Create the VAO if it is empty.
	if (!m_bufferIndex) { appendDataCPU(vertices); }
	// Otherwise replace current data.
	else { m_vertexCPU.insert(m_vertexCPU.begin()+index, vertices->begin(), vertices->end()); }
}

void VertexArrayObject::assignDataGPU(std::vector<Vertex*>* vertices, unsigned int index)
{
	// Bind VBO.
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
	// Populate with vertex data.
	for (Vertex* vertex : *vertices)
	{
		// Write the data to the buffer.
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex->totalSize, vertex->dataSize, vertex->dataGL()));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex->idOffset, vertex->idSize, vertex->idGL()));
		index += 1;
	}
}

//=============================================================================================================================================//
//  Entity.																																	   //
//=============================================================================================================================================//

void VertexArrayObject::appendDataCPU(Entity* entity)
{
	// Add the polygon to the vector.
	m_entityCPU.push_back(entity);
	entity->m_bufferStartIndex = m_bufferIndex;
	m_bufferIndex += entity->m_vertices.size();
}

void VertexArrayObject::assignDataGPU(Entity* entity)
{
	unsigned index = entity->m_bufferStartIndex;
	// Bind VBO.
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
	// Populate with vertex data.
	for (Vertex* vertex : entity->m_vertices)
	{
		// Write the data to the buffer.
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex->totalSize, vertex->dataSize, vertex->dataGL()));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex->totalSize + vertex->idOffset, vertex->idSize, vertex->idGL()));
		index += 1;
	}
}

void VertexArrayObject::deleteDataCPU(Entity* entity)
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
	}
	// Entity was not found.
	else { std::cout << red << "\n[OPENGL] [ERROR]: " << white << "Tried to delete entity, but it is not in the list."; }
}

//=============================================================================================================================================//
//  Memory management.																														   //
//=============================================================================================================================================//

void VertexArrayObject::updateGPU()
{
	// --------------------------------------- //
	//  V E R T I C E S   &   E N T I T I E S  //
	// --------------------------------------- //
	if (m_vertexCPU.size() && m_entityCPU.size())
	{
		// Calculate the size of the new VBO.
		unsigned int vertexCount = 0;
		for (Entity* entity : m_entityCPU)
		{ vertexCount += entity->m_vertices.size(); }		// Add entities vertices.
		vertexCount += m_vertexCPU.size();					// Add normal vertices.
		// Reset the buffer pointer.
		unsigned int index = 0;
		// Bind VBO.
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
		// Define buffer size.
		GLCall(glBufferData(GL_ARRAY_BUFFER, vertexCount * m_vertexCPU[0]->totalSize, NULL, GL_DYNAMIC_DRAW))
		// Populate with entity vertex data.
		for (Entity* entity : m_entityCPU)
		{
			for (Vertex* vertex : entity->m_vertices)
			{
				// Write the data to the buffer.
				GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex->totalSize, vertex->dataSize, vertex->dataGL()));
				GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex->totalSize + vertex->idOffset, vertex->idSize, vertex->idGL()));
				index += 1;
			}
		}
		// Populate with normal vertex data.
		for (Vertex* vertex : m_vertexCPU)
		{
			// Write the data to the buffer.
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex->totalSize, vertex->dataSize, vertex->dataGL()));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex->totalSize + vertex->idOffset, vertex->idSize, vertex->idGL()));
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
		GLCall(glBufferData(GL_ARRAY_BUFFER, (m_bufferIndex) * m_vertexCPU[0]->totalSize, NULL, GL_DYNAMIC_DRAW));
		// Populate with vertex data.
		for (Vertex* vertex : m_vertexCPU)
		{
			// Write the data to the buffer.
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex->totalSize, vertex->dataSize, vertex->dataGL()));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex->totalSize + vertex->idOffset, vertex->idSize, vertex->idGL()));
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
		for (Entity* entity : m_entityCPU) { vertexCount += entity->m_vertices.size(); }
		// Reset the buffer pointer.
		unsigned int index = 0;
		// Bind VBO.
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBOID));
		// Define buffer size.
		GLCall(glBufferData(GL_ARRAY_BUFFER, vertexCount * m_entityCPU[0]->m_vertices[0]->totalSize, NULL, GL_DYNAMIC_DRAW));
		// Populate with vertex data.
		for (Entity* entity : m_entityCPU) 
		{
			for (Vertex* vertex : entity->m_vertices)
			{
				// Write the data to the buffer.
				GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex->totalSize, vertex->dataSize, vertex->dataGL()));
				GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * vertex->totalSize + vertex->idOffset, vertex->idSize, vertex->idGL()));
				index += 1;
			}
		}
	}
}

void VertexArrayObject::wipeCPU()
{
	if (m_vertexCPU.size()) { m_vertexCPU.clear(); m_vertexCPU.shrink_to_fit(); }
	if (m_entityCPU.size()) { m_entityCPU.clear(); m_entityCPU.shrink_to_fit(); }
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//