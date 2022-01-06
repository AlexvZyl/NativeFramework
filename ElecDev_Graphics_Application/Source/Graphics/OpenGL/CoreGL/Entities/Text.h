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

	// Constructor.
	Text(std::string text, glm::vec3& position, glm::vec4& color, float scale,
		 VertexArrayObject<VertexDataTextured>* vao, Font& font, Entity* parent,
		 std::string horizontalAlignment = "L", std::string verticalAlignment = "B");
	// Updates the text of the text entity.
	void updateText(std::string text);
	// Sets the text box colour.
	void setBoxColour(glm::vec4 colour);
	// Sets the text colour only.
	virtual void setColor(glm::vec4& color) override;
	// Sets the later of the text entity.
	// Currently the text box layer is automatically set to be right behind the text.
	virtual void setLayer(float layer) override;

private:

	// Data.
	glm::vec3 m_cursorStart = { 0.f, 0.f, 0.f };
	glm::vec4 m_boxColor = { 0.f, 0.f, 0.f, 0.f };
	Font* m_font = nullptr;
	float m_textScale = 1;
	std::string m_verticalAlign = "L";
	std::string m_horizontalAlign = "B";
	float m_textLength = 0;

	// Generates the textured quads.
	void generateText(std::string text);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
