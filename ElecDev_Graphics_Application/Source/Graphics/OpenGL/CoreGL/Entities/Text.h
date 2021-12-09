#pragma once

// The first 4 vertices in the text vertex buffer contains the text box.

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Entity.h"
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

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

template<typename VertexType=VertexDataTextured>
class Text : public Entity<VertexType>
{
public:

	// ------------ //
	//  E N T I T Y //
	// ------------ //

	using Entity<VertexType>::m_vertices;
	using Entity<VertexType>::m_indices;
	using Entity<VertexType>::m_vertexCount;
	using Entity<VertexType>::m_indexCount;
	using Entity<VertexType>::m_VAO;
	using Entity<VertexType>::m_bufferStartIndex;
	using Entity<VertexType>::m_trackedCenter;
	using Entity<VertexType>::m_colour;
	using Entity<VertexType>::m_entityID;

	// --------- //
	//  T E X T  //
	// --------- //

	float m_textScale = 1;
	Font* m_font;
	std::string m_verticalAlign;
	std::string m_horizontalAlign;
	float m_textLength = 0;
	glm::vec3 m_cursorStart = {0.f, 0.f, 0.f};

	// Constructor.
	Text(std::string text, glm::vec3& position, glm::vec4& color, float scale,
		 VertexArrayObject<VertexType>* vao, Font& font, ManagedEntity* parent,
		 std::string horizontalAlignment = "L", std::string verticalAlignment = "B");
	// Initiliser.  Used to allow the changing of the class without having
	// to destroy it.
	void generateText(std::string text);
	// Destructor.
	~Text();

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
