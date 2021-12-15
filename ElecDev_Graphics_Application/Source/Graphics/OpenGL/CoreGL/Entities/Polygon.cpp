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

template<typename VertexType>
Polygon2D<VertexType>::Polygon2D(std::vector<glm::vec3> vertices, VertexArrayObject<VertexType>* VAO, Entity* parent) 
	: Primitive<VertexType>(parent)
{
	// General setup.
	m_trackedCenter = glm::vec3(0.f, 0.f, 0.f);
	m_colour = glm::vec4(1.f, 0.f, 0.f, 0.5f);
	m_vertexCount = vertices.size();
	m_VAO = VAO;

	// Create the vertices.
	for (glm::vec3 vertex : vertices)
		m_vertices.push_back(VertexData(vertex, m_colour, m_entityID));

	// Assign the indices based on the VAO type.
	if (m_VAO->m_bufferType == GL_TRIANGLES)
	{
		for (int i = 2; i < m_vertexCount; i++) {
			m_indices.push_back(0);
			m_indices.push_back(i-1);
			m_indices.push_back(i);
		}
	}
	else if (m_VAO->m_bufferType == GL_LINES)
	{
		for (int i = 1; i < m_vertexCount; i++) {
			m_indices.push_back(i -1);
			m_indices.push_back(i);
		}
		m_indices.push_back(0);
		m_indices.push_back(m_vertexCount-1);
	}
	m_indexCount = m_indices.size();
	// Pass to VAO.
	m_VAO->appendDataCPU(this);
}

//=============================================================================================================================================//
//  Instantiations.																															   //
//=============================================================================================================================================//

template class Polygon2D<VertexData>;

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
