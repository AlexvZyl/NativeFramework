#include "Cable.h"
#include "GUI/GUIState.h"


Cable::Cable(Port* startPort, VertexArrayObject<VertexData>* VAO, Circuit* parent):Entity(EntityType::CABLE, parent)
{
	//Keep the VAO and start port
	engine_VAO = VAO;
	m_startPort = startPort;

	//get the initial points and orientation of the first segment
	glm::vec2 endPt = m_startPort->centre;
	float initial_length = 0.01f;
	switch (startPort->m_position) {
	case PortPosition::TOP:
		m_curOrientation = LineOrientation::VERTICAL;
		endPt += glm::vec2(0.f, initial_length);
		break;
	case PortPosition::BOTTOM:
		m_curOrientation = LineOrientation::VERTICAL;
		endPt += glm::vec2(0.f, -initial_length);
		break;
	case PortPosition::LEFT:
		m_curOrientation = LineOrientation::HORIZONTAL;
		endPt += glm::vec2(-initial_length, 0.f);
		break;
	case PortPosition::RIGHT:
		m_curOrientation = LineOrientation::HORIZONTAL;
		endPt += glm::vec2(0.f, initial_length);
		break;
	}

	//Add the first line segment
	m_lines.push_back(std::make_shared<LineSegment>(m_startPort->centre, endPt, engine_VAO, this));

}

void Cable::addSegment(glm::vec2 nextPoint)
{
	switch (m_curOrientation) {
	case LineOrientation::HORIZONTAL:

		break;
	case LineOrientation::VERTICAL:
		break;
	}
}

void Cable::setContext(GUIState* guiState)
{
	guiState->clickedZone.component = true;
}


