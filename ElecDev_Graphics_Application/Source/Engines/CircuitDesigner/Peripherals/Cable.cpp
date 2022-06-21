//==============================================================================================================================================//
//  Includes.  																																    //
//==============================================================================================================================================//

#include "Cable.h"
#include "Circuit.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/SceneGL.h"
#include "Port.h"
#include "OpenGL/Primitives/LineSegment.h"
#include "Graphics/OpenGL/Primitives/PolyLine.h"
#include "Component2D.h"

unsigned Cable::cableID = 0;

//==============================================================================================================================================//
//  Constructor & Destructor.  																													//
//==============================================================================================================================================//

Cable::Cable(Port* startPort, Circuit* parent) 
	: Entity(EntityType::CABLE, parent)
{
	// Default dictionary.
	cableDict = {
		{ "FromTagNumber",		"From(Circuit Database)" },
		{ "ToTagNumber",		"From(Circuit Database)" },
		{ "From",				"From(Circuit Database)" },
		{ "To",					"From(Circuit Database)" },
		{ "Description",		"From(Circuit Database)" },
		{ "Voltage",			"From(Circuit Database)" },
		{ "Circuit",			"From(Circuit Database)" },
		{ "CableRating",		"From(Circuit Database)" },
		{ "CableIsolation",		"From(Circuit Database)" },
		{ "CableSuffix",		"From(Circuit Database)" },
		{ "CableLength",		"From(Circuit Database)" },
		{ "CableTAG",			"From(Circuit Database)" },
		{ "IndoorTermCount",	"From(Circuit Database)" },
		{ "OutdoorTermCount",	"From(Circuit Database)" },
		{ "CableSize",			"size()"				 },
		{ "CoreCount",			"From(Circuit Database)" },
		{ "Metal",				"From(Circuit Database)" },
		{ "DBRef",				"From(Circuit Database)" },
		{ "CableDiameter",		"From(Circuit Database)" },
		{ "CableMass",			"From(Circuit Database)" }
	};
	
	// Setup data.
	m_startPort = startPort;
	m_titleString = "Cable " + std::to_string(cableID++);
	startPort->attachCable(this);
	float initial_length = m_titleOffset;

	// First line.
	m_polyLine = Renderer::addPolyLine({ m_startPort->centre,  m_startPort->centre }, m_thickness, m_colour, true, this);

}

Cable::Cable(Port* startPort, std::vector<glm::vec2> nodeList, Port* endPort, Circuit* parent, std::string titleString)
	: Entity(EntityType::CABLE, parent)
{
	// Setup data.
	m_startPort = startPort;
	m_endPort = endPort;
	m_titleString = titleString;

	constructCable(startPort, nodeList, endPort);
}

Cable::Cable(const YAML::Node& node, Circuit* parent) 
	: Entity(EntityType::CABLE, parent)
{
	Port* startPort = nullptr;
	Port* endPort = nullptr;

	m_cableType = std::filesystem::path(node["Filename"].as<std::string>()).filename().stem().string();

	// Find indces.
	int startComponentIndex = node["Start Component Index"].as<int>();
	int endComponentIndex = node["End Component Index"].as<int>();
	int startPortIndex = node["Start Port Index"].as<int>();
	int endPortIndex = node["End Port Index"].as<int>();
	// Total components in the circuit.
	int totalComponents = parent->m_components.size();
	
	// Start port.
	if (startComponentIndex < totalComponents)
	{
		// Get total ports belonging to component.
		int startComponentPortCount = parent->m_components[startComponentIndex]->ports.size();
		// Get port.
		if (startPortIndex < startComponentPortCount)
			startPort = parent->m_components[startComponentIndex]->ports[startPortIndex].get();
	}

	// End port.
	if (endComponentIndex < totalComponents)
	{
		// Get total ports belonging to component.
		int endComponentPortCount = parent->m_components[endComponentIndex]->ports.size();
		// Get port.
		if (endPortIndex < endComponentPortCount)
			endPort = parent->m_components[endComponentIndex]->ports[endPortIndex].get();
	}
	
	// Load all of the nodes belonging to the cable.
	std::vector<glm::vec2> nodeVector;
	nodeVector.reserve(node["Nodes"].size()+2);
	for (const auto& nodeIt : node["Nodes"])
	{
		// Add to node vector.
		glm::vec2 node = { nodeIt.second[0].as<float>(), nodeIt.second[1].as<float>() };
		nodeVector.emplace_back(node);
	}
	
	// Call constructor with data.
	constructCable(startPort, nodeVector, endPort);

	// Dictionary.
	cableDict.clear();
	for (const auto& entry : node["Dictionary"])
		cableDict.insert({ entry.first.as<std::string>(), entry.second.as<std::string>() });

}

Cable::~Cable()
{
	// Remove the cable from the start and end ports.
	if (m_startPort) m_startPort->detachCable(this);
	if (m_endPort)   m_endPort->detachCable(this); 

	// Remove the renderer primitives.
	Renderer::remove(m_polyLine.get());
}

void Cable::constructCable(Port* startPort, std::vector<glm::vec2> nodeList, Port* endPort) 
{
	// Attach the ports
	m_startPort = startPort;
	m_endPort = endPort;
	if (m_startPort) 
	{
		m_startPort->attachCable(this);
		if (m_endPort) 
		{
			m_endPort->attachCable(this);
			// Set fromTag(s).
			if (m_endPort->m_type == PortType::PORT_OUT || m_endPort->m_type == PortType::PORT_INOUT) 
			{
				if (m_startPort->m_type == PortType::PORT_IN || m_startPort->m_type == PortType::PORT_INOUT) 
				{
					m_startPort->fromTag = &dynamic_cast<Component2D*>(m_endPort->m_parent)->m_toTagNumber;
					m_startPort->voltage = m_endPort->voltage;
				}
			}
			if (m_startPort->m_type == PortType::PORT_OUT || m_startPort->m_type == PortType::PORT_INOUT) 
			{
				if (m_endPort->m_type == PortType::PORT_IN || m_endPort->m_type == PortType::PORT_INOUT) 
				{
					m_endPort->fromTag = &dynamic_cast<Component2D*>(m_startPort->m_parent)->m_toTagNumber;
					m_endPort->voltage = m_startPort->voltage;
				}
			}
		}
	}

	m_polyLine = Renderer::addPolyLine(nodeList, m_thickness, m_colour, true, this);
}

//==============================================================================================================================================//
//  Methods.  																																    //
//==============================================================================================================================================//

void Cable::extendSegment(glm::vec2 nextPoint)
{
	// Extend the pevious segment. 
	m_polyLine->translateVertexAtIndexTo(m_polyLine->m_vertices.size() - 1, nextPoint);
}

void Cable::addSegment(glm::vec2 nextPoint)
{
	m_polyLine->pushVertex(nextPoint);
}

bool Cable::attach(Port* endPort)
{
	//check input/output relationship is respected
	if (endPort == m_startPort) return false;
	if (endPort->m_type != m_startPort->m_type || endPort->m_type == PortType::PORT_INOUT) 
	{
		// Keep pointer to end port.
		m_endPort = endPort;
		extendSegment(m_endPort->centre);
		setColour(m_colour);
		endPort->attachCable(this);

		// Set fromTag(s)
		if (m_endPort->m_type == PortType::PORT_OUT || m_endPort->m_type == PortType::PORT_INOUT) 
		{
			if (m_startPort->m_type == PortType::PORT_IN || m_startPort->m_type == PortType::PORT_INOUT) 
			{
				m_startPort->fromTag = &dynamic_cast<Component2D*>(m_endPort->m_parent)->m_toTagNumber;
				m_startPort->voltage = m_endPort->voltage;
			}
		}
		if (m_startPort->m_type == PortType::PORT_OUT || m_startPort->m_type == PortType::PORT_INOUT) 
		{
			if (m_endPort->m_type == PortType::PORT_IN || m_endPort->m_type == PortType::PORT_INOUT) 
			{
				m_endPort->fromTag = &dynamic_cast<Component2D*>(m_startPort->m_parent)->m_toTagNumber;
				m_endPort->voltage = m_startPort->voltage;
			}
		}
		return true;
	}
	return false;
}

void Cable::followPort(Port* movedPort)
{
	if (movedPort == m_endPort)
	{
		// Extend the pevious segment. 
		m_polyLine->translateVertexAtIndexTo(m_polyLine->m_vertices.size() - 1, movedPort->centre);
	}

	else if (movedPort == m_startPort)
	{
		// Extend the first segment. 
		m_polyLine->translateVertexAtIndexTo(0, movedPort->centre);
	}
}

void Cable::setColour(glm::vec4 colour, bool save)
{
	if(save) m_colour = colour;
	m_polyLine->setColor(colour);
}

void Cable::translateVertexAtIndex(unsigned vertexIdx, glm::vec2 translation)
{
	if (vertexIdx == 0 || vertexIdx == m_polyLine->m_vertices.size() - 1) 
	{
		// Don't allow the ends to be moved off the port.
		return;
	}
	m_polyLine->translateVertexAtIndex(vertexIdx, translation);
}

void Cable::translateVertexAtIndexTo(unsigned vertexIdx, glm::vec2 position)
{
	if (vertexIdx == 0 || vertexIdx == m_polyLine->m_vertices.size() - 1) 
	{
		// Don't allow the ends to be moved off the port.
		return;
	}
	m_polyLine->translateVertexAtIndexTo(vertexIdx, position);
}

void Cable::enableOutline()
{
	m_polyLine->enableOutline();
}

void Cable::disableOutline()
{
	m_polyLine->disableOutline();
}

std::tuple<unsigned, float>  Cable::getNearestVertexIdx(glm::vec2 pos)
{
	return m_polyLine->getNearestVertexIndex(pos);
}

//==============================================================================================================================================//
//  EOF.  																																		//
//==============================================================================================================================================//