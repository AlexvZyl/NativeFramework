//==============================================================================================================================================//
//  Includes.																															        //
//==============================================================================================================================================//

#include "IOIndicator.h"
#include "Port.h"

//==============================================================================================================================================//
//  Methods.																															        //
//==============================================================================================================================================//

IOIndicator::IOIndicator(PortType type, PortPosition position, GraphicsTrianglesBuffer<VertexData>* gtb, Port* parent)
	: Primitive(parent),
	m_portType(type),
	m_portPosition(position)
{
	// General setup.
	m_colour = parent->indicatorColour;
	setGraphicsBuffer(gtb);

	// ----------------- //
	//  V E R T I C E S  //
	// ----------------- //

	m_vertexCount = 4;
	const VertexData vertices[4] = {
		{ { 0.f, 0.008f, 0.9f  }, m_colour, m_entityID },
		{ { 0.f, -0.008f, 0.9f }, m_colour, m_entityID },
		{ { 0.008f, 0.f, 0.9f  }, m_colour, m_entityID },
		{ { -0.008f, 0.f, 0.9f }, m_colour, m_entityID }
	};

	// --------------- //
	//  I N D I C E S  //
	// --------------- //

	// Default indices.
	m_indexCount = 2;
	UInt3 indices[2] = {
		{0, 1, 2},
		{0, 2, 3}
	};

	// In these cases we need different indices.
	if ((m_portType == PortType::PORT_OUT && m_portPosition == PortPosition::LEFT) || (m_portType == PortType::PORT_IN && m_portPosition == PortPosition::RIGHT))
	{
		indices[0] = { 0, 1, 3 };
	}
	if ((m_portType == PortType::PORT_IN && m_portPosition == PortPosition::TOP) || (m_portType == PortType::PORT_OUT && m_portPosition == PortPosition::BOTTOM))
	{
		indices[1] = { 3, 1, 2 };
	}
	else
	{
		// Handle PORT_INOUT.
	}

	// Push data and translate to correct position.
	pushToGraphicsBuffer(vertices, 4, indices, 2);
	translateTo(parent->centre);
}

void IOIndicator::setType(PortType type, PortPosition position)
{
	removeFromGraphicsBuffer();
	m_portType = type;
	m_portPosition = position;

	// ----------------- //
	//  V E R T I C E S  //
	// ----------------- //

	m_vertexCount = 4;
	const VertexData vertices[4] = {
		{ { 0.f, 0.008f, 0.9f  }, m_colour, m_entityID },
		{ { 0.f, -0.008f, 0.9f }, m_colour, m_entityID },
		{ { 0.008f, 0.f, 0.9f  }, m_colour, m_entityID },
		{ { -0.008f, 0.f, 0.9f }, m_colour, m_entityID }
	};

	// --------------- //
	//  I N D I C E S  //
	// --------------- //

	// Default indices.
	m_indexCount = 2;
	UInt3 indices[2] = {
		{0, 1, 2},
		{0, 2, 3}
	};

	// In these cases we need different indices.
	if ((m_portType == PortType::PORT_OUT && m_portPosition == PortPosition::LEFT) || (m_portType == PortType::PORT_IN && m_portPosition == PortPosition::RIGHT))
	{
		indices[0] = { 0, 1, 3 };
	}
	if ((m_portType == PortType::PORT_IN && m_portPosition == PortPosition::TOP) || (m_portType == PortType::PORT_OUT && m_portPosition == PortPosition::BOTTOM))
	{
		indices[1] = { 3, 1, 2 };
	}
	else
	{
		// Handle PORT_INOUT.
	}

	// Push data and translate to correct position.
	pushToGraphicsBuffer(vertices, 4, indices, 2);
	translateTo(dynamic_cast<Port*>(m_parent)->centre);
}

//==============================================================================================================================================//
//  EOF.																																        //
//==============================================================================================================================================//