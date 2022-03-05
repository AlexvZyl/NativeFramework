#pragma once

//==============================================================================================================================================//
//  Includes.  																																    //
//==============================================================================================================================================//

#include "OpenGL/Primitives/LineSegment.h"
#include "Graphics/Entities/Entity.h"
#include "Port.h"
#include <unordered_map>

//==============================================================================================================================================//
//  Forward declerations.  																													    //
//==============================================================================================================================================//

class Circuit;

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
        
    Port* m_startPort;
    Port* m_endPort = nullptr;

    // Data Automation
    std::unordered_map<std::string, std::string> cableDict;

    float m_thickness = 0.005f;
    glm::vec4 m_colour = { 0.5f, 0.5f, 0.5f, 1.f };
    //std::vector<glm::vec2> m_vertices;
    std::vector<LineSegment*> m_lines;
    std::vector<Circle*> m_nodes;
    LineOrientation m_curOrientation = LineOrientation::HORIZONTAL;
    LineSegment* m_activeLine = nullptr;
    Circle* m_activeNode = nullptr;

    std::string m_titleString ="Cable";
    Text* m_title1 = nullptr;
    Text* m_title2 = nullptr;
    static Font m_titleFont;
    float m_titleOffset = 0.01f;
    glm::vec4 m_titleColour = glm::vec4(0.f, 0.f, 0.f, 1.f);
    float m_titleSize = 0.02f;

public:
    
    //Create a new cable attached to the start port.
    Cable(Port* startPort, Circuit* parent);
    //Create a new cable from one port to another that gots through each node in the node list
    Cable(Port* startPort, std::vector<glm::vec2> nodeList, Port* endPort, Circuit* parent, std::string titleString);
    ~Cable();
    void extendSegment(glm::vec2 nextPoint);
    void extendPrevSegment(glm::vec2 nextPoint);
    void addSegment(glm::vec2 nextPoint);
    void setContext(GUIState* guiState);
    void attach(Port* endPort);
    void followPort(Port* movedPort);
    void setColour(glm::vec4 colour, bool save = true);
    void highlight();
    void moveActivePrimitiveTo(glm::vec2 screenCoords);
    void setActivePrimitive(Entity* primitive);
    void unhighlight();
};

//==============================================================================================================================================//
//  EOF.  																																        //
//==============================================================================================================================================//