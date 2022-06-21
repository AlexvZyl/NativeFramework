#pragma once

//==============================================================================================================================================//
//  Includes.  																																    //
//==============================================================================================================================================//

#include "Graphics/Entities/Entity.h"
#include <unordered_map>
#include "yaml-cpp/yaml.h"
#include <glm/glm.hpp>

//==============================================================================================================================================//
//  Forward declerations.  																													    //
//==============================================================================================================================================//

class Circuit;
class Port;
class LineSegment;
class PolyLine;

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

class Cable : public Entity
{
public:

    static unsigned cableID;
        
    // Data.
    std::unordered_map<std::string, std::string> cableDict;
    std::string m_cableType = "";
    std::string m_titleString ="Cable";
    glm::vec4 m_colour = { 0.f / 255.f, 0.f / 255.f, 0.f / 255.f, 1.f };
    glm::vec4 m_titleColour = glm::vec4(0.f, 0.f, 0.f, 1.f);
    float m_thickness = 0.0004f;
    float m_titleOffset = 0.01f;
    float m_titleSize = 0.02f;

    // Connections.
    Port* m_startPort;
    Port* m_endPort = nullptr;

    // Components.
    std::unique_ptr<PolyLine> m_polyLine;

public:

    // Describe the connections relative to the position in memory.
    int startComponentIndex = 0;
    int endComponentIndex = 0;
    int startPortIndex = 0;
    int endPortIndex = 0;
    
    // Constructors.
    Cable(Port* startPort, Circuit* parent);
    Cable(const YAML::Node& node, Circuit* parent);
    Cable(Port* startPort, std::vector<glm::vec2> nodeList, Port* endPort, Circuit* parent, std::string titleString);

    // Destructor.
    ~Cable();

    // Common.
    void extendSegment(glm::vec2 nextPoint);
    void addSegment(glm::vec2 nextPoint);
    bool attach(Port* endPort);
    void followPort(Port* movedPort);
    void setColour(glm::vec4 colour, bool save = true);
    void translateVertexAtIndex(unsigned vertexIdx, glm::vec2 translation);
    void translateVertexAtIndexTo(unsigned vertexIdx, glm::vec2 position);
    void enableOutline();
    void disableOutline();
    void constructCable(Port* startPort, std::vector<glm::vec2> nodeList, Port* endPort);
    std::tuple<unsigned, float>  getNearestVertexIdx(glm::vec2 pos);
};

//==============================================================================================================================================//
//  EOF.  																																        //
//==============================================================================================================================================//