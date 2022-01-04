#pragma once

// The first 4 vertices in the text vertex buffer contains the text box.

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Primitive.h"
#include <string>
#include <map>

//=============================================================================================================================================//
//  Forward declarations.																													   //
//=============================================================================================================================================//

template<typename VertexType>
class VertexArrayObject;

class VertexDataTextured;

struct Character;
struct Font;

//==============================================================================================================================================//
//  Class.																																	    //
//==============================================================================================================================================//

class Text : public Primitive<VertexDataTextured>
{
public:

	float m_textScale = 1;
	Font* m_font;
	std::string m_verticalAlign;
	std::string m_horizontalAlign;
	float m_textLength = 0;
	glm::vec3 m_cursorStart = { 0.f, 0.f, 0.f };
	glm::vec4 m_boxColor = { 0.f, 0.f, 0.f, 0.f };

	// Constructor.
	Text(std::string text, glm::vec3& position, glm::vec4& color, float scale,
		 VertexArrayObject<VertexDataTextured>* vao, Font& font, Entity* parent,
		 std::string horizontalAlignment = "L", std::string verticalAlignment = "B");
	// Generates the textured quads.
	void generateText(std::string text);

	// Updates the text of the text entity.
	void updateText(std::string text);
	// Sets the text box colour.
	void setBoxColour(glm::vec4 colour);
	// Sets the text colour only.
	// Has to be overridden from the parent class since the
	// parent class will change the text box colour as well.
	virtual void setColor(glm::vec4& color) override;
	// Sets the later of the text entity.
	// This has to be overridden so that the box and text can be 
	// set seperately.
	virtual void setLayer(float layer) override;
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
