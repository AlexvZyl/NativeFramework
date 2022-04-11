#pragma once
#include "Primitive.h"
#include "OpenGL/Primitives/LineSegment.h"
#include "OpenGL/Primitives/Circle.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "Utilities/Logger/Logger.h"
class VertexData;


class PolyLine :
    public Entity
{

    public:
    std::vector<std::shared_ptr<LineSegment>> m_lines;
    std::vector< std::shared_ptr<Circle>> m_nodes;
    glm::vec4 m_colour{ 0.f, 0.f, 0.f, 1.f };
    float m_thickness = 0.005f;
    
    PolyLine(std::vector<glm::vec2> vertices, Entity* parent);
    //Add a new line segment
    void addSegment(glm::vec2 nextPoint);
    // Move a vertex
    void translateVertex(Circle* vertex, const glm::vec2 translation);


};

