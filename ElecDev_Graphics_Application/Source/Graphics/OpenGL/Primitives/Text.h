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
	Text(const std::string& text, const glm::vec3& position, const glm::vec4& color, float scale,
		 VertexArrayObject<VertexDataTextured>* VAO, Font* font, Entity* parent,
		 const std::string& horizontalAlignment = "L", const std::string& verticalAlignment = "B");

	// Updates the text of the text entity.
	void updateText(const std::string& text);
	void updateAlignment(const std::string& horizontalAlignment, const std::string& verticalAlignment = "B");

	// Sets the text box colour.
	void setBoxColour(const glm::vec4& colour);

	// Sets the text colour only.
	virtual void setColor(const glm::vec4& color) override;

	// Sets the later of the text entity.
	// Currently the text box layer is automatically set to be right behind the text.
	virtual void setLayer(float layer) override;

	// Data.
	glm::vec4 m_boxColor = { 0.f, 0.f, 0.f, 0.f };
	Font* m_font = nullptr;
	float m_textScale = 1;
	std::string m_verticalAlign = "L";
	std::string m_horizontalAlign = "B";
	std::string m_string = "";
	float m_textLength = 0;

	// Generates the textured quads.
	void generateText(const std::string& text);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
