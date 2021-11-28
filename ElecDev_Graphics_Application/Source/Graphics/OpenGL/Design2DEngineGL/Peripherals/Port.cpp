#include "Port.h"
#include "Component2D.h"
#include "CoreGL/Entities/Polygon.h"
#include "CoreGL/Entities/Text.h"
#include "GUI/GUIState.h"
#include "CoreGL/FontLoader.h"
#include "Resources/ResourceHandler.h"

//Add font for component titles
Font Port::titleFont = msdfLoadFont(ROBOTO_MEDIUM_MSDF_JSON);

Port::Port(glm::vec2 offset, PortType type, Component2D* parent, std::string label) 
	:ManagedEntity(parent), 
	 bodyColour( 0.7f, 0.7f, 0.7f, 1.f ),
	 borderColour(0.8f, 1.0f, .8f, 1.f),
	 m_offset(offset),
	 centre(parent->centre + offset),
	 body(parent->engine_circleVAO, centre, 0.01f, bodyColour, 1.0f, 0.0f, this),
	 border(parent->engine_circleVAO, centre, 0.011f, borderColour, 1.0f, 0.01f, this),
	 m_type(type)
{
	engine_circleVAO = parent->engine_circleVAO;
	engine_texturedTrianglesVAO = parent->engine_texturedTrianglesVAO;
	portLayer = parent->componentLayer + parent->portLayerOffset;
	if (label == "default") {
		m_label = "Port " + std::to_string(parent->numPorts++);
	}
	else {
		m_label = label;
	}
	//infer the port position from the offset, and set the title
	if (m_offset.y > 0.099) {//top
		titleOffset = glm::vec2{ 0.f, -0.012f-titleSize };
		glm::vec3 titlePos = glm::vec3(centre + titleOffset, portLayer);
		title = std::make_shared<Text<VertexDataTextured>>(m_label, titlePos, titleColour, titleSize, engine_texturedTrianglesVAO, titleFont, this);
	}	
	else if (m_offset.y < -0.099) {//bottom
		titleOffset = glm::vec2{ 0.f, 0.012f };
		glm::vec3 titlePos = glm::vec3(centre + titleOffset, portLayer);
		title = std::make_shared<Text<VertexDataTextured>>(m_label, titlePos, titleColour, titleSize, engine_texturedTrianglesVAO, titleFont, this);
	}
	else if (m_offset.x > 0.099) {//right
		titleOffset = glm::vec2{ -0.012f, 0.0f };
		glm::vec3 titlePos = glm::vec3(centre + titleOffset, portLayer);
		title = std::make_shared<Text<VertexDataTextured>>(m_label, titlePos, titleColour, titleSize, engine_texturedTrianglesVAO, titleFont, this, "R");
	}
	else if (m_offset.x < -0.099) {//left
		titleOffset = glm::vec2{ 0.012f, 0.0f };
		glm::vec3 titlePos = glm::vec3(centre + titleOffset, portLayer);
		title = std::make_shared<Text<VertexDataTextured>>(m_label, titlePos, titleColour, titleSize, engine_texturedTrianglesVAO, titleFont, this, "L");
	}
	body.setColor(bodyColour);
	body.setLayer(portLayer);
	body.update();
	//new (&border) Circle(engine_circleVAO, centre3, 0.011f, borderColour, 1.0f, 0.0f, this);
	border.setColor(borderColour);
	border.setLayer(portLayer);
	border.update();

	title->update();
}

void Port::moveTo(glm::vec2 destination)
{
	//update the port centre
	centre = destination + m_offset;
	glm::vec2 titlePos = centre + titleOffset;
	//move each primative
	body.translateTo(centre);
	border.translateTo(centre);
	title->translateTo(titlePos);
	body.update();
	border.update();
	title->update();
}

Port& Port::operator=(const Port& t)
{
	return *this;
}

void Port::setLayer(float layer)
{
	body.setLayer(layer);
	border.setLayer(layer);
	title->setLayer(layer);
	body.update();
	border.update();
	title->update();
}

void Port::update()
{
	body.update();
	border.update();
	title->update();
}

void Port::highlight()
{
	borderColour = { 0.8f, 1.0f, .8f, 1.f };
	border.setColor(borderColour);
	border.update();
}

void Port::unhighlight()
{
	borderColour = { 0.f, 0.f, 0.f, 1.f };
	border.setColor(borderColour);
	border.update();
}

void Port::setOffset(glm::vec2 offset)
{
	//move port to new offset (trust the math)
	moveTo(centre - m_offset - m_offset + offset);
	//update internal offset
	m_offset = offset;
}

void Port::setContext(GUIState* guiState)
{
	guiState->clickedZone.port = true;
	m_parent->setContext(guiState);
}