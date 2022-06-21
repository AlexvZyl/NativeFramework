#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <string>
#include "glm/glm.hpp"
#include <memory>
#include <vector>
#include <unordered_map>
#include "Graphics/Entities/Entity.h"
#include "External/YAML-CPP/Includes/yaml-cpp/yaml.h"
#include <filesystem>

//=============================================================================================================================================//
//  Forward declerations.																													   //
//=============================================================================================================================================//

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
class PolyLine;
class Port;
enum class PortType;

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

class Component2D : public Entity
{
public:

	inline static unsigned componentID = 0;

	//  ---  Common  ---  //

	// Constructors.
	Component2D(Circuit* parent);
	Component2D(const glm::vec2& centreCoords, Circuit* parent);
	Component2D(const std::filesystem::path& path, Circuit* parent = nullptr);
	Component2D(const YAML::Node& path, Circuit* parent = nullptr);

	// Destructor.
	inline ~Component2D() = default;

	// Place the component.
	void place(const glm::vec2& pos);
	
	// Manipulation.
	void removePort(Port* port);
	void rotate(float degrees);
	void translateTitle(glm::vec2 translation);
	void enableOutline();
	void moveTo(const glm::vec2& pointerPos);
	void move(const glm::vec2& translation);
	void disableOutline();
	void setLayer(float layer);
	// Updates the component label.
	void updateText();
	// Updates the Component type, without a label (for component designer).
	void updateTextWithoutLabel();
	void setColour(const glm::vec4& colour);
	
	// Primitives.
	void addPoly(std::unique_ptr<Polygon2D>&& poly);
	void addCircle(std::unique_ptr<Circle>&& circle);
	void addLine(std::unique_ptr<PolyLine>&& line);
	void addPort(std::unique_ptr<Port>&& port);
	void removePoly(Polygon2D* poly);
	void removeCircle(Circle* circle);
	void removeLine(PolyLine* line);
	void removeText(Text* text);

	//  ---  Data  --  //

	// Entites & Primitives describing the component.
	std::vector<std::unique_ptr<Polygon2D>> m_polygons;
	std::vector<std::unique_ptr<PolyLine>> m_lines;
	std::vector<std::unique_ptr<Circle>> m_circles;
	std::vector<std::unique_ptr<Text>> m_text;
	std::unique_ptr<Text> title;
	std::unique_ptr<Text> designator;
	std::vector<std::unique_ptr<Port>> ports;

	// Specify the type of the equipment
	std::string equipType = "Block";
	std::string type = "Unspecified";
	std::string designatorSym = "?";
	int designatorIdx = -1;

	// Data for ElecDec software.
	std::unordered_map<std::string, std::string> dataDict;
	std::string m_internalCircuit;
	std::string m_fromTagNumber = "None";
	std::string m_toTagNumber = "None";

	// Component shape attributes.
	float height = 0.08f;
	float width = 0.08f;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> vertices2;

	// Colour attributes.
	glm::vec4 shapeColour = { 14.f/255.f, 63.f/255.f, 174.f/255.f, 1.f };
	glm::vec4 borderColour = { 0.f, 0.f, 0.f, 1.f };
	bool m_highlighted = false;

	// Text.
	glm::vec2 titleOffset = glm::vec2(0.01f, -0.001f);
	glm::vec2 designatorOffset = glm::vec2(0.01f, 0.001f);
	glm::vec4 titleColour = glm::vec4(0.f, 0.f, 0.f, 1.f);
	float titleSize = 0.0018f;

	glm::vec2 portOffset;
	bool selected = true;
	unsigned numPorts = 0;
	float componentLayer = 0.9f;
	float borderLayerOffset = 0.01f;
	float portLayerOffset = 0.02f;
	glm::vec2 centre = {0.f, 0.f};
	float m_rotation = 0.f;

private:

	PortType getPortType(YAML::Node node);
};
