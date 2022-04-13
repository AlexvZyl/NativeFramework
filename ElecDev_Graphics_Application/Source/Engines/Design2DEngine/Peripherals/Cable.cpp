//==============================================================================================================================================//
//  Includes.  																																    //
//==============================================================================================================================================//

#include "Cable.h"
#include "Circuit.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/SceneGL.h"

unsigned Cable::cableID = 0;

//==============================================================================================================================================//
//  Constructor & Destructor.  																													//
//==============================================================================================================================================//

Cable::Cable(Port* startPort, Circuit* parent) 
	: Entity(EntityType::CABLE, parent)
{

	// Dictionary for GUI of component for data automation.
	cableDict.insert(std::pair<std::string, std::string>("FromTagNumber", "From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("ToTagNumber", "From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("From", "From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("To", "From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("Description", "From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("Voltage", "From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("Circuit", "From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("CableRating", "From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("CableIsolation", "From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("CableSuffix", "From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("CableLength", "From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("CableTAG", "From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("IndoorTermCount", "From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("OutdoorTermCount", "From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("CableSize", "size()"));
	cableDict.insert(std::pair<std::string, std::string>("CoreCount", "From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("Metal", "From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("DBRef", "From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("CableDiameter", "From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("CableMass", "From(Circuit Database)"));
	// Keep the VAO and start port.
	m_startPort = startPort;

	//Set the default title string
	m_titleString = "Cable " + std::to_string(cableID++);

	startPort->attachCable(this);

	// Get the initial points and orientation of the first segment.
	float initial_length = m_titleOffset;
	/*switch (startPort->m_position)
	{
	case PortPosition::TOP:
		m_curOrientation = LineOrientation::VERTICAL;
		endPt += glm::vec2(0.f, initial_length);
		endPt2 += glm::vec2(-initial_length, 0.f);
		m_title1 = Renderer::addText2D(m_titleString, glm::vec3(endPt - glm::vec2(m_thickness, 0.f), 0.f), m_titleColour, m_titleSize, "L", "U");
		m_title1->rotate(90);
		break;
	case PortPosition::BOTTOM:
		m_curOrientation = LineOrientation::VERTICAL;
		endPt += glm::vec2(0.f, -initial_length);
		endPt2 += glm::vec2(-initial_length, 0.f);
		m_title1 = Renderer::addText2D(m_titleString, glm::vec3(endPt + glm::vec2(m_thickness, 0.f), 0.f), m_titleColour, m_titleSize, "L", "U");
		m_title1->rotate(-90);
		break;
	case PortPosition::LEFT:
		m_curOrientation = LineOrientation::HORIZONTAL;
		endPt += glm::vec2(-initial_length, 0.f);
		endPt2 += glm::vec2(0.f, initial_length);
		m_title1 = Renderer::addText2D(m_titleString, glm::vec3(endPt + glm::vec2(0.f, m_thickness), 0.f), m_titleColour, m_titleSize, "R", "U");
		break;
	case PortPosition::RIGHT:
		m_curOrientation = LineOrientation::HORIZONTAL;
		endPt += glm::vec2(initial_length, 0.f);
		endPt2 += glm::vec2(0.f, initial_length);
		m_title1 = Renderer::addText2D(m_titleString, glm::vec3(endPt + glm::vec2(0.f, m_thickness), 0.f), m_titleColour, m_titleSize, "L", "U");
		break;
	}
	*/

	// --------------------- //
	//  P R I M I T I V E S  //
	// --------------------- //

	// First line.
	m_polyLine = Renderer::addPolyLine({ m_startPort->centre,  m_startPort->centre }, this);

}

Cable::Cable(Port* startPort, std::vector<glm::vec2> nodeList, Port* endPort, Circuit* parent, std::string titleString)
	: Entity(EntityType::CABLE, parent)
{
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

	m_cableType = std::filesystem::path(node["File"].as<std::string>()).filename().stem().string();

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
		{
			startPort = parent->m_components[startComponentIndex]->ports[startPortIndex].get();
		}
	}

	// End port.
	if (endComponentIndex < totalComponents)
	{
		// Get total ports belonging to component.
		int endComponentPortCount = parent->m_components[endComponentIndex]->ports.size();
		// Get port.
		if (endPortIndex < endComponentPortCount)
		{
			endPort = parent->m_components[endComponentIndex]->ports[endPortIndex].get();
		}
	}
	
	// Load all of the nodes belonging to the cable.
	std::vector<glm::vec2> nodeVector;
	nodeVector.reserve(node["Nodes"].size()+2);
	if (startPort) {
		nodeVector.emplace_back(startPort->centre);
	}
	for (const auto& nodeIt : node["Nodes"])
	{
		// Add to node vector.
		glm::vec2 node = { nodeIt.second[0].as<float>(), nodeIt.second[1].as<float>() };
		nodeVector.emplace_back(node);
	}
	if (endPort) {
		nodeVector.emplace_back(endPort->centre);
	}
	// Call constructor with data.
	constructCable(startPort, nodeVector, endPort);

	// Dictionary.
	cableDict.clear();
	for (const auto& entry : node["Dictionary"])
	{
		cableDict.insert({ entry.first.as<std::string>(), entry.second.as<std::string>() });
	}
}

Cable::~Cable()
{
	// Remove the cable from the start and end ports.
	if (m_startPort) m_startPort->detachCable(this);
	if (m_endPort)   m_endPort->detachCable(this); 

	// Remove the renderer primitives.
	Renderer::remove(m_polyLine);
	
	/*/
	//remove title text
	//Renderer::remove(m_title1);
	if (m_title2) {
		Renderer::remove(m_title2);
	}
	*/
}

void Cable::constructCable(Port* startPort, std::vector<glm::vec2> nodeList, Port* endPort) 
{
	// Attach the ports
	m_startPort = startPort;
	m_endPort = endPort;
	if(m_startPort)
		m_startPort->attachCable(this);
	if(m_endPort)
		m_endPort->attachCable(this);

	m_polyLine = Renderer::addPolyLine(nodeList, this);
}

//==============================================================================================================================================//
//  Methods.  																																    //
//==============================================================================================================================================//

void Cable::extendSegment(glm::vec2 nextPoint)
{
	// Extend the pevious segment. 
	m_polyLine->translateToVertexAtIndex(m_polyLine->m_vertices.size() - 1, nextPoint);
}

void Cable::addSegment(glm::vec2 nextPoint)
{
	m_polyLine->pushVertex(nextPoint);
}

void Cable::setContext(GUIState* guiState)
{
	//DEPRECATED
	//guiState->clickedZone.component = true;
}

void Cable::attach(Port* endPort)
{
	// Keep pointer to end port.
	m_endPort = endPort;
	extendSegment(m_endPort->centre);
	setColour(m_colour);
	endPort->attachCable(this);
}

void Cable::followPort(Port* movedPort)
{
	if (movedPort == m_endPort)
	{
		// Extend the pevious segment. 
		m_polyLine->translateToVertexAtIndex(m_polyLine->m_vertices.size() - 1, movedPort->centre);
	}

	else if (movedPort == m_startPort)
	{
		// Extend the first segment. 
		m_polyLine->translateToVertexAtIndex(0, movedPort->centre);
	}
}

void Cable::setColour(glm::vec4 colour, bool save)
{
	if(save) m_colour = colour;
	m_polyLine->setColor(colour);
}

void Cable::translateVertexAtIndex(unsigned vertexIdx, glm::vec2 translation)
{
	if (vertexIdx == 0 || vertexIdx == m_polyLine->m_vertices.size() - 1) {
		//Don't allow the ends to be moved off the port
		return;
	}
	m_polyLine->translateVertexAtIndex(vertexIdx, translation);
}

void Cable::translateVertexAtIndexTo(unsigned vertexIdx, glm::vec2 position)
{
	if (vertexIdx == 0 || vertexIdx == m_polyLine->m_vertices.size() - 1) {
		//Don't allow the ends to be moved off the port
		return;
	}
	m_polyLine->translateToVertexAtIndex(vertexIdx, position);
}

void Cable::enableOutline()
{
	m_polyLine->enableOutline();
}

/*
void Cable::moveActivePrimitiveTo(glm::vec2 screenCoords)
{
	// Add code to move necessary primatives around.
	// Move line segment if user grabs a line.
	if (m_activeLine) 
	{
		// First check that we are not moving the first or last line. At the monemt this is not supported, as it requires the line segments to be added.
		// This should be fixed in the future.
		auto it = std::find_if(begin(m_lines), end(m_lines), [&](LineSegment* current)
			{
				return current == m_activeLine;
			});
		if (it == m_lines.begin() || it >= (m_lines.end() - 1)) 
		{
			// The active line is the first or last line segment.
			// Could print a warning to the user here for now. Ideally we should handle this by addin an additional segment. For now, just return.
			return;
		}


		// Get the line orientation -  We move horizontal lines vertically and we move vertical lines horizontally.
		glm::vec2 translation = {};
		if (m_activeLine->m_start.x == m_activeLine->m_end.x) 
		{
			// Vertical line
			translation = { screenCoords[0] - m_activeLine->m_start.x, 0.f };
		}
		else 
		{
			// Horizontal line
			translation = { 0.f, screenCoords[1] - m_activeLine->m_start.y };
		}

		// Translate the line.
		m_activeLine->translate(translation);

		// Extend the adjacent lines.
		(*std::prev(it))->setEnd(m_activeLine->m_start);
		(*std::next(it))->setStart(m_activeLine->m_end);
		
		//Move the adjacent nodes
		m_nodes.at(it - m_lines.begin() - 1)->translate(translation);
		m_nodes.at(it - m_lines.begin())->translate(translation);
	}
	else if (m_activeNode) 
	{
		// Handle node movement.

		// This is the same as dragging the two adjacent line segments.

		// Get an iterator to the node.
		auto it = std::find_if(begin(m_nodes), end(m_nodes), [&](Circle* current)
			{
				return current == m_activeNode;
			});

		// Translate the adjacent lines.
		m_activeLine = m_lines.at(it - m_nodes.begin());
		moveActivePrimitiveTo(screenCoords);
		m_activeLine = m_lines.at(it - m_nodes.begin()+1);
		moveActivePrimitiveTo(screenCoords);
		m_activeLine = nullptr;
	}
}
void Cable::moveActivePrimitive(glm::vec2 translation)
{
	// Add code to move necessary primatives around.
	// Move line segment if user grabs a line.
	if (m_activeLine)
	{
		// First check that we are not moving the first or last line. At the monemt this is not supported, as it requires the line segments to be added.
		// This should be fixed in the future.
		auto it = std::find_if(begin(m_lines), end(m_lines), [&](LineSegment* current)
			{
				return current == m_activeLine;
			});
		if (it == m_lines.begin() || it >= (m_lines.end() - 1))
		{
			// The active line is the first or last line segment.
			// Could print a warning to the user here for now. Ideally we should handle this by addin an additional segment. For now, just return.
			return;
		}

		// Get the line orientation -  We move horizontal lines vertically and we move vertical lines horizontally.
		if (m_activeLine->m_start.x == m_activeLine->m_end.x)
		{
			// Vertical line
			translation.y = 0;
		}
		else
		{
			// Horizontal line
			translation.x = 0;
		}

		// Translate the line.
		m_activeLine->translate(translation);

		// Extend the adjacent lines.
		(*std::prev(it))->setEnd(m_activeLine->m_start);
		(*std::next(it))->setStart(m_activeLine->m_end);

		//Move the adjacent nodes
		m_nodes.at(it - m_lines.begin() - 1)->translate(translation);
		m_nodes.at(it - m_lines.begin())->translate(translation);
	}
	else if (m_activeNode)
	{
		// Handle node movement.

		// This is the same as dragging the two adjacent line segments.

		// Get an iterator to the node.
		auto it = std::find_if(begin(m_nodes), end(m_nodes), [&](Circle* current)
			{
				return current == m_activeNode;
			});

		// Translate the adjacent lines.
		m_activeLine = m_lines.at(it - m_nodes.begin());
		moveActivePrimitive(translation);
		m_activeLine = m_lines.at(it - m_nodes.begin() + 1);
		moveActivePrimitive(translation);
		m_activeLine = nullptr;
	}
}

void Cable::setActivePrimitive(Entity* primative)
{
	m_activeLine = dynamic_cast<LineSegment*>(primative);
	m_activeNode = dynamic_cast<Circle*>(primative);
}
*/

void Cable::disableOutline()
{
	m_polyLine->disableOutline();
}

std::tuple<unsigned, float>  Cable::getNearestVertexIdx(glm::vec2 pos)
{
	return m_polyLine->getNearestVertexIdx(pos);
}

//==============================================================================================================================================//
//  EOF.  																																		//
//==============================================================================================================================================//