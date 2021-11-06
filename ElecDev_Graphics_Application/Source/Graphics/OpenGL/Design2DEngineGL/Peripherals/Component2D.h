#pragma once

#include "../../CoreGL/VertexArrayObjectGL.h"

class Component2D
{
	//Private Members
private:
	//shape and edge data
	std::shared_ptr<VertexArrayObject> shape;
	std::shared_ptr<VertexArrayObject> edges;

	//component shape attributes
	float height = 0.1f;
	float width = 0.1f;
	float3 vertices[4];
	float2 centre;

	//colour attributes
	Colour shapeColour;
	Colour edgeColour;

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
};
