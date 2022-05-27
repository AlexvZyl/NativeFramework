//==============================================================================================================================================//
//  Includes.																															        //
//==============================================================================================================================================//

#include "IOIndicator.h"
#include "Port.h"

//==============================================================================================================================================//
//  Methods.																															        //
//==============================================================================================================================================//

IOIndicator::IOIndicator(PortType type, PortPosition position, VertexArrayObject<VertexData, IndexData3>* VAO, Port* parent)
	: Primitive(parent),
	  m_portType(type),
	  m_portPosition(position)
{
	// General setup.
	m_trackedCenter = glm::vec3(0.f, 0.f, 0.f);
	m_colour = parent->indicatorColour;//glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	m_VAO = VAO;

	// ----------------- //
	//  V E R T I C E S  //
	// ----------------- //

	std::vector<VertexData> vertexVector;
	vertexVector.reserve(4);
	std::vector<glm::vec3> vertices;
	vertices.reserve(4);
	vertices.emplace_back(glm::vec3(0.f, 0.008f, 0.9f));
	vertices.emplace_back(glm::vec3(0.f, -0.008f, 0.9f));
	vertices.emplace_back(glm::vec3(0.008f, 0.f, 0.9f));
	vertices.emplace_back(glm::vec3(-0.008f, 0.f, 0.9f));
	for (glm::vec3 vertex : vertices)
		vertexVector.emplace_back(VertexData(vertex, m_colour, m_entityID));
	m_vertexCount = vertexVector.size();

	// --------------- //
	//  I N D I C E S  //
	// --------------- //

	std::vector<unsigned> indices;
		
	// Set the vertices based on the Port type and position.
	if ((m_portType == PortType::PORT_IN && m_portPosition == PortPosition::LEFT) || (m_portType == PortType::PORT_OUT && m_portPosition == PortPosition::RIGHT)) 
	{
		// Draw a right pointing triangle.
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
	}
	else if ((m_portType == PortType::PORT_OUT && m_portPosition == PortPosition::LEFT) || (m_portType == PortType::PORT_IN && m_portPosition == PortPosition::RIGHT)) 
	{
		// Draw a left pointing triangle.
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(3);
	}
	if ((m_portType == PortType::PORT_IN && m_portPosition == PortPosition::BOTTOM) || (m_portType == PortType::PORT_OUT && m_portPosition == PortPosition::TOP)) 
	{
		// Draw a up pointing triangle.
		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(2);
	}
	else if ((m_portType == PortType::PORT_IN && m_portPosition == PortPosition::TOP) || (m_portType == PortType::PORT_OUT && m_portPosition == PortPosition::BOTTOM)) 
	{
		// Draw a down pointing triangle.
		indices.push_back(3);
		indices.push_back(1);
		indices.push_back(2);
	}
	else 
	{
		// Handle PORT_INOUT.
	}
	m_indexCount = indices.size();
	translateTo(parent->centre);

	m_VAO->pushPrimitive(this, vertexVector, indices);
}

void IOIndicator::setType(PortType type, PortPosition position)
{
	wipeGPU();

	m_portType = type;
	m_portPosition = position;

	// ----------------- //
	//  V E R T I C E S  //
	// ----------------- //

	std::vector<VertexData> vertexVector;
	std::vector<glm::vec3> vertices;
	vertices.push_back(glm::vec3(0.f, 0.008f, 0.9f));
	vertices.push_back(glm::vec3(0.f, -0.008f, 0.9f));
	vertices.push_back(glm::vec3(0.008f, 0.f, 0.9f));
	vertices.push_back(glm::vec3(-0.008f, 0.f, 0.9f));
	for (glm::vec3 vertex : vertices)
		vertexVector.emplace_back(VertexData(vertex, m_colour, m_entityID));
	m_vertexCount = vertexVector.size();

	// --------------- //
	//  I N D I C E S  //
	// --------------- //

	std::vector<unsigned> indices;
	indices.reserve(3);

	// Set the vertices based on the Port type and position
	if ((m_portType == PortType::PORT_IN && m_portPosition == PortPosition::LEFT) || (m_portType == PortType::PORT_OUT && m_portPosition == PortPosition::RIGHT)) 
	{
		// Draw a right pointing triangle.
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
	}
	else if ((m_portType == PortType::PORT_OUT && m_portPosition == PortPosition::LEFT) || (m_portType == PortType::PORT_IN && m_portPosition == PortPosition::RIGHT)) 
	{
		// Draw a left pointing triangle.
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(3);
	}
	if ((m_portType == PortType::PORT_IN && m_portPosition == PortPosition::BOTTOM) || (m_portType == PortType::PORT_OUT && m_portPosition == PortPosition::TOP)) 
	{
		// Draw a up pointing triangle.
		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(2);
	}
	else if ((m_portType == PortType::PORT_IN && m_portPosition == PortPosition::TOP) || (m_portType == PortType::PORT_OUT && m_portPosition == PortPosition::BOTTOM)) 
	{
		// Draw a down pointing triangle.
		indices.push_back(3);
		indices.push_back(1);
		indices.push_back(2);
	}
	else 
	{
		// Handle PORT_INOUT.
	}
	m_indexCount = indices.size();
	m_VAO->pushPrimitive(this, vertexVector, indices);
}

//==============================================================================================================================================//
//  EOF.																																        //
//==============================================================================================================================================//