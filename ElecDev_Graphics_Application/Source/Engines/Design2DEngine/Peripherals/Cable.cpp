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
	glm::vec2 endPt = m_startPort->centre;
	glm::vec2 endPt2 = m_startPort->centre;
	float initial_length = m_titleOffset;
	switch (startPort->m_position) 
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

	// --------------------- //
	//  P R I M I T I V E S  //
	// --------------------- //

	// First line.
	m_lines.emplace_back(Renderer::addLineSegment2D(m_startPort->centre, endPt, m_thickness, m_colour, this));
	// First node.
	m_nodes.emplace_back(Renderer::addCircle2D(endPt, m_thickness, m_colour, 1.f, 0.f, this));
	// Add the second (perpendicular) line segment.
	m_lines.emplace_back(Renderer::addLineSegment2D(m_startPort->centre, endPt2, m_thickness, m_colour, this));

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
	nodeVector.reserve(node["Nodes"].size());
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
	for (Circle* circle : m_nodes) Renderer::remove(circle);
	m_nodes.clear();
	m_nodes.shrink_to_fit();
	for (LineSegment* lineSegment : m_lines) Renderer::remove(lineSegment);
	m_lines.clear();
	m_lines.shrink_to_fit();
	

	//remove title text
	Renderer::remove(m_title1);
	if (m_title2) {
		Renderer::remove(m_title2);
	}
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

	// Add the first line segment and first node.
	if (startPort)
	{
		m_lines.emplace_back(Renderer::addLineSegment2D(m_startPort->centre, nodeList[0], m_thickness, m_colour, this));
	}
	m_nodes.emplace_back(Renderer::addCircle2D(nodeList[0], m_thickness, m_colour, 1.f, 0.f, this));

	// Add all inter-node line segments, and the rest of the nodes.
	for (int i = 1; i < nodeList.size(); i++)
	{
		m_lines.emplace_back(Renderer::addLineSegment2D(nodeList[i - 1], nodeList[i], m_thickness, m_colour, this));
		m_nodes.emplace_back(Renderer::addCircle2D(nodeList[i], m_thickness, m_colour, 1.f, 0.f, this));
	}
	
	// Add final line segment.
	if (endPort)
	{
		m_lines.emplace_back(Renderer::addLineSegment2D(nodeList.back(), m_endPort->centre, m_thickness, m_colour, this));
	}

	if (endPort)
	{
		switch (endPort->m_position)
		{
		case PortPosition::TOP:
			m_title2 = Renderer::addText2D(m_titleString, glm::vec3(endPort->centre + glm::vec2(-m_thickness, m_titleOffset), 0.f), m_titleColour, m_titleSize, "L", "U");
			m_title2->rotate(90);
			//endPt += glm::vec2(0.f, initial_length);
			break;
		case PortPosition::BOTTOM:
			m_title2 = Renderer::addText2D(m_titleString, glm::vec3(endPort->centre + glm::vec2(m_thickness, -m_titleOffset), 0.f), m_titleColour, m_titleSize, "L", "U");
			m_title2->rotate(-90);
			//endPt += glm::vec2(0.f, -initial_length);
			break;
		case PortPosition::LEFT:
			m_title2 = Renderer::addText2D(m_titleString, glm::vec3(endPort->centre + glm::vec2(-m_titleOffset, m_thickness), 0.f), m_titleColour, m_titleSize, "R", "U");
			//endPt += glm::vec2(-initial_length, 0.f);
			break;
		case PortPosition::RIGHT:
			m_title2 = Renderer::addText2D(m_titleString, glm::vec3(endPort->centre + glm::vec2(m_titleOffset, m_thickness), 0.f), m_titleColour, m_titleSize, "L", "U");
			//endPt += glm::vec2(initial_length, 0.f);
			break;
		}
	}

	if (startPort)
	{
		switch (startPort->m_position)
		{
		case PortPosition::TOP:
			m_title1 = Renderer::addText2D(m_titleString, glm::vec3(startPort->centre + glm::vec2(-m_thickness, m_titleOffset), 0.f), m_titleColour, m_titleSize, "L", "U");
			//endPt += glm::vec2(0.f, initial_length);
			break;
		case PortPosition::BOTTOM:
			m_title1 = Renderer::addText2D(m_titleString, glm::vec3(startPort->centre + glm::vec2(m_thickness, -m_titleOffset), 0.f), m_titleColour, m_titleSize, "L", "U");
			//endPt += glm::vec2(0.f, -initial_length);
			break;
		case PortPosition::LEFT:
			m_title1 = Renderer::addText2D(m_titleString, glm::vec3(startPort->centre + glm::vec2(-m_titleOffset, m_thickness), 0.f), m_titleColour, m_titleSize, "R", "U");
			//endPt += glm::vec2(-initial_length, 0.f);
			break;
		case PortPosition::RIGHT:
			m_title1 = Renderer::addText2D(m_titleString, glm::vec3(startPort->centre + glm::vec2(m_titleOffset, m_thickness), 0.f), m_titleColour, m_titleSize, "L", "U");
			//endPt += glm::vec2(initial_length, 0.f);
			break;
		}
	}
}

//==============================================================================================================================================//
//  Methods.  																																    //
//==============================================================================================================================================//

void Cable::extendPrevSegment(glm::vec2 nextPoint)
{
	glm::vec2 endPt = m_lines.back()->m_start;
	switch (m_curOrientation) 
	{
	// Update X-values for horizontal lines.
	case LineOrientation::HORIZONTAL:
		endPt.x = nextPoint.x;
		break;
	// Update Y-values for vertical lines.
	case LineOrientation::VERTICAL:
		endPt.y = nextPoint.y;
		break;
	}
	m_lines.end()[-2]->setEnd(endPt);
	m_nodes.back()->translateTo(endPt);
	m_lines.back()->setStart(endPt);
	m_nodes.back()->translateTo(endPt);
}

void Cable::extendSegment(glm::vec2 nextPoint)
{
	// Extend the pevious segment. 
	extendPrevSegment(nextPoint);
	m_lines.back()->setEnd(nextPoint);
}

void Cable::addSegment(glm::vec2 nextPoint)
{
	extendSegment(nextPoint);
	glm::vec2 endPt = nextPoint;
	float initial_length = 0.01f;
	switch (m_curOrientation) 
	{
	case LineOrientation::HORIZONTAL:
		// Switch the orientation for the next line.
		m_curOrientation = LineOrientation::VERTICAL;
		endPt += glm::vec2(initial_length, 0.f);
		break;
		// Update Y-values for vertical lines.
	case LineOrientation::VERTICAL:
		// Switch the orientation for the next line.
		m_curOrientation = LineOrientation::HORIZONTAL;
		endPt += glm::vec2(0.f, initial_length);
		break;
	}
	m_nodes.emplace_back(Renderer::addCircle2D(m_lines.back()->m_end, m_thickness, m_colour, 1.f, 0.f, this));
	m_lines.emplace_back(Renderer::addLineSegment2D(m_lines.back()->m_end, endPt, m_thickness, m_colour, this));
}


void Cable::setContext(GUIState* guiState)
{
	//guiState->clickedZone.component = true;
}

void Cable::attach(Port* endPort)
{
	// Keep pointer to end port.
	m_endPort = endPort;

	// Test to see ensure the cable enters perpendicular to the component edge.
	LineOrientation portOrientation = LineOrientation::VERTICAL;;
	switch (endPort->m_position) {
	case PortPosition::TOP:
		portOrientation = LineOrientation::VERTICAL;
		m_title2 = Renderer::addText2D(m_titleString, glm::vec3(endPort->centre + glm::vec2(-m_thickness, m_titleOffset), 0.f), m_titleColour, m_titleSize, "L", "U");
		m_title2->rotate(90);
		//endPt += glm::vec2(0.f, initial_length);
		break;
	case PortPosition::BOTTOM:
		portOrientation = LineOrientation::VERTICAL;
		m_title2 = Renderer::addText2D(m_titleString, glm::vec3(endPort->centre + glm::vec2(m_thickness, -m_titleOffset), 0.f), m_titleColour, m_titleSize, "L", "U");
		m_title2->rotate(-90);
		//endPt += glm::vec2(0.f, -initial_length);
		break;
	case PortPosition::LEFT:
		portOrientation = LineOrientation::HORIZONTAL;
		m_title2 = Renderer::addText2D(m_titleString, glm::vec3(endPort->centre + glm::vec2(-m_titleOffset, m_thickness), 0.f), m_titleColour, m_titleSize, "R", "U");
		//endPt += glm::vec2(-initial_length, 0.f);
		break;
	case PortPosition::RIGHT:
		portOrientation = LineOrientation::HORIZONTAL;
		m_title2 = Renderer::addText2D(m_titleString, glm::vec3(endPort->centre + glm::vec2(m_titleOffset, m_thickness), 0.f), m_titleColour, m_titleSize, "L", "U");
		//endPt += glm::vec2(initial_length, 0.f);
		break;
	}
	if (portOrientation == m_curOrientation) 
	{
		// Line is not perpendicular, so we need to add a segment.
		addSegment(m_endPort->centre);
	}
	else {
		// Line is already perpendicular, so we can extend to the port.
		extendSegment(m_endPort->centre);
	}
	setColour(m_colour);
	endPort->attachCable(this);
}

void Cable::followPort(Port* movedPort)
{
	glm::vec3 titlePos;
	switch (movedPort->m_position)
	{
	case PortPosition::TOP:
		m_curOrientation = LineOrientation::HORIZONTAL;
		titlePos = glm::vec3(movedPort->centre + glm::vec2(-m_thickness, m_titleOffset), 0.f);
		break;
	case PortPosition::BOTTOM:
		m_curOrientation = LineOrientation::HORIZONTAL;
		titlePos = glm::vec3(movedPort->centre + glm::vec2(m_thickness, -m_titleOffset), 0.f);
		break;
	case PortPosition::LEFT:
		m_curOrientation = LineOrientation::VERTICAL;
		titlePos = glm::vec3(movedPort->centre + glm::vec2(-m_titleOffset, m_thickness), 0.f);
		break;
	case PortPosition::RIGHT:
		m_curOrientation = LineOrientation::VERTICAL;
		titlePos = glm::vec3(movedPort->centre + glm::vec2(m_titleOffset, m_thickness), 0.f);
		break;
	}

	if (movedPort == m_endPort) 
	{
		extendSegment(movedPort->centre);
		m_title2->translateTo(titlePos);
	}

	else if (movedPort == m_startPort) 
	{
		glm::vec2 startPoint = movedPort->centre;
		// Move second segment.
		glm::vec2 startPt = m_lines[1]->m_end;
		switch (m_curOrientation) 
		{
		// Update X-values for horizontal lines.
		case LineOrientation::HORIZONTAL:
			startPt.x = startPoint.x;
			break;
		// Update Y-values for vertical lines.
		case LineOrientation::VERTICAL:
			startPt.y = startPoint.y;
			break;
		}

		m_lines[1]->setStart(startPt);
		// Move the first node.
		m_nodes.front()->translateTo(startPt);
		// Move first segment.
		m_lines[0]->setStart(startPoint);
		m_lines[0]->setEnd(m_lines[1]->m_start);

		//move the title
		m_title1->translateTo(titlePos);
	}
}

void Cable::setColour(glm::vec4 colour, bool save)
{
	if(save) m_colour = colour;
	for (int i = 0; i < m_lines.size(); i++) 
	{
		LineSegment* line = m_lines[i];
		line->setColor(colour);
	}
	for (int i = 0; i < m_nodes.size(); i++) 
	{
		Circle* circ = m_nodes[i];
		circ->setColor(colour);
	}
}

void Cable::enableOutline()
{
	for (auto& line : m_lines)
		line->enableOutline();

	for (auto& node : m_nodes)
		node->enableOutline();

	m_title1->enableOutline();
	m_title2->enableOutline();
}

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

void Cable::disableOutline()
{
	for (auto& line : m_lines)
		line->disableOutline();

	for (auto& node : m_nodes)
		node->disableOutline();

	//m_title1->disableOutline();
	//m_title2->disableOutline();
}

//==============================================================================================================================================//
//  EOF.  																																		//
//==============================================================================================================================================//