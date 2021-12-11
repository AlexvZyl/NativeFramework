#include "Cable.h"
#include "GUI/GUIState.h"
#include "Circuit.h"


Cable::Cable(Port* startPort, VertexArrayObject<VertexData>* triangleVAO, VertexArrayObject<VertexDataCircle>* circleVAO, Circuit* parent):Entity(EntityType::CABLE, parent)
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
	m_nodes.push_back(std::make_shared<Circle<VertexDataCircle>>(engine_circleVAO, endPt, m_thickness, m_colour, 1.f, 0.f, this));
	//Add the second (perpendicular) line segment.
	m_lines.push_back(std::make_shared<LineSegment>(m_startPort->centre, endPt, engine_triangleVAO, this, m_thickness, m_colour));

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
	m_nodes.push_back(std::make_shared<Circle<VertexDataCircle>>(engine_circleVAO, m_lines.back()->m_end, m_thickness, m_colour, 1.f, 0.f, this));
	m_lines.push_back(std::make_shared<LineSegment>(m_lines.back()->m_end, nextPoint, engine_triangleVAO, this, m_thickness, m_colour));
}


void Cable::setContext(GUIState* guiState)
{
	guiState->clickedZone.component = true;
}

void Cable::attach(Port* endPort)
{
	m_endPort = endPort;
	extendSegment(m_endPort->centre);
	endPort->attachCable(this);
	setColour(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
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
		Circle<VertexDataCircle>* circ = m_nodes[i].get();
		circ->setColor(m_colour);
	}
}

void Cable::highlight()
{
	setColour(glm::vec4{ 0.f, 0.f, 1.0f, 1.f });
}

void Cable::moveActivePrimativeTo(float screenCoords[2])
{
	//Add code to move necessary primatives around.
}

void Cable::setActivePrimative(Entity* primative)
{
	m_activeLine = dynamic_cast<LineSegment*>(primative);
	m_activeNode = dynamic_cast<Circle<VertexDataCircle>*>(primative);
}

void Cable::unhighlight()
{
	setColour(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
}


