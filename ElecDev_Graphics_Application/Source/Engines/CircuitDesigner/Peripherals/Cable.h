#pragma once

//==============================================================================================================================================//
//  Includes.  																																    //
//==============================================================================================================================================//

#include "Graphics/Entities/Entity.h"
#include <unordered_map>
#include "yaml-cpp/yaml.h"
#include <glm/glm.hpp>
#include "Engines/EntityComponents/Mutable.h"
#include "Graphics/OpenGL/Primitives/PolyLine.h"

//==============================================================================================================================================//
//  Forward declerations.  																													    //
//==============================================================================================================================================//

class Circuit;
class Port;

//==============================================================================================================================================//
//  Data.  																																        //
//==============================================================================================================================================//

enum class LineOrientation
{
	HORIZONTAL,
	VERTICAL
};

//==============================================================================================================================================//
//  Cable class.  																																//
//==============================================================================================================================================//

class Cable : public Entity, public Reshapable
{
public:

	static unsigned cableID;

	Port* m_startPort;
	Port* m_endPort = nullptr;

	// Data Automation
	std::unordered_map<std::string, std::string> cableDict;

	std::string m_cableType = "";

	float m_thickness = 0.0004f;
	glm::vec4 m_colour = { 0.f / 255.f, 0.f / 255.f, 0.f / 255.f, 1.f };
	PolyLine* m_polyLine;
	//std::vector<glm::vec2> m_vertices;
	//std::vector<LineSegment*> m_lines;
   // std::vector<Circle*> m_nodes;
   // LineOrientation m_curOrientation = LineOrientation::HORIZONTAL;
	//LineSegment* m_activeLine = nullptr;
	//Circle* m_activeNode = nullptr;

	std::string m_titleString = "Cable";
	//Text* m_title1 = nullptr;
	//Text* m_title2 = nullptr;
	float m_titleOffset = 0.01f;
	glm::vec4 m_titleColour = glm::vec4(0.f, 0.f, 0.f, 1.f);
	float m_titleSize = 0.02f;

public:

	// Describe the connections relative to the position in memory.
	int startComponentIndex = 0;
	int endComponentIndex = 0;
	int startPortIndex = 0;
	int endPortIndex = 0;

	//Create a new cable attached to the start port.
	Cable(Port* startPort, Circuit* parent);
	Cable(const YAML::Node& node, Circuit* parent);
	//Create a new cable from one port to another that gots through each node in the node list
	Cable(Port* startPort, std::vector<glm::vec2> nodeList, Port* endPort, Circuit* parent, std::string titleString);
	~Cable();
	void extendSegment(glm::vec2 nextPoint);
	void addSegment(glm::vec2 nextPoint);
	bool attach(Port* endPort);
	void followPort(Port* movedPort);
	void setColour(glm::vec4 colour, bool save = true);
	void translateVertexAtIndex(const unsigned& vertexIdx, const glm::vec2& translation) override;
	void translateVertexAtIndexTo(const unsigned& vertexIdx, const glm::vec2& position) override;
	inline unsigned logicalVertexCount() { return m_polyLine->logicalVertexCount(); };
	inline glm::vec2 getLogicalVertex(unsigned localIndex) { return m_polyLine->getLogicalVertex(localIndex); };
	inline std::tuple<unsigned, float> getNearestVertexIndex(const glm::vec3& position) { return m_polyLine->getNearestVertexIndex(position); };
	inline std::tuple<unsigned, float> getNearestVertexIndex(const glm::vec2& position) { return m_polyLine->getNearestVertexIndex(position); };
	inline void enableOutline() { m_polyLine->enableOutline(); };
	inline void disableOutline() { m_polyLine->disableOutline(); };

	void constructCable(Port* startPort, std::vector<glm::vec2> nodeList, Port* endPort);
};

//==============================================================================================================================================//
//  EOF.  																																        //
//==============================================================================================================================================//