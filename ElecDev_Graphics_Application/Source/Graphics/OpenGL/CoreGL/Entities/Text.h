#pragma once
#include "../VertexArrayObjectGL.h"
#include "Entity.h"
class Text : public Entity
{


	//Private Members
private:
	//position of the entity
	glm::vec2 m_pos;
	float length = 0.f;
	float height = 0.f;

	//other entity attributes
	Colour m_colour;
	unsigned m_eID = 0;

	//Public Members
public:

	//Construct
	Text() = default;


	//Adds text centred at (0,0) to a VBO
	Text(std::string text, glm::vec2 position, Colour col, float scale, std::string align, VertexArrayObject * VAO);

	//Remove a polygon from the VBO and deconstruct
	~Text();

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
};

