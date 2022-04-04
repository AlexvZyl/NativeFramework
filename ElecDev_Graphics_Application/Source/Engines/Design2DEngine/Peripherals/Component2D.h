#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <string>
#include "glm/glm.hpp"
#include <memory>
#include <vector>
#include "Port.h"
#include <unordered_map>
#include "Graphics/Entities/Entity.h"
#include "External/YAML-CPP/Includes/yaml-cpp/yaml.h"
//=============================================================================================================================================//
//  Forward declerations.																													   //
//=============================================================================================================================================//

template<typename VertexType>
class VertexArrayObject;

class Circle;
class Text;
class Polygon2D;
struct Font;
class VertexData;
class VertexDataTextured;
class VertexDataCircle;
class Circuit;
class Scene;
class LineSegment;

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

class Component2D : public Entity
{
public:

	// Shape and edge data.
	std::vector<Polygon2D*> m_polygons;
	std::vector<LineSegment*> m_lines;
	std::vector<Circle*> m_circles;

	Polygon2D* shape;
	Polygon2D* border;
	Text* title;

	static unsigned componentID;

	// Specify the type of the equipment
	std::string equipType = "Block";

	// 
	std::unordered_map<std::string, std::string> dataDict;

	// Component shape attributes.
	float height = 0.08f;
	float width = 0.08f;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> vertices2;

	// Colour attributes.
	glm::vec4 shapeColour = { 14.f/255.f, 63.f/255.f, 174.f/255.f, 1.f };
	glm::vec4 borderColour = { 0.f, 0.f, 0.f, 1.f };
	bool m_highlighted = false;

	//title
	static Font titleFont;
	glm::vec2 titleOffset = glm::vec2(0.f, -0.11f);
	glm::vec4 titleColour = glm::vec4(0.f, 0.f, 0.f, 1.f);
	std::string titleString;
	float titleSize = 0.035f;

	//port specifications
	unsigned n_ports_north = 0;
	unsigned n_ports_south = 0;
	unsigned n_ports_east = 0;
	unsigned n_ports_west = 0;
	glm::vec2 portOffset;

	// Interaction attributes.
	bool selected = true;

	// Port lists.
	std::vector<std::shared_ptr<Port>> portsNorth;
	std::vector<std::shared_ptr<Port>> portsSouth;
	std::vector<std::shared_ptr<Port>> portsEast;
	std::vector<std::shared_ptr<Port>> portsWest;

	unsigned numPorts = 0;

	float componentLayer = 0.9f;
	float borderLayerOffset = 0.01f;
	float portLayerOffset = 0.02f;
	glm::vec2 centre;
	
	// Creates a generic component centred at (0, 0).
	Component2D(Circuit* parent);
	// Creates a generic component centred at the specified coordinates.
	Component2D(const glm::vec2& centreCoords, Circuit* parent);
	// Creates a component from a .lmcp file definition
	Component2D(YAML::Node& lmcpFile, Circuit* parent);

	// Deconstructor.s
	~Component2D();
	//Move the component to a new positioned centred at the given coordinates
	void moveTo(const glm::vec2& pointerPos);
	//Move the component by the  given vector
	void move(const glm::vec2& translation);
	//Place the component.
	void place(const glm::vec2& pos);
	//Move the component to a new layer.
	void setLayer(float layer);
	//set the clickedZone.component flag in the GUIState.
	void setContext(GUIState* guiState);
	//Highlight the component.
	void highlight();
	//Remove the component highlighting.
	void unhighlight();
	//Add a port with the given definition to the component.
	unsigned addPort(int side, PortType type, const std::string& name);
	//remove a specified port from the component.
	void removePort(std::shared_ptr<Port> port);
	//Update the positions(offsets) of each port. Note: Currently, all ports are updated by this regardless if they have been changed or not. This may lead to inefficiencies, and should be changed in the future.
	void updatePortPositions();
	//move the title text relative to the component
	void translateTitle(glm::vec2 translation);
	//updates the Component label
	void updateText();
	//Adds a new polygon to the polygon list
	 Polygon2D* addPoly(std::vector<glm::vec2> vertices);
	void addCircle(Circle*);
	void addLine(LineSegment*);
	

private:
	PortType getPortType(YAML::Node node);
	//void destroy();

	void enableOutline();
	void disableOutline();
};
