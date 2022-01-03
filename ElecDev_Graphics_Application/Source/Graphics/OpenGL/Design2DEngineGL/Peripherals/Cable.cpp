#include "Cable.h"
#include "GUI/GUIState.h"
#include "Circuit.h"

Cable::Cable(Port* startPort, VertexArrayObject<VertexData>* triangleVAO, VertexArrayObject<VertexDataCircle>* circleVAO, Circuit* parent) :Entity(EntityType::CABLE, parent)
{
	//Keep the VAO and start port
	engine_triangleVAO = triangleVAO;
	engine_circleVAO = circleVAO;
	m_startPort = startPort;

	startPort->attachCable(this);

	//get the initial points and orientation of the first segment
	glm::vec2 endPt = m_startPort->centre;
	float initial_length = 0.01f;
	switch (startPort->m_position) {
	case PortPosition::TOP:
		m_curOrientation = LineOrientation::VERTICAL;
		//endPt += glm::vec2(0.f, initial_length);
		break;
	case PortPosition::BOTTOM:
		m_curOrientation = LineOrientation::VERTICAL;
		//endPt += glm::vec2(0.f, -initial_length);
		break;
	case PortPosition::LEFT:
		m_curOrientation = LineOrientation::HORIZONTAL;
		//endPt += glm::vec2(-initial_length, 0.f);
		break;
	case PortPosition::RIGHT:
		m_curOrientation = LineOrientation::HORIZONTAL;
		//endPt += glm::vec2(initial_length, 0.f);
		break;
	}

	//Add the first line segment.
	m_lines.push_back(std::make_shared<LineSegment>(m_startPort->centre, endPt, engine_triangleVAO, this, m_thickness, m_colour));
	m_nodes.push_back(std::make_shared<Circle>(engine_circleVAO, endPt, m_thickness, m_colour, 1.f, 0.f, this));
	//Add the second (perpendicular) line segment.
	m_lines.push_back(std::make_shared<LineSegment>(m_startPort->centre, endPt, engine_triangleVAO, this, m_thickness, m_colour));

}

Cable::Cable(Port* startPort, std::vector<glm::vec2> nodeList, Port* endPort, VertexArrayObject<VertexData>* triangleVAO, VertexArrayObject<VertexDataCircle>* circleVAO, Circuit* parent) :Entity(EntityType::CABLE, parent)
{
	//Keep the VAO and start port
	engine_triangleVAO = triangleVAO;
	engine_circleVAO = circleVAO;
	m_startPort = startPort;
	m_endPort = endPort;
	m_colour = { 0.4f, 0.4f, 0.5f, 1.0f };

	//Attach the ports
	m_startPort->attachCable(this);
	m_endPort->attachCable(this);


	//Add the first line segment and first node.
	m_lines.push_back(std::make_shared<LineSegment>(m_startPort->centre, nodeList[0], engine_triangleVAO, this, m_thickness, m_colour));
	m_nodes.push_back(std::make_shared<Circle>(engine_circleVAO, nodeList[0], m_thickness, m_colour, 1.f, 0.f, this));
	//Add all inter-nore line segments, and the rest of the nodes
	for (int i = 1; i < nodeList.size(); i++) {
		m_lines.push_back(std::make_shared<LineSegment>(nodeList[i-1], nodeList[i], engine_triangleVAO, this, m_thickness, m_colour));
		m_nodes.push_back(std::make_shared<Circle>(engine_circleVAO, nodeList[i], m_thickness, m_colour, 1.f, 0.f, this));
	}
	//Add final line segment.
	m_lines.push_back(std::make_shared<LineSegment>(nodeList.back(), m_endPort->centre, engine_triangleVAO, this, m_thickness, m_colour));

}

Cable::~Cable()
{
	//remove the cable from the start and end ports.
	if (m_startPort) {
		m_startPort->detachCable(this);
	}
	if (m_endPort) {
		m_endPort->detachCable(this);
	}
}

void Cable::extendPrevSegment(glm::vec2 nextPoint)
{
	glm::vec2 endPt = m_lines.back()->m_start;
	switch (m_curOrientation) {
		//Update X-values for horizontal lines
	case LineOrientation::HORIZONTAL:
		endPt.x = nextPoint.x;
		break;
		//Update Y-values for vertical lines
	case LineOrientation::VERTICAL:
		endPt.y = nextPoint.y;
		break;
	}

	m_lines.end()[-2] = std::make_shared<LineSegment>(m_lines.end()[-2]->m_start, endPt, engine_triangleVAO, this, m_thickness, m_colour);
	m_nodes.back()->translateTo(endPt);
}

void Cable::extendSegment(glm::vec2 nextPoint)
{
	//Extend the pevious segment 
	extendPrevSegment(nextPoint);
	m_lines.back() = std::make_shared<LineSegment>(m_lines.end()[-2]->m_end, nextPoint, engine_triangleVAO, this, m_thickness, m_colour);
}

void Cable::addSegment(glm::vec2 nextPoint)
{
	extendSegment(nextPoint);
	switch (m_curOrientation) {
	case LineOrientation::HORIZONTAL:
		//switch the orientation for the next line
		m_curOrientation = LineOrientation::VERTICAL;
		break;
		//Update Y-values for vertical lines
	case LineOrientation::VERTICAL:
		//switch the orientation for the next line
		m_curOrientation = LineOrientation::HORIZONTAL;
		break;
	}
	m_nodes.push_back(std::make_shared<Circle>(engine_circleVAO, m_lines.back()->m_end, m_thickness, m_colour, 1.f, 0.f, this));
	m_lines.push_back(std::make_shared<LineSegment>(m_lines.back()->m_end, nextPoint, engine_triangleVAO, this, m_thickness, m_colour));
}


void Cable::setContext(GUIState* guiState)
{
	guiState->clickedZone.component = true;
}

void Cable::attach(Port* endPort)
{
	//keep pointer to end port
	m_endPort = endPort;

	//test to see ensure the cable enters perpendicular to the component edge
	LineOrientation portOrientation;
	switch (endPort->m_position) {
	case PortPosition::TOP:
		portOrientation = LineOrientation::VERTICAL;
		//endPt += glm::vec2(0.f, initial_length);
		break;
	case PortPosition::BOTTOM:
		portOrientation = LineOrientation::VERTICAL;
		//endPt += glm::vec2(0.f, -initial_length);
		break;
	case PortPosition::LEFT:
		portOrientation = LineOrientation::HORIZONTAL;
		//endPt += glm::vec2(-initial_length, 0.f);
		break;
	case PortPosition::RIGHT:
		portOrientation = LineOrientation::HORIZONTAL;
		//endPt += glm::vec2(initial_length, 0.f);
		break;
	}
	if (portOrientation == m_curOrientation) {
		//Line is not perpendicular, so we need to add a segment
		addSegment(m_endPort->centre);
	}
	else {
		//Line is already perpendicular, so we can extend to the port.
		extendSegment(m_endPort->centre);
	}
	setColour(glm::vec4{ 0.4f, 0.4f, 0.5f, 1.0f });
	endPort->attachCable(this);
}

void Cable::followPort(Port* movedPort)
{

	switch (movedPort->m_position) {
	case PortPosition::TOP:
		m_curOrientation = LineOrientation::HORIZONTAL;
		break;
	case PortPosition::BOTTOM:
		m_curOrientation = LineOrientation::HORIZONTAL;
		break;
	case PortPosition::LEFT:
		m_curOrientation = LineOrientation::VERTICAL;
		break;
	case PortPosition::RIGHT:
		m_curOrientation = LineOrientation::VERTICAL;
		break;
	}
	if (movedPort == m_endPort) {
		extendSegment(movedPort->centre);
	}
	else if (movedPort == m_startPort) {

		glm::vec2 startPoint = movedPort->centre;
		//move second segment
		glm::vec2 startPt = m_lines[1]->m_end;
		switch (m_curOrientation) {
			//Update X-values for horizontal lines
		case LineOrientation::HORIZONTAL:
			startPt.x = startPoint.x;
			break;
			//Update Y-values for vertical lines
		case LineOrientation::VERTICAL:
			startPt.y = startPoint.y;
			break;
		}

		m_lines[1] = std::make_shared<LineSegment>(startPt, m_lines[1]->m_end, engine_triangleVAO, this, m_thickness, m_colour);
		//move the first node
		m_nodes.front()->translateTo(startPt);
		//move first segment
		m_lines[0] = std::make_shared<LineSegment>(startPoint, m_lines[1]->m_start, engine_triangleVAO, this, m_thickness, m_colour);

}
}

void Cable::setColour(glm::vec4 colour)
{
	m_colour = colour;
	for (int i = 0; i < m_lines.size(); i++) {
		LineSegment* line = m_lines[i].get();
		line->setColor(m_colour);
	}
	for (int i = 0; i < m_nodes.size(); i++) {
		Circle* circ = m_nodes[i].get();
		circ->setColor(m_colour);
	}
}

void Cable::highlight()
{
	setColour(glm::vec4{ 0.f, 0.f, 1.0f, 1.f });
}

void Cable::moveActivePrimitiveTo(glm::vec2 screenCoords)
{
	//Add code to move necessary primatives around.
	//Move line segment if user grabs a line
	if (m_activeLine) {
		//First check that we are not moving the first or last line. At the monemt this is not supported, as it requires the line segments to be added.
		//This should be fixed in the future.
		auto it = std::find_if(begin(m_lines), end(m_lines), [&](std::shared_ptr<LineSegment> current)
			{
				return current.get() == m_activeLine;
			});
		if (it == m_lines.begin() || it >= (m_lines.end() - 1)) {
			//The active line is the first or last line segment.
			//Could print a warning to the user here for now. Ideally we should handle this by addin an additional segment. For now, just return.
			return;
		}


		//Get the line orientation -  We move horizontal lines vertically and we move vertical lines horizontally.
		glm::vec2 translation = {};
		if (m_activeLine->m_start.x == m_activeLine->m_end.x) {
			//Vertical line
			translation = { screenCoords[0] - m_activeLine->m_start.x, 0.f };
		}
		else {
			//Horizontal line
			translation = { 0.f, screenCoords[1] - m_activeLine->m_start.y };
		}

		//translate the line
		m_activeLine->translate(translation);

		//extend the adjacent lines
		*std::prev(it) = std::make_shared<LineSegment>((*std::prev(it))->m_start, m_activeLine->m_start, engine_triangleVAO, this, m_thickness, m_colour);
		*std::next(it) = std::make_shared<LineSegment>(m_activeLine->m_end, (*std::next(it))->m_end, engine_triangleVAO, this, m_thickness, m_colour);
		
		//Move the adjacent nodes
		m_nodes.at(it - m_lines.begin() - 1)->translate(translation);
		m_nodes.at(it - m_lines.begin())->translate(translation);
	}
	else if (m_activeNode) {
		//handle node movement

		//This is the same as dragging the two adjacent line segments.

		//get an iterator to the node
		auto it = std::find_if(begin(m_nodes), end(m_nodes), [&](std::shared_ptr<Circle> current)
			{
				return current.get() == m_activeNode;
			});

		//translate the adjacent lines
		m_activeLine = m_lines.at(it - m_nodes.begin()).get();
		moveActivePrimitiveTo(screenCoords);
		m_activeLine = m_lines.at(it - m_nodes.begin()+1).get();
		moveActivePrimitiveTo(screenCoords);
		m_activeLine = nullptr;


	}
}

void Cable::setActivePrimitive(Entity* primative)
{
	m_activeLine = dynamic_cast<LineSegment*>(primative);
	m_activeNode = dynamic_cast<Circle*>(primative);
}

void Cable::unhighlight()
{
	setColour(glm::vec4{ 0.4f, 0.4f, 0.5f, 1.0f });
}


