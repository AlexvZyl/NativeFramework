#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "CoreGL/Entities/Polygon.h"
#include <string>
#include "glm.hpp"
#include <memory>

//=============================================================================================================================================//
//  Forward declerations.																													   //
//=============================================================================================================================================//

class VertexArrayObject;

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

class Component2D
{
	//Private Members
private:
	//shape and edge data
	std::shared_ptr<VertexArrayObject> shapeVAO;
	std::shared_ptr<VertexArrayObject> borderVAO;
	std::shared_ptr<Polygon2D> shape;
	std::shared_ptr<Polygon2D> border;

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
	std::string title;
	glm::vec2 titlePos;

	//port specifications
	int n_ports_north = 0;
	int n_ports_south = 0;
	int n_ports_east = 0;
	int n_ports_west = 0;

	//interaction attributes
	bool selected = true;

	//Public Members
public:

	//Constructors

	//Creates a generic component centred at (0, 0)
	Component2D();

	//Creates a generic component centred at the specified coordinates
	Component2D(float centreCoords[2]);

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
