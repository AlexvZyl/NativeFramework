//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Port.h"
#include "Component2D.h"
#include "OpenGL/Primitives/Polygon.h"
#include "OpenGL/Primitives/Text.h"
#include "Graphics/Fonts/FontLoader.h"
#include "Resources/ResourceHandler.h"
#include "Cable.h"
#include "Circuit.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/RendererGL.h"

//==============================================================================================================================================//
//  Methods.																																	//
//==============================================================================================================================================//

Port::Port(const glm::vec2& offset, PortType type, Component2D* parent, const std::string& label) 
	: Entity(EntityType::PORT, parent), 
	  bodyColour( 0.7f, 0.7f, 0.7f, 1.f ),
	  borderColour(0.f, 0.f, 0.f, 1.f),
	  m_offset(offset),
	  centre(parent->centre + offset),
	  m_type(type)
{
	// --------------------- //
	//  P R I M I T I V E S  //
	// --------------------- //

	body = Renderer::addCircle2D(centre, 0.01f, bodyColour, 1.0f, 0.0f, this);
	border = Renderer::addCircle2D(centre, 0.011f, borderColour, 1.0f, 0.01f, this);
	attachmentIndicator = Renderer::addCircle2D(centre, 0.005f, indicatorColour, 1.0f, 0.01f, this);

	portLayer = parent->componentLayer + parent->portLayerOffset;

	// Assign port label.
	if (label == "default") 
		m_label = "Port " + std::to_string(parent->numPorts++);
	else m_label = label;

	float textMargin = 0.015;
	//infer the port position from the offset, and set the title
	if (m_offset.y > 0.099) 
	{//top
		m_position = PortPosition::TOP;
		titleOffset = glm::vec2{ 0.f, -textMargin };
		glm::vec3 titlePos = glm::vec3(centre + titleOffset, portLayer);
		title = Renderer::addText2D(m_label, titlePos, titleColour, titleSize, "C", "T", this);
	}	
	else if (m_offset.y < -0.099) 
	{//bottom
		m_position = PortPosition::BOTTOM;
		titleOffset = glm::vec2{ 0.f, textMargin };
		glm::vec3 titlePos = glm::vec3(centre + titleOffset, portLayer);
		title = Renderer::addText2D(m_label, titlePos, titleColour, titleSize, "C", "U", this);
	}
	else if (m_offset.x > 0.099) 
	{//right
		m_position = PortPosition::RIGHT;
		titleOffset = glm::vec2{ -textMargin, 0.0f };
		glm::vec3 titlePos = glm::vec3(centre + titleOffset, portLayer);
		title = Renderer::addText2D(m_label, titlePos, titleColour, titleSize, "R", "C", this);
	}
	else if (m_offset.x < -0.099) 
	{//left
		m_position = PortPosition::LEFT;
		titleOffset = glm::vec2{ textMargin, 0.0f };
		glm::vec3 titlePos = glm::vec3(centre + titleOffset, portLayer);
		title = Renderer::addText2D(m_label, titlePos, titleColour, titleSize, "L", "C", this);
	}
	body->setColor(bodyColour);
	border->setColor(borderColour);
	setLayer(portLayer);

	highlight();
}

Port::~Port()
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

	// Remove the renderer primitives.
	Renderer::remove(body);
	Renderer::remove(border);
	Renderer::remove(attachmentIndicator);
	Renderer::remove(title);
}

void Port::moveTo(const glm::vec2& destination)
{
	//update the port centre
	centre = destination + m_offset;
	glm::vec2 titlePos = centre + titleOffset;
	//move each primative
	body->translateTo(centre);
	border->translateTo(centre);
	attachmentIndicator->translateTo(centre);
	title->translateTo(titlePos);
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

void Port::highlight()
{
	borderColour = { 0.f, 0.f, 1.0f, 1.f };
	border->setColor(borderColour);
}

void Port::unhighlight()
{
	borderColour = { 0.f, 0.f, 0.f, 1.f };
	border->setColor(borderColour);
}

void Port::setOffset(const glm::vec2& offset)
{
	//move port to new offset (trust the math)
	moveTo(centre - m_offset - m_offset + offset);
	//update internal offset
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
	//guiState->clickedZone.port = true;
	m_parent->setContext();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//