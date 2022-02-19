#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <string>
#include <memory>
#include "glm/glm.hpp"
#include "Graphics/Entities/Entity.h"
#include "OpenGL/Primitives/Circle.h"
#include "OpenGL/Primitives/Vertex.h"
#include "OpenGL/Primitives/Text.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class Polygon2D;
class Component2D;
class Cable;
class Scene;
class IOIndicator;

struct Font;

//==============================================================================================================================================//
//  Data.																																		//
//==============================================================================================================================================//

enum class PortType
{
	PORT_IN, PORT_OUT, PORT_INOUT
};

enum class PortPosition
{
	TOP, BOTTOM, LEFT, RIGHT
};

//==============================================================================================================================================//
//  Port class.																																	//
//==============================================================================================================================================//

class Port: public Entity
{
public:
	
	glm::vec2 titleOffset;
	static Font titleFont;
	glm::vec4 titleColour = glm::vec4(0.f, 0.f, 0.f, 1.f);
	float titleSize = 0.02f;
	glm::vec2 centre;
	static unsigned portID;
	glm::vec4 bodyColour;
	glm::vec4 borderColour;
	glm::vec4 indicatorColour = {0.5f, 0.5f, 0.5f, 0.f};
	float portLayer;

	Circle* body;
	Circle* border;
	Circle* attachmentIndicator;
	Text* title;

	std::string m_label;
	glm::vec2 m_offset = { 0, 0 };
	PortPosition m_position;
	PortType m_type;

	std::vector<Cable*> m_cables;

	// Constructor.
	Port(const glm::vec2& pos, PortType type, Component2D* parent, const std::string& label = "default");
	// Destructor.
	~Port();

	Port& operator = (const Port &t);
	
	// Helper methods.
	void moveTo(const glm::vec2& destination);
	void move(const glm::vec2& translation);
	void setLayer(float layer);
	void setContext(GUIState* guiState);
	void highlight();
	void unhighlight();
	void setOffset(const glm::vec2& offset);
	void attachCable(Cable* cable);
	void detachCable(Cable* cable);
	void showAttachIndicator();
	void hideAttachIndicator();
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//