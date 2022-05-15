//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Utilities/Logger/Logger.h"
#include "Port.h"
#include "Component2D.h"
#include "OpenGL/Primitives/Polygon.h"
#include "OpenGL/Primitives/Text.h"
#include "Graphics/Fonts/FontLoader.h"
#include "Resources/ResourceHandler.h"
#include "Cable.h"
#include "Circuit.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/Renderer/RendererGL.h"
#include <iostream>

//==============================================================================================================================================//
//  Methods.																																	//
//==============================================================================================================================================//

Port::Port(const glm::vec2& centre, PortType type, Component2D* parent, const std::string& label)
	: Entity(EntityType::PORT, parent),
	bodyColour(0.7f, 0.7f, 0.7f, 1.f),
	borderColour(0.f, 0.f, 0.f, 1.f),
	// m_offset(offset),
	centre(centre),
	m_type(type)
{
	// --------------------- //
	//  P R I M I T I V E S  //
	// --------------------- //

	body = Renderer::addCircle2D(centre, portSize, bodyColour, 1.0f, 0.0f, this);
	border = Renderer::addCircle2D(centre, 1.1f*portSize, borderColour, 1.0f, 0.01f, this);
	attachmentIndicator = Renderer::addCircle2D(centre, portSize*0.5f, indicatorColour, 1.0f, 0.01f, this);
	portLayer = parent->componentLayer + parent->portLayerOffset;

	// Assign port label.
	if (label == "default")
		m_label = "Port " + std::to_string(parent->numPorts++);
	else m_label = label;

	float textMargin = 0.0015;
	//OLD DEPRECATED CODE
	//infer the port position from the offset, and set the title
	/*if (m_offset.y > 0.078)
	{//top
		m_position = PortPosition::TOP;
		titleOffset = glm::vec2{ 0.f, -textMargin };
		glm::vec3 titlePos = glm::vec3(centre + titleOffset, portLayer);
		title = Renderer::addText2D(m_label, titlePos, titleColour, titleSize, "L", "C");
		title->rotate(-90);
	}
	else if (m_offset.y < -0.078)
	{//bottom
		m_position = PortPosition::BOTTOM;
		titleOffset = glm::vec2{ 0.f, textMargin };
		glm::vec3 titlePos = glm::vec3(centre + titleOffset, portLayer);
		title = Renderer::addText2D(m_label, titlePos, titleColour, titleSize, "L", "C");
		title->rotate(90);
	}
	else if (m_offset.x > 0.078)
	{//right
		m_position = PortPosition::RIGHT;
		titleOffset = glm::vec2{ -textMargin, 0.0f };
		glm::vec3 titlePos = glm::vec3(centre + titleOffset, portLayer);
		title = Renderer::addText2D(m_label, titlePos, titleColour, titleSize, "R", "C");
	}
	else if (m_offset.x < -0.078)
	{//left
		m_position = PortPosition::LEFT;
		titleOffset = glm::vec2{ textMargin, 0.0f };
		glm::vec3 titlePos = glm::vec3(centre + titleOffset, portLayer);
		title = Renderer::addText2D(m_label, titlePos, titleColour, titleSize, "L", "C");
	}
	else {
		//This should never happen. Print a warning!
		LUMEN_LOG_ERROR("Invalid port offset.", "Design Engine");
	}*/
	//m_position = PortPosition::RIGHT;//deprecated
	if (centre.x < 0){
		titleOffset = glm::vec2{ -textMargin, 0.0f };
		glm::vec3 titlePos = glm::vec3(centre + titleOffset, portLayer);
		title = Renderer::addText2D(m_label, titlePos, titleColour, titleSize, "R", "B", this);
	}
	else {
		titleOffset = glm::vec2{ textMargin, 0.0f };
		glm::vec3 titlePos = glm::vec3(centre + titleOffset, portLayer);
		title = Renderer::addText2D(m_label, titlePos, titleColour, titleSize, "L", "B", this);
	}
	body->setColor(bodyColour);
	border->setColor(borderColour);
	setLayer(portLayer);

	if (parent->m_highlighted)
	{
		enableOutline();
	}
}

Port::Port(const YAML::Node& node, Component2D* parent)
	: Entity(EntityType::PORT, parent)
{
	// General data.
	m_label = node["Label"].as<std::string>();
	portLayer = parent->componentLayer + parent->portLayerOffset;

	// Set the port type.
	std::string type = node["Type"].as<std::string>();
	if (type == "PORT_INOUT")
	{
		m_type = PortType::PORT_INOUT;
	}
	else if (type == "PORT_IN")
	{
		m_type = PortType::PORT_IN;
	}
	else if (type == "PORT_OUT")
	{
		m_type = PortType::PORT_OUT;
	}

	// Add shapes.
	body = Renderer::addCircle2D(node["Body"], this);
	border = Renderer::addCircle2D(node["Border"], this);
	title = Renderer::addText2D(node["Title"], this);
	bodyColour = body->m_colour;
	borderColour = border->m_colour;
	centre = body->m_trackedCenter;
	attachmentIndicator = Renderer::addCircle2D(centre, portSize * 0.5f, indicatorColour, 1.0f, 0.01f, this);
	setLayer(portLayer);
}

Port::~Port()
{
	// Check if port is in a circuit.
	if (m_parent && dynamic_cast<Circuit*>(m_parent->m_parent))
	{
		// If a port is removed, we need to find and destroy any linked cables.
		auto& cableList = dynamic_cast<Circuit*>(m_parent->m_parent)->m_cables;
		for (Cable* cable : m_cables)
		{
			auto toRemove = std::find_if(cableList.begin(), cableList.end(), [&](std::shared_ptr<Cable> current)
				{
					return current.get() == cable;
				});

			// Check that the cable is in the list.
			if (toRemove != cableList.end())
			{
				cableList.erase(toRemove);
			}
		}
	}

	// Remove the renderer primitives.
	Renderer::remove(body);
	Renderer::remove(border);
	Renderer::remove(attachmentIndicator);
	Renderer::remove(title);
}

void Port::moveTo(const glm::vec2& destination)
{
	//update the port centre
	title->translate(destination - centre);
	centre = destination + m_offset;
	glm::vec2 titlePos = centre + titleOffset;
	//move each primative
	body->translateTo(centre);
	border->translateTo(centre);
	attachmentIndicator->translateTo(centre);
	
	for (Cable* cable: m_cables)
		cable->followPort(this);
}

void Port::move(const glm::vec2& translation)
{	
	//update the port centre
	centre += translation;
	//move each primative
	body->translate(translation);
	border->translate(translation);
	attachmentIndicator->translate(translation);
	title->translate(translation);
	for (Cable* cable : m_cables)
		cable->followPort(this);
}

Port& Port::operator=(const Port& t)
{
	return *this;
}

void Port::setLayer(float layer)
{
	portLayer = layer;
	body->setLayer(layer);
	border->setLayer(layer);
	attachmentIndicator->setLayer(layer + 0.001f);
	title->setLayer(layer);
}

void Port::enableOutline()
{
	border->enableOutline();
	body->enableOutline();
	title->enableOutline();
}

void Port::disableOutline()
{
	border->disableOutline();
	body->disableOutline();
	title->disableOutline();
}

void Port::setOffset(const glm::vec2& offset)
{
	// Move port to new offset (trust the math).
	moveTo(centre - m_offset - m_offset + offset);
	// Update internal offset.
	m_offset = offset;
}

void Port::attachCable(Cable* cable)
{
	m_cables.push_back(cable);
	indicatorColour = { 0.f, 0.f, 0.f, 1.0f };
	attachmentIndicator->setColor(indicatorColour);
}

void Port::detachCable(Cable* cable)
{
	// Find the cable in the internal list.
	auto cableIt = std::find(m_cables.begin(), m_cables.end(), cable);

	// Remove the cable if found in the list.
	if (cableIt != m_cables.end()) 
		m_cables.erase(cableIt);

	if (m_cables.empty()) 
	{
		indicatorColour = { 0.5f, 0.5f, 0.5f, 0.f };
		attachmentIndicator->setColor(indicatorColour);
	}
}

void Port::showAttachIndicator()
{
	if (m_cables.empty()) 
	{
		indicatorColour.a = 1.f;
		attachmentIndicator->setColor(indicatorColour);
	}
}

void Port::hideAttachIndicator()
{
	if (m_cables.empty()) 
	{
		indicatorColour.a = 0.f;
		attachmentIndicator->setColor(indicatorColour);
	}
}

void Port::setContext(GUIState* guiState)
{
	m_parent->setContext();
}

void Port::rotate(float degrees, const glm::vec3& rotatePoint, const glm::vec3& rotateNormal) 
{
	// Rotate.
	m_rotation += degrees;
	body->rotate(degrees, rotatePoint, rotateNormal);
	border->rotate(degrees, rotatePoint, rotateNormal);
	attachmentIndicator->rotate(degrees, rotatePoint, rotateNormal);
	title->rotate(degrees, rotatePoint, rotateNormal);

	// Update the port center.


	// Update cables.
	for (Cable* cable : m_cables) cable->followPort(this);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//