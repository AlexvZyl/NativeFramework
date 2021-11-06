#include "Polygon.h"



Polygon2D::Polygon2D(std::vector<glm::vec3> vertices, VertexArrayObject* VAO)
{
	//set it all up
	m_pos = glm::vec2(0.f, 0.f);
	m_colour = Colour(1.f, 0.f, 0.f, 0.5f);
	VertexData Vertices[4];

	// Populate VertexData structures.
	for (int i = 0; i < 4; i++) {
		m_vertices[i] = VertexData(vertices[i], m_colour, eID);
	}
	std::vector<VertexData> shapeVerticesVec = { m_vertices[0], m_vertices[1], m_vertices[2], m_vertices[2], m_vertices[3], m_vertices[0] };




	//pass to VAO
	VAO->appendDataCPU(this);
}
