//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Vertex.h"
#include "Polygon.h"
#include "EntityManager.h"
#include "CoreGL/VertexArrayObjectGL.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																										       	   //
//=============================================================================================================================================//

Polygon2D::Polygon2D(std::vector<glm::vec3> vertices, VertexArrayObject<VertexData>* VAO, Entity* parent) 
	: Primitive<VertexData>(parent)
{
	// General setup.
	m_trackedCenter = glm::vec3(0.f, 0.f, 0.f);
	m_colour = glm::vec4(1.f, 0.f, 0.f, 0.5f);
	m_vertexCount = vertices.size();
	m_VAO = VAO;
	std::vector<VertexData> vertexVector;
	std::vector<unsigned> indices;

	// Create the vertices.
	for (glm::vec3 vertex : vertices)
	{
		vertexVector.push_back(VertexData(vertex, m_colour, m_entityID));
	}

	// Assign the indices based on the VAO type.
	if (m_VAO->m_bufferType == GL_TRIANGLES)
	{
		for (int i = 2; i < m_vertexCount; i++) 
    {
			m_indices.push_back(0);
			m_indices.push_back(i-1);
			m_indices.push_back(i);
		}
	}
	else if (m_VAO->m_bufferType == GL_LINES)
	{
		for (int i = 1; i < m_vertexCount; i++) 
    {
			m_indices.push_back(i -1);
			m_indices.push_back(i);
		}
		m_indices.push_back(0);
		m_indices.push_back(m_vertexCount-1);
	}
	m_indexCount = m_indices.size();
    
	// Pass to VAO.
	m_VAO->appendVertexData(vertexVector, indices, &m_vertexBufferPos, &m_indexBufferPos);
	m_VAO->pushPrimitive(this);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
