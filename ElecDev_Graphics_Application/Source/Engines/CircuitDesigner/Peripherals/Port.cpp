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
#include "glm/gtc/matrix_transform.hpp"
#include "Utilities/Serialisation/Serialiser.h"
#include "OpenGL/Primitives/Circle.h"
#include "OpenGL/Primitives/Vertex.h"
#include "OpenGL/Primitives/Text.h"

//==============================================================================================================================================//
//  Methods.																																	//
//==============================================================================================================================================//

Port::Port(const glm::vec2& centre, PortType type, Component2D* parent, const std::string& label)
	: Entity(EntityType::PORT, parent),
	bodyColour(1.f, 1.f, 1.f, 1.f),
	borderColour(0.f, 0.f, 0.f, 1.f),
	centre(centre),
	m_type(type)
{
	// Setup indicator.
	switch (m_type) 
	{
	case PortType::PORT_IN:
		indicatorColour = { 0.4f, 0.8f, 0.4f, 0.5f };
		break;
	case PortType::PORT_OUT:
		indicatorColour = { 0.8f, 0.4f, 0.4f, 0.5f };
		break;
	case PortType::PORT_INOUT:
		indicatorColour = { 0.4f, 0.4f, 0.8f, 0.5f };
		break;
	}

	// Primitives.
	body = Renderer::addCircle2D(centre, portSize, bodyColour, -1.0f, 0.0f, this);
	border = Renderer::addCircle2D(centre, 1.1f * portSize, borderColour, -1.0f, 0.01f, this);
	attachmentIndicator = Renderer::addCircle2D(centre, portSize* indicatorFraction, indicatorColour, -1.0f, 0.f, this);
	portLayer = parent->componentLayer + parent->portLayerOffset;
	body->setColor(bodyColour);
	border->setColor(borderColour);
	setLayer(portLayer);

	// Assign port label.
	std::string labelLocal = label;
	if (label == "default") labelLocal = "Port " + std::to_string(parent->numPorts++);
	
	// Title.
	float textMargin = 0.0015;
	if (centre.x < 0)
	{
		titleOffset = glm::vec2{ -textMargin, 0.0f };
		glm::vec3 titlePos = glm::vec3(centre + titleOffset, portLayer);
		title = Renderer::addText2D(labelLocal, titlePos, titleColour, titleSize, "R", "B", this);
	}
	else 
	{
		titleOffset = glm::vec2{ textMargin, 0.0f };
		glm::vec3 titlePos = glm::vec3(centre + titleOffset, portLayer);
		title = Renderer::addText2D(labelLocal, titlePos, titleColour, titleSize, "L", "B", this);
	}

	if (parent->m_highlighted) enableOutline();
}

Port::Port(const YAML::Node& node, Component2D* parent)
	: Entity(EntityType::PORT, parent), bodyColour(1.f, 1.f, 1.f, 1.f)
{
	// General data.
	portLayer = parent->componentLayer + parent->portLayerOffset;

	// Set the port type.
	std::string type = node["Type"].as<std::string>();
	if (type == "PORT_INOUT")
	{
		m_type = PortType::PORT_INOUT;
		indicatorColour = { 0.4f, 0.4f, 0.8f, 0.5f };
	}
	else if (type == "PORT_IN")
	{
		m_type = PortType::PORT_IN;
		indicatorColour = { 0.4f, 0.8f, 0.4f, 0.5f };
	}
	else if (type == "PORT_OUT")
	{
		m_type = PortType::PORT_OUT;
		indicatorColour = { 0.8f, 0.4f, 0.4f, 0.5f };
	}

	// Add primitives.
	centre = { node["Centre"][0].as<float>(),  node["Centre"][1].as<float>() };
	borderColour = node["Border"]["Color"].as<glm::vec4>();
	body = Renderer::addCircle2D(centre, portSize, bodyColour, -1.0f, 0.0f, this);
	border = Renderer::addCircle2D(centre, 1.1f * portSize, borderColour, -1.0f, 0.01f, this);
	title = Renderer::addText2D(node["Title"], this);
	attachmentIndicator = Renderer::addCircle2D(centre, portSize * indicatorFraction, indicatorColour, -1.0f, 0.f, this);
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
			if (toRemove != cableList.end()) cableList.erase(toRemove);
		}
	}
}

void Port::moveTo(const glm::vec2& destination)
{
	// Update the port centre.
	title->translate(destination - centre);
	centre = destination + m_offset;
	glm::vec2 titlePos = centre + titleOffset;

	// Move each primative.
	body->translateTo(centre);
	border->translateTo(centre);
	attachmentIndicator->translateTo(centre);
	for (auto cable : m_cables) cable->followPort(this);
}

void Port::move(const glm::vec2& translation)
{	
	//update the port centre
	centre += translation;

	// Move each primative.
	body->translate(translation);
	border->translate(translation);
	attachmentIndicator->translate(translation);
	title->translate(translation);
	for (auto cable : m_cables) cable->followPort(this);
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

	if (cable->m_endPort) 
	{
		switch (cable->m_endPort->m_type) 
		{
		case PortType::PORT_IN:

			break;
		case PortType::PORT_OUT:

			break;
		case PortType::PORT_INOUT:

			break;
		}
	}
	
	switch (m_type) 
	{
	case PortType::PORT_IN:
		indicatorColour = { 0.f, 0.8f, 0.f, 1.f };
		break;
	case PortType::PORT_OUT:
		indicatorColour = { 0.8f, 0.f, 0.f, 1.f };
		break;
	case PortType::PORT_INOUT:
		indicatorColour = { 0.f, 0.f, 0.8f, 1.f };
		break;
	}

	attachmentIndicator->setColor(indicatorColour);
}

void Port::detachCable(Cable* cable)
{
	// Find the cable in the internal list.
	auto cableIt = std::find(m_cables.begin(), m_cables.end(), cable);

	// Remove the cable if found in the list.
	if (cableIt != m_cables.end()) m_cables.erase(cableIt);

	if (m_cables.empty()) 
	{
		switch (m_type) 
		{
		case PortType::PORT_IN:
			indicatorColour = { 0.4f, 0.6f, 0.4f, 0.5f };
			break;
		case PortType::PORT_OUT:
			indicatorColour = { 0.6f, 0.4f, 0.4f, 0.5f };
			break;
		case PortType::PORT_INOUT:
			indicatorColour = { 0.4f, 0.4f, 0.6f, 0.5f };
			break;
		}
		attachmentIndicator->setColor(indicatorColour);
		voltage = 0.f;
		fromTag = nullptr;
	}
}

void Port::updateType()
{
	// Assumes no hovered/attached ports (port types should only change in component designer).
	switch (m_type) 
	{
	case PortType::PORT_IN:
		indicatorColour = { 0.4f, 0.6f, 0.4f, 0.5f };
		break;
	case PortType::PORT_OUT:
		indicatorColour = { 0.6f, 0.4f, 0.4f, 0.5f };
		break;
	case PortType::PORT_INOUT:
		indicatorColour = { 0.4f, 0.4f, 0.6f, 0.5f };
		break;
	}

	attachmentIndicator->setColor(indicatorColour);
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
		indicatorColour.a = 0.5f;
		attachmentIndicator->setColor(indicatorColour);
	}
}

void Port::rotate(float degrees, const glm::vec3& rotatePoint, const glm::vec3& rotateNormal) 
{
	// Calculate the rotation transform.
	glm::mat4 transform = glm::translate(glm::mat4(1.f), rotatePoint);
	transform = glm::rotate(transform, glm::radians(degrees), rotateNormal);
	transform = glm::translate(transform, -rotatePoint);

	// Rotate.
	m_rotation += degrees;
	body->transform(transform);
	border->transform(transform);
	attachmentIndicator->transform(transform);
	title->transform(transform);

	// Update the port center.
	centre = glm::vec2(transform * glm::vec4(centre, 0.f, 1.f));

	// Update cables.
	for (auto cable : m_cables) cable->followPort(this);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//