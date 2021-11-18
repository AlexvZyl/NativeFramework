#include "Port.h"
#include "Component2D.h"
#include "CoreGL/Entities/Polygon.h"
#include "CoreGL/Entities/Text.h"
#include "CoreGL/Entities/Circle.h"
#include "GUI/GUIState.h"

Port::Port(glm::vec2 pos, PortType type, Component2D* parent):ManagedEntity(parent)
{
	engine_circleVAO = parent->engine_circleVAO;
	engine_texturedTrianglesVAO = parent->engine_texturedTrianglesVAO;
	centre = pos;
	portLayer = parent->componentLayer + parent->portLayerOffset;
	glm::vec3 centre3 = glm::vec3(centre, portLayer);
	body = std::make_unique<Circle<VertexDataCircle>>(engine_circleVAO, centre3, 0.01, bodyColour, 1.0f, 0.0f, this);
	body->setColor(bodyColour);
	body->setLayer(portLayer);
	body->update();
	border = std::make_unique<Circle<VertexDataCircle>>(engine_circleVAO, centre3, 0.011, borderColour, 1.0f, 0.0f, this);
	border->setColor(borderColour);
	border->setLayer(portLayer);
	border->update();
}

Port::~Port()
{
}

void Port::moveTo(glm::vec2 destination)
{
	body->translateTo(destination);
	border->translateTo(destination);
	body->update();
	border->update();
}

void Port::setLayer(float layer)
{
	body->setLayer(layer);
	border->setLayer(layer);
	body->update();
	border->update();
}

void Port::update()
{
	body->update();
	border->update();
}

void Port::setContext(GUIState* guiState)
{
	guiState->clickedZone.port = true;
	m_parent->setContext(guiState);
}
