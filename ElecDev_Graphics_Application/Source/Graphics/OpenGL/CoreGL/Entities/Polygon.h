#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "../VertexArrayObjectGL.h"
#include "Entity.h"

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

class Polygon2D : public Entity
{
private:

	// Center position of the polygon.
	glm::vec2 m_pos;

	// Other vertex data.
	Colour m_colour;
	unsigned m_eID = 0;

public:

	//polygon shape attributes
	unsigned n_vertices;

	//Construct
	Polygon2D() = default;

	//Creates a regular polygon with n_vertices vertices centred at (0, 0)
	//**should change this to only accept GL_TRIANGLE type VAOs**
	//Polygon2D(unsigned n_vertices, VertexArrayObject* VAO);

	//Adds a polygon with the given vertices centred at (0,0) to a VBO
	Polygon2D(std::vector<glm::vec3> vertices, VertexArrayObject* VAO);
	//Remove a polygon from the VBO and deconstruct
	~Polygon2D();
	//Update the polygon
	void update();
	//Perform a translation operation
	void translate(glm::vec2 transVec);
	//Perform a translation to the specified position
	void translateTo(glm::vec2 pos);
	//Perform a rotation operation
	void rotate(float angle);
	//Set the colour of the polygon
	void setColour(Colour col);
	//Set the entity ID
	void setEID(unsigned eID);
	// Deletes the entity
	void destroy();
};

//=============================================================================================================================================//
//  EOF.																																   //
//=============================================================================================================================================//