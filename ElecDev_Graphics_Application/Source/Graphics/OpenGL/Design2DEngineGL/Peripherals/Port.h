#pragma once
#include "CoreGL/Entities/ManagedEntity.h"
#include <string>
#include <memory>
#include "glm.hpp"

template<typename VertexType>
class Text;
template<typename VertexType>
class Polygon2D;
template<typename VertexType>
class Circle;
template<typename VertexType>
class VertexArrayObject;

struct Font;
class VertexData;
class VertexDataTextured;
class VertexDataCircle;

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
	std::unique_ptr<Circle<VertexDataCircle>> body;
	std::unique_ptr<Circle<VertexDataCircle>> border;
	std::shared_ptr<Text<VertexDataTextured>> title;
	glm::vec2 centre;
	static unsigned portID;
	glm::vec4 bodyColour = { 0.7f, 0.7f, 0.7f, 1.f };
	glm::vec4 borderColour = { 0.1f, 0.1f, 0.1f, 1.f };
	float portLayer;


public:
	Port(glm::vec2 pos, PortType type, Component2D* parent);
	~Port();
	void moveTo(glm::vec2 destination);
	void setLayer(float layer);
	void setContext(GUIState* guiState);
	void update();

};

