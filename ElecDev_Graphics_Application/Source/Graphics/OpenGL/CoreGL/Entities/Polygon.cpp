//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Polygon.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																										       	   //
//=============================================================================================================================================//

Polygon2D::Polygon2D(std::vector<glm::vec3> vertices, VertexArrayObject* VAO)
{
	//set it all up
	m_pos = glm::vec2(0.f, 0.f);
	m_colour = Colour(1.f, 0.f, 0.f, 0.5f);
	n_vertices = vertices.size();
	m_VAO = VAO;
	// Populate VertexData structures.
	for (int i = 1; i < n_vertices-1; i++) {
		m_vertices.insert(m_vertices.end(), VertexData(vertices[0], m_colour, m_eID));
		m_vertices.insert(m_vertices.end(), VertexData(vertices[i], m_colour, m_eID));
		m_vertices.insert(m_vertices.end(), VertexData(vertices[i+1], m_colour, m_eID));
	}
	//pass to VAO
	n_vertices = m_vertices.size();
	m_VAO->appendDataCPU(this);
	m_VAO->updateGPU();
}

Polygon2D::~Polygon2D() 
{

}

void Polygon2D::update()
{
	m_VAO->assignDataGPU(this);
}

void Polygon2D::translate(glm::vec2 transVec)
{
	for (int i = 0; i < n_vertices; i++) {
		m_vertices[i].position += glm::vec3(transVec, 0);
	}
	m_pos += transVec;
	m_VAO->assignDataGPU(this);
}

void Polygon2D::translateTo(glm::vec2 pos)
{
	translate(pos - m_pos);
}

void Polygon2D::rotate(float angle)
{
}

void Polygon2D::setColour(Colour col)
{
	m_colour = col;
	for (int i = 0; i < n_vertices; i++) {
		m_vertices[i].color = m_colour;
	}

}

void Polygon2D::setEID(unsigned eID)
{
	m_eID = eID;
	for (int i = 0; i < n_vertices; i++) {
		m_vertices[i].entityID[0] = m_eID;
	}
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
