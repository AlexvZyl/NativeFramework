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
//  Variables.																																   //
//=============================================================================================================================================//

//Add font for component titles
Font Component2D::titleFont = msdfLoadFont(ARIAL_NORMAL_JSON);
//Font Component2D::titleFont = msdfLoadFont(ARIAL_BOLD_MSDF_JSON);
unsigned Component2D::componentID = 0;

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

Component2D::Component2D(Circuit* parent)
	: Entity(EntityType::COMPONENT, parent)
{
	// ----------- //
	//  S E T U P  //
	// ----------- //

	// Create vertices.
	centre = glm::vec2(0.0f, 0.0f);
	vertices.reserve(5);
	vertices.emplace_back(glm::vec3(centre.x - width, centre.y - height, 0.0f));
	vertices.emplace_back(glm::vec3(centre.x + width, centre.y - height, 0.0f));
	vertices.emplace_back(glm::vec3(centre.x + width, centre.y + height, 0.0f));
	//vertices.emplace_back(glm::vec3(centre.x - width, centre.y + height, 0.0f));
	//vertices.emplace_back(glm::vec3(centre.x - width/2, centre.y + height/2, 0.0f));

	// --------------------- //
	//  P R I M I T I V E S  //
	// --------------------- //

	// Main shape.
	//shape = Renderer::addPolygon2D(vertices, this);
	//shape->setColor({shapeColour.r, shapeColour.g, shapeColour.b, 0.5f});
	//shape->setLayer(0.001f);//temp fix
	// Component border.
	//border = Renderer::addPolygon2DClear(vertices, this);
	//border->setColor(borderColour);
	//border->setLayer(componentLayer + borderLayerOffset);
	// Component title.
	glm::vec3 titlePos = glm::vec3(centre + titleOffset, componentLayer + borderLayerOffset);
	//titleString = "Component " + std::to_string(componentID++);
	//std::string textString = equipType + std::string(": ") + titleString;
	title = Renderer::addText2D("Type", titlePos, titleColour, titleSize, "L", "T", this);
	designator = Renderer::addText2D("?", designatorOffset, titleColour, titleSize, "L", "B", this);
	// Add some test ports. (TO BE REMOVED). PLease keep this here while we are testing (at least until we have some generic components that can be added). 
	// It is a bit of a pain setting up ports every time we test.
	//addPort(0, PortType::PORT_IN, "LX1");
	//addPort(1, PortType::PORT_OUT, "RX1");
	enableOutline();

	// General dictionary data.
	dataDict.insert({ "Metric", "1" });
	dataDict.insert({ "Description", "From(Circuit Database)" });
	dataDict.insert({ "Unit", "ea" });
	dataDict.insert({ "DBRef", "From(Circuit Database)" });
}

Component2D::Component2D(const glm::vec2& centreCoords, Circuit* parent)
	: Component2D(parent)
{
	translateTo(centreCoords);
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

	/*/ Add tags.
	if (componentNode["Tag"].IsDefined())
		m_toTagNumber = componentNode["Tag"].as<std::string>();
	if (componentNode["From Tag"].IsDefined())
		m_fromTagNumber = componentNode["FromTag"].as<std::string>();
	*/

	// Add the equipmemnt type.
	if (componentNode["Title"].IsDefined())
	{
		title = Renderer::addText2D(componentNode["Title"], this);
		//equipType = title->m_string;
	}
	else if (componentNode["Equipment Type"].IsDefined())
	{
		title = Renderer::addText2D(componentNode["Equipment Type"], this);
		//equipType = title->m_string;
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
		ports.push_back(std::make_shared<Port>(port.second, this));

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

Component2D::~Component2D() 
{
	ports.clear();
	Renderer::remove(title);
	Renderer::remove(designator);
	for (auto circle : m_circles) Renderer::remove(circle);
	for (auto line : m_lines)     Renderer::remove(line);
	for (auto poly : m_polygons)  Renderer::remove(poly);
	for (auto text : m_text)	  Renderer::remove(text);
}

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

void Component2D::translateTo(const glm::vec2& pointerPos)
{
	glm::vec2 translation = pointerPos - centre;
	translate(translation);
}

void Component2D::translate(const glm::vec2& translation)
{
	title->translate(translation);
	designator->translate(translation);
	for (auto poly : m_polygons)	poly->translate(translation);
	for (auto line : m_lines)		line->translate(translation);
	for (auto circ : m_circles)	    circ->translate(translation);
	for (auto text : m_text)        text->translate(translation);
	for (auto& port : ports)		port->translate(translation);
	centre += translation;
}

void Component2D::place(const glm::vec2& pos)
{	
	// Ensure the component is at the desired position.
	translateTo(pos);
	setLayer(0.0f);
	title->setColor(titleColour);
	for (auto poly : m_polygons) poly->setColor(shapeColour);
	for (auto circ : m_circles)  circ->setColor(shapeColour);
}

void Component2D::setLayer(float layer)
{
	title->setLayer(layer);
	designator->setLayer(layer);
	for (auto poly : m_polygons)  poly->setLayer(layer);
	for (auto line : m_lines)	  line->setLayer(layer);
	for (auto circ : m_circles)   circ->setLayer(layer);
	for (auto text : m_text)      text->setLayer(layer);
	for (auto& port : ports)	  port->setLayer(layer + portLayerOffset);
	componentLayer = layer;
}

void Component2D::enableOutline()
{
	m_highlighted = true;
	title->enableOutline();
	designator->enableOutline();
	for (auto poly : m_polygons)  poly->enableOutline();
	for (auto line : m_lines)	  line->enableOutline();
	for (auto circ : m_circles)	  circ->enableOutline();
	for (auto text : m_text)      text->enableOutline();
	for (auto& port : ports)	  port->enableOutline();
}

void Component2D::disableOutline()
{
	m_highlighted = false;
	title->disableOutline();
	designator->disableOutline();
	for (auto poly : m_polygons) poly->disableOutline();
	for (auto line : m_lines)    line->disableOutline();
	for (auto circ : m_circles)  circ->disableOutline();
	for (auto text : m_text)     text->disableOutline();
	for (auto& port : ports)	 port->disableOutline();
}

void Component2D::removePort(std::shared_ptr<Port> port)
{
	auto port_to_remove = std::find(begin(ports), end(ports), port);
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
	//std::string textString = designatorSym + std::to_string(designatorIdx);
	title->update();
	title->rotate(m_rotation, glm::vec3(centre, 0.f), { 0.f, 0.f, 1.f });

	if (designator->updateText(m_tag))
		designator->rotate(m_rotation, glm::vec3(centre, 0.f), {0.f, 0.f, 1.f});
}

void Component2D::updateTextWithoutLabel()
{
	title->update();
	title->rotate(m_rotation, glm::vec3(centre, 0.f), { 0.f, 0.f, 1.f });

	if (designator->updateText(designatorSym))
		designator->rotate(m_rotation, glm::vec3(centre, 0.f), { 0.f, 0.f, 1.f });
}

void Component2D::setColour(const glm::vec4& colour)
{
	shapeColour = colour;
	for (auto poly : m_polygons) poly->setColor(shapeColour);
	for (auto circ : m_circles)  circ->setColor(shapeColour);
}

void Component2D::addPoly(Polygon2D* poly)
{
	m_polygons.push_back(poly);
	m_polygons.back()->setLayer(0.001f);//temp fix
}

void Component2D::addCircle(Circle* circle)
{
	m_circles.push_back(circle);
}

void Component2D::addLine(PolyLine* line)
{
	m_lines.push_back(line);
}

void Component2D::addPort(std::shared_ptr<Port> port)
{
	ports.push_back(port);
}

void Component2D::removePoly(Polygon2D* poly)
{
	auto to_remove = std::find(begin(m_polygons), end(m_polygons), poly);
	if (to_remove != m_polygons.end()) 
	{
		Renderer::remove(poly);
		m_polygons.erase(to_remove);
	}
	else 
	{
		LUMEN_LOG_WARN("Attempted to remove a polygon that is not a member of m_polygons.", "Component2D");
	}
}

void Component2D::removeCircle(Circle* circle)
{
	auto to_remove = std::find(begin(m_circles), end(m_circles), circle);
	if (to_remove != m_circles.end()) 
	{
		Renderer::remove(circle);
		m_circles.erase(to_remove);
	}
	else 
	{
		LUMEN_LOG_WARN("Attempted to remove a circle that is not a member of m_circles.", "Component2D");
	}
}

void Component2D::removeLine(PolyLine* line)
{
	auto to_remove = std::find(begin(m_lines), end(m_lines), line);
	if (to_remove != m_lines.end()) 
	{
		Renderer::remove(line);
		m_lines.erase(to_remove);
	}
	else 
	{
		LUMEN_LOG_WARN("Attempted to remove a line that is not a member of m_lines.", "Component2D");
	}
}

void Component2D::removeText(Text* text)
{
	auto to_remove = std::find(begin(m_text), end(m_text), text);
	if (to_remove != m_text.end())
	{
		Renderer::remove(text);
		m_text.erase(to_remove);
	}
	else
	{
		LUMEN_LOG_WARN("Attempted to remove a text object that is not a member of m_text. This can be expected when attempting to delete port or component names.", "Component2D");
	}
}

void Component2D::setTag(const std::string& tag)
{
	if (tag == "None") {//Need to stop tag being re-set here if it has been manually set
		m_tag = designatorSym + std::to_string(designatorIdx);
	}
	else {
		m_tag = tag;
	}

	//update the text whenever the tag is updated
	updateText();
}

int Component2D::getDesignatorIdx()
{
	return designatorIdx;
}

void Component2D::setDesignatorIdx(const int& idx)
{
	//when designator Idx changes, we need to update the default tag (do not overwrite manually edited tags)
	if (m_tag == designatorSym + std::to_string(designatorIdx)) {
		setTag(designatorSym + std::to_string(idx));
	}
	designatorIdx = idx;
}

void Component2D::rotate(float degrees)
{
	glm::vec3 rotateNormal = { 0.f, 0.f, 1.f };
	rotate(degrees, rotateNormal);
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

void Component2D::rotate(float degrees, const glm::vec3& rotateNormal)
{
	m_rotation += degrees;
	glm::vec3 rotationPoint = { centre.x, centre.y, 0.f };
	title->rotate(degrees, rotationPoint, rotateNormal);
	designator->rotate(degrees, rotationPoint, rotateNormal);
	for (auto poly : m_polygons) poly->rotate(degrees, rotationPoint, rotateNormal);
	for (auto line : m_lines)    line->rotate(degrees, rotationPoint, rotateNormal);
	for (auto circ : m_circles)  circ->rotate(degrees, rotationPoint, rotateNormal);
	for (auto text : m_text)     text->rotate(degrees, rotationPoint, rotateNormal);
	for (auto& port : ports)	 port->rotate(degrees, rotationPoint, rotateNormal);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//