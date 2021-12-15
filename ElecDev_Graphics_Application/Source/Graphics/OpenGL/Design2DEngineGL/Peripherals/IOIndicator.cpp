#include "IOIndicator.h"
#include "Port.h"

IOIndicator::IOIndicator(PortType type, PortPosition position, VertexArrayObject<VertexData>* VAO, Port* parent):
	Primitive(parent), 
	m_portType(type), 
	m_portPosition(position)
{

	// General setup.
	m_trackedCenter = glm::vec3(0.f, 0.f, 0.f);
	m_colour = parent->indicatorColour;//glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	m_VAO = VAO;

	std::vector<glm::vec3> vertices;

	vertices.push_back(glm::vec3(0.f, 0.008f, 0.9f));
	vertices.push_back(glm::vec3(0.f, -0.008f, 0.9f));
	vertices.push_back(glm::vec3(0.008f, 0.f, 0.9f));
	vertices.push_back(glm::vec3(-0.008f, 0.f, 0.9f));

	for (glm::vec3 vertex : vertices)
		m_vertices.push_back(VertexData(vertex, m_colour, m_entityID));
	m_vertexCount = m_vertices.size();
		
	//set the vertices based on the Port type and position
	if ((m_portType == PORT_IN && m_portPosition == PortPosition::LEFT) || (m_portType == PORT_OUT && m_portPosition == PortPosition::RIGHT)) {
		//Draw a right pointing triangle
		m_indices.push_back(0);
		m_indices.push_back(1);
		m_indices.push_back(2);
	}
	else if ((m_portType == PORT_OUT && m_portPosition == PortPosition::LEFT) || (m_portType == PORT_IN && m_portPosition == PortPosition::RIGHT)) {
		//Draw a left pointing triangle
		m_indices.push_back(0);
		m_indices.push_back(1);
		m_indices.push_back(3);
	}
	if ((m_portType == PORT_IN && m_portPosition == PortPosition::BOTTOM) || (m_portType == PORT_OUT && m_portPosition == PortPosition::TOP)) {
		//Draw a up pointing triangle
		m_indices.push_back(0);
		m_indices.push_back(3);
		m_indices.push_back(2);
	}
	else if ((m_portType == PORT_IN && m_portPosition == PortPosition::TOP) || (m_portType == PORT_OUT && m_portPosition == PortPosition::BOTTOM)) {
		//Draw a down pointing triangle
		m_indices.push_back(3);
		m_indices.push_back(1);
		m_indices.push_back(2);
	}
	else {
		//Handle PORT_INOUT

	}
	m_indexCount = m_indices.size();
	translateTo(parent->centre);
}

void IOIndicator::setType(PortType type, PortPosition position)
{
	m_portType = type;
	m_portPosition = position;
	m_indices.clear();
	//set the vertices based on the Port type and position
	if ((m_portType == PORT_IN && m_portPosition == PortPosition::LEFT) || (m_portType == PORT_OUT && m_portPosition == PortPosition::RIGHT)) {
		//Draw a right pointing triangle
		m_indices.push_back(0);
		m_indices.push_back(1);
		m_indices.push_back(2);
	}
	else if ((m_portType == PORT_OUT && m_portPosition == PortPosition::LEFT) || (m_portType == PORT_IN && m_portPosition == PortPosition::RIGHT)) {
		//Draw a left pointing triangle
		m_indices.push_back(0);
		m_indices.push_back(1);
		m_indices.push_back(3);
	}
	if ((m_portType == PORT_IN && m_portPosition == PortPosition::BOTTOM) || (m_portType == PORT_OUT && m_portPosition == PortPosition::TOP)) {
		//Draw a up pointing triangle
		m_indices.push_back(0);
		m_indices.push_back(3);
		m_indices.push_back(2);
	}
	else if ((m_portType == PORT_IN && m_portPosition == PortPosition::TOP) || (m_portType == PORT_OUT && m_portPosition == PortPosition::BOTTOM)) {
		//Draw a down pointing triangle
		m_indices.push_back(3);
		m_indices.push_back(1);
		m_indices.push_back(2);
	}
	else {
		//Handle PORT_INOUT

	}
	m_indices.shrink_to_fit();
	m_indexCount = m_indices.size();
}
