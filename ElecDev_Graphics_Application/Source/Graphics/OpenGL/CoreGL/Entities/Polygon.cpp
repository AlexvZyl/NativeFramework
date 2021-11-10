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

Polygon2D::Polygon2D(std::vector<glm::vec3> vertices, VertexArrayObject* VAO)
{
	m_trackedCenter = glm::vec3(0.f, 0.f, 0.f);
	m_colour = glm::vec4(1.f, 0.f, 0.f, 0.5f);
	n_vertices = vertices.size();
	m_VAO = VAO;
	m_entityID = EntityManager::generateEID();

	// Populate VertexData structures.
	if (m_VAO->m_bufferType == GL_TRIANGLES) {
		//Tile the polygon in triangles sharing vertex 0.
		for (int i = 1; i < n_vertices - 1; i++) {
			m_vertices.insert(m_vertices.end(), std::make_shared<VertexData>(vertices[0], m_colour, m_entityID));
			m_vertices.insert(m_vertices.end(), std::make_shared<VertexData>(vertices[i], m_colour, m_entityID));
			m_vertices.insert(m_vertices.end(), std::make_shared<VertexData>(vertices[i + 1], m_colour, m_entityID));
		}
	}
	else if (m_VAO->m_bufferType == GL_LINES) {
		//add lines in a loop
		for (int i = 0; i < n_vertices - 1; i++) {
			m_vertices.insert(m_vertices.end(), std::make_shared<VertexData>(vertices[i], m_colour, m_entityID));
			m_vertices.insert(m_vertices.end(), std::make_shared<VertexData>(vertices[i + 1], m_colour, m_entityID));
		}
		//close loop
		m_vertices.insert(m_vertices.end(), std::make_shared<VertexData>(vertices[n_vertices - 1], m_colour, m_entityID));
		m_vertices.insert(m_vertices.end(), std::make_shared<VertexData>(vertices[0], m_colour, m_entityID));
	}

	// Pass to VAO.
	n_vertices = m_vertices.size();
	m_VAO->appendDataCPU(this);
	m_VAO->updateGPU();
}

Polygon2D::~Polygon2D() 
{

}

void Polygon2D::setLayer(float layer)
{
	for (int i = 0; i < n_vertices; i++) {
		m_vertices[i]->position.z = layer;
	}
	m_VAO->assignDataGPU(this);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
