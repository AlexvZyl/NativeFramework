//============================================================================================================================================//
// Includes.																																   //
//=============================================================================================================================================//

#include "Component2D.h"
#include "OpenGL/Primitives/Vertex.h"
#include "OpenGL/Primitives/Polygon.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "Graphics/Fonts/FontLoader.h"
#include "OpenGL/Primitives/Text.h"
#include "Resources/ResourceHandler.h"
#include "OpenGL/Primitives/Circle.h"
#include <iostream>
#include "External/Misc/ConsoleColor.h"
#include "Circuit.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "Utilities/Logger/Logger.h"
#include "Utilities/Serialisation/Serialiser.h"

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
	titleString = "Component " + std::to_string(componentID++);
	std::string textString = equipType + std::string(": ") + titleString;
	title = Renderer::addText2D(textString, titlePos, titleColour, titleSize, "C", "B", this);
	// Add some test ports. (TO BE REMOVED). PLease keep this here while we are testing (at least until we have some generic components that can be added). 
	// It is a bit of a pain setting up ports every time we test.
	//addPort(0, PortType::PORT_IN, "LX1");
	//addPort(1, PortType::PORT_OUT, "RX1");
	enableOutline();

	// General dictionary data.
	dataDict.insert({ "ToTagNumber", "From(Circuit Database)" });
	dataDict.insert({ "Metric", "1" });
	dataDict.insert({ "Description", "From(Circuit Database)" });
	dataDict.insert({ "Unit", "ea" });
	dataDict.insert({ "DBRef", "From(Circuit Database)" });
}

Component2D::Component2D(const glm::vec2& centreCoords, Circuit* parent)
	: Component2D(parent)
{
	moveTo(centreCoords);
}

Component2D::Component2D(const YAML::Node& lmcpFile, Circuit* parent) 
	: Entity(EntityType::COMPONENT, parent)
{
	// Check that the node is from a .lmcp file
	if (lmcpFile["Lumen File Info"]["Type"].as<std::string>() != "Component") 
	{
		LUMEN_LOG_ERROR("Components can only be constructed from a .lmcp file.", "Component2D");
	}
	
	// Load the component node.
	YAML::Node componentNode = lmcpFile["Component"];

	// General data.
	borderLayerOffset = componentNode["Border layer offset"].as<float>();
	m_internalCircuit = componentNode["Internal circuit"].as<std::string>();

	// The data dictionary.
	YAML::Node dictNode = componentNode["Dictionary"];
	for (const auto& node : dictNode) 
	{
		dataDict.insert({node.first.as<std::string>(), node.second.as<std::string>()});
	}

	// Add the title.
	title = Renderer::addText2D(componentNode["Title"], this);

	// Add the lines.
	for (const auto& line : componentNode["Line Segments"])
	{
		m_lines.push_back(Renderer::addLineSegment2D(line.second, this));
	}

	// Add polygons.
	for (const auto& poly : componentNode["Polygons"])
	{
		m_polygons.push_back(Renderer::addPolygon2D(poly.second, this));
	}

	// Add circles.
	for (const auto& circle : componentNode["Circles"]) 
	{
		m_circles.push_back(Renderer::addCircle2D(circle.second, this));
	}

	// Add ports.
	for (const auto& port : componentNode["Ports"])
	{
		ports.push_back(std::make_shared<Port>(port.second, this));
	}

	enableOutline();
}

Component2D::~Component2D() 
{
	//Renderer::remove(shape);
	//Renderer::remove(border);
	Renderer::remove(title);
	for (auto circle : m_circles) {
		Renderer::remove(circle);
	}
	for (auto line : m_lines) {
		Renderer::remove(line);
	}
	for (auto poly : m_polygons) {
		Renderer::remove(poly);
	}
}

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

void Component2D::moveTo(const glm::vec2& pointerPos)
{
	//shape->translateTo(pointerPos);
	//border->translateTo(pointerPos);
	glm::vec2 titleDest = pointerPos + titleOffset;
	title->translateTo(titleDest);

	for (auto poly : m_polygons) {
		poly->translateTo(pointerPos);
	}
	for (auto line : m_lines) {
		line->translateTo(pointerPos);
	}
	for (auto circ : m_circles) {
		circ->translateTo(pointerPos);
	}

	for (std::shared_ptr port : ports) {
		port->move(pointerPos - port->centre);
	}
	
	centre = glm::vec2(pointerPos[0], pointerPos[1]);
}

void Component2D::move(const glm::vec2& translation)
{
	//shape->translate(translation);
	//border->translate(translation);
	title->translate(translation);
	for (auto poly : m_polygons) {
		poly->translate(translation);
	}
	for (auto line : m_lines) {
		line->translate(translation);
	}
	for (auto circ : m_circles) {
		circ->translate(translation);
	}

	for (std::shared_ptr port : ports) {
		port->move(translation);
	}
	
	centre += translation;
}

void Component2D::place(const glm::vec2& pos)
{	
	// Ensure the component is at the desired position.
	moveTo(pos);
	setLayer(0.0f);
	//shape->setColor(shapeColour);
	title->setColor(titleColour);
	for (auto poly : m_polygons) {
		poly->setColor(shapeColour);
	}
	for (auto circ : m_circles) {
		circ->setColor(shapeColour);
	}
	// Move to placement layer.
}

void Component2D::setLayer(float layer)
{
	//shape->setLayer(layer);
	//border->setLayer(layer + borderLayerOffset);
	title->setLayer(layer + borderLayerOffset);
	for (auto poly : m_polygons) {
		poly->setLayer(layer);
	}
	for (auto line : m_lines) {
		line->setLayer(layer);
	}
	for (auto circ : m_circles) {
		circ->setLayer(layer);
	}
	for (std::shared_ptr port : ports) {
		port->setLayer(layer + portLayerOffset);
	}
	
	componentLayer = layer;
}

void Component2D::setContext(GUIState* guiState)
{
	//guiState->clickedZone.component = true;
}

void Component2D::enableOutline()
{
	m_highlighted = true;

	title->enableOutline();

	for (auto poly : m_polygons) 
	{
		poly->enableOutline();
	}

	for (auto line : m_lines) 
	{
		line->enableOutline();
	}

	for (auto circ : m_circles) 
	{
		circ->enableOutline();
	}
	
	for (std::shared_ptr port : ports) 
	{
		port->enableOutline();
	}
}

void Component2D::disableOutline()
{
	m_highlighted = false;

	title->disableOutline();

	for (auto poly : m_polygons)
	{
		poly->disableOutline();
	}

	for (auto line : m_lines)
	{
		line->disableOutline();
	}

	for (auto circ : m_circles)
	{
		circ->disableOutline();
	}

	for (std::shared_ptr port : ports)
	{
		port->disableOutline();
	}
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
	//Warn the user if the port was not found on this component (i.e. if we have not returned yet).
	std::cout << yellow << "\n[Design2D] [WARNING]: " << white << "Tried to delete port "<<port->m_label<< ", but it does not belong to component "<< titleString <<".";
}


void Component2D::translateTitle(glm::vec2 translation)
{
	titleOffset += translation;
	title->translate(translation);
}

void Component2D::updateText()
{
	std::string textString = equipType + std::string(": ") + titleString;
	title->updateText(textString);
}

void Component2D::setColour(const glm::vec4& colour)
{
	shapeColour = colour;
	for (auto poly : m_polygons) {
		poly->setColor(shapeColour);
	}
	for (auto circ : m_circles) {
		circ->setColor(shapeColour);
	}
}

void Component2D::addPoly(Polygon2D* poly)
{
	m_polygons.push_back(poly);
	m_polygons.back()->setColor(shapeColour);
	m_polygons.back()->setLayer(0.001f);//temp fix
}

void Component2D::addCircle(Circle* circle)
{
	m_circles.push_back(circle);
}

void Component2D::addLine(LineSegment* line)
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
	if (to_remove != m_polygons.end()) {
		Renderer::remove(poly);
		m_polygons.erase(to_remove);
	}
	else {
		LUMEN_LOG_WARN("Attempted to remove a polygon that is not a member of m_polygons.", "Component2D");
	}
}

void Component2D::removeCircle(Circle* circle)
{
	auto to_remove = std::find(begin(m_circles), end(m_circles), circle);
	if (to_remove != m_circles.end()) {
		Renderer::remove(circle);
		m_circles.erase(to_remove);
	}
	else {
		LUMEN_LOG_WARN("Attempted to remove a circle that is not a member of m_circles.", "Component2D");
	}
}

void Component2D::removeLine(LineSegment* line)
{
	auto to_remove = std::find(begin(m_lines), end(m_lines), line);
	if (to_remove != m_lines.end()) {
		Renderer::remove(line);
		m_lines.erase(to_remove);
	}
	else {
		LUMEN_LOG_WARN("Attempted to remove a line that is not a member of m_lines.", "Component2D");
	}
}

//=============================================================================================================================================//
//  Utilities.				     																											   //
//=============================================================================================================================================//

PortType Component2D::getPortType(YAML::Node node)
{
	if (node["Type"].as<std::string>() == "PORT_IN") { return PortType::PORT_IN; }
	else if (node["Type"].as<std::string>() == "PORT_OUT") { return PortType::PORT_OUT; }
	else if (node["Type"].as<std::string>() == "PORT_INOUT") { return PortType::PORT_INOUT; }
}

//void Component2D::destroy()
//{
	/*this->~Component2D();*/
	//shape->destroy();
	//border->destroy();
	//title->destroy();
//}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//