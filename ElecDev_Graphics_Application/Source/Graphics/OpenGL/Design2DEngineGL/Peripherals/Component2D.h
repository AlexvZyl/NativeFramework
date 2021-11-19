#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <string>
#include "glm.hpp"
#include <memory>
#include <vector>
#include "CoreGL/Entities/ManagedEntity.h"
#include "Port.h"

//=============================================================================================================================================//
//  Forward declerations.																													   //
//=============================================================================================================================================//

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

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

class Component2D: public ManagedEntity
{
	//Private Members
private:
	//shape and edge data
	std::shared_ptr<Polygon2D<VertexData>> shape;
	std::shared_ptr<Polygon2D<VertexData>> border;
	std::shared_ptr<Text<VertexDataTextured>> title;
	//std::shared_ptr<Port> port1;
	//std::shared_ptr<Port> port2;
	static unsigned componentID;

	//component shape attributes
	float height = 0.1f;
	float width = 0.1f;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> vertices2;
	glm::vec2 centre;

	//colour attributes
	glm::vec4 shapeColour;
	glm::vec4 borderColour;

	//title
	static Font titleFont;
	glm::vec2 titleOffset = glm::vec2(0, -0.15);
	glm::vec4 titleColour = glm::vec4(0.f, 0.f, 1.f, 1.f);
	std::string titleString;
	float titleSize = 0.2f;

	//port specifications
	int n_ports_north = 0;
	int n_ports_south = 0;
	int n_ports_east = 0;
	int n_ports_west = 0;
	glm::vec2 portOffset;

	//interaction attributes
	bool selected = true;

public:

	std::vector<std::shared_ptr<Port>> portsNorth;
	std::vector<std::shared_ptr<Port>> portsSouth;
	std::vector<std::shared_ptr<Port>> portsEast;
	std::vector<std::shared_ptr<Port>> portsWest;

	unsigned numPorts = 0;

	float componentLayer = 0.9f;
	float borderLayerOffset = 0.01f;
	float portLayerOffset = 0.02f;

	VertexArrayObject<VertexData>* engine_trianglesVAO;
	VertexArrayObject<VertexData>* engine_linesVAO;
	VertexArrayObject<VertexDataTextured>* engine_texturedTrianglesVAO;
	VertexArrayObject<VertexDataCircle>* engine_circleVAO;

	//Creates a generic component centred at (0, 0)
	Component2D(VertexArrayObject<VertexData>* trianglesVAO,
			    VertexArrayObject<VertexData>* linesVAO, 
				VertexArrayObject<VertexDataTextured>* texturedTrianglesVAO, 
				VertexArrayObject<VertexDataCircle>* circleVAO);

	//Creates a generic component centred at the specified coordinates
	Component2D(float centreCoords[2], 
				VertexArrayObject<VertexData>* trianglesVAO,	
				VertexArrayObject<VertexData>* linesVAO, 
				VertexArrayObject<VertexDataTextured>* texturedTrianglesVAO,
				VertexArrayObject<VertexDataCircle>* circleVAO);

	//Deconstructor
	~Component2D();
	//Move the component to a new positioned centred at the given coordinates
	void moveTo(float pointerPos[2]);
	//Place a component
	void place(float pos[2]);
	void setLayer(float layer);
	void setContext(GUIState* guiState);
	void update();
	void destroy();
};
