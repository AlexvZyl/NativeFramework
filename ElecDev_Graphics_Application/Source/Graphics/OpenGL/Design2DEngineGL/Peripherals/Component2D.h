#pragma once

#include "CoreGL/VertexArrayObjectGL.h"
#include "CoreGL/Entities/Polygon.h"

class Component2D
{
	//Private Members
private:
	//shape and edge data
	std::shared_ptr<VertexArrayObject> shapeVAO;
	std::shared_ptr<VertexArrayObject> edges;
	Polygon2D shape;

	//component shape attributes
	float height = 0.1f;
	float width = 0.1f;
	std::vector<glm::vec3> vertices;
	glm::vec2 centre;

	//colour attributes
	Colour shapeColour;
	Colour edgeColour;

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

	//Draw the component
	void draw();

	//Render the component
	void render();

	//Move the component to a new positioned centred at the given coordinates
	void moveTo(float pointerPos[2]);

	//Place a component
	void place(float pos[2]);
};
