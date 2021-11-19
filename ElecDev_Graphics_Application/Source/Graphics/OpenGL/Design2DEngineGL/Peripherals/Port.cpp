#include "Port.h"
#include "Component2D.h"
#include "CoreGL/Entities/Polygon.h"
#include "CoreGL/Entities/Text.h"
#include "GUI/GUIState.h"

Port::Port(glm::vec2 pos, PortType type, Component2D* parent) :ManagedEntity(parent), 
bodyColour( 0.7f, 0.7f, 0.7f, 1.f ),
borderColour(0.1f, 0.1f, 0.1f, 1.f),
body(parent->engine_circleVAO, pos, 0.01f, bodyColour, 1.0f, 0.0f, this),
border(parent->engine_circleVAO, pos, 0.011f, borderColour, 1.0f, 0.0f, this)
{
	engine_circleVAO = parent->engine_circleVAO;
	engine_texturedTrianglesVAO = parent->engine_texturedTrianglesVAO;
	centre = pos;
	portLayer = parent->componentLayer + parent->portLayerOffset;
	label = "Port " + std::to_string(parent->numPorts++);
	//glm::vec3 centre3 = glm::vec3(centre, portLayer);
	//new (&body) Circle(engine_circleVAO, centre3, 0.01f, bodyColour, 1.0f, 0.0f, this);
	body.setColor(bodyColour);
	body.setLayer(portLayer);
	body.update();
	//new (&border) Circle(engine_circleVAO, centre3, 0.011f, borderColour, 1.0f, 0.0f, this);
	border.setColor(borderColour);
	border.setLayer(portLayer);
	border.update();
	
}

void Port::moveTo(glm::vec2 destination)
{
	body.translateTo(destination);
	border.translateTo(destination);
	body.update();
	border.update();
}

Port& Port::operator=(const Port& t)
{
	return *this;
}

void Port::setLayer(float layer)
{
	body.setLayer(layer);
	border.setLayer(layer);
	body.update();
	border.update();
}

void Port::update()
{
	body.update();
	border.update();
}

void Port::setContext(GUIState* guiState)
{
	guiState->clickedZone.port = true;
	m_parent->setContext(guiState);
}
