//============================================================================================================================================//
// Includes.																																   //
//=============================================================================================================================================//

#include "Component2D.h"
#include "OpenGL/Primitives/Vertex.h"
#include "OpenGL/Primitives/Polygon.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "Graphics/Fonts/FontLoader.h"
#include "OpenGL/Primitives/Text.h"
#include "OpenGL/Primitives/PolyLine.h"
#include "Resources/ResourceHandler.h"
#include "OpenGL/Primitives/Circle.h"
#include <iostream>
#include "External/Misc/ConsoleColor.h"
#include "Circuit.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "Utilities/Logger/Logger.h"
#include "Utilities/Serialisation/Serialiser.h"
#include "Port.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

Component2D::Component2D(Circuit* parent)
	: Entity(EntityType::COMPONENT, parent)
{
	// Create vertices.
	centre = glm::vec2(0.0f, 0.0f);
	vertices.reserve(3);
	vertices.emplace_back(glm::vec3(centre.x - width, centre.y - height, 0.0f));
	vertices.emplace_back(glm::vec3(centre.x + width, centre.y - height, 0.0f));
	vertices.emplace_back(glm::vec3(centre.x + width, centre.y + height, 0.0f));

	// Component title.
	glm::vec3 titlePos = glm::vec3(centre + titleOffset, componentLayer + borderLayerOffset);
	title = Renderer::addText2D("Type", titlePos, titleColour, titleSize, "L", "T", this);
	designator = Renderer::addText2D("?", designatorOffset, titleColour, titleSize, "L", "B", this);
	enableOutline();

	// Default data dictionary.
	dataDict = {
		{ "Metric",			"1" },
		{ "Description",	"From(Circuit Database)" },
		{ "Unit",			"ea" },
		{ "DBRef",			"From(Circuit Database)" },
	};
}

Component2D::Component2D(const glm::vec2& centreCoords, Circuit* parent)
	: Component2D(parent)
{
	moveTo(centreCoords);
}

Component2D::Component2D(const YAML::Node& node, Circuit* parent)
	: Entity(EntityType::COMPONENT, parent)
{
	// Ensure the node is valid.
	YAML::Node componentNode = node;
	if (componentNode["Component"].IsDefined())
		componentNode = componentNode["Component"];

	// General data.
	m_internalCircuit = componentNode["Internal Circuit"].as<std::string>();

	// The data dictionary.
	YAML::Node dictNode = componentNode["Dictionary"];
	for (const auto& node : dictNode)
	{
		dataDict.insert({ node.first.as<std::string>(), node.second.as<std::string>() });
	}

	// Add tags.
	if (componentNode["Tag"].IsDefined())
		m_toTagNumber = componentNode["Tag"].as<std::string>();
	if (componentNode["From Tag"].IsDefined())
		m_fromTagNumber = componentNode["FromTag"].as<std::string>();

	// Add the equipmemnt type.
	if (componentNode["Title"].IsDefined())
	{
		title = Renderer::addText2D(componentNode["Title"], this);
		equipType = title->m_string;
	}
	else if (componentNode["Equipment Type"].IsDefined())
	{
		title = Renderer::addText2D(componentNode["Equipment Type"], this);
		equipType = title->m_string;
	}
	// Default title.
	else
	{
		glm::vec3 titlePos = glm::vec3(centre + titleOffset, componentLayer + borderLayerOffset);
		title = Renderer::addText2D("Type", titlePos, titleColour, titleSize, "L", "T", this);
	}

	// Designator.
	if (componentNode["Designator"].IsDefined())
	{
		designator = Renderer::addText2D(componentNode["Designator"], this);
		designatorSym = designator->m_string;
	}
	// Default designator.
	else 
	{
		designator = Renderer::addText2D("?", designatorOffset, titleColour, titleSize, "L", "B", this);
		designatorSym = designator->m_string;
	}

	//Add type
	if (componentNode["Type"].IsDefined())
	{
		type = componentNode["Type"].as<std::string>();
	}
	
	// Add the lines.
	for (const auto& line : componentNode["PolyLines"])
	{
		m_lines.push_back(Renderer::addPolyLine(line.second, this));
	}

	// Add polygons.
	int count = 0;
	for (const auto& poly : componentNode["Polygons"])
	{
		m_polygons.push_back(Renderer::addPolygon2D(poly.second, this));
		count++;
	}

	// Add circles.
	for (const auto& circle : componentNode["Circles"]) 
		m_circles.push_back(Renderer::addCircle2D(circle.second, this));

	// Add ports.
	for (const auto& port : componentNode["Ports"])
		ports.push_back(std::make_unique<Port>(port.second, this));

	// Add text.
	for (const auto& text : componentNode["Text"])
		m_text.push_back(Renderer::addText2D(text.second, this));

	// Rotate the component.
	if(componentNode["Rotation"].IsDefined())
		rotate(componentNode["Rotation"].as<float>());

	enableOutline();
}

Component2D::Component2D(const std::filesystem::path& path, Circuit* parent)
	: Component2D(YAML::LoadFile(path.string())["Component"], parent)
{}

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

void Component2D::moveTo(const glm::vec2& pointerPos)
{
	glm::vec2 translation = pointerPos - centre;
	move(translation);
}

void Component2D::move(const glm::vec2& translation)
{
	title->translate(translation);
	designator->translate(translation);
	for (auto& poly : m_polygons)	poly->translate(translation);
	for (auto& line : m_lines)		line->translate(translation);
	for (auto& circ : m_circles)	circ->translate(translation);
	for (auto& text : m_text)       text->translate(translation);
	for (auto& port : ports)		port->move(translation);
	centre += translation;
}

void Component2D::place(const glm::vec2& pos)
{	
	// Ensure the component is at the desired position.
	moveTo(pos);
	setLayer(0.0f);
	title->setColor(titleColour);
	for (auto& poly : m_polygons) poly->setColor(shapeColour);
	for (auto& circ : m_circles)  circ->setColor(shapeColour);
}

void Component2D::setLayer(float layer)
{
	title->setLayer(layer);
	designator->setLayer(layer);
	for (auto& poly : m_polygons)  poly->setLayer(layer);
	for (auto& line : m_lines)	   line->setLayer(layer);
	for (auto& circ : m_circles)   circ->setLayer(layer);
	for (auto& text : m_text)      text->setLayer(layer);
	for (auto& port : ports)	   port->setLayer(layer + portLayerOffset);
	componentLayer = layer;
}

void Component2D::enableOutline()
{
	m_highlighted = true;
	title->enableOutline();
	designator->enableOutline();
	for (auto& poly : m_polygons)  poly->enableOutline();
	for (auto& line : m_lines)	   line->enableOutline();
	for (auto& circ : m_circles)   circ->enableOutline();
	for (auto& text : m_text)      text->enableOutline();
	for (auto& port : ports)	   port->enableOutline();
}

void Component2D::disableOutline()
{
	m_highlighted = false;
	title->disableOutline();
	designator->disableOutline();
	for (auto& poly : m_polygons) poly->disableOutline();
	for (auto& line : m_lines)    line->disableOutline();
	for (auto& circ : m_circles)  circ->disableOutline();
	for (auto& text : m_text)     text->disableOutline();
	for (auto& port : ports)	  port->disableOutline();
}

void Component2D::removePort(Port* port)
{
	auto port_to_remove = std::find_if(begin(ports), end(ports), [&](std::shared_ptr<Port> current)
		{
			return current.get() == port;
		});
	if (port_to_remove != end(ports))
	{
		ports.erase(port_to_remove);
		ports.shrink_to_fit();
		return;
	}

	// Port was not found on this component.
	std::string msg = "Tried to delete port '" + port->title->m_string + "', but it does not belong to component '" + designator->m_string + std::to_string(designatorIdx) + "'.";
	LUMEN_LOG_WARN(msg, "");
}


void Component2D::translateTitle(glm::vec2 translation)
{
	titleOffset += translation;
	title->translate(translation);
}

void Component2D::updateText()
{
	std::string textString = designatorSym + std::to_string(designatorIdx);
	if (title->updateText(equipType)) 
		title->rotate(m_rotation, glm::vec3(centre, 0.f), { 0.f, 0.f, 1.f });

	if (designator->updateText(textString))
		designator->rotate(m_rotation, glm::vec3(centre, 0.f), {0.f, 0.f, 1.f});
}

void Component2D::updateTextWithoutLabel()
{
	if (title->updateText(equipType))
		title->rotate(m_rotation, glm::vec3(centre, 0.f), { 0.f, 0.f, 1.f });

	if (designator->updateText(designatorSym))
		designator->rotate(m_rotation, glm::vec3(centre, 0.f), { 0.f, 0.f, 1.f });
}

void Component2D::setColour(const glm::vec4& colour)
{
	shapeColour = colour;
	for (auto& poly : m_polygons) poly->setColor(shapeColour);
	for (auto& circ : m_circles)  circ->setColor(shapeColour);
}

void Component2D::addPoly(std::unique_ptr<Polygon2D>&& poly)
{
	m_polygons.push_back(poly);
	m_polygons.back()->setLayer(0.001f);//temp fix
}

void Component2D::addCircle(std::unique_ptr<Circle>&& circle)
{
	m_circles.push_back(circle);
}

void Component2D::addLine(std::unique_ptr<PolyLine>&& line)
{
	m_lines.push_back(line);
}

void Component2D::addPort(std::unique_ptr<Port>&& port)
{
	ports.push_back(port);
}

void Component2D::removePoly(Polygon2D* poly)
{
	auto to_remove = std::find(begin(m_polygons), end(m_polygons), poly);
	if (to_remove != m_polygons.end()) 
	{
		m_polygons.erase(to_remove);
	}
	else LUMEN_LOG_WARN("Attempted to remove a polygon that is not a member of m_polygons.", "Component2D");
}

void Component2D::removeCircle(Circle* circle)
{
	auto to_remove = std::find(begin(m_circles), end(m_circles), circle);
	if (to_remove != m_circles.end()) 
	{
		m_circles.erase(to_remove);
	}
	else LUMEN_LOG_WARN("Attempted to remove a circle that is not a member of m_circles.", "Component2D");
}

void Component2D::removeLine(PolyLine* line)
{
	auto to_remove = std::find(begin(m_lines), end(m_lines), line);
	if (to_remove != m_lines.end()) 
	{
		m_lines.erase(to_remove);
	}
	else LUMEN_LOG_WARN("Attempted to remove a line that is not a member of m_lines.", "Component2D");
}

void Component2D::removeText(Text* text)
{
	auto to_remove = std::find(begin(m_text), end(m_text), text);
	if (to_remove != m_text.end())
	{
		m_text.erase(to_remove);
	}
	else LUMEN_LOG_WARN("Attempted to remove a text object that is not a member of m_text. This can be expected when attempting to delete port or component names.", "Component2D");
}

void Component2D::rotate(float degrees)
{
	m_rotation += degrees;
	glm::vec3 rotationPoint = { centre.x, centre.y, 0.f };
	glm::vec3 rotateNormal = {0.f, 0.f, 1.f};
	title->rotate(degrees, rotationPoint, rotateNormal);
	designator->rotate(degrees, rotationPoint, rotateNormal);
	for (auto& poly : m_polygons) poly->rotate(degrees, rotationPoint, rotateNormal);
	for (auto& line : m_lines)    line->rotate(degrees, rotationPoint, rotateNormal);
	for (auto& circ : m_circles)  circ->rotate(degrees, rotationPoint, rotateNormal);
	for (auto& text : m_text)     text->rotate(degrees, rotationPoint, rotateNormal);
	for (auto& port : ports)	  port->rotate(degrees, rotationPoint, rotateNormal);
}

//=============================================================================================================================================//
//  Utilities.				     																											   //
//=============================================================================================================================================//

PortType Component2D::getPortType(YAML::Node node)
{
	if		(node["Type"].as<std::string>() == "PORT_IN")	 { return PortType::PORT_IN; }
	else if (node["Type"].as<std::string>() == "PORT_OUT")	 { return PortType::PORT_OUT; }
	else if (node["Type"].as<std::string>() == "PORT_INOUT") { return PortType::PORT_INOUT; }
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//