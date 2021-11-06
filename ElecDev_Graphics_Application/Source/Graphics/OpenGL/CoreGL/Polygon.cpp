#include "Polygon.h"



Polygon2D::Polygon2D(std::vector<glm::vec3> vertices, VertexArrayObject* VAO)
{
	//set it all up
	m_pos = glm::vec2(0.f, 0.f);
	m_colour = Colour(1.f, 0.f, 0.f, 0.5f);
	n_vertices = vertices.size();
	m_VAO = VAO;
	// Populate VertexData structures.
	for (int i = 1; i < n_vertices-1; i++) {
		m_vertices.insert(m_vertices.end(), VertexData(vertices[0], m_colour, eID));
		m_vertices.insert(m_vertices.end(), VertexData(vertices[i], m_colour, eID));
		m_vertices.insert(m_vertices.end(), VertexData(vertices[i+1], m_colour, eID));
	}

	//pass to VAO
	m_VAO->appendDataCPU(this);
}
