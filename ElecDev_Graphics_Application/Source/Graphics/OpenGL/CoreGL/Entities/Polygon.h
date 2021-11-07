#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <vector>
#include <glm.hpp>
#include "../VertexArrayObjectGL.h"

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

class Polygon2D
{

	//Private Members
private:
	//VBO members
	//Pointer to the VAO to use for the polygon
	VertexArrayObject* m_VAO = NULL;

	//position of the polygon
	glm::vec2 m_pos;

	//other polygon attributes
	Colour m_colour;
	unsigned m_eID = 0;

	//Public Members
public:
	//indices for VBO access
	unsigned int start_idx;



	//polygon shape attributes
	unsigned n_vertices;
	std::vector<VertexData> m_vertices;

	//Constructors

	//Creates a regular polygon with n_vertices vertices centred at (0, 0)
	//**should change this to only accept GL_TRIANGLE type VAOs**
	//Polygon2D(unsigned n_vertices, VertexArrayObject* VAO);

	//Adds a polygon with the given vertices centred at (0,0) to a VBO
	Polygon2D(std::vector<glm::vec3> vertices, VertexArrayObject* VAO);

	//Remove a polygon from the VBO and deconstruct
	~Polygon2D();

	//Draw the polygon
	void draw();

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
};

//=============================================================================================================================================//
//  EOF.																																   //
//=============================================================================================================================================//