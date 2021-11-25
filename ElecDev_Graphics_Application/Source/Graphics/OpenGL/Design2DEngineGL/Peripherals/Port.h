#pragma once
#include "CoreGL/Entities/ManagedEntity.h"
#include <string>
#include <memory>
#include "glm.hpp"
#include "CoreGL/Entities/Circle.h"
#include "CoreGL/Entities/Vertex.h"
template<typename VertexType>
class Text;
template<typename VertexType>
class Polygon2D;
template<typename VertexType>
class VertexArrayObject;

struct Font;
//class VertexData;
//class VertexDataTextured;
//class VertexDataCircle;

class Component2D;

enum PortType
{
	PORT_IN,
	PORT_OUT,
	PORT_INOUT
};

class Port: public ManagedEntity
{
private:
	//port VAOs and primatives
	VertexArrayObject<VertexDataTextured>* engine_texturedTrianglesVAO;
	VertexArrayObject<VertexDataCircle>* engine_circleVAO;
	std::shared_ptr<Text<VertexDataTextured>> title;
	glm::vec2 centre;
	static unsigned portID;
	glm::vec4 bodyColour;
	glm::vec4 borderColour;
	float portLayer;

	Circle<VertexDataCircle> body;
	Circle<VertexDataCircle> border;


public:
	std::string label;
	Port(glm::vec2 pos, PortType type, Component2D* parent);
	Port& operator = (const Port &t);
	//Port(const Port&) = default;
	void moveTo(glm::vec2 destination);
	void setLayer(float layer);
	void setContext(GUIState* guiState);
	void update();

};

