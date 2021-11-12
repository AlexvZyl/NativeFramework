#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <string>
#include "glm.hpp"
#include <memory>
#include <vector>

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

class Component2D
{
	//Private Members
private:
	//shape and edge data
	VertexArrayObject<VertexData>* engine_trianglesVAO;
	VertexArrayObject<VertexData>* engine_linesVAO;
	VertexArrayObject<VertexDataTextured>* engine_texturedTrianglesVAO;
	VertexArrayObject<VertexDataCircle>* engine_circleVAO;
	std::shared_ptr<Polygon2D<VertexData>> shape;
	std::shared_ptr<Polygon2D<VertexData>> border;
	std::shared_ptr<Text<VertexDataTextured>> title;
	std::shared_ptr<Circle<VertexDataCircle>> port1;
	static unsigned componentID;

	//component shape attributes
	float height = 0.1f;
	float width = 0.1f;
	std::vector<glm::vec3> vertices;
	glm::vec2 centre;
	float componentLayer = 0.9f;
	float borderLayerOffset = 0.01f;

	//colour attributes
	glm::vec4 shapeColour;
	glm::vec4 borderColour;

	//title
	static Font titleFont;
	glm::vec3 titlePos;
	glm::vec4 titleColour = glm::vec4(0.f, 0.f, 1.f, 1.f);
	std::string titleString;
	float titleSize = 0.2f;

	//port specifications
	int n_ports_north = 0;
	int n_ports_south = 0;
	int n_ports_east = 0;
	int n_ports_west = 0;

	//interaction attributes
	bool selected = true;

public:

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

	//Render the component
	void render();
	//Move the component to a new positioned centred at the given coordinates
	void moveTo(float pointerPos[2]);
	//Place a component
	void place(float pos[2]);
	void setLayer(float layer);
	void destroy();
};
