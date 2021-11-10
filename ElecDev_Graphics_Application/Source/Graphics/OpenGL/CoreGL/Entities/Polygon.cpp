//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Vertex.h"
#include "Polygon.h"
#include "CoreGL/VertexArrayObjectGL.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																										       	   //
//=============================================================================================================================================//

Polygon2D::Polygon2D(std::vector<glm::vec3> vertices, VertexArrayObject* VAO)
{
	// Set it all up.
	m_trackedCenter = glm::vec3(0.f, 0.f, 0.f);
	m_globalColor = glm::vec4(1.f, 0.f, 0.f, 0.5f);
	n_vertices = vertices.size();
	m_VAO = VAO;

	// Populate VertexData structures.
	for (int i = 1; i < n_vertices-1; i++) {
		m_vertices.insert(m_vertices.end(), std::make_shared<VertexData>(vertices[0], m_globalColor, m_globalEntityID));
		m_vertices.insert(m_vertices.end(), std::make_shared<VertexData>(vertices[i], m_globalColor, m_globalEntityID));
		m_vertices.insert(m_vertices.end(), std::make_shared<VertexData>(vertices[i+1], m_globalColor, m_globalEntityID));
	}

	// Pass to VAO.
	n_vertices = m_vertices.size();
	m_VAO->appendDataCPU(this);
	m_VAO->updateGPU();
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
